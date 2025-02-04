CC=gcc
exe_file=program

$(exe_file): main.c serial.o gcode.o
	$(CC) `pkg-config --cflags gtk4` main.c serial.o gcode.o -o $(exe_file) `pkg-config --libs gtk4`
serial.o: serial.c serial.h
	$(CC) -c serial.c
gcode.o: gcode.c gcode.h
	$(CC) -c gcode.c

clean:
	rm -f *.o $(exe_file)
