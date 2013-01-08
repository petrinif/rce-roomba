
#include "Serial_lib.h"

#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <SerialStream.h>
#include <time.h>

#ifndef Timeout
#define Timeout


/*This function checks every 0.2 seconds that a message has arrived
in the previous 5 seconds. if this is not the case it sends a stop signal
to the Roomba, in order to avoid loss of control.*/
void* check_message_timeout(void *ptr);

#endif
