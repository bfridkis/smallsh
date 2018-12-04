/****************************************************************************
** Program name: getCommand.h
** Class name: CS344
** Author: Ben Fridkis
** Date: 11/01/2018
** Description: Header file for getCommand.c. See getCommand.c for details.
****************************************************************************/

#ifndef getCommand_h
#define getCommand_h

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern int bgToggled;

char* getCommand(int* argCount);

#endif /* getCommand_h */
