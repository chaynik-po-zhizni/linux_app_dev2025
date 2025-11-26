//~~~  https://habr.com/ru/articles/176671/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 


#define SIZE 6
#define MATRIX(arr, i, j, size) arr[(i) * (size) + (j)]

typedef struct {
	int number;
	char type;
} Cell;


void init(Cell *lab, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i % 2 == 0 || j % 2 == 0) {
				MATRIX(lab, i, j, size).type = '#';
			} else {
				MATRIX(lab, i, j, size).type = '.';
			}
			if (i == 0 || j == 0 || i == size - 1 || j == size - 1) {
				MATRIX(lab, i, j, size).number = 1;
			} else {
				MATRIX(lab, i, j, size).number = 0;
			}
		}
	}
}


void right(Cell *lab, int *counts, int line, int size) {
	counts[MATRIX(lab, line, 1, size).number] += 1;
	for (int i = 3; i < size; i += 2) {
		if (rand() % 2 == 1) {
			MATRIX(lab, line, i - 1, size).number = 1;
			MATRIX(lab, line - 1, i - 1, size).number = 1;
		} else {
			if (MATRIX(lab, line, i, size).number == MATRIX(lab, line, i - 2, size).number) {
				MATRIX(lab, line, i - 1, size).number = 1;
				MATRIX(lab, line - 1, i - 1, size).number = 1;
			} else {
				MATRIX(lab, line, i, size).number = MATRIX(lab, line, i - 2, size).number;
				MATRIX(lab, line, i - 1, size).number = 0;
			}
		}
		counts[MATRIX(lab, line, i, size).number] += 1;
	}
}

void last(Cell *lab, int size) {
	for (int i = 3; i < size; i += 2) {
		if (MATRIX(lab, size - 2, i - 2, size).number != MATRIX(lab, size - 2, i, size).number) {
			MATRIX(lab, size - 2, i - 1, size).number = 0;
		}
	}
}

void down(Cell *lab, int *counts, int line, int size) {
	for (int i = 1; i < size; i += 2) {
		if (rand() % 2 == 1) {
			if (counts[MATRIX(lab, line, i, size).number] == 1) {
				continue;
			}
			counts[MATRIX(lab, line, i, size).number] -= 1;
			MATRIX(lab, line + 1, i, size).number = 1;
		}
	}
	for (int i = 1; i < size - 1; i++) {
		if (MATRIX(lab, line + 1, i, size).number == 1) {
			MATRIX(lab, line + 1, i - 1, size).number = 1;
		}
		if (MATRIX(lab, line, i, size).type == '#' && MATRIX(lab, line, i, size).number == 1) {
			MATRIX(lab, line + 1, i, size).number = 1;
		}
	}
}

void next_line(Cell *lab, int *counts, int count, int line, int size) {
	for (int i = 1; i < size - 1; i += 1) {
		if (MATRIX(lab, line - 2, i, size).type == '#') {
			MATRIX(lab, line, i, size).number = 0;
		} else {
			if (MATRIX(lab, line - 1, i, size).number == 1) {
				MATRIX(lab, line, i, size).number = -1;
			} else {
				MATRIX(lab, line, i, size).number = MATRIX(lab, line - 2, i, size).number;
			}
		}
	}
	memset(counts, 0, count * sizeof(int));
	for (int i = 1; i < size; i += 2) {
		if (MATRIX(lab, line, i, size).number != - 1) {
			counts[MATRIX(lab, line, i, size).number] = 1;
		}
	}
	int cur = 0;
	for (int i = 1; i < size; i += 2) {
		while (counts[cur]) {
			cur++;
		}
		if (MATRIX(lab, line, i, size).number == - 1) {
			counts[cur] = 1;
			MATRIX(lab, line, i, size).number = cur++;
		}
	}
	memset(counts, 0, count * sizeof(int));
}

void print(Cell *lab, int *counts, int count, int line, int size) {
	for (int i = 0; i < size; i++) {
		printf("%c:%d ", MATRIX(lab, line - 2, i, size).type, MATRIX(lab, line - 2, i, size).number);
	}
	printf("\n");
	for (int i = 0; i < size; i++) {
		printf("%c:%d ", MATRIX(lab, line - 1, i, size).type, MATRIX(lab, line - 1, i, size).number);
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
	Cell *lab = malloc(size * size * sizeof(Cell));
	if (lab == NULL) {
		fprintf(stderr, "Error: allocation failed\n");
		exit(1);
	}
	init(lab, size);

	int count = 0;
	for (int j = 1; j < size; j += 2) {
		MATRIX(lab, 1, j, size).number = count++;
	}
	int *counts = malloc(count * sizeof(int));
	if (counts == NULL) {
		fprintf(stderr, "Error: allocation failed\n");
		free(lab);
		exit(1);
	}
	memset(counts, 0, count * sizeof(int));
	right(lab, counts, 1, size);
	down(lab, counts, 1, size);
	for (int j = 3; j < size; j += 2) {
		next_line(lab, counts, count, j, size);
		right(lab, counts, j, size);
		down(lab, counts, j, size);
	}
	last(lab, size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (MATRIX(lab, i, j, size).type == '#' && MATRIX(lab, i, j, size).number == 1) {
				printf("%c", '#');
			} else {
				printf("%c", '.');
			}
		}
		printf("\n");
	}
	free(counts);
	free(lab);
}