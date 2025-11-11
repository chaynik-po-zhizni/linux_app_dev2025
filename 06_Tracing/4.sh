#!/bin/sh

#Test 4: Error reading input file (ENOENT)
exit_handler() {
    EXIT_STATUS=$?
    trap - EXIT
    rm -f $INPUT_FILE $OUTPUT_FILE $TEST_FILE
    exit $EXIT_STATUS
}
trap exit_handler EXIT HUP INT QUIT PIPE TERM


MOVE="./move"
INPUT_FILE=`mktemp`
OUTPUT_FILE="4.out"
TEST_FILE=`mktemp`
ERR_NAME=ENOENT
ERR_CODE=2
ERR_DESCRIPTION="No such file or directory"
FIRST_STRING="Error: reading input file $INPUT_FILE: $ERR_DESCRIPTION"
SECOND_STRING="+++ exited with $ERR_CODE +++"
printf '%s\n%s\n' "$FIRST_STRING" "$SECOND_STRING" > $TEST_FILE

echo "test data" > $INPUT_FILE

strace -e inject=read:error=$ERR_NAME -P $INPUT_FILE $MOVE $INPUT_FILE $OUTPUT_FILE 2>&1 | grep -E "\+\+\+|Error:" | cmp $TEST_FILE || exit 1
test ! -f $OUTPUT_FILE || exit 1
test -f $INPUT_FILE || exit 1

strace -e inject=read:error=$ERR_NAME -P $INPUT_FILE $MOVE $INPUT_FILE $OUTPUT_FILE 2>/dev/null 
RETURN_CODE=$?
test $RETURN_CODE -eq $ERR_CODE || exit 1
test ! -f $OUTPUT_FILE || exit 1
test -f $INPUT_FILE || exit 1

echo "PASS Test 4: ENOENT reading input file"