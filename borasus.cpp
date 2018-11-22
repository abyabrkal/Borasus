/******************************************
 *  BORASUS 
 *   - A Text Editor in C++
 ******************************************/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// Original State of terminal is saved here
struct termios orig_termios;

/***************************************
 *   die -  ERROR HANDLING
 *
 * Prints an error message and exits the program
 ***************************************/
void die(const char *s) {
  perror(s);
  exit(1);
}


/***************************************
 *   disableRawMode -  TURN ON ECHOING BACK
 *
 * Turn on echo mode back before program exits
 	Initial saved state of terminal is restored back
 ***************************************/
void disableRawMode () {
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
		die("tcsetattr");
}

/***************************************
 *   enableRawMode -  TURN OFF ECHOING
 *
 * Set Terminal attributes by
 	1. Using tcgetattr() to read the attributes to struct
 	2. Modify the struct manually
 	3. pass the modified struct to write new terminal attr.

 	ECHO is bitflag (0x001000) is inverted with bitwise-NOT(~)
 	and is bitwise-AND with local flag to make it zero and retain 
 	other bits.
 ***************************************/
void enableRawMode() {

	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
	//register the disable function to be called automatically
	// when program exits from main(), or by exit()
	atexit(disableRawMode);


	struct termios raw = orig_termios;
	// c_lflag field stands for “local flags 
	// ICANON turns off canonical mode and reading starts byte-by-byte
	// ISIG disbales Ctrl-C and Ctrl-Z/Y (Process terimate/Suspend signal)
	// IXON disbles Ctrl-S and Ctrl-Q (Software Control Flow)
	// IEXTEN disbales Ctrl-V
	// ICRNL fixes Ctrl-M - read as a 13(carraige return)
	// OPOST turns off all output processing
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_lflag &= ~(ICRNL | IXON);
	raw.c_lflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON |IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
  	raw.c_cc[VTIME] = 1;

	// TCSAFLUSH argument specifies when to apply the change
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}



int main() {
	//turn off echo in Raw mode
	enableRawMode();

	char c;
	//read one byte from std input into c until EOF(zero) or meets 'q'
	while (1) {
    	char c = '\0';
    	if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
		if (iscntrl(c)) {
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
		if (c == 'q') break;
	}
  	return 0;
}