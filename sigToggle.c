/****************************************************************************
** Program name: sigToggle.c
** Class name: CS344
** Author: Ben Fridkis
** Date: 11/01/2018
** Description: This function toggles a signal's action on or off (or more 
**				specifically, between default and ignore, based on the isOn 
**				parameter.
****************************************************************************/

#include <signal.h>
#include "sigToggle.h"

void sigToggle(int signal, int isOn){
	
	struct sigaction signal_action = {{0}};
	if(isOn){
		signal_action.sa_handler = SIG_DFL;
	}
	else{
		signal_action.sa_handler = SIG_IGN;
	}
	sigaction(signal, &signal_action, 0);
}

/*************************************************************************************************************************
 * References *
 * https://stackoverflow.com/questions/13746033/how-to-repair-warning-missing-braces-around-initializer/13758286#13758286
**************************************************************************************************************************/
