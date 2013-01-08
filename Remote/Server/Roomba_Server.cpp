#include "websocketpp/src/websocketpp.hpp"
#include "JSON_Handling.h"
#include "Serial_lib.h"
#include "Timeout.h"

#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <SerialStream.h>
#include <pthread.h> // Used in order to create a new thread which will watch the Roomba timeout


using namespace LibSerial ;
using websocketpp::server;
using namespace std;

// Packet numbers which are sent to the Roomba
const char         Start = 128;
const char         FullMode = 132;
const char         Drive = 137;                // 4:   [Vel. Hi] [Vel Low] [Rad. Hi] [Rad. Low]
const char         LED_Color = 139;
extern int 	   Distance_trav;
double	   	   elapsed = 0;
extern clock_t     begin;


class echo_server_handler : public server::handler {
public:

 	/*When a message arrives this function gets the string
	and by calling the function read_incoming_message it decides
	which commands should get to the roomba. After that it 
	opens a serial port with open_serial_port and starts streaming
	the command*/
    void on_message(connection_ptr con, message_ptr msg) {
	begin = clock();
	std::string message = msg ->get_payload();
	int i,result = 0;
	const int BUFFER_SIZE = 11;
	char buffer[BUFFER_SIZE] = {0};
	Command_1 cmd1;
	Command_2 cmd2;

	read_incoming_message(cmd1, cmd2, message);

	//Opening Serial Port
	SerialStream serial_port;

	//Requesting traveled distance

	result = open_serial_port(serial_port);
	if (result == 1){
		std::cout << "Unable to connect to Roomba" << std:: endl;
	}
	else if (cmd1.ID == 1){
		//Speed [mm/s]

		int speed = cmd1.Speed;
		char speed_low_byte = speed & 0xFF;
		char speed_high_byte = (speed >> 8) & 0xFF;

		//Radius [mm]


		int radius = cmd1.Radius;
		char radius_low_byte = radius & 0xFF;
		char radius_high_byte = (radius >> 8) & 0xFF;

		// Composing the buffer which will be sent
		buffer[0]=  Start;
		buffer[1]=  FullMode;
		buffer[2] = LED_Color;
		buffer[3] = 0;
		buffer[4] = 255;
		buffer[5] = 255;
		buffer[6] = Drive;
		buffer[7] = speed_high_byte;
		buffer[8] = speed_low_byte;
		buffer[9] = radius_high_byte;
		buffer[10] = radius_low_byte;
	
		sleep(0.1);

		// Writing data on the serial port
		for(i=0;i<BUFFER_SIZE;i++)
		{
			serial_port.write( &buffer[i], 1 ) ;
		}
		get_distance(serial_port);

		serial_port.Close();
		std::cout << "\ndone" << std::endl;
	}
	else if (cmd1.ID == 2){		
		int radius = 0;
		int speed = cmd2.Speed;

		if (cmd2.Moving_Code == 87){
		// Forwards
			radius = 32768;
		}
		else if (cmd2.Moving_Code == 65){
		// Left
			radius = 0;		
		}
		else if (cmd2.Moving_Code == 83){
		// Backwards
			radius = 32768;
			speed = -speed;		

		}
		else if (cmd2.Moving_Code == 68){
		// Right
			radius = 65535;		
		}
		cout << "Radius: " << radius <<endl;

		//Speed [mm/s]

		char speed_low_byte = speed & 0xFF;
		char speed_high_byte = (speed >> 8) & 0xFF;

		//Radius [mm]
		char radius_low_byte = radius & 0xFF;
		char radius_high_byte = (radius >> 8) & 0xFF;

		// Composing the buffer which will be sent
		buffer[0]=  Start;
		buffer[1]=  FullMode;
		buffer[2] = LED_Color;
		buffer[3] = 0;
		buffer[4] = 0;
		buffer[5] = 255;
		buffer[6] = Drive;
		buffer[7] = speed_high_byte;
		buffer[8] = speed_low_byte;
		buffer[9] = radius_high_byte;
		buffer[10] = radius_low_byte;
	
		sleep(0.1);

		// Writing data on the serial port
		for(i=0;i<BUFFER_SIZE;i++)
		{
			serial_port.write( &buffer[i], 1 ) ;
		}
		get_distance(serial_port);


		serial_port.Close();
		std::cout << "\ndone" << std::endl;
	}
	else if (cmd1.ID==3)
	{	
		   Distance_trav = 0;		
	}
	else{
		std::cout << "ID not defined, could not choose a mode \ndone" << std::endl;
	}
	
	
	std::cout << "Dist: " << Distance_trav << std::endl;

	//Send message back

	stringstream ss;//create a stringstream
   	ss << Distance_trav;//add number to the stream

	// Sending distance to client
        con->send(ss.str(),msg->get_opcode());
    }
};

int main(int argc, char* argv[]) {
    unsigned short port = 9000;
        
    if (argc == 2) {
        port = atoi(argv[1]);
        
        if (port == 0) {
            std::cout << "Unable to parse port input " << argv[1] << std::endl;
            return 1;
        }
    }
    
    try {   
 
	/* Check for timeout on message income in order to stop the Roomba if the client terminates the connection
	This is done by creating a watchdog thread located in the timeout.h file*/
	pthread_t thread1;
     	int  iret1,arg = 0;
     	iret1 = pthread_create( &thread1, NULL, check_message_timeout, (void*) arg);
	std::cout << "Timeout check Thread started: " << iret1 << std::endl;


	// Starting server stuff
        server::handler::ptr h(new echo_server_handler());
        server echo_endpoint(h);
        
        echo_endpoint.alog().unset_level(websocketpp::log::alevel::ALL);
        echo_endpoint.elog().unset_level(websocketpp::log::elevel::ALL);
        
        echo_endpoint.alog().set_level(websocketpp::log::alevel::CONNECT);
        echo_endpoint.alog().set_level(websocketpp::log::alevel::DISCONNECT);
        
        echo_endpoint.elog().set_level(websocketpp::log::elevel::RERROR);
        echo_endpoint.elog().set_level(websocketpp::log::elevel::FATAL);
        
        std::cout << "Starting Roomba server on port " << port << std::endl;
        echo_endpoint.listen(port);


    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
