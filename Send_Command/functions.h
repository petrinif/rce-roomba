#include <string.h>
#include <time.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <SerialStream.h>
using namespace LibSerial ;


int recognize_command(char** command,char* message, int argc );


void stop_the_roomba(SerialStream &serial_port);


void open_serial_port(SerialStream &serial_port);


void get_distance(SerialStream &serial_port, int stop_at_distance);
