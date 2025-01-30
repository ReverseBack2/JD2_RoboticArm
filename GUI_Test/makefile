CC=gcc
exe_file=test

$(exe_file): test.c
	$(CC) `pkg-config --cflags gtk4` test.c -o $(exe_file) `pkg-config --libs gtk4`

clean:
	rm -f *.o $(exe_file)
