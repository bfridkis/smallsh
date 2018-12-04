/*******************************************
** Program name: loadArgs.h
** Class name: CS344
** Author: Ben Fridkis
** Date: 10/29/2018
** Description: Header file for loadArgs.c.
********************************************/

#ifndef loadArgs_h
#define loadArgs_h

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "dynarr.h"

char** loadArgs(char* command, Array* exArgs, int argCount);

#endif /* loadArgs_h */
