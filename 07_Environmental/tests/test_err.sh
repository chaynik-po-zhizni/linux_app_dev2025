#!/bin/sh

exit_handler() {
    EXIT_STATUS=$?
    trap - EXIT
    rm -f $INPUT_FILE $TEST_FILE $OUTPUT_FILE
    exit $EXIT_STATUS
}
trap exit_handler EXIT HUP INT QUIT PIPE TERM


RHASHER="./rhasher"

INPUT_FILE=`mktemp`
TEST_FILE=`mktemp`
OUTPUT_FILE=`mktemp`
TEST_STRING="test_data_for_string"


echo "test data for file" > $INPUT_FILE


echo "lalasum $INPUT_FILE" | $RHASHER  2> $TEST_FILE >/dev/null
echo "lalasum \"$TEST_STRING\"" | $RHASHER   2>> $TEST_FILE >/dev/null
echo "MD5 \"$TEST_STRING" | $RHASHER  2>> $TEST_FILE >/dev/null
echo "MD5 " | $RHASHER  2>> $TEST_FILE >/dev/null
echo "MD5 non_exist_file" | $RHASHER  2>> $TEST_FILE >/dev/null

echo "Error: unknown algorithm lalasum" > $OUTPUT_FILE
echo "Error: unknown algorithm lalasum" >> $OUTPUT_FILE
echo "Error: unclosed quote" >> $OUTPUT_FILE
echo "Error: use params <hash_algorithm> <input>" >> $OUTPUT_FILE
echo "Error processing file non_exist_file" >> $OUTPUT_FILE

cmp $OUTPUT_FILE $TEST_FILE




