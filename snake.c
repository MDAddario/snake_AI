#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Board parameters
#define HEIGHT 6
#define WIDTH  6

// Snake parameters
#define SNAKE_INIT_LENGTH  3
#define SNAKE_APPLE_GROWTH 1

// Keeps track of all game information
typedef struct struct_game{

	char snake_direction[HEIGHT][WIDTH];
	
	/* '\0' : Square is empty
	 *  'h' : Snake head
	 *  'u' : Snake body, direction up
	 *  'd' : Snake body, direction down
	 *  'l' : Snake body, direction left
	 *  'r' : Snake body, direction right
	 */

	// Head, tail, and apple locations
	char head_i;
	char head_j;
	char tail_i;
	char tail_j;
	char apple_i;
	char apple_j;

	// Tail growth scalar
	char tail_delay;

	// Whether game is active or not
	char game_state;

} Game;

// Initialize all game settings
void gameInit(Game* game){

	// Clear board
	for (char i = 0; i < HEIGHT; i++)
		for (char j = 0; j < WIDTH; j++)
			game->snake_direction[i][j] = '\0';

	// Randomly generate snake head
	game->head_i = rand() % (HEIGHT - 2 * SNAKE_INIT_LENGTH + 2) + SNAKE_INIT_LENGTH - 1;
	game->head_j = rand() % (WIDTH  - 2 * SNAKE_INIT_LENGTH + 2) + SNAKE_INIT_LENGTH - 1;
	game->snake_direction[game->head_i][game->head_j] = 'h';

	// Build tail in random direction
	char direction = rand() % 4;
	for (char k = 1; k < SNAKE_INIT_LENGTH; k++){

		if (direction == 0){    // Head pointing up
			game->tail_i = game->head_i + k;
			game->tail_j = game->head_j;
			game->snake_direction[game->tail_i][game->tail_j] = 'u';
		}
		else if (direction == 1){    // Head pointing down
			game->tail_i = game->head_i - k;
			game->tail_j = game->head_j;
			game->snake_direction[game->tail_i][game->tail_j] = 'd';
		}
		else if (direction == 2){    // Head pointing left
			game->tail_i = game->head_i;
			game->tail_j = game->head_j + k;
			game->snake_direction[game->tail_i][game->tail_j] = 'l';
		}
		else if (direction == 3){    // Head pointing right
			game->tail_i = game->head_i;
			game->tail_j = game->head_j - k;
			game->snake_direction[game->tail_i][game->tail_j] = 'r';
		}
	}

	// Spawn apple at random location, not within snake
	game->apple_i = rand() % HEIGHT;
	game->apple_j = rand() % WIDTH;

	while (game->snake_direction[game->apple_i][game->apple_j]){
		game->apple_i = rand() % HEIGHT;
		game->apple_j = rand() % WIDTH;
	}

	// Begin game
	game->tail_delay = 0;
	game->game_state = 1;
	return;
}

// Display the snake board
void gamePrint(Game* game){

	char k;

	printf("/");
	for (char j = 0; j < WIDTH; j++)
		printf("---");
	printf("\\\n");

	for (char i = 0; i < HEIGHT; i++){
		printf("|");
		for (char j = 0; j < WIDTH; j++){

			if (i == game->head_i && j == game->head_j){
				printf(" H ");
			}
			else if (i == game->apple_i && j == game->apple_j){
				printf(" A ");
			}
			else if (k = game->snake_direction[i][j]){

				if (k == 'u'){
					printf(" u ");
				}
				else if (k == 'd'){
					printf(" d ");
				}
				else if (k == 'r'){
					printf(" r ");
				}
				else if (k == 'l'){
					printf(" l ");
				}
				else{
					printf("Critical error.");
				}
			}
			else{
				printf(" . ");
			}
		}
		printf("|\n");
	}
	printf("\\");
	for (char j = 0; j < WIDTH; j++)
		printf("---");
	printf("/\n");

	return;
}

// Return a hamiltonian cycle for the given snake board
int** generateHamiltonian(){

	if (HEIGHT != 6 || WIDTH != 6){
		printf("We currently cannot handle this.\n");
		return NULL;
	}

	// Allocate cycle
	int** cycle = (int**)malloc(HEIGHT * sizeof(int*));
	for (char i = 0; i < HEIGHT; i++)
		cycle[i] = (int*)malloc(WIDTH * sizeof(int));

	// Hardcode the cycle
	int bad_practice[6][6] = {
							{14, 13, 10,  9,  6,  5},
							{15, 12, 11,  8,  7,  4},
							{16, 17, 18,  1,  2,  3},
							{21, 20, 19, 36, 35, 34},
							{22, 25, 26, 29, 30, 33},
							{23, 24, 27, 28, 31, 32}};

	for (char i = 0; i < HEIGHT; i++)
		memcpy(cycle[i], bad_practice[i], 6*sizeof(int));

	return cycle;
}

// Print the cycle
void printHamiltonian(int** cycle){

	printf("/");
	for (char j = 0; j < WIDTH; j++)
		printf("---");
	printf("\\\n");

	for (char i = 0; i < HEIGHT; i++){
		printf("|");
		for (char j = 0; j < WIDTH; j++){
			printf(" %2.d ", cycle[i][j]);
		}
		printf("|\n");
	}
	printf("\\");
	for (char j = 0; j < WIDTH; j++)
		printf("---");
	printf("/\n");

	return;
}

