/***********************************************************************************
** Program name: sigTSTPHandler.h
** Class name: CS344
** Author: Ben Fridkis
** Date: 11/01/2018
** Description: Header file for sigTSTPHandler.c. See sigTSTPHandler.c for details.
************************************************************************************/

#ifndef sigTSTPHandler_h
#define sigTSTPHandler_h

#include <signal.h> 
#include <unistd.h>
#include <stdio.h>

extern int bgIGN, bgToggled;

void sigTSTPHandler();

#endif /* sigTSTPHandler.h */
