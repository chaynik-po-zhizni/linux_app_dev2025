#!/bin/sh

#Test 9:EACCES removing input file
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
ERR_NAME=EACCES
ERR_CODE=13
ERR_DESCRIPTION="Permission denied"
FIRST_STRING="Error: can\`t remove file $INPUT_FILE: $ERR_DESCRIPTION"
SECOND_STRING="+++ exited with $ERR_CODE +++"
printf '%s\n%s\n' "$FIRST_STRING" "$SECOND_STRING" > $TEST_FILE

echo "test data" > $OUTPUT_FILE

strace -e inject=unlink:error=$ERR_NAME -P $INPUT_FILE $MOVE $INPUT_FILE $OUTPUT_FILE  2>&1 | grep -E "\+\+\+|Error:" | cmp $TEST_FILE || exit 1
test -f $OUTPUT_FILE || exit 1
test -f $INPUT_FILE || exit 1


strace -e inject=unlink:error=$ERR_NAME -P $INPUT_FILE $MOVE $INPUT_FILE $OUTPUT_FILE  2>/dev/null 
RETURN_CODE=$?
test $RETURN_CODE -eq $ERR_CODE || exit 1
test -f $OUTPUT_FILE || exit 1
test -f $INPUT_FILE || exit 1

echo "PASS Test 9: EACCES removing input file"