/*********************************************************************
** Program name: dynarr.h
** Class name: CS344
** Author: Ben Fridkis
** Date: 10/26/2018
** Description: Header file for dynarr.c. See dynarr.c for details.
*********************************************************************/

#ifndef dynarr_h
#define dynarr_h

#include <stdlib.h>

typedef struct {
	  int *array;
	  int used;
	  int size;
	  } Array;

void initArray(Array *a, int initialSize);
void insertArray(Array *a, int element);
void freeArray(Array *a);

#endif /* dynarr.h */	  
