GENERATES = esub *.o *.tmp
PROG1 = esub
PROG2 = sed.sh

.PHONY: all clean test run

all:  $(PROG1) $(PROG2)

clean:
	rm -rf $(GENERATES)

run: test.txt $(PROG1)
	./test.sh test.txt ./$(PROG1) 0.tmp

test: test.txt $(PROG1) $(PROG2)
	./test.sh test.txt ./$(PROG1) 0.tmp | sed 's/\x1b\[[0-9;]*m//g' > 1.tmp 
	./test.sh test_err.txt ./$(PROG1) 0.tmp || true >> 1.tmp 
	./test.sh test.txt ./$(PROG2) 0.tmp > 2.tmp 
	./test.sh test_err.txt ./$(PROG2) 0.tmp || true >> 2.tmp 
	cmp 1.tmp 2.tmp

