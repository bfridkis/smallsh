/****************************************************************************
** Program name: executeCMD.h
** Class name: CS344
** Author: Ben Fridkis
** Date: 11/01/2018
** Description: Header file for executeCMD.h. See executeCMD.c for details.
****************************************************************************/

#ifndef executeCMD_h
#define executeCMD_h

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include "dynarr.h"
#include "sigToggle.h"

extern int bgIGN;

void executeCMD(char** args, int argCount,  Array* CPIDs, int isBG, int* status, 
					char* iRedrct, char* oRedrct);

#endif /* executeCMD_h */
