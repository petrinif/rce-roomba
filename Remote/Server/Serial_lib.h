#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <SerialStream.h>

#ifndef Serial_lib
#define Serial_lib

using namespace LibSerial;


/* This function opens a serial stream to a port located in the
usb device ttyUSB0, which is the serial port where the roomba
should be connected */

int open_serial_port(SerialStream &serial_port);


/* This function returns the distance traveled by the Roomba, the 
distance is written in the global variable Distance_trav*/
void get_distance(SerialStream &serial_port);
#endif
