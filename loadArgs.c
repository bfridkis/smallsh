/******************************************************************************
** Program name: loadArgs.c
** Class name: CS344
** Author: Ben Fridkis
** Date: 10/29/2018
** Description: Function to convert the user's command into an array of 
**				arguments. Performs variable expansion on '$$', converting
**				this string into the current PID wherever it is encountered.
******************************************************************************/

#include "loadArgs.h"

char** loadArgs(char* command, Array* exArgs, int argCount){
	//Use strktok_r to save an array of pointers
	//to each command argument. (Note the maximum size
	//of this array is 513, because max number of 
	//arguments is 512, and a final pointer must be
	//set to NULL for the exec call.
	char** args = calloc(513, sizeof(char*));
	char* tok = command, *saveptr, *pidExpCh;
	for(int i = 0, j = 0, k = 1; i < argCount && tok != NULL; i++, j = 0, k = 1){
		//First call to strtok_r should have command string to tokenize 
		//as arg1. Subsequent calls should have arg1 = NULL.
		//See https://stackoverflow.com/questions/15961253/c-correct-usage-of-strtok-r
		//& https://linux.die.net/man/3/strtok_r
		if(i == 0){
			args[i] = tok = strtok_r(command, " ", &saveptr);
		}
		else{
			args[i] = tok = strtok_r(NULL, " ", &saveptr);
		}
		//The remainder of this function/outer loop performs variable
		//expansion on $$, substituting the current PID for the string "$$"
		if((pidExpCh = strstr(args[i], "$$")) != NULL){	
			args[i] = NULL;
			char pid[6];
			snprintf(pid, sizeof(pid), "%d", (int)getpid());
			int tokSize = strlen(tok) + 1;
			while((pidExpCh = strstr(tok, "$$")) != NULL){
				args[i] = realloc(args[i], sizeof(char) * 
							(tokSize + ((strlen(pid) - 2) * k++)));	
				while(tok != pidExpCh){
					args[i][j++] = *tok;
					tok++;
				}
				args[i][j] = '\0';
				strcat(args[i], pid);
				tok += 2;
				j += strlen(pid);
			}
			while(*tok != '\0'){
				args[i][j++] = *tok;
				tok++;
			}
			args[i][j] = '\0';
			insertArray(exArgs, i);
		}	
	}

	return args;
}
