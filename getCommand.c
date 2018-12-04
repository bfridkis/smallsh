/****************************************************************************
** Program name: getCommand.c
** Class name: CS344
** Author: Ben Fridkis
** Date: 11/01/2018
** Description: Function to get user command for smallsh program. Reprompts
**				user for command if input exceeds 2048 characters or 512
**				arguments. Must be passed an argument count variable 
**				(argCount) that has been initialized to 1 (as there must be
**				at least 1 argument for any given command.)
****************************************************************************/

#include "getCommand.h"

char* getCommand(int* argCount){
	
	char* command;
	while(1){

		command = malloc(2050 * sizeof(char));

		//Set bgToggled to 0, in case user toggled background availability
		//during the execution of a foreground process. This flag is only
		//used here, so a user can be reprompted if necessary after 
		//toggling the background command availability. (A user only needs to
		//be reprompted if this toggling occurred during a pause for command
		//input.)
		bgToggled = 0;

		//Get the users command. Allows for one character beyond max
		//so an input exceeding the max allowable characters can be
		//detected. An error message is printed below if this occurs.
		//(fgets will read in the newline character, which is stripped
		//below.)
		fgets(command, 2051, stdin);
		
		//Need to restart loop if background command availability was
		//toggled while execution was paused at an input prompt.
		if(bgToggled){
			bgToggled = 0;
			printf(": ");
			fflush(stdout);
			free(command);
			continue;
		}
		//Prints error message if command exceeds 2048 characters
		if(strlen(command) > 2048){
			printf("\nCommand must not exceed 2048 characters.\n");
			fflush(stdout);
			free(command);
			*argCount = 1;
			printf(": ");
			fflush(stdout);
			continue;
		}
		
		//Strip leading whitespace if applicable, and return
		//NULL if input is empty or leads with the '#' character.
		while(command[0] == ' ' || command[0] == '\t'){
			command++;
		}
		if(command[0] == '\n' || command[0] == '#'){ 
			//Free memory
			free(command);
			return NULL;
		}

		//Count the number of arguments (i.e. number of spaces) and
		//stop at redirection operators if applicable.
		for(int i = 0; i < strlen(command) - 1 && *argCount < 513; i++){
			if(command[i] == ' '){
				
				//The following loop and subsequent decrement statement
				//prevent errors if the user enters additional whitespace
				//between commands.
				while(command[++i] == ' ' || command[i] == '\t'){}
				i--;
				
				//This check ensures argCount is not incremented for trailing
				//spaces in the user input.
				if(command[i + 1] != '\n'){
					(*argCount)++;
				}
			}
			if(command[i] == '>' || command[i] == '<'){
				(*argCount)--;
				break;
			}
		}
		//argCount cannot exceed 512. Print error message and restart
		//loop if argCount in excess of 512.
		if(*argCount > 512){
			printf("\nMax argument count = 512.\n");
			fflush(stdout);
			free(command);
			*argCount = 1;
			printf(": ");
			fflush(stdout);
			continue;
		}
		break;
	}

	//Strip trailing newline from command
	command[strlen(command) - 1] = '\0';

	//An extra argument is counted if the user indicates a background command
	//with '&' preceeded by a space, but only if neither redirection operator
	//is used. Decrement argCount accordingly, if necessary.
	if(command[strlen(command) - 1] == '&' && command[strlen(command) - 2] == ' '
		&& strstr(command, "<") == NULL && strstr(command, ">") == NULL){
		(*argCount)--;
	}

	return command;
}

/*****************************************************************************************
 *  * References *
 *  https://sourceware.org/gdb/onlinedocs/gdb/Signals.html
 *  https://linux.die.net/man/3/fgets
*****************************************************************************************/
