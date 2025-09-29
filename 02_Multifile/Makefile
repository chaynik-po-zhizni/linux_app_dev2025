GENERATES = prog README prog-so prog-a *.o *~ o.* *.a *.so
CFLAGS = -Wall -fPIC
ARG = arg 
ARGS = arg1 arg2 arg3

%.o:    %.c
	$(CC) $(CFLAGS) $< -c -o $@

all:    README prog prog-a prog-so test

prog:   const.o fun.o prog.o
	$(CC) $^ -o $@

prog-a:  liboutput_static.a prog.o
	$(CC) -L. $^ -loutput_static -o $@

liboutput_static.a:	const.o fun.o
	ar -rcs $@ $^

prog-so: prog.o liboutput.so
	$(CC) -L. $^ -loutput -o $@

liboutput.so: fun.o const.o
	$(CC) -shared $^ -o $@

fun.o const.o: outlib.h

README: prog
	./$< 2> $@

clean:
	rm -rf $(TRASH)
	rm -rf $(GENERATES)

test: 1.check 2.check 3.check

%.check: prog%.test prog-a%.test prog-so%.test 
	cmp prog$*.test prog-a$*.test
	cmp prog$*.test prog-so$*.test

%1.test: %
	LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH) ./$* > $@ 2>&1 
%2.test: %
	LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH) ./$* $(ARG) > $@ 2>&1 
%3.test: %
	LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH) ./$* $(ARGS) > $@ 2>&1 



