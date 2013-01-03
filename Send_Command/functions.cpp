#include "functions.h"
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <time.h>
#include <SerialStream.h>
using namespace LibSerial;

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


int recognize_command(char** command,char* message , int argc){
	int expect_return = 0;
	
	//Comparing strings in order to decide what the user wants, and storing the data into a
	//a message array which will be sent to the serial port

	if (strcmp("alt", command[1])== 0){

		message[0] = Drive;
		message[1] = 0;
		message[2] = 0;
		message[3] = 0;	
		message[3] = 0;	
		
	}
	else if (strcmp("play_led_on", command[1])== 0){
		std::cout << "Turning on the play LED" << std::endl;
		message[0] = LED_Color;
		message[1] = 2;
		message[2] = 0;
		message[3] = 0;
		
		
	}
	else if (strcmp("leds_off", command[1])== 0){
		std::cout << "Turning off all LEDs" << std::endl;
		message[0] = LED_Color;
		message[1] = 0;
		message[2] = 0;
		message[3] = 0;
		
		
	}
	else if (strcmp("advance_led_on", command[1])== 0){
		std::cout << "Turning on the advance LED" << std::endl;
		message[0] = LED_Color;
		message[1] = 8;
		message[2] = 0;
		message[3] = 0;
		
		
	}
	else if (strcmp("power_led_on", command[1])== 0){
		std::cout << "Turning on the power LED" << std::endl;
		message[0] = LED_Color;
		message[1] = 0;
		
		if (strcmp("green", command[2])== 0)
			message[2] = 0;
		if (strcmp("red", command[2])== 0)
			message[2] = 255;
		message[3] = 255;
		
		
	}
	else if (strcmp("move_roomba", command[1]) == 0){
		std::cout << "Moving Roomba";
		message[0] = Drive;
		
		if (argc < 4){
			//Maximal Angle (Default)
			message[3] = 7;
			message[4] = 208;

			int speed = atoi(command[2]);
			char speed_low_byte = speed & 0xFF;
			char speed_high_byte = (speed >> 8) & 0xFF;
			message[1] = speed_high_byte;
			message[2] = speed_low_byte;

			std::cout << " straigthforward" << std::endl;

		} 
		else if (argc < 3){
			std::cout << " with default speed 0" << std::endl;
			message[1] = 0;
			message[2] = 0;
			
		} 
		else{
			std::cout << "\n" << std::endl;

			//Speed [mm/s]

			int speed = atoi(command[2]);
			char speed_low_byte = speed & 0xFF;
			char speed_high_byte = (speed >> 8) & 0xFF;

			//Radius [mm]


			int radius = atoi(command[3]);
			char radius_low_byte = radius & 0xFF;
			char radius_high_byte = (radius >> 8) & 0xFF;

			message[1] = speed_high_byte;
			message[2] = speed_low_byte;

			message[3] = radius_high_byte;
			message[4] = radius_low_byte;
		}
		
		expect_return = 0;


	}
	else 
	{
		std::cout << "Command not recognized" << std::endl;
		exit(1);
	}
	return expect_return;
}

void stop_the_roomba(SerialStream &serial_port){
	//This function sends the stop command to the roomba, the variable stop sets the timeout
	int stop = 0;
	
	std::cout << "Stopping Roomba in:" << std::endl;
	time_t start,end;
	int elapsed_time=0;
	int counter = stop;
	time(&start);
	
	
	// counter
	while(true){
		time(&end);
		elapsed_time = stop-difftime(end,start);	
		if (counter == elapsed_time){
			std:: cout << elapsed_time << "s" << std::endl;
			counter --;
		}
		
		if(elapsed_time==0){
			break;
		}
	}
	
	
	// Stop command
	char message[11] = {0};
	int message_size = 11,i;
	            
	message[0] = 128;
	message[1] = 132;
	message[2] = Drive;
	message[3] = 0;
	message[4] = 0;
	message[5] = 0;
	message[6] = 0;
	message[7] = LED_Color;
	message[8] = 2;
	message[9] = 0;
	message[10] = 0;

    for(i=0;i<message_size;i++)
    {
        serial_port.write( &message[i], 1 ) ;
    }
    
    std::cout << "stop signal sent" << std::endl;
}



void open_serial_port(SerialStream &serial_port){
	
	
	// This function opens a serial port with the below defined parameters:
	
	const char* const SERIAL_PORT_DEVICE = "/dev/ttyUSB0" ;
	
	
	
	
    // Open the serial port.
    serial_port.Open(SERIAL_PORT_DEVICE,std::ios_base::in | std::ios_base::out ) ;

    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not open serial port "
                  << SERIAL_PORT_DEVICE
                  << std::endl ;
        exit(1) ;
    }
    // Set the baud rate of the serial port.
    serial_port.SetBaudRate( SerialStreamBuf::BAUD_57600) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the baud rate." << std::endl ;
        exit(1) ;
    }
    // Set the number of data bits.
    serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the character size." << std::endl ;
        exit(1) ;
    }
    // Disable parity.
    serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not disable the parity." << std::endl ;
        exit(1) ;
    }
    // Set the number of stop bits.
    serial_port.SetNumOfStopBits( 1 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the number of stop bits."
                  << std::endl ;
        exit(1) ;
    }
    // Turn on hardware flow control.
    serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not use hardware flow control."
                  << std::endl ;
        exit(1) ;
    }
    
}


void get_distance(SerialStream &serial_port,int stop_at_distance){
			char data_stream[200] = {0};
			const int distance_size = 11;
			char request[distance_size] = {0};
			char next_byte=0;
			int i;
			short distance_traveled = 0;
			char distance_low_byte = 0;
			char distance_high_byte = 0;
			double first = 1;
			int sum = 0;

			
			while(true){
				// Message request for distsance sensor and toggle an advance LED.
				request[0] = 0;
				request[1] = 0;
				request[2] = LED_Color;
				request[3] = 8;
				request[4] = 0;
				request[5] = 0;
				request[6] = SensorStream;
				request[7] = 1;
				request[8] = Distance;
				
				//Sending request to the serial port
				for (i=0; i<distance_size;i++){
					serial_port.write( &request[i], 1 ) ;
					request[i] = 0;
				}
	
				
				//Reading request and streaming the response into data_stream (which is an array)
				
				while(next_byte==0){
					serial_port.read( &next_byte, 1 );				
				}

				
				
				for (i=0; i<15; i++) {
					serial_port.read( &next_byte, 1 );
					data_stream[i] = next_byte;
					
					if(i>4 && next_byte ==19)
						break;
				}
	
				request[0] = 150;
				request[1] = 0;
	
				sleep(0.5);
				//Sending request to the serial port in order to interrupt stream
				for (i=0; i<2;i++){
					serial_port.write( &request[i], 1 ) ;
					request[i] = 0;
				}
	
				sleep(1);
				
	
				distance_low_byte = data_stream[4];
				distance_high_byte = data_stream[3];
				
				distance_traveled = (distance_high_byte << 8);
				distance_traveled = (distance_traveled | distance_low_byte);
	
				if (first == 1){
					distance_traveled = 0;
					first = 0;
				}
				
				// calculating traveled distance: [mm]
				sum = (sum + distance_traveled);
				std::cout <<(sum)<<"mm  " ;
				std::cout <<"\n";
				
				serial_port.Close();
				open_serial_port(serial_port);
				
				if (sum > stop_at_distance)
					break;
				
			}
}

