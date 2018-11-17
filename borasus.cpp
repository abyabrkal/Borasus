/******************************************
 *  BORASUS 
 *   - A Text Editor in C++
 ******************************************/

//#include<iostream>
#include <termios.h>
#include <unistd.h>

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
	struct termios raw;

	tcgetattr(STDIN_FILENO, &raw);

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