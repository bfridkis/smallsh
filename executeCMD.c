/******************************************************************************
** Program name: executeCMD.c
** Class name: CS344
** Author: Ben Fridkis
** Date: 11/01/2018
** Description: This function establishes redirection as specified by the 
**				user, or if not specified sets stdin and stdout to /dev/null
**				for background commands. It then uses execvp to call the 
**				the user's command. PIDs of background commands are loaded
**				into the CPIDs dynamic array.
******************************************************************************/

#include "executeCMD.h"

void executeCMD(char** args, int argCount, Array* CPIDs, int isBG, int* status, 
					char* iRedrct, char* oRedrct){

	//Set up file paths for redirection if applicable.
	if(iRedrct){
		//iRedrct and oRedrct provide pointers to the redirection operators
		//if they are present. The first character of the file path will be two 
		//characters beyond this, accounting for the redirection operator itself 
		//and a space that follows. Hence, we increment this pointer by 2 below,
		//before finding the appropriate position for the null terminator at the
		//next space if applicable. However, if no space seperates the redirection
		//operator from the following filepath, the pointer is only incremented
		//one position. The same applies for oRedrct below.
		if(*(iRedrct + 1) != ' '){
			iRedrct += 1;
		}
		else{
			iRedrct += 2;
		}
		int i = 0;
		while(*(iRedrct + i) != ' ' && *(iRedrct + i) != '\0'){
			i++;
		}
		//(Note the assignment below is only needed if two redirection
		//operators are used, and only for the first of the two at that.
		//Same applies to oRedrct below.)
		*(iRedrct + i) = '\0';
	}
	if(oRedrct){
		if(*(oRedrct + 1) != ' '){
			oRedrct += 1;
		}
		else{
			oRedrct += 2;
		}
		int i = 0;
		while(*(oRedrct + i) != ' ' && *(oRedrct + i) != '\0'){
			i++;
		}
		*(oRedrct + i) = '\0';
	}

	pid_t childPid = fork();
	
	if(childPid != 0){
		if(!isBG || bgIGN == 1){
			//Block SIGSTPT signal during foreground command execution
			sigset_t sigTSTP, oldSet;
			sigemptyset(&sigTSTP);
			sigaddset(&sigTSTP, SIGTSTP);
			sigprocmask(SIG_BLOCK, &sigTSTP, &oldSet);

			//Store foreground process exit or termination status once
			//process finishes and "clean-up" foreground process. 
			waitpid(childPid, status, 0);
			if(WIFSIGNALED(*status) != 0){
				printf("terminated by signal %d\n", WTERMSIG(*status));
				fflush(stdout);
			}

			//Unblock SIGSTPT signal
			sigprocmask(SIG_SETMASK, &oldSet, NULL);
		}
		else{
			int i;
			for(i = 0; i < CPIDs->used; i++){
				if(CPIDs->array[i] == 0){
					CPIDs->array[i] = (int)childPid;
					break;
				}
			}
			if(i == CPIDs->used){
				insertArray(CPIDs, (int)childPid);
			}

			//Print process id of background process, if applicable
			printf("background pid %d\n", childPid);	
			fflush(stdout);
		}
	}
	//All non-built in commands are executed via a call to execvp. Their
	//stdin and stdout streams are redirected to the user specified
	//locations if provided (or to /dev/null for background commands, if
	//not specified,) before the call to execvp.
	else{
		//For all child processes, set SIGINT behavior back to default
		//(as it was disabled in the parent, see smallsh.c).
		sigToggle(SIGINT, 1);

		//For all child processes, ignore SIGTSTP
		sigToggle(SIGTSTP, 0);

		int ifd;
		if(iRedrct){
			if((ifd = open(iRedrct, O_RDONLY)) == -1){
				perror("Could not open input redirection file ");
				exit(1);
			}
			dup2(ifd, STDIN_FILENO);
		}
		else if(isBG){
			if((ifd = open("/dev/null", O_RDONLY)) == -1){
				perror("Could not open input redirection file ");
				exit(1);
			}
			dup2(ifd, STDIN_FILENO);
			
		}
		int ofd;
		if(oRedrct){
			if((ofd = open(oRedrct, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1){
				perror("Could not open output redirection file ");
				exit(1);
			}
			dup2(ofd, STDOUT_FILENO);
		}
		else if(isBG){
			if((ofd = open("/dev/null", O_WRONLY)) == -1){
				perror("Could not open output redirection file ");
				exit(1);
			}
			dup2(ofd, STDOUT_FILENO);
		}
		
		//execvp only returns if an error occurs.
		//Print a message only in this case.
		if(execvp(args[0], args) == -1){
			//printf("smallsh: Command not found :: ");
			//fflush(stdout);
			perror(args[0]);
			exit(1);
		} 
	}
}

/*****************************************************************************************
 *  * References *
 *  http://man7.org/linux/man-pages/man2/sigprocmask.2.html
*****************************************************************************************/
