BOOST_LIBS=boost_system boost_date_time boost_program_options boost_thread boost_regex

include websocketpp/common.mk

LDFLAGS := $(LDFLAGS) -lpthread /usr/local/lib/libserial.so /usr/local/lib/libjson_spirit.a


Roomba_Server: Serial_lib.cpp JSON_Handling.cpp Roomba_Server.o 
	$(CXX) $(CFLAGS) $^ -o $@ $(LDFLAGS) 

%.o: %.cpp
	$(CXX) -c $(CFLAGS) -o $@ $^





# cleanup by removing generated files
#
.PHONY:		clean
clean:
		rm -f *.o Roomba_Server
