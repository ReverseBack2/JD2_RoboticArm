#include "gcode.h"
#include <stdio.h>

// Serial Setup PassThrough
int GCS_setup(){
	return Serial_setup();
}

// Rapid Travel
void G0(int* dest, int* pos, int* config, int ser_port){
	// Log command
	unsigned char msg[] = {'f','a','s','t','\n'};
  	ser_msg(msg, 5, ser_port);

  	//Move Quickly
  	unsigned char buf [3];
  	intToChar(dest[0], buf);
  	unsigned char msg1[] = {'a',buf[0],buf[1],buf[2],'\n'};
  	msg1[5] = '\0';
  	ser_msg(msg1, 5, ser_port);
  	printf("%s",msg1);

  	intToChar(dest[1], buf);
  	unsigned char msg2[] = {'b',buf[0],buf[1],buf[2],'\n'};
  	msg2[5] = '\0';
  	ser_msg(msg2, 5, ser_port);
  	printf("%s",msg2);
}

// Straight Line
void G1(int* dest, int* pos, int* config, int ser_port){
	// Log command
	unsigned char msg[] = {'l','i','n','e','\n'};
  	ser_msg(msg, 5, ser_port);

  	//Move in Line
}

// Absolute Mode
void G90(int* config, int ser_port){
	config[0] = 0;
	// Log command
	unsigned char msg[] = {'c','\n'};
  	ser_msg(msg, 2, ser_port);
}

// Relative Mode
void G91(int* config, int ser_port){
	config[0] = 1;
	// Log command
	unsigned char msg[] = {'d','\n'};
  	ser_msg(msg, 2, ser_port);
}

// Inch Mode (in)
void G20(int* config, int ser_port){
	config[1] = 0;
	// Log command
	unsigned char msg[] = {'e','\n'};
  	ser_msg(msg, 2, ser_port);
}

// Metric Mode (mm)
void G21(int* config, int ser_port){
	config[1] = 1;
	// Log command
	unsigned char msg[] = {'f','\n'};
  	ser_msg(msg, 2, ser_port);
}

// End
void M2(int* pos, int ser_port){
	// Log command
	unsigned char msg[] = {'g','\n'};
  	ser_msg(msg, 2, ser_port);

  	// Move to End

}

// Tool Change (Pause)
void M6(int ser_port){
	// Log command
	unsigned char msg[] = {'h','\n'};
  	ser_msg(msg, 2, ser_port);

  	// Pause
}

static void intToChar(int num, char* buf){
    unsigned char buf2 [3];
    sprintf(buf2, "%d",num);

    if (num/100) {
        // printf("1\n");
        buf[0] = buf2[0];
	    buf[1] = buf2[1];
	    buf[2] = buf2[2];
    }else if (num/10) {
        // printf("2\n");
        buf[0] = '0';
        buf[1] = buf2[0];
        buf[2] = buf2[1];
    }else{
        // printf("3\n");
        buf[0] = '0';
        buf[1] = '0';
        buf[2] = buf2[0];
    }

    buf[3] = '\0';	
}