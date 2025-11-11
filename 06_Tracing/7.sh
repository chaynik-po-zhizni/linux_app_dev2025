#!/bin/sh

#Test 1: Error opening outout file (ENOENT)
exit_handler() {
    EXIT_STATUS=$?
    trap - EXIT
    rm -f $INPUT_FILE $OUTPUT_FILE $TEST_FILE $OUTPUT_FILE2
    exit $EXIT_STATUS
}
trap exit_handler EXIT HUP INT QUIT PIPE TERM


MOVE="./move"
INPUT_FILE=`mktemp`
OUTPUT_FILE=`mktemp`
OUTPUT_FILE2=`mktemp`
TEST_FILE=`mktemp`
ERR_NAME=ENOENT
ERR_CODE=2
ERR_DESCRIPTION="No such file or directory"
FIRST_STRING="Error: can\`t open file $OUTPUT_FILE: $ERR_DESCRIPTION"
SECOND_STRING="+++ exited with $ERR_CODE +++"
printf '%s\n%s\n' "$FIRST_STRING" "$SECOND_STRING" > $TEST_FILE

echo "test data" > $OUTPUT_FILE
cp $OUTPUT_FILE $OUTPUT_FILE2

strace -e inject=openat:error=$ERR_NAME -P $OUTPUT_FILE $MOVE $INPUT_FILE $OUTPUT_FILE  2>&1 | grep -E "\+\+\+|Error:" | cmp $TEST_FILE || exit 1
test -f $OUTPUT_FILE || exit 1
test -f $INPUT_FILE || exit 1
cmp $OUTPUT_FILE $OUTPUT_FILE2 || exit 1

strace -e inject=openat:error=$ERR_NAME -P $OUTPUT_FILE $MOVE $INPUT_FILE $OUTPUT_FILE  2>/dev/null 
RETURN_CODE=$?
test $RETURN_CODE -eq $ERR_CODE || exit 1
test -f $OUTPUT_FILE || exit 1
test -f $INPUT_FILE || exit 1
cmp $OUTPUT_FILE $OUTPUT_FILE2 || exit 1

echo "PASS Test 7: ENOENT opening output file"