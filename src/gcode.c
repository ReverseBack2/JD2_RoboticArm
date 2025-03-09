#include "gcode.h"
#include <math.h>
// #include

#define STEP_COUNT 100
#define STEP_WAIT 320000

#define A 8.0625
#define B 6.125
#define THETA_SCALE 1.005
#define THETA_OFFEST 0
#define PHI_SCALE 1.02
#define PHI_OFFSET 135
#define PI 3.1415926535 



// Serial Setup PassThrough
int GCS_setup(){
	return Serial_setup();
}

// Rapid Travel
void G0(float* dest, float* pos, int* config, int ser_port){
	// Log command
	unsigned char msg[] = {'f','a','s','t','\n'};
  	ser_msg(msg, 5, ser_port);
  	move(dest, pos, config, ser_port);
}

// Straight Line
void G1(float* dest, float* pos, int* config, int ser_port){
	// Log command
	unsigned char msg[] = {'l','i','n','e','\n'};
  	ser_msg(msg, 5, ser_port);

  	//calc line movements
  	posChange(dest, pos, config);
  	float xT = dest[0] -  pos[0];
  	float yT = dest[1] -  pos[1];

  	float dx = xT/((float)STEP_COUNT);
  	float dy = yT/((float)STEP_COUNT);

  	float ds = sqrtf(dx*dx+dy*dy);

  	//Move in Line
  	int tempConfig [2] = {1,0};
  	float tempDest [2] = {dx,dy};

  	for (int i = 0; i < STEP_COUNT; ++i)
  	{
		move(tempDest, pos, tempConfig, ser_port);
		tempDest[0] = dx;
		tempDest[1] = dy;
		delay((int)(ds * ((float)STEP_WAIT)));
  	}

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

// Metric Mode (cm)
void G21(int* config, int ser_port){
	config[1] = 1;
	// Log command
	unsigned char msg[] = {'f','\n'};
  	ser_msg(msg, 2, ser_port);
}

// End
void M2(float* pos, int ser_port){
	// Log command
	unsigned char msg[] = {'g','\n'};
  	ser_msg(msg, 2, ser_port);

  	// Move to End
  	float dest [2] = {1.0,2.0};
  	int config [2] = {0,0};

  	G0(dest, pos, config, ser_port);

}

// Tool Change (Pause)
void M6(int ser_port){
	// Log command
	unsigned char msg[] = {'h','\n'};
  	ser_msg(msg, 2, ser_port);

  	// Pause
}

// Reset Driver
void RD(int ser_port){
	// Log command
	unsigned char msg[] = {'z','\n'};
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

static void posChange(float* dest, float* pos, int* config) {
	printf("\n\nStart posChange\n");
	
	// Metric Conversion
	if (config[1] == 1) {
		dest[0] = dest[0] /2.54;
		dest[1] = dest[1] /2.54;
	}

	//Relative conversion
	if (config[0] == 1) {
		printf("%f + %f = ",dest[0],pos[0]);
		dest[0] = dest[0] + pos[0];
		printf("%f\n",dest[0]);
		printf("%f + %f = ",dest[1],pos[1]);
		dest[1] = dest[1] + pos[1];
		printf("%f\n",dest[1]);
	}

	// Page Clamping
	dest[0] = clampf(dest[0],0.01,8.49);
	dest[1] = clampf(dest[1],0.01,10.99);

	printf("destination: %f, %f \n",dest[0],dest[1]);
	printf("position: %f, %f \n",pos[0],pos[1]);

	printf("End posChange\n\n\n");
}

static void angChange(float *dest, int *angle) {

	printf("\n\nStart angChange\n");
	float x1 = dest[0]+3;
	float y1 = dest[1]-5.5;
	float x2 = x1*x1;
	float y2 = y1*y1;

	printf("%f, %f \n",x1,y1);

	float s2 = x2+y2;
	
	float theta = PI - acosf(clampf((A*A+B*B-s2)/(2*A*B),-1.0,1.0));

	// float theta = 50;

	float phi = atanf(y1/x1) - asinf(clampf(B*sinf(PI-theta)/sqrtf(s2),-1.0,1.0));
	// float phi = 50;

	// float test1 = B*sinf(PI-theta)/sqrtf(s2);
	// float test2 = asinf(clampf(B*sinf(PI-theta)/sqrtf(s2),-1.0,1.0));
	// float test3 = atanf(y1/x1);

	// printf("%f,%f,%f\n",test1,test2, test3);

	printf("phi: %f, theta: %f\n",phi,theta);

	//radian conversion
	phi = phi * 180/PI;
	theta = theta * 180/PI;


	theta = (theta + THETA_OFFEST) * THETA_SCALE;
	phi = (phi + PHI_OFFSET) * PHI_SCALE;
	phi = powf(phi, 0.985);

	printf("phi: %f, theta: %f\n",phi,theta);

	angle[1] = (int)phi;
	angle[0] = (int)theta;

	printf("End angChange\n\n\n");
}

static void move(float* dest, float* pos, int* config, int ser_port) {
	printf("\n\nStart Move\n");
	
	// Go from cm to inch
	// Change XY from relative to absolute
  	printf("1: %f, %f \n",dest[0],dest[1]);
  	posChange(dest, pos, config);
  	printf("2: %f, %f \n",dest[0],dest[1]);

  	//go from XY to Angles
	int angle [2] = {45,45};
  	angChange(dest, angle);

  	angle[0] = clampi(angle[0], 0, 175);
  	angle[1] = clampi(angle[1], 0, 175);


  	//Move Quickly
  	unsigned char buf [3];
  	intToChar(angle[0], buf);
  	unsigned char msg1[] = {'a',buf[0],buf[1],buf[2],'\n'};
  	msg1[5] = '\0';
  	ser_msg(msg1, 5, ser_port);
  	printf("%s",msg1);

  	intToChar(angle[1], buf);
  	unsigned char msg2[] = {'b',buf[0],buf[1],buf[2],'\n'};
  	msg2[5] = '\0';
  	ser_msg(msg2, 5, ser_port);
  	printf("%s",msg2);

  	//Update Position
  	pos[0] = dest[0];
  	pos[1] = dest[1];

  	printf("End Move\n\n\n");
}

static float clampf(float input, float low, float high) {
	if( input < low ) {
		return low;
	}else if( input > high ) {
		return high;
	}
	return input;
}

static int clampi(int input, int low, int high) {
	if( input < low ) {
		return low;
	}else if( input > high ) {
		return high;
	}
	return input;
}

static void delay(int us) {
	clock_t start_time = clock();
	while (clock() < start_time + us){;}
}