#include "serial.h"

int Serial_setup() {
	// Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
  int serial_port = open("/dev/ttyUSB0", O_RDWR);


  // Create new termios struct, we call it 'tty' for convention
  struct termios tty;

  // Read in existing settings, and handle any error
  if(tcgetattr(serial_port, &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return 1;
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 120;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 0;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B115200);
  cfsetospeed(&tty, B115200);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return 1;
  }

  printf("%d\n\n",serial_port);	
}

void ser_msg(char* msg, int len, int ser_port) {
	unsigned char write_buf [len];
	for (int i = 0; i < len; ++i)
	{
		write_buf[i] = msg[i];
	}
	write(ser_port, write_buf, sizeof(write_buf));	
}

void ser_read(char* input, int* len, int ser_port) {

	// Buffer message so reading doesn't crash program, then waiting 2000us to allows buffer message reply
	char temp [2] = {'*','\n'};
	ser_msg(temp, 2, ser_port);
	clock_t start_time = clock();
    while (clock() < start_time + 2000){;}

	// initialize read_bif and num_bytes
	char read_buf [256];
	for (int i = 0; i < 256; ++i)
	{
		read_buf[i] = '\0';
	}
	int num_bytes = 0;

	// Read bytes. The behaviour of read() (e.g. does it block?,
	// how long does it block for?) depends on the configuration
	// settings above, specifically VMIN and VTIME

	while(1) {
		num_bytes += read(ser_port, &read_buf[num_bytes], sizeof(read_buf));
		if(read_buf[num_bytes-1] == '\n'){
		  break;
		}
		if(num_bytes >= 224){printf("overflow\n");break;}
	}

	//Remove padding
	for (int i = 0; i < 2; ++i)
	{
		read_buf[num_bytes-2-i] = '\0';
	}

	// n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
	if (num_bytes < 0) {
	  printf("Error reading: %s", strerror(errno));
	  return;
	}

	// Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
	// print it to the screen like this!)
	printf("%s",read_buf);

	// *len = num_bytes;
	// for (int i = 0; i < num_bytes; ++i)
	// {
	// 	input[i] = read_buf[i];
	// }

	// printf("read\n");
}