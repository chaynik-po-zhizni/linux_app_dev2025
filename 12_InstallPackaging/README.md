@mainpage Stupid Guessor - Documentation

## Program description

'stupid_guessor' is a program for guessing numbers using a binary search algorithm.

## Main features

- Guessing numbers in a given range
- Support for Roman and Arabic numerals
- Internationalization (Russian/English)

## Program structure

The program consists of the following main modules:

1. **The main algorithm** is binary number search
2. **Roman Number Converter** - via 'roman' library 
3. **Argument processing** - via argp library
4. **Internationalization** - using gettext

## Usage

```
# Basic work
./stupid_guessor

# With Roman numerals
./stupid_guessor -r

# Extended range
./stupid_guessor -x
```

You also can define min and max boundaries via macros MAX_NUMBER and MIN_NUMBER

## Build
```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<path> ..

#all targets
make with-deps-all

#unit tests for roman library
make build_tests 
cd lib/tests/
ctest

#unit tests for roman library with coverage
cmake -DCMAKE_INSTALL_PREFIX=<path> -DENABLE_GCOV=ON ..
make coverage
make coverage-report
make coverage-full

#documentation with Doxygen
make roman-documentation
make stupid-guessor-documentation

#internalization
make update-po
make po-all

#deinstallization
make uninistall

#clean
make clean-all  #delete build dir
make clean-build


```

