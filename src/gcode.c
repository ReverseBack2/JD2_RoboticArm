#include "gcode.h"

// Rapid Travel
void G0(int* dest, int* pos, int* config, int ser_port){

}

// Straight Line
void G1(int* dest, int* pos, int* config, int ser_port){

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