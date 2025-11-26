//~~~  https://habr.com/ru/articles/176671/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 


#define SIZE 6

typedef struct {
	int number;
	char type;
} Cell;

typedef struct {
	Cell *cells;
	int size;
} Maze;

#define MATRIX(arr, i, j) (arr)->cells[(i) * ((arr)->size) + (j)]

#define MAZE_CELL_TYPE(maze, i, j) MATRIX(maze, i, j).type
#define MAZE_CELL_VAL(maze, i, j) MATRIX(maze, i, j).number

int init(Maze *maze) {
	for (int i = 0; i < maze->size; i++) {
		for (int j = 0; j < maze->size; j++) {
			if (i % 2 == 0 || j % 2 == 0) {
				MAZE_CELL_TYPE(maze, i, j) = '#';
			} else {
				MAZE_CELL_TYPE(maze, i, j) = '.';
			}
			if (i == 0 || j == 0 || i == maze->size - 1 || j == maze->size - 1) {
				MAZE_CELL_VAL(maze, i, j) = 1;
			} else {
				MAZE_CELL_VAL(maze, i, j) = 0;
			}
		}
	}
	int count = 0;
	for (int j = 1; j < maze->size; j += 2) {
		MAZE_CELL_VAL(maze, 1, j) = count++;
	}
	return count;
}


void right(Maze *maze, int *counts, int line) {
	counts[MAZE_CELL_VAL(maze, line, 1)] += 1;
	for (int i = 3; i < maze->size; i += 2) {
		if (rand() % 2 == 1) {
			MAZE_CELL_VAL(maze, line, i - 1) = 1;
			MAZE_CELL_VAL(maze, line - 1, i - 1) = 1;
		} else {
			if (MAZE_CELL_VAL(maze, line, i) == MAZE_CELL_VAL(maze, line, i - 2)) {
				MAZE_CELL_VAL(maze, line, i - 1) = 1;
				MAZE_CELL_VAL(maze, line - 1, i - 1) = 1;
			} else {
				MAZE_CELL_VAL(maze, line, i) = MAZE_CELL_VAL(maze, line, i - 2);
				MAZE_CELL_VAL(maze, line, i - 1) = 0;
			}
		}
		counts[MAZE_CELL_VAL(maze, line, i)] += 1;
	}
}

void last(Maze *maze) {
	for (int i = 3; i < maze->size; i += 2) {
		if (MAZE_CELL_VAL(maze, maze->size - 2, i - 2) != MAZE_CELL_VAL(maze, maze->size - 2, i)) {
			MAZE_CELL_VAL(maze, maze->size - 2, i - 1) = 0;
		}
	}
}

void down(Maze *maze, int *counts, int line) {
	for (int i = 1; i < maze->size; i += 2) {
		if (rand() % 2 == 1) {
			if (counts[MAZE_CELL_VAL(maze, line, i)] == 1) {
				continue;
			}
			counts[MAZE_CELL_VAL(maze, line, i)] -= 1;
			MAZE_CELL_VAL(maze, line + 1, i) = 1;
		}
	}
	for (int i = 1; i < maze->size - 1; i++) {
		if (MAZE_CELL_VAL(maze, line + 1, i) == 1) {
			MAZE_CELL_VAL(maze, line + 1, i - 1) = 1;
		}
		if (MAZE_CELL_TYPE(maze, line, i) == '#' && MAZE_CELL_VAL(maze, line, i) == 1) {
			MAZE_CELL_VAL(maze, line + 1, i) = 1;
		}
	}
}

void next_line(Maze *maze, int *counts, int count, int line) {
	for (int i = 1; i < maze->size - 1; i += 1) {
		if (MAZE_CELL_TYPE(maze, line - 2, i) == '#') {
			MAZE_CELL_VAL(maze, line, i) = 0;
		} else {
			if (MAZE_CELL_VAL(maze, line - 1, i) == 1) {
				MAZE_CELL_VAL(maze, line, i) = -1;
			} else {
				MAZE_CELL_VAL(maze, line, i) = MAZE_CELL_VAL(maze, line - 2, i);
			}
		}
	}
	memset(counts, 0, count * sizeof(int));
	for (int i = 1; i < maze->size; i += 2) {
		if (MAZE_CELL_VAL(maze, line, i) != - 1) {
			counts[MAZE_CELL_VAL(maze, line, i)] = 1;
		}
	}
	int cur = 0;
	for (int i = 1; i < maze->size; i += 2) {
		while (counts[cur]) {
			cur++;
		}
		if (MAZE_CELL_VAL(maze, line, i) == - 1) {
			counts[cur] = 1;
			MAZE_CELL_VAL(maze, line, i) = cur++;
		}
	}
	memset(counts, 0, count * sizeof(int));
}

void print(Maze *maze, int *counts, int count, int line) {
	for (int i = 0; i < maze->size; i++) {
		printf("%c:%d ", MAZE_CELL_TYPE(maze, line - 2, i), MAZE_CELL_VAL(maze, line - 2, i));
	}
	printf("\n");
	for (int i = 0; i < maze->size; i++) {
		printf("%c:%d ", MAZE_CELL_TYPE(maze, line - 1, i), MAZE_CELL_VAL(maze, line - 1, i));
	}
	printf("\n");
	for (int i = 0; i<count; i++) {
		printf("%d ", counts[i]);
	}
	printf("\n");
}

int main() {
	// srand(time(NULL)); 
	srand(19978); 
	int size = SIZE * 2 + 1; 

	Maze maze;
	maze.size = size;
	maze.cells = malloc(maze.size * maze.size * sizeof(Cell));
	if (maze.cells == NULL) {
		fprintf(stderr, "Error: allocation failed\n");
		exit(1);
	}

	int count  = init(&maze);
	int *counts = malloc(count * sizeof(int));
	if (counts == NULL) {
		fprintf(stderr, "Error: allocation failed\n");
		free(maze.cells);
		exit(1);
	}

	memset(counts, 0, count * sizeof(int));
	right(&maze, counts, 1);
	down(&maze, counts, 1);

	for (int j = 3; j < maze.size; j += 2) {
		next_line(&maze, counts, count, j);
		right(&maze, counts, j);
		down(&maze, counts, j);
	}

	last(&maze);

	for (int i = 0; i < maze.size; i++) {
		for (int j = 0; j < maze.size; j++) {
			if (MAZE_CELL_TYPE(&maze, i, j) == '#' && MAZE_CELL_VAL(&maze, i, j) == 1) {
				printf("%c", '#');
			} else {
				printf("%c", '.');
			}
		}
		printf("\n");
	}

	free(counts);
	free(maze.cells);
}