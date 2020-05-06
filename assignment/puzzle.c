#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "boardADT.h"

// check the correctness of the grid configuration
// type: 0 - want to validate start grid; 1 - want to validate goal grid
void validateOneGrid(Input * pInput, int type)
{
	int i;
	int len = getGridLen(pInput);// get the grid length

	// init a buffer to check correctness
	int * pNum = (int*)malloc(len * sizeof(int));
	if (pNum == NULL)
	{
		fprintf(stderr, "Memory Error\n");
		exit(EXIT_FAILURE);
	}
	memset(pNum, 0, len * sizeof(int));
	for (i = 0; i < len; i++)
	{
		if (type == 0)
		{
			if (getStartAt(pInput, i) >= len)
			{
				fprintf(stdout, "Error: illegal number %d!\n", getStartAt(pInput, i));
				exit(EXIT_FAILURE);
			}
			*(pNum + getStartAt(pInput, i)) += 1;
		}
		else
		{
			if (getGoalAt(pInput, i) >= len)
			{
				fprintf(stdout, "Error: illegal number %d!\n", getGoalAt(pInput, i));
				exit(EXIT_FAILURE);
			}
			*(pNum + getGoalAt(pInput, i)) += 1;
		}
	}

	// check whether have illegal character
	if (*pNum > 1)
	{
		fprintf(stdout, "Error: Input contains something other than a number or b!\n");
		exit(EXIT_FAILURE);
	}

	// check whether blank existing
	if (*pNum < 1)
	{
		fprintf(stdout, "Error: blank is missing!\n");
		exit(EXIT_FAILURE);
	}

	// check whether each elements exist
	for (i = 1; i < len; i++)
	{
		if (*(pNum + i) > 1)
		{
			fprintf(stdout, "Error: There is more than one %d\n", i);
			exit(EXIT_FAILURE);
		}
		if (*(pNum + i) < 1)
		{
			fprintf(stdout, "Error: %d is missing\n", i);
			exit(EXIT_FAILURE);
		}
	}
	free(pNum);
}

// check the correctness of the each configuration
void validate(Input *pInput)
{
	int i;

	int len = getGridLen(pInput);
	int N = (int)sqrt(len);

	// check whether the length of grid is NxN
	if (N*N != len)
	{
		fprintf(stdout, "Error. The input line not a NxN board!\n");
		exit(0);
	}

	validateOneGrid(pInput, 0);
	validateOneGrid(pInput, 1);
}

int calculateTileDisorder(int cur, int * grid, int len)
{
	int i;
	int disorder = 0;
	if (*(grid + cur) != 0)
	{
		for (i = cur + 1; i < len; i++)
		{
			if (*(grid + i) < *(grid + cur) && *(grid + i) != 0)
			{
				disorder++;
			}
		}
	}
	return disorder;
}

int calculateBoardDisorder(int * grid, int len)
{
	int i;
	int N = (int)sqrt(len);

	int disorder = 0;

	// odd-size of board: the sum of the tile disorder
	for (i = 0; i < len; i++)
	{
		disorder += calculateTileDisorder(i, grid, len);
	}
	if (N % 2 == 0)
	{
		// even-sized board: the sum of tile disorders + the row number of the blank
		for (i = 0; i < len; i++)
		{
			if (*(grid + i) == 0)
			{
				break;
			}
		}
		// the row number of the blank (start with 1)
		disorder += 1 + i / N;
	}
	return disorder;
}

int isSolvable(Input *pInput)
{
	int i;
	int disorder1, disorder2;

	// grid length
	int len = getGridLen(pInput);
	int * grid = (int *)malloc(len * sizeof(int));
	if (grid == NULL)
	{
		fprintf(stderr, "Memory Error\n");
		exit(EXIT_FAILURE);
	}

	// calculate the disorder of start grid
	for (i = 0; i < len; i++)
	{
		*(grid + i) = getStartAt(pInput, i);
	}
	disorder1 = calculateBoardDisorder(grid, len);

	// calculate the disorder of goal grid
	for (i = 0; i < len; i++)
	{
		*(grid + i) = getGoalAt(pInput, i);
	}
	disorder2 = calculateBoardDisorder(grid, len);

	free(grid);
	// whether all is odd or even
	return (disorder1 - disorder2) % 2 == 0;
}


int main(int argc, char **argv)
{
	int i, len;
	int n;
	Input input;

	InitInput(&input);
	readInput(&input);

	validate(&input);

	// display the result
	len = getGridLen(&input);

	// first line
	printf("start:");
	for (i = 0; i < len; i++)
	{
		n = getStartAt(&input, i);
		if (n == 0)
		{
			printf(" b");
		}
		else
		{
			printf(" %d", n);
		}
	}
	printf("\n");

	// second line
	printf("goal:");
	for (i = 0; i < len; i++)
	{
		n = getGoalAt(&input, i);
		if (n == 0)
		{
			printf(" b");
		}
		else
		{
			printf(" %d", n);
		}
	}
	printf("\n");

	// solvable
	if (isSolvable(&input))
	{
		printf("solvable\n");
	}
	else
	{
		printf("unsolvable\n");
	}

	DestroyInput(&input);

	return EXIT_SUCCESS;
}


