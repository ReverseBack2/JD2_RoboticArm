#include <stdio.h>
// #include "serial.h"
#include "gcode.h"
#include <time.h>


//  gcc example.c serial.o gcode.o -o example.out -lm


static float num = 99;
static int serial_port;
static char read_buf [256];
static int* len;
static float destination [2];
static float position [2];
static int config [2];
static int vars [4] = {45,45,45,45};
static int numb [8] = {0,1,2,3,4,5,6,7};

static void delay(int delay) {
	clock_t start_time = clock();
	while (clock() < start_time + delay*1000){;}
}

int main(int argc, char const *argv[])
{
	position[0] = 0;
	position[1] = 0;
	destination[0] = 0;
	destination[1] = 0;
	config[0] = 0;
	config[1] = 0;


	serial_port = GCS_setup();

	while (1) {
		destination[0] = 0.0;
		destination[1] = 4.0;
		G1(destination, position, config, serial_port);
		delay(1000);
		destination[0] = 6.0;
		destination[1] = 4.0;
		G1(destination, position, config, serial_port);
		delay(1000);
		destination[0] = 6.0;
		destination[1] = 11.0;
		G1(destination, position, config, serial_port);
		delay(1000);
		destination[0] = 0.0;
		destination[1] = 11.0;
		G1(destination, position, config, serial_port);
		delay(1000);
	}
	return 0;
}