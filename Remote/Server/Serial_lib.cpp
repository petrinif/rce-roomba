#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <SerialStream.h>



using namespace LibSerial;

// Packet numbers which are sent to the Roomba
const char         Start = 128;
const char         FullMode = 132;
const char         Drive = 137;                // 4:   [Vel. Hi] [Vel Low] [Rad. Hi] [Rad. Low]
const char         LED_Color = 139;
const char         SensorStream = 148;         // x+1: [# of packets requested] IDs of requested packets to stream
const char	   Distance = 19; 
int 		   Distance_trav = 0;
int 		   first = 1;

/* This function opens a serial stream to a port located in the
usb device ttyUSB0, which is the serial port where the roomba
should be connected */

int open_serial_port(SerialStream &serial_port){
	
	
    // This function opens a serial port with the below defined parameters:
	
    const char* const SERIAL_PORT_DEVICE = "/dev/ttyUSB0" ;
	
	
	
	
    // Open the serial port.
    serial_port.Open(SERIAL_PORT_DEVICE,std::ios_base::in | std::ios_base::out ) ;

    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not open serial port "
                  << SERIAL_PORT_DEVICE
                  << std::endl ;
	return 1;
    }
    // Set the baud rate of the serial port.
    serial_port.SetBaudRate( SerialStreamBuf::BAUD_57600) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the baud rate." << std::endl ;
	return 1;
    }
    // Set the number of data bits.
    serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the character size." << std::endl ;
	return 1;
    }
    // Disable parity.
    serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not disable the parity." << std::endl ;
	return 1;
    }
    // Set the number of stop bits.
    serial_port.SetNumOfStopBits( 1 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the number of stop bits."
                  << std::endl ;
	return 1;
    }
    // Turn on hardware flow control.
    serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not use hardware flow control."
                  << std::endl ;
	return 1;
    }
	return 0;
    
}



// This function returns the distance traveled by the roomba

void get_distance(SerialStream &serial_port){
			char data_stream[200] = {0};
			const int distance_size = 11;
			char request[distance_size] = {0};
			char next_byte=0;
			int i;
			short distance_traveled = 0;
			char distance_low_byte = 0;
			char distance_high_byte = 0;

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

			
			
			for (i=0; i<20; i++) {
				serial_port.read( &next_byte, 1 );
				data_stream[i] = next_byte;
			std::cout << short(next_byte) <<  std::endl;
				
				if(i>4 && next_byte ==19)
					break;
			}

			request[0] = 150;
			request[1] = 0;

			sleep(0.2);
			//Sending request to the serial port in order to interrupt stream
			for (i=0; i<2;i++){
				serial_port.write( &request[i], 1 ) ;
				request[i] = 0;
			}

			
			

			distance_low_byte = data_stream[3];
			distance_high_byte = data_stream[2];
			std::cout << short(distance_low_byte) <<  std::endl;
			std::cout << short(distance_high_byte) <<  std::endl;
		
			distance_traveled = (distance_high_byte << 8);
			distance_traveled = (distance_traveled | distance_low_byte);
			std::cout << distance_traveled <<  std::endl;

			if (first == 1){
				distance_traveled = 0;
				first = 0;
			}
			
			// calculating traveled distance: [mm]
			Distance_trav = (Distance_trav + distance_traveled);
			
			serial_port.Close();
			open_serial_port(serial_port);
			
}

