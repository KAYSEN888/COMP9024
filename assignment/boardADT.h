#ifndef _BOARD_ADT_H_
#define _BOARD_ADT_H_

#define MAX_LEN (1024*1024*16)  // 16MB

typedef struct _Input
{
	int * startGrid;
	int * goalGrid;
	int gridLen;
}Input;


// initialize a input struct
void InitInput(Input * pInput);

// destroy a input struct
void DestroyInput(Input * pInput);

// get the length of input grid
int getGridLen(Input * pInput);

// read the 2 grid from stdin
void readInput(Input * pInput);

// get the start grid value at certain position
int getStartAt(Input * pInput, int n);

// get the goal grid value at certain position
int getGoalAt(Input * pInput, int n);



#endif