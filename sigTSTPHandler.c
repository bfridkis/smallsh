/************************************************************************************
** Program name: sigTSTPHandler.c
** Class name: CS344
** Author: Ben Fridkis
** Date: 11/01/2018
** Description: This function registers a signal handler for the parent process
**				such that the SIGTSTP signal will "toggle" the global variable 
**				bgIGN. This variable serves as a flag to enable/disable background
**				process execution via the SIGTSTP command. If the value of bgIGN is
**				-1, background processing is enabled, while a value of 1 indicates
**				background processing is disabled.
*************************************************************************************/

#include "sigTSTPHandler.h"

static void toggle_bgIGN(int signal){
	bgIGN *= -1;
	if(bgIGN == 1){
		char* message = "Entering foreground-only mode (& is now ignored)\n";
		write(STDOUT_FILENO, message, 49); 
	}
	else{
		char* message = "Exiting foreground-only mode\n";
		write(STDOUT_FILENO, message, 29);
	}
	bgToggled = 1;
}

void sigTSTPHandler(){
	
	struct sigaction signal_action = {{0}};
	signal_action.sa_handler = toggle_bgIGN;
	sigfillset(&signal_action.sa_mask);
	sigaction(SIGTSTP, &signal_action, 0);
}

/*************************************************************************************************************************
 * References *
 * https://stackoverflow.com/questions/13746033/how-to-repair-warning-missing-braces-around-initializer/13758286#13758286
 * https://stackoverflow.com/questions/6792930/how-do-i-share-a-global-variable-between-c-files
 * https://sourceware.org/gdb/onlinedocs/gdb/Signals.html
**************************************************************************************************************************/
