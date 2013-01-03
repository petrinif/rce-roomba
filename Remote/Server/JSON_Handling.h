#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include "json_spirit.h"
#include "json_spirit_stream_reader.h"
#include <SerialStream.h>

#ifndef JSON_Handling
#define JSON_Handling
using namespace std;
using namespace json_spirit;

struct Command_1
{
    int Speed;
    int Radius;
    int ID;
};



struct Command_2
{
    int Speed;
    int Moving_Code;
    int ID;
};


/* This function reads the incoming JSON message (string s) and reads the values into
Command 1 if the id of the message is equal to one (low level control, speed and radius
of the roomba) or into the Command 2 if the id of the mssage is two. (required x,y and orientation
of the Roomba are provided). */

void read_incoming_message(Command_1 &cmd1, Command_2  &cmd2, const string s);
#endif
