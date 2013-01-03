
#include "JSON_Handling.h"
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include "json_spirit.h"
#include "json_spirit_stream_reader.h"

using namespace std;
using namespace json_spirit;




/* This function reads the incoming JSON message (string s) and reads the values into
Command 1 if the id of the message is equal to one (low level control, speed and radius
of the roomba) or into the Command 2 if the id of the mssage is two. (required x,y and orientation
of the Roomba are provided). */

void read_incoming_message(Command_1 &cmd1,  Command_2 &cmd2, const string s){

	std::string type, dest, orig;
	int ID;
	int Radius, Speed, Moving_Code;
	Value value;
	read(s, value); //read the jsonInput to the value

	const Pair& pair = value.get_obj()[0]; //gets the name:value pair, named "request"
	try{

		type = pair.value_.get_str(); //gets the object DM

		const Pair& data = value.get_obj()[1]; //gets the name:value pair, named "data"
		const Pair& d = data.value_.get_obj()[0]; //gets the objects
		dest = d.value_.get_str();

		const Pair& e = data.value_.get_obj()[1]; //gets the objects
		orig = e.value_.get_str();

		const Pair& f = data.value_.get_obj()[3]; //gets the objects
		stringstream(f.value_.get_str()) >> ID; // Reading the speed as string into an integer
		cout << "ID: " << ID << endl;

		if (ID==1)
		{

			const Pair& msg = data.value_.get_obj()[4]; //gets the name:value pair, named "msg"
			const Pair& g = msg.value_.get_obj()[0]; //gets the objects
			stringstream(g.value_.get_str()) >> Speed; // Reading the speed as string into an integer
			const Pair& h = msg.value_.get_obj()[1]; //gets the objects
			stringstream(h.value_.get_str()) >> Radius; // Reading the speed as string into an integer

			cmd1.Radius = Radius;
			cmd1.Speed = Speed;
			cmd1.ID = 1;
			cmd2.ID = 1;
			cout << "Speed: " << cmd1.Speed << "mm/s" <<endl;
			cout << "Radius: " << cmd1.Radius << "mm" <<endl;

		}
		else if (ID==2)
		{	
			const Pair& msg = data.value_.get_obj()[4]; //gets the name:value pair, named "msg"
			const Pair& g = msg.value_.get_obj()[0]; //gets the objects
			stringstream(g.value_.get_str()) >> Speed; // Reading the speed as string into an integer
			const Pair& h = msg.value_.get_obj()[1]; //gets the objects
			stringstream(h.value_.get_str()) >> Moving_Code; // Reading the speed as string into an integer

			cmd2.Moving_Code = Moving_Code;
			cmd2.Speed = Speed;
			cmd2.ID = 2;
			cmd1.ID = 2;

		
			cout << "Speed: " << cmd2.Speed<< "mm/s" <<endl;

		}
		else if (ID==3)
		{	
			cmd2.ID = 3;
			cmd1.ID = 3;
		
			cout << "Reset the distance: 0" <<endl;


		}
		else
		{
			cout << "Unable to read the incoming message: ID not defined"<< endl;
		}

	}
	catch(std::exception& e){
		cout << "Unable to read the incoming message"<< endl;
	}


}