// Choose whether to move or not
void choosePath(char direction, char* head_dir, int possible_energy, int head_energy,
				int apple_energy, int* next_energy, char is_boring){

	// Guaranteed to work, but boring solution
	if (is_boring){

		// Need to move
		if (*head_dir == '\0'){
			*head_dir = direction;
			*next_energy = possible_energy;
		}
		// End of the line
		else if (head_energy == HEIGHT * WIDTH && possible_energy == 1){
			*head_dir = direction;
			*next_energy = possible_energy;
		}
		// Moving one forward
		else if (possible_energy - head_energy == 1){
			*head_dir = direction;
			*next_energy = possible_energy;
		}
		return;
	}

	// Not guaranteed to work, but interesting solution
	if (*head_dir == '\0'){
		*head_dir = direction;
		*next_energy = possible_energy;
	}
	else{

		// Can reach apple
		if (possible_energy <= apple_energy){

			// If not already going for apple
			if (*next_energy > apple_energy){
				*head_dir = direction;
				*next_energy = possible_energy;
			}

			// If already going for apple
			else if (possible_energy > *next_energy){
				*head_dir = direction;
				*next_energy = possible_energy;
			}
		}
		// Cannot reach apple
		else if (*next_energy > apple_energy){

			// If was not already going for apple
			if (possible_energy > *next_energy){
				*head_dir = direction;
				*next_energy = possible_energy;
			}
		}
	}
	return;
}

// The main attraction
int main(){

	// Seed rand
	srand(time(NULL));

	// Create board
	Game* game = (Game*)malloc(sizeof(Game));
	gameInit(game);
	gamePrint(game);

	// Retrieve hamiltonian cycle
	int** cycle = generateHamiltonian();
	printHamiltonian(cycle);

	// Play the game
	while (game->game_state){

		// Print and pause
		gamePrint(game);
		usleep(50000);

		// Select movement direction from Hamiltonian
		int apple_energy = cycle[game->apple_i][game->apple_j];
		int head_energy = cycle[game->head_i][game->head_j];
		int next_energy = 0;
		char head_dir = '\0';
		int possible_energy;
		char is_boring = 1;

		// Check all vacant squares around snake head

		if (game->head_i != 0){  // Moving up
			if (!(game->snake_direction[game->head_i - 1][game->head_j])){

				possible_energy = cycle[game->head_i - 1][game->head_j];
				choosePath('u', &head_dir, possible_energy, head_energy, apple_energy, &next_energy, is_boring);
			}
		}
		if (game->head_i != HEIGHT-1){  // Moving down
			if (!(game->snake_direction[game->head_i + 1][game->head_j])){

				possible_energy = cycle[game->head_i + 1][game->head_j];
				choosePath('d', &head_dir, possible_energy, head_energy, apple_energy, &next_energy, is_boring);
			}
		}
		if (game->head_j != 0){  // Moving left
			if (!(game->snake_direction[game->head_i][game->head_j - 1])){

				possible_energy = cycle[game->head_i][game->head_j - 1];
				choosePath('l', &head_dir, possible_energy, head_energy, apple_energy, &next_energy, is_boring);
			}
		}
		if (game->head_j != WIDTH-1){  // Moving right
			if (!(game->snake_direction[game->head_i][game->head_j + 1])){

				possible_energy = cycle[game->head_i][game->head_j + 1];
				choosePath('r', &head_dir, possible_energy, head_energy, apple_energy, &next_energy, is_boring);
			}
		}

		// Game is over
		if (!head_dir){
			printf("Game is over\n");
			game->game_state = 0;
			break;
		}

		// Dissipate the tail
		if (game->tail_delay)
			(game->tail_delay)--;
		
		else{
			char tail_dir = game->snake_direction[game->tail_i][game->tail_j];
			game->snake_direction[game->tail_i][game->tail_j] = '\0';

			if (tail_dir == 'u')
				(game->tail_i)--;
			else if (tail_dir == 'd')
				(game->tail_i)++;
			else if (tail_dir == 'l')
				(game->tail_j)--;
			else if (tail_dir == 'r')
				(game->tail_j)++;
		}

		// Advance the head
		game->snake_direction[game->head_i][game->head_j] = head_dir;

		if (head_dir == 'u')
			(game->head_i)--;
		else if (head_dir == 'd')
			(game->head_i)++;
		else if (head_dir == 'l')
			(game->head_j)--;
		else if (head_dir == 'r')
			(game->head_j)++;

		game->snake_direction[game->head_i][game->head_j] = 'h';

		// Check for apple capture
		if (game->head_i == game->apple_i && game->head_j == game->apple_j){

			game->tail_delay += SNAKE_APPLE_GROWTH;

			// Spawn apple at random location, not within snake
			game->apple_i = rand() % HEIGHT;
			game->apple_j = rand() % WIDTH;

			while (game->snake_direction[game->apple_i][game->apple_j]){
				game->apple_i = rand() % HEIGHT;
				game->apple_j = rand() % WIDTH;
			}
		}
	}
	return 0;
}
