#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int parseint(char *s, const char *name) {
	char* endptr = NULL; 
	int result = strtol(s, &endptr, 10);
	if (*endptr != '\0' || errno == ERANGE) {
		fprintf(stderr, "Error parse parameter: %s\n", name);
		exit(1);
	}
	return result;
}

int main(int argc, char *argv[]) {
	int N, M = 0, S = 1;
	if (argc == 1) {
		printf("Usage: %s STOP | START STOP [STEP]\n", argv[0]);
		exit(0);
	} else if (argc == 2) {
		N = parseint(argv[1], "STOP");
	} else if (argc == 3) {
		M = parseint(argv[1], "START");
		N = parseint(argv[2], "STOP");
	} else if (argc == 4) {
		M = parseint(argv[1], "START");
		N = parseint(argv[2], "STOP");
		S = parseint(argv[3], "STEP");
	} else {
		fprintf(stderr, "Wrong number of parameters\n");
		exit(1);
	}
	for (int i = M; i < N; i += S) {
		printf("%d\n", i);
	}
}


