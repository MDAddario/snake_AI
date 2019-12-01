#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Read the function name lmao
void gridPrinter(int** grid, int height, int width){
	
	printf("/");
	for (char j = 0; j < width; j++)
		printf("----");
	printf("\\\n");

	for (int i = 0; i < height; i++){
		printf("|");
		for (int j = 0; j < width; j++){
			if (grid[i][j])
				printf(" %2.d ", grid[i][j]);
			else
				printf("  0 ");
		}
		printf("|\n");
	}

	printf("\\");
	for (char j = 0; j < width; j++)
		printf("----");
	printf("/\n");

	return;
}

// Brute force determine all possible closed curve hamiltonian cycles for a given grid size
void hamiltonianCycleGenerator(int** grid, int** cycle, int height, int width, int i, int j, 
								int* length, long* tot_cycles){
	
	// Update current node
	grid[i][j] = ++(*length);
	
	// End condition
	if (*length == height * width){

		// Check to see if closed curve
		if (i != 0)
			if (grid[i-1][j] == 1){   // Up

				gridPrinter(grid, height, width);
				for (int i = 0; i < height; i++)
					memcpy(cycle[i], grid[i], width*sizeof(int));
				(*tot_cycles)++;
				return;
			}
		if (i != height - 1)
			if (grid[i+1][j] == 1){   // Down
				
				gridPrinter(grid, height, width);
				for (int i = 0; i < height; i++)
					memcpy(cycle[i], grid[i], width*sizeof(int));
				(*tot_cycles)++;
				return;
			}
		if (j != 0)
			if (grid[i][j-1] == 1){   // Left
				
				gridPrinter(grid, height, width);
				for (int i = 0; i < height; i++)
					memcpy(cycle[i], grid[i], width*sizeof(int));
				(*tot_cycles)++;
				return;
			}
		if (j != width - 1)
			if (grid[i][j+1] == 1){   // Right
				
				gridPrinter(grid, height, width);
				for (int i = 0; i < height; i++)
					memcpy(cycle[i], grid[i], width*sizeof(int));
				(*tot_cycles)++;
				return;
			}
	}

	// Develop curve along all possible paths
	if (i != 0)
		if (!grid[i-1][j]){   // Up
			hamiltonianCycleGenerator(grid, cycle, height, width, i-1, j, length, tot_cycles);
			grid[i-1][j] = 0;
			(*length)--;
		}
	if (i != height - 1)
		if (!grid[i+1][j]){   // Down
			hamiltonianCycleGenerator(grid, cycle, height, width, i+1, j, length, tot_cycles);
			grid[i+1][j] = 0;
			(*length)--;
		}
	if (j != 0)
		if (!grid[i][j-1]){   // Left
			hamiltonianCycleGenerator(grid, cycle, height, width, i, j-1, length, tot_cycles);
			grid[i][j-1] = 0;
			(*length)--;
		}
	if (j != width - 1)
		if (!grid[i][j+1]){   // Right
			hamiltonianCycleGenerator(grid, cycle, height, width, i, j+1, length, tot_cycles);
			grid[i][j+1] = 0;
			(*length)--;
		} 
	return;
}

// The main attraction
int main(int argc, char** argv){

	// Retrieve grid size
	if (argc < 3){
		printf("Please run as ./hamilton (height) (width)\n");
		return -1;
	}

	// Grid parameters
	int height  = atoi(argv[1]);
	int width   = atoi(argv[2]);
	int start_i = 0;
	int start_j = 0;
	int length  = 0;
	long tot_cycles = 0;
	
	// Allocate grid and cycle
	int** grid = (int**)calloc(height, sizeof(int*));
	int** cycle = (int**)calloc(height, sizeof(int*));
	
	for (int i = 0; i < height; i++){
		grid[i] = (int*)calloc(width, sizeof(int));
		cycle[i] = (int*)calloc(width, sizeof(int));
	}
	
	// Generate hamiltonian cycle
	hamiltonianCycleGenerator(grid, cycle, height, width, start_i, start_j, &length, &tot_cycles);
	printf("Total hamiltonian cycles: %ld\n", tot_cycles);

	return 0;
}
