#include <string.h>
#include <dlfcn.h>
#include <errno.h>

int (*original_remove)(const char *pathname) = NULL;

void init_originals() {
    original_remove = dlsym(RTLD_NEXT, "remove");
}

int is_protected(const char *filename) {
    return strstr(filename, "PROTECT") != NULL;
}

int remove(const char *filename) {
    if (original_remove == NULL) {
        init_originals();
    }
    if (is_protected(filename)) {
        errno = EACCES;
        return errno;
    }
    return original_remove(filename);
}
