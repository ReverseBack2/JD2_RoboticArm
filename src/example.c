#include <stdio.h>
// #include "serial.h"
#include "gcode.h"
#include <time.h>

static float num = 99;
static int serial_port;
static char read_buf [256];
static int* len;
static int destination [2];
static int position [2];
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
		for (int i = 0; i < 181; i+=5) {
			destination[0] = i;
			G0(destination, position, config, serial_port);

			delay(200);
		}
	}
	return 0;
}