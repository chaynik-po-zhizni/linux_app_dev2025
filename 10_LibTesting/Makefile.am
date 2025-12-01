SUBDIRS = src tests

checklog:	check
	cat tests/*.log

gcov:	check
	$(MAKE) $(MAKEFLAGS) -C src gcov

clean-all: clean
	git clean -xdf .