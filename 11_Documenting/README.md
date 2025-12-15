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
2. **Roman Number Converter** - functions `from_roman()` and `to_roman()`
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

