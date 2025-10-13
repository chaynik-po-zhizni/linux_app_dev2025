GENERATES = range *.o *.test
CFLAGS = -O0 -g
PROG = range

.PHONY: all clean test

all:  $(PROG)

clean:
	rm -rf $(GENERATES)

test: 1.check 2.check 

%.check: %.gdb %.txt $(PROG)
	gdb --batch --quiet --command=$*.gdb ./$(PROG) | grep "@@@" > $*.test
	cmp $*.test $*.txt
