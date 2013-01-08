
#include "Timeout.h"
#include "Serial_lib.h"
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <SerialStream.h>
#include <time.h>

using namespace LibSerial ;

clock_t begin;

/*This function checks every 0.2 seconds that a message has arrived
in the previous 5 seconds. if this is not the case it sends a stop signal
to the Roomba, in order to avoid loss of control.*/

void* check_message_timeout(void *ptr){
	int timeout = 5;

	double elapsed_secs = 0;
	int i = 0;
	const int BUFFER_SIZE = 11;
	char buffer[BUFFER_SIZE] = {0};
	while (true){
		clock_t end = clock();
		sleep(0.2);
	  	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		if (elapsed_secs > timeout){
			//Opening Serial Port
			SerialStream serial_port;

			open_serial_port(serial_port);

			// Composing the buffer which will be sent
			buffer[0]=  128;
			buffer[1]=  132;
			buffer[2] = 139;
			buffer[3] = 0;
			buffer[4] = 255;
			buffer[5] = 255;
			buffer[6] = 137;
			buffer[7] = 0;
			buffer[8] = 0;
			buffer[9] = 0;
			buffer[10] = 0;
	
			sleep(0.1);

			// Writing data on the serial port
			for(i=0;i<BUFFER_SIZE;i++)
			{
				serial_port.write( &buffer[i], 1 ) ;
			}
			serial_port.Close();
			begin = clock();
		}

	}

	return 0;
}
