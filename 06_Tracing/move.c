#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
	int error;
	if (argc != 3) {
		if (argc > 1) {
			fprintf(stderr, "Error: use %s <infile> <outfile>\n", argv[0]);
		} else {
			fprintf(stderr, "Error: use params <infile> <outfile>\n");
		}
		return EINVAL;
	}
	if (strcmp(argv[1], argv[2]) == 0) {
		return 0;
	}
	FILE *infile;
	infile = fopen(argv[1], "rb");
	if (infile == NULL) {
		error = errno;
		fprintf(stderr, "Error: can`t open file %s: %s\n", argv[1], strerror(errno));
		return error;
	}
	if (fseek(infile, 0L, SEEK_END) != 0) {
		error = errno;
		fprintf(stderr, "Error: fseek() failed on file %s: %s\n", argv[1], strerror(errno));
        fclose(infile);
        return error;
	}
	off_t len = ftello(infile);
	if (len == -1) {
		error = errno;
		fprintf(stderr, "Error: ftell() failed on file: %s: %s\n", argv[1], strerror(errno));
        fclose(infile);
        return error;
	}
	if (fseek(infile, 0L, SEEK_SET) != 0) {
		error = errno;
		fprintf(stderr, "Error: fseek() failed on file %s: %s\n", argv[1], strerror(errno));
        fclose(infile);
        return error;
	}

	char *data = malloc(len);
	if (data == NULL) {
		error = errno;
		fprintf(stderr, "Error: can`t allocate memory: %s\n", strerror(errno));
        fclose(infile);
        return error;
	}

    if (fread(data, 1, len, infile) != len || ferror(infile)) {
    	error = errno;
        fprintf(stderr, "Error: reading input file %s: %s\n", argv[1], strerror(errno));
        free(data);
        fclose(infile);
        return error;
    }

    if (fclose(infile) != 0) {
		error = errno;
		fprintf(stderr, "Error: can`t close file %s: %s\n", argv[1], strerror(errno));
		free(data);
		return error;
	}

    FILE *outfile;
    outfile = fopen(argv[2], "wb");
	if (outfile == NULL) {
		error = errno;
		fprintf(stderr, "Error: can`t open file %s: %s\n", argv[2], strerror(errno));
		free(data);
		return error;
	}
	
	size_t write_bytes = fwrite(data, 1, len, outfile);
    setbuf(outfile, NULL);
    if (write_bytes != len) {
    	error = errno;
        fprintf(stderr, "Error: writing output file %s: %s\n", argv[2], strerror(errno));
        free(data);
        fclose(outfile);
        remove(argv[2]);
        return error;
    }

    free(data);
    if (fclose(outfile) != 0) {
		error = errno;
		fprintf(stderr, "Error: can`t close file %s: %s\n", argv[2], strerror(errno));
		remove(argv[2]);
		return error;
	}

    if (remove(argv[1]) != 0) {
    	error = errno;
		fprintf(stderr, "Error: can`t remove file %s: %s\n", argv[1], strerror(errno));
		return error;
    }

    return 0;

}