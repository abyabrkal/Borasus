/******************************************
 *  BORASUS 
 *   - A Text Editor in C++
 ******************************************/

#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// Original State of terminal is saved here
struct termios orig_termios;

/***************************************
 *   disableRawMode -  TURN ON ECHOING BACK
 *
 * Turn on echo mode back before program exits
 	Initial saved state of terminal is restored back
 ***************************************/
void disableRawMode () {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
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

	tcgetattr(STDIN_FILENO, &orig_termios);

	//register the disable function to be called automatically
	// when program exits from main(), or by exit()
	atexit(disableRawMode);

	struct termios raw = orig_termios;

	// c_lflag field stands for “local flags
	raw.c_lflag &= ~(ECHO);

	// TCSAFLUSH argument specifies when to apply the change
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}



int main() {
	//turn off echo in Raw mode
	enableRawMode();

	char c;
	//read one byte from std input into c until EOF(zero) or meets 'q'
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
		;
  	return 0;
}