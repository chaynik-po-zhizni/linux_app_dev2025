#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <rhash.h>


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef USE_READLINE
#include <readline/readline.h>
#endif

#define MAX_DIGEST_SIZE 64
#define MAX_OUTPUT_SIZE 130

typedef enum {
    OUTPUT_HEX,
    OUTPUT_BASE64
} output_format;

static int get_hash_id(const char *name) {
    if (strcasecmp(name, "MD5") == 0) return RHASH_MD5;
    if (strcasecmp(name, "SHA1") == 0) return RHASH_SHA1;
    if (strcasecmp(name, "TTH") == 0) return RHASH_TTH;
    return 0;
}

static int print_hash(const unsigned char *digest, size_t length, output_format format) {
    char output[MAX_OUTPUT_SIZE];
    int flag;
    switch (format) {
    case OUTPUT_HEX:
        flag = RHPR_HEX;
        break;
    case OUTPUT_BASE64:
        flag = RHPR_BASE64;
        break;
    default:
        return -1;
    }
    rhash_print_bytes(output, digest, length, flag);
    printf("%s\n", output);
    return 0;
}

static int process_string(int hash_id, const char *data, size_t len, output_format format) {
    unsigned char digest[MAX_DIGEST_SIZE];
    if (rhash_msg(hash_id, data, len, digest) < 0) {
        return -1;
    }
    return print_hash(digest, rhash_get_digest_size(hash_id), format);
}

static int process_file(const char *filename, int hash_id, output_format format) {
    unsigned char digest[MAX_DIGEST_SIZE];
    if (rhash_file(hash_id, filename, digest) < 0) {
        return -1;
    }
    print_hash(digest, rhash_get_digest_size(hash_id), format);
    return 0;
}

int main() {
    rhash_library_init();
    
#ifdef USE_READLINE
    printf("Using readline version\n");
#else
    printf("Using getline version\n");
    size_t len = 0;
#endif

    char *line = NULL;
    
    while(1) {
#ifdef USE_READLINE
        line = readline("rhasher> ");
        if (!line) break;
#else
        printf("rhasher> ");
        fflush(stdout);
        if (getline(&line, &len, stdin) == -1) {
            free(line);
            line = NULL;
            if (feof(stdin)) {
                printf("\n");
                break;
            } else {
                fprintf(stderr, "Error: failed getline\n");
                return 1;
            }
        }
#endif

        if (strlen(line) == 0) {
            free(line);
            line = NULL;
            continue;
        }

        char *alg_name = strtok(line, " \n");
        char *data = strtok(NULL, " \n");
        
        if (!alg_name || !data) {
            fprintf(stderr, "Error: use params <hash_algorithm> <input>\n");
            free(line);
            line = NULL;
            continue;
        }

        output_format format = isupper(alg_name[0]) ? OUTPUT_HEX : OUTPUT_BASE64;
        int hash_id = get_hash_id(alg_name);
        
        if (!hash_id) {
            fprintf(stderr, "Error: unknown algorithm %s\n", alg_name);
            free(line);
            line = NULL;
            continue;
        }
        
        if (data[0] == '"') {
            size_t data_len = strlen(data);
            if (data[data_len-1] == '"') {
                data[data_len-1] = 0;
                if (process_string(hash_id, data + 1, strlen(data + 1), format)) {
                    fprintf(stderr, "Error processing string %s\n", data);
                }
            } else {
                fprintf(stderr, "Error: unclosed quote\n");
            }
        } else {
            if (process_file(data, hash_id, format)) {
                fprintf(stderr, "Error processing file %s\n", data);
            }
        }

        free(line);
        line = NULL;
    }
    
    return 0;
}
