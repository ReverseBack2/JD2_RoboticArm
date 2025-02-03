CC=gcc
exe_file=program

$(exe_file): main.c
	$(CC) `pkg-config --cflags gtk4` main.c -o $(exe_file) `pkg-config --libs gtk4`

clean:
	rm -f *.o $(exe_file)
