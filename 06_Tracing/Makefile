
GENERATES = *.o move protect.so
PROG1 = move
PROG2 = protect

TEST_SCRIPTS = 0.sh 1.sh 2.sh 3.sh 4.sh 5.sh 6.sh 7.sh 8.sh 9.sh protect.sh

.PHONY: all clean test 

all: $(PROG1) $(PROG2).so

$(PROG2).so: $(PROG2).c
	$(CC) $(CFLAGS) -fPIC -shared ./$^ -o $@ 

clean:
	rm -rf $(GENERATES)


test: $(TEST_SCRIPTS) $(PROG1) $(PROG2).so
	for script in $(TEST_SCRIPTS); do \
		./$$script; \
	done

