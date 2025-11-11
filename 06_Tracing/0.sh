#!/bin/sh

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


echo "test data" > $INPUT_FILE
cp $INPUT_FILE $TEST_FILE

$MOVE $INPUT_FILE $OUTPUT_FILE 
RETURN_CODE=$?
test $RETURN_CODE -eq 0 || exit 1
test -f $OUTPUT_FILE || exit 1
test ! -f $INPUT_FILE || exit 1
cmp $TEST_FILE $OUTPUT_FILE

echo "PASS Test 0: 1"
echo "test data" > $OUTPUT_FILE
cp $OUTPUT_FILE $TEST_FILE

$MOVE $OUTPUT_FILE $OUTPUT_FILE 
RETURN_CODE=$?
test $RETURN_CODE -eq 0 || exit 1
test -f $OUTPUT_FILE || exit 1
test ! -f $INPUT_FILE || exit 1
cmp $TEST_FILE $OUTPUT_FILE


echo "PASS Test 0: 2"