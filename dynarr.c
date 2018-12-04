/****************************************************************************
** Program name: dynarr.c
** Class name: CS344
** Author: Ben Fridkis
** Date: 10/26/2018
** Description: Code to implement a dynamic array. (A dynamic array will be
**				used to store the process ids of all child processes.) THIS
**				CODE IS TAKEN DIRECTLY FROM THE WEBPAGE BELOW:
**	https://stackoverflow.com/questions/3536153/c-dynamically-growing-array
****************************************************************************/

#include "dynarr.h"

void initArray(Array *a, int initialSize) {
	a->array = (int *)calloc(initialSize, sizeof(int));
	a->used = 0;
	a->size = initialSize;
}

void insertArray(Array *a, int element) {
    // a->used is the number of used entries, 
    // because a->array[a->used++] updates 
    // a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size 
	if (a->used == a->size) {
		a->size *= 2;
		a->array = (int *)realloc(a->array, a->size * sizeof(int));
		for(int i = a->used; i < a->size; i++){
			a->array[i] = 0;		//Initializes newly allocated memory to 0
		}
	}
	a->array[a->used++] = element;
}

void freeArray(Array *a) {
	free(a->array);
	a->array = NULL;
	a->used = a->size = 0;
}

/*********************************************************************************
 * References *
 * https://stackoverflow.com/questions/3536153/c-dynamically-growing-array
*********************************************************************************/
