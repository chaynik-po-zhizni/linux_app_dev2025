CFLAGS = -std=c99 -Wall -Wextra
if COND_GCOV
CFLAGS += -O0 -g -fprofile-arcs -ftest-coverage
else
CFLAGS += -O3 -g3
endif

lib_LTLIBRARIES = libgrowbuf.la
libgrowbuf_la_LDFLAGS = -version-info 1:0:0

libgrowbuf.c: buf.h

gcov:
	gcov -o .libs libgrowbuf -t -k -b | egrep -v '[[:digit:]]:[ /][*]'