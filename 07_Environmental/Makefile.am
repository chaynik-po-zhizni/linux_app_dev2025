AM_CFLAGS = -Wall

bin_PROGRAMS=rhasher

rhasher_SOURCES=src/rhasher.c

SH_TESTS_DIR = test
SH_TESTS = $(shell ls $(SH_TESTS_DIR)/*.sh)

test: $(SH_TESTS)

$(SH_TESTS_DIR)/%: rhasher
	$@

clean-autotools-generates:
	rm -rf autom4te.cache src/.deps src/.dirstamp aclocal.m4 compile config.h config.h.in config.log config.status configure depcomp install-sh Makefile Makefile.in missing stamp-h1

clean-all: clean clean-autotools-generates