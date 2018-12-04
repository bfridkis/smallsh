/*****************************************************************************
** Program name: smallsh.c
** Class name: CS344
** Author: Ben Fridkis
** Date: 11/5/2018
** Description: smallsh.c is a linux shell emulator program, with the 
**				following 3 built-in commands:
**					1. status - prints the exit status or terminating
**								signal number of the most recently 
**								completed foreground process.
**					2. cd   -   Changes the working directory of the process.
**								(Note this does NOT change the directory of
**								 the outer shell which calls smallsh.)
**					3. exit  -  Cleans up any pending child (background)
**								processes and terminates smallsh.
**			
**				All other linux commands are executed using the fork and 
**				execvp standard library functions. Processes can be specified
**				in the "background" (i.e. execution does not impede the user's
**				ability to enter a subsequent command) if a trailing '&' is
**				provided with the command. Processes executed using this 
**				specification will print their corresponding PIDs immediately
**				preceding their initiation and will print their termination
**				status or signal number immediately preceding the next command
**				input after they have terminated.
**
**				smallsh foreground processes (i.e. those initiated by commands
**				without a trailing '&', or when background processing is disabled
**				as described below) will terminate upon receiving a SIGINT
**				(CTRL-C) signal. All other smallsh processes (i.e. the outer
**				'parent' process and all background processes) will ignore 
**				SIGINT.
**
**				SIGTSTP (CTRL-Z) toggles the availability of background 
**				processing (i.e. when background processing is disabled, the
**				trailing '&' in the command is ignored). Whenever this mode
**				is toggled, a message will print to this effect. SIGTSPT has
**				no other effect on smallsh.
**
**				'$$' expands to the current process's ID. No other variable
**				expansions are performed.
**
**						The command syntax is as follows:
**			
**			command [arg1 arg2 ...] [< input_file] [> output_file] [&]
**
******************************************************************************/

#include "dynarr.h"
#include "loadArgs.h"
#include "getCommand.h"
#include "executeCMD.h"
#include "sigTSTPHandler.h"

//This global variable is a flag to indicate the availability of
//background processing. The user can toggle this value with the SIGTSTP
//signal. A value of -1 indicates background processing is enabled 
//(i.e. commands for background processing are not ignored), while
//a value of 1 indicates background processing is disabled (i.e commands
//for background processing are ignored).
int bgIGN = -1;

//The flag indicates a toggling occurred, which is needed to re-prompt
//the user if the signal was received while the user was at the prompt
//(i.e. the signal was not received during the execution of a foreground
//process).
int bgToggled = 0;

