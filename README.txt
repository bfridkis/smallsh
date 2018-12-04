					     smallsh
					
smallsh.c is a linux shell emulator program, with the following 3 built-in commands:
	
**		1. status - prints the exit status or terminating
**					signal number of the most recently 
**					completed foreground process.
**
**		2. cd   -   Changes the working directory of the process.
**					(Note this does NOT change the directory of
**					the outer shell which calls smallsh.)
**
**		3. exit  -  Cleans up any pending child (background)
**					processes and terminates smallsh.
**			
**	All other linux commands are executed using the fork and 
**	execvp standard library functions. Processes can be specified
**	in the "background" (i.e. execution does not impede the user's
**	ability to enter a subsequent command) if a trailing '&' is
**	provided with the command. Processes executed using this 
**	specification will print their corresponding PIDs immediately
**	preceding their initiation and will print their termination
**	status or signal number immediately preceding the next command
**	input after they have terminated.
**
**	smallsh foreground processes (i.e. those initiated by commands
**	without a trailing '&', or when background processing is disabled
**	as described below) will terminate upon receiving a SIGINT
**	(CTRL-C) signal. All other smallsh processes (i.e. the outer
**	'parent' process and all background processes) will ignore 
**	SIGINT.
**
**	SIGTSTP (CTRL-Z) toggles the availability of background 
**	processing (i.e. when background processing is disabled, the
**	trailing '&' in the command is ignored). Whenever this mode
**	is toggled, a message will print to this effect. SIGTSPT has
**	no other effect on smallsh.
**
**	'$$' expands to the current process's ID. No other variable
**	expansions are performed.
**
**			The command syntax is as follows:
**			
**	command [arg1 arg2 ...] [< input_file] [> output_file] [&]
**
**     (The command line is prefaced with a ":" character.)
	
	
Getting Started:

1.  Copy all repository files to a directory of your choice.

2.  Type the command "make" (without quotes)

3.  Type "./smallsh" (without quotes) to launch the program.

4.  When finished, type "make clean" (without quotes) to remove
	all object and executable files associated with smallsh.

