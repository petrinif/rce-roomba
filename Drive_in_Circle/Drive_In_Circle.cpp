#include <SerialStream.h>
#include <iostream>
#include <fstream>
#include <cstdlib>


int
main(int argc, char* argv[])
{

	if (argc < 3 ){
		std::cout << "Too few arguments!" << std::endl;
		std::cout << "Requested: Speed [mm/s] and Radius [mm]"<< std::endl;
		return 1;
	}



	//Speed [mm/s]

	int speed = atoi(argv[1]);
	int speed_low_byte = speed & 0xFF;
	int speed_high_byte = (speed >> 8) & 0xFF;



	//Radius [mm]


	int radius = atoi(argv[2]);
	int radius_low_byte = radius & 0xFF;
	int radius_high_byte = (radius >> 8) & 0xFF;


	const int size_message = 50;


	int message[size_message]= 	{128,132,139,2,0,0,152,23,158,
								17,139,8,0,0,137,speed_high_byte,speed_low_byte,
								radius_high_byte,radius_low_byte,158,
								16,139,2,0,0,137,0,0,0,0,153,153};



	char next_byte;
	int BUFFER_SIZE = 50;
	int i;
	int *buffer = new int[BUFFER_SIZE];


	for(i=0;i<BUFFER_SIZE;i++){
		buffer[i] = 0;
	}


	for(i=0;i<size_message;i++){
		buffer[i] = message[i];
	}


    // Open the serial port.
    //


    const char* const SERIAL_PORT_DEVICE = "/dev/ttyUSB0" ;
    using namespace LibSerial ;
    SerialStream serial_port ;
    serial_port.Open(SERIAL_PORT_DEVICE,std::ios_base::in | std::ios_base::out ) ;

    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not open serial port "
                  << SERIAL_PORT_DEVICE
                  << std::endl ;
        exit(1) ;
    }
    //
    // Set the baud rate of the serial port.
    //
    serial_port.SetBaudRate( SerialStreamBuf::BAUD_57600) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the baud rate." << std::endl ;
        exit(1) ;
    }
    //
    // Set the number of data bits.
    //
    serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the character size." << std::endl ;
        exit(1) ;
    }
    //
    // Disable parity.
    //
    serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not disable the parity." << std::endl ;
        exit(1) ;
    }
    //
    // Set the number of stop bits.
    //
    serial_port.SetNumOfStopBits( 1 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the number of stop bits."
                  << std::endl ;
        exit(1) ;
    }
    //
    // Turn on hardware flow control.
    //
    serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not use hardware flow control."
                  << std::endl ;
        exit(1) ;
    }

    for(i=0;i<BUFFER_SIZE;i++)
    {
    	next_byte = buffer[i];
        serial_port.write( &next_byte, 1 ) ;
    }


    serial_port.Close();

    std::cout << "done" << std::endl;
    return 0;


}
