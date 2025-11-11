#!/bin/sh

#Test 7: ENOSPC close outout file
exit_handler() {
    EXIT_STATUS=$?
    trap - EXIT
    rm -f $INPUT_FILE $OUTPUT_FILE $TEST_FILE
    exit $EXIT_STATUS
}
trap exit_handler EXIT HUP INT QUIT PIPE TERM


MOVE="./move"
INPUT_FILE=`mktemp`
OUTPUT_FILE=`mktemp`
TEST_FILE=`mktemp`
ERR_NAME=ENOSPC
ERR_CODE=28
ERR_DESCRIPTION="No space left on device"
FIRST_STRING="Error: can\`t close file $OUTPUT_FILE: $ERR_DESCRIPTION"
SECOND_STRING="+++ exited with $ERR_CODE +++"
printf '%s\n%s\n' "$FIRST_STRING" "$SECOND_STRING" > $TEST_FILE

echo "test data" > $INPUT_FILE

strace -e inject=close:error=$ERR_NAME -P $OUTPUT_FILE $MOVE $INPUT_FILE $OUTPUT_FILE 2>&1 | grep -E "\+\+\+|Error:" | cmp $TEST_FILE || exit 1
test ! -f $OUTPUT_FILE || exit 1
test -f $INPUT_FILE || exit 1

strace -e inject=close:error=$ERR_NAME -P $OUTPUT_FILE $MOVE $INPUT_FILE $OUTPUT_FILE 2>/dev/null 
RETURN_CODE=$?
test $RETURN_CODE -eq $ERR_CODE || exit 1
test ! -f $OUTPUT_FILE || exit 1
test -f $INPUT_FILE || exit 1

echo "PASS Test 8: ENOSPC close outout file"