
LABYRINTH = labyrinth
GENERATES = *.o

.PHONY: all run clean

all:  $(LABYRINTH) $(LABYRINTH)0 $(LABYRINTH)1 $(LABYRINTH)2 $(LABYRINTH)3

$(LABYRINTH)%.c:
	cp $< $@ 
	patch $@ < $*.patch

$(LABYRINTH)0.c: $(LABYRINTH).c 0.patch
$(LABYRINTH)1.c: $(LABYRINTH)0.c 1.patch
$(LABYRINTH)2.c: $(LABYRINTH)1.c 2.patch
$(LABYRINTH)3.c: $(LABYRINTH)2.c 3.patch

run:
	./$(LABYRINTH)
	./$(LABYRINTH)0
	./$(LABYRINTH)1 6
	./$(LABYRINTH)1 8
	./$(LABYRINTH)2 ".#" 8
	./$(LABYRINTH)2 " x" 11
	./$(LABYRINTH)3 19978 " x" 11
	./$(LABYRINTH)3 49 " x" 11
	./$(LABYRINTH)3 47 " x" 11
	./$(LABYRINTH)3 49 " x" 11

clean:
	rm -f $(GENERATES) $(LABYRINTH) $(LABYRINTH)?.c $(LABYRINTH)? 



