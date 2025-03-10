# Junior Design RoboticArm
### JD2 robotic arm code repository, including test/learning code

## Requirements / Setup
Firs things first, this was developed on Arch Linux.
You'll need to be on a linux operating system, or you will have to largely modify the underlying serial.c code due to how linux machines handle serial ports.
Once you're on a linux machine, you'll likely have to find what file descriptor path the ESP32 shows up as on your machine.
Once you have this path, you'll need to replace the default file path in the Serial_setup() function inside serial.c.


If you wish to use the included GUI, you will likely have to install the GTK-4.0 library if it's not installed already, since you're using a linux machine you can figure out how to do that.

To compile the GUI code, run the makefile from within the src directory. I recomend running the compiled program from the terminal since there will be alot of useful information printed to the terminal, including responses from the ESP32.
Other than that the GUI should be pretty self explanatory. The first line is XY for direct movement. They second line is XY for linear movement.
![GUI](https://github.com/user-attachments/assets/315b5d41-8a39-4c6b-a3f0-47f0ff4fc610)


If you wish to use the gcode implementation as library instead, then just make sure to link the gcode.h and serial.h files.
