
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "boardADT.h"


// get the start grid value at certain position
int getStartAt(Input * pInput, int n)
{
	return *(pInput->startGrid + n);
}

// get the goal grid value at certain position
int getGoalAt(Input * pInput, int n)
{
	return *(pInput->goalGrid + n);
}

// initialize a input struct
void InitInput(Input * pInput)
{
	pInput->startGrid = (int *)malloc(sizeof(int)*MAX_LEN);
	if (pInput->startGrid == NULL)
	{
		fprintf(stderr, "Memory Error\n");
		exit(EXIT_FAILURE);
	}
	pInput->goalGrid = (int *)malloc(sizeof(int)*MAX_LEN);
	if (pInput->startGrid == NULL)
	{
		fprintf(stderr, "Memory Error\n");
		exit(EXIT_FAILURE);
	}
	pInput->gridLen = 0;
}

// destroy a input struct
void DestroyInput(Input * pInput)
{
	free(pInput->startGrid);
	free(pInput->goalGrid);
}

// get the length of input grid
int getGridLen(Input * pInput)
{
	return pInput->gridLen;
}

// read the 2 grid from stdin
void readInput(Input * pInput)
{
	char *nBuf = (char *)malloc(sizeof(char) * MAX_LEN);
	if (nBuf == NULL)
	{
		fprintf(stderr, "Memory Error\n");
		exit(EXIT_FAILURE);
	}
	int *grid = (int *)malloc(sizeof(int)*MAX_LEN * 2);
	if (grid == NULL)
	{
		fprintf(stderr, "Memory Error\n");
		exit(EXIT_FAILURE);
	}
	int gridLen = 0;

	while (scanf("%s", nBuf) >= 0)
	{
		*(grid + gridLen++) = atoi(nBuf);
	}

	// whether start grid and goal grid has the same length
	if (gridLen % 2 != 0)
	{
		free(grid);
		free(nBuf);
		fprintf(stdout, "Error: 2 board not be the same size!\n");
		exit(EXIT_FAILURE);
	}

	// fill the input struct
	pInput->gridLen = gridLen / 2;
	memcpy(pInput->startGrid, grid, pInput->gridLen * sizeof(int));
	memcpy(pInput->goalGrid, grid + pInput->gridLen, pInput->gridLen * sizeof(int));

	free(grid);
	free(nBuf);
}