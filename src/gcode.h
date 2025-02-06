// C library headers
#include <stdio.h>
#include <string.h>
#include <time.h>
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
void G0(int* dest, int* pos, int* config, int ser_port);

// Straight Line
void G1(int* dest, int* pos, int* config, int ser_port);

// Absolute Mode
void G90(int* config, int ser_port);

// Relative Mode
void G91(int* config, int ser_port);

// Inch Mode (in)
void G20(int* config, int ser_port);

// Metric Mode (mm)
void G21(int* config, int ser_port);

// End
void M2(int* pos, int ser_port);

// Tool Change (Pause)
void M6(int ser_port);

// Helper Functions
static void intToChar(int num, char* buf);