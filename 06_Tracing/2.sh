#!/bin/sh

# Test 2: Error opening input file (EACCES)
exit_handler() {
    EXIT_STATUS=$?
    trap - EXIT
    rm -f $INPUT_FILE $OUTPUT_FILE $TEST_FILE
    exit $EXIT_STATUS
}
trap exit_handler EXIT HUP INT QUIT PIPE TERM

MOVE="./move"
INPUT_FILE=`mktemp`
OUTPUT_FILE="2.out"
TEST_FILE=`mktemp`
ERR_NAME=EACCES
ERR_CODE=13
ERR_DESCRIPTION="Permission denied"

echo "test data" > $INPUT_FILE
chmod 000 $INPUT_FILE

FIRST_STRING="Error: can\`t open file $INPUT_FILE: $ERR_DESCRIPTION"
SECOND_STRING="+++ exited with $ERR_CODE +++"
printf '%s\n%s\n' "$FIRST_STRING" "$SECOND_STRING" > $TEST_FILE


strace -e inject=openat:error=$ERR_NAME -P $INPUT_FILE $MOVE $INPUT_FILE $OUTPUT_FILE 2>&1 | grep -E "\+\+\+|Error:" |  cmp $TEST_FILE || exit 1
test ! -f $OUTPUT_FILE || exit 1
test -f $INPUT_FILE || exit 1

strace -e inject=openat:error=$ERR_NAME -P $INPUT_FILE $MOVE $INPUT_FILE $OUTPUT_FILE  2>/dev/null 
RETURN_CODE=$?
test $RETURN_CODE -eq $ERR_CODE || exit 1
test ! -f $OUTPUT_FILE || exit 1
test -f $INPUT_FILE || exit 1

chmod 644 $INPUT_FILE
echo "PASS Test 2: EACCES opening input file"