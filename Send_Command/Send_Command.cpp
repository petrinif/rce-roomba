#include <string.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "functions.h"

using namespace LibSerial ;
const int BUFFER_SIZE = 100, MESSAGE_SIZE = 50, DATA_STREAM_SIZE = 20, ELAPSED =150;


//                 Create Command              // Arguments
const char         Start = 128;
const char         SafeMode = 131;
const char         FullMode = 132;
const char         Drive = 137;                // 4:   [Vel. Hi] [Vel Low] [Rad. Hi] [Rad. Low]
const char         DriveDirect = 145;          // 4:   [Right Hi] [Right Low] [Left Hi] [Left Low]
const char         Demo = 136;                 // 2:    Run Demo x
const char         Sensors = 142;              // 1:    Sensor Packet ID
const char         SensorStream = 148;         // x+1: [# of packets requested] IDs of requested packets to stream
const char         QueryList = 149;            // x+1: [# of packets requested] IDs of requested packets to stream
const char         StreamPause = 150;          // 1:    0 = stop stream, 1 = start stream
const char         LED_Color = 139;
const char         PlaySong = 141;
const char         Song = 140;
const char	   	   Distance = 19; 				// Request distance sensor data
const char 	   	   Cliff_front_left = 10;		// Request cliff sensor front left data
const char 		   Packet_ID_2 = 2;				// Request packet sensors 17-20
		
		



int
main(int argc, char* argv[])
{
	if (argc < 2 ){
		std::cout << "Missing command" << std::endl;
		exit(1);
	}

	int i,expected_return = 0;

	SerialStream serial_port ;

	char message[MESSAGE_SIZE] = {0};
	char buffer[BUFFER_SIZE] = {0};
	char data_stream[DATA_STREAM_SIZE] = {0};

	open_serial_port(serial_port);
	expected_return = recognize_command(argv,message,argc);


	
	
	// Composing the buffer which will be sent
	buffer[0]=  Start;
	buffer[1]=  FullMode;
	buffer[2] = LED_Color;
	buffer[3] = 0;
	buffer[4] = 255;
	buffer[5] = 255;
	
	for(i=0;i<MESSAGE_SIZE;i++){
		buffer[i+6] = message[i];
	}

	sleep(0.2);

    // Writing data on the serial port
    for(i=0;i<BUFFER_SIZE;i++)
    {
        serial_port.write( &buffer[i], 1 ) ;
    }


    // Reading from the Serial port in case of expected return of data

    if (expected_return == 0){
    	get_distance(serial_port,1500);
    }


    //END
    //stop_the_roomba(serial_port);
    serial_port.Close();
    std::cout << "\ndone" << std::endl;
    return 0;


}





