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

// Rapid Travel
void G0(int* dest, int* pos, int* config);

// Straight Line
void G1(int* dest, int* pos, int* config);

// Absolute Mode
void G90(int* config);

// Relative Mode
void G91(int* config);

// Inch Mode (in)
void G20(int* config);

// Metric Mode (mm)
void G21(int* config);

// End
void M2(int* pos);

// Tool Change (Pause)
void M6();

