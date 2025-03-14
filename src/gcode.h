// C library headers
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "serial.h"

// Dest is a two int array for the coordinates of the destination
// Ignores relative/absolute mode

// Pos is a two int array for the coordinates of where the arm currently is

// Config is a two int array for the modes the arm is currently in
// [0] = 0 ==> absolute mode
// [0] = 1 ==> relative mode
// [1] = 0 ==> Inch Mode
// [1] = 1 ==> Metric Mode

// Serial Setup PassThrough
int GCS_setup();

// Rapid Travel
void G0(float* dest, float* pos, int* config, int ser_port);

// Straight Line
void G1(float* dest, float* pos, int* config, int ser_port);

// Absolute Mode
void G90(int* config, int ser_port);

// Relative Mode
void G91(int* config, int ser_port);

// Inch Mode (in)
void G20(int* config, int ser_port);

// Metric Mode (mm)
void G21(int* config, int ser_port);

// End
void M2(float* pos, int ser_port);

// Tool Change (Pause)
void M6(int ser_port);

// Reset Driver
void RD(int ser_port);

// Helper Functions
static void intToChar(int num, char* buf);
static void posChange(float* dest, float* pos, int* config);
static void angChange(float* dest, int* angle);
static void move(float* dest, float* pos, int* config, int ser_port);
static float clampf(float input, float low, float high);
static int clampi(int input, int low, int high);
static void delay(int us);