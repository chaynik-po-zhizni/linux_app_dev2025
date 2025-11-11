#!/bin/sh


exit_handler() {
    EXIT_STATUS=$?
    trap - EXIT
    rm -f $INPUT_FILE $OUTPUT_FILE
    exit $EXIT_STATUS
}
trap exit_handler EXIT HUP INT QUIT PIPE TERM

MOVE="./move"
PROTECT="PROTECT"
PROTECT_LIB="./protect.so"
INPUT_FILE=`mktemp`
OUTPUT_FILE=`mktemp`

echo "test data" > $INPUT_FILE 
mv $INPUT_FILE "$INPUT_FILE$PROTECT"
INPUT_FILE="$INPUT_FILE$PROTECT"

LD_PRELOAD="./$PROTECT_LIB" $MOVE $INPUT_FILE $OUTPUT_FILE 
test -f $INPUT_FILE || exit 1

echo "PASS Test 10: protect"