int main(int argc, char *argv[]){
	
	//This dynamic array will be used to hold PIDs for background child
	//processes. (These array elements will be reset to values of 0 when
	//cleanup occurs and reused as they become available accordingly.
	Array* CPIDs = malloc(sizeof(Array));
	initArray(CPIDs, 10);

	//Integer to hold exit status of either most recently terminated foreground
	//process, or a value of 1 if exec call fails for either foreground or 
	//background process.
	int status = 0;

	//The smallsh "outer" shell is to ignore SIGINT signals. This is registered
	//below. (See sigToggle wrapper function for additional details.)
	sigToggle(SIGINT, 0);
	
	//Establish signal handler for SIGTSTP, which allows the user to toggle 
	//background processing availability (i.e. the ability to process commands
	//in the background using a trailing '&' character) via this signal. This
	//means that SIGTSTP will NOT utilize its default behavior for the parent
	//shell (i.e. will not stop the process).
	sigTSTPHandler();

	while(1){

		//Block SIGSTPT signal while printing termination/exit
		//statuses for background processes.
		sigset_t sigTSTP, oldSet;
		sigemptyset(&sigTSTP);
		sigaddset(&sigTSTP, SIGTSTP);
		sigprocmask(SIG_BLOCK, &sigTSTP, &oldSet);

		//Check for any terminated background processes, and print related
		//message(s)if applicable.
		for(int i = 0; i < CPIDs->used; i++){
			if(CPIDs->array[i] != 0){
				int bgStatus;
				if(waitpid(CPIDs->array[i], &bgStatus, WNOHANG)){
					printf("background pid %d is done: ", CPIDs->array[i]);
					fflush(stdout);
					if(WIFEXITED(bgStatus) != 0){
						printf("exit value %d\n", WEXITSTATUS(bgStatus));
						fflush(stdout);
					}
					else{
						printf("terminated by signal %d\n", WTERMSIG(bgStatus));
						fflush(stdout);
					}
					//Reset array element to 0 for "cleaned up" process
					CPIDs->array[i] = 0;
				}
			}
		}

		//Unblock SIGSTPT signal
		sigprocmask(SIG_SETMASK, &oldSet, NULL);

		printf(": ");
		fflush(stdout);
		
		//Get command using getCommand(), which updates
		//argCount as appropriate.
		int argCount = 1;
		char* command;
		if((command = getCommand(&argCount)) == NULL){
			continue;
		}
		
		//Before 'tokenizing' user command string (in call to
		//loadArgs below), determine if process is to be run in
		//foreground or background, by identifying an '&' 
		//(or lack thereof) at the end of the command.
		int isBG = (command[strlen(command) - 1] == '&');
		
		//Strip off trailing '&' if a background command has been
		//specified.
		if(isBG){
			command[strlen(command) - 1] = '\0';
		}

		//This dynamic array will be used to track arguments that are expanded
		//(due to the presence of "$$"), for memory management purposes.
		//It is passed to loadArgs below (see loadArgs.c for more details).
		Array* exArgs = malloc(sizeof(Array));
		initArray(exArgs, 5);
		
		//The function loadArgs will tokenize the command string.
		//Before tokenizing, save pointers to redirection operators
		//if they are provided. (These will be based to the executeCMD
		//function below).
		char* iRedrct = strstr(command, "<");
		char* oRedrct = strstr(command, ">");

		//Load arguments into array. (This function is passed
		//the exArgs array to track which arguments are expanded
		//due to the presence of '$$', so memory can be freed for
		//such arguments below.)
		char** args = loadArgs(command, exArgs, argCount);

		//If the user enters the "exit" command, kill all child processes,
		//clean-up all child processed, and exit.
		if(strcmp(args[0], "exit") == 0 && args[1] == NULL){
			int chldProcExitStat;
			for(int i = 1; i < CPIDs->used; i++){
				if(CPIDs->array[i] != 0){
					kill(CPIDs->array[i], SIGKILL);	
					waitpid(CPIDs->array[i], &chldProcExitStat, 0);
				}
			}
			//Free memory
			for(int i = 0; i < exArgs->used; i++){
				free(args[exArgs->array[i]]);
			}
			freeArray(exArgs);
			free(exArgs);
			free(args);
			free(command);
			freeArray(CPIDs);
			free(CPIDs);

			return 0;
		}

		//If the user enters the "status" command, print out the exit
		//status of the most recently terminated foreground process,
		//or 0 if no foreground processes have been executed.
		else if(strcmp(args[0], "status") == 0 && args[1] == NULL){
			if(WIFEXITED(status) != 0){
				printf("exit value %d\n", WEXITSTATUS(status));
				fflush(stdout);
			}
			else{
				printf("terminated by signal %d\n", WTERMSIG(status));
				fflush(stdout);
			}
			//Free memory
			for(int i = 0; i < exArgs->used; i++){
				free(args[exArgs->array[i]]);
			}
			freeArray(exArgs);
			free(exArgs);
			free(args);
			free(command);

			continue;	
		}

		//User changes directory with "cd" [path] command.
		//If [path] is not specified, change directory to home.
		if(strcmp(args[0], "cd") == 0){
			if(args[1] == NULL){
				if(chdir(getenv("HOME")) == -1){
					perror("Error Changing Directory.\n");
				}
			}
			else{
				if(chdir(args[1]) == -1){
					perror("Error Changing Directory.\n");
				}
			}
		}
		
		//If command is not a built in, execute with executeCMD.
		//(See executeCMD.c and executeCMD.h for more details.)
		else{
			executeCMD(args, argCount, CPIDs, isBG, &status, iRedrct, oRedrct);
		}				

		//Free memory
		for(int i = 0; i < exArgs->used; i++){
			free(args[exArgs->array[i]]);
		}
		freeArray(exArgs);
		free(exArgs);
		free(args);
		free(command);
	}	
}

/*****************************************************************************************************************************
 * References *
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/kill.html
 * https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
 * https://stackoverflow.com/questions/20904175/do-i-need-to-free-the-strtok-resulting-string
 * https://stackoverflow.com/questions/21097253/how-does-the-strtok-function-in-c-work
 * https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
 * https://stackoverflow.com/questions/12970439/gcc-error-undefined-reference-to-itoa
 * https://linux.die.net/man/3/snprintf
 * https://linux.die.net/man/3/execvp
 * https://linux.die.net/man/2/waitpid
 * http://man7.org/linux/man-pages/man2/sigprocmask.2.html
 * https://stackoverflow.com/questions/6970224/providing-passing-argument-to-signal-handler
 * https://www.gnu.org/software/libc/manual/html_node/Process-Completion-Status.html
 * https://stackoverflow.com/questions/2612447/pinpointing-conditional-jump-or-move-depends-on-uninitialized-values-valgrin
*****************************************************************************************************************************/
