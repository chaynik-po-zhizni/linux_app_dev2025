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


#$1 hash, $2 original, $3'base64' or 'hex' $4 'string' or 'file'
if [ $4 = "file" ]; then
    if [ $2 != "tthsum" ]; then
        if [ $3 = "base64" ]; then
            $2 $INPUT_FILE | cut -d' ' -f1 | xxd -r -p | base64 > $TEST_FILE
        elif [ $3 = "hex" ]; then
            $2 $INPUT_FILE | cut -d' ' -f1 > $TEST_FILE
        fi
    else
        if [ $3 = "base64" ]; then
            $2 $INPUT_FILE | cut -d' ' -f1 | base32 -d | base64 > $TEST_FILE
        elif [ $3 = "hex" ]; then
            $2 $INPUT_FILE | cut -d' ' -f1  | base32 -d | xxd -p > $TEST_FILE
        fi
    fi
    echo "$1 $INPUT_FILE" | $RHASHER 2>&1 | grep -v -E '^(Using|rhasher\>$)' | cut -d' ' -f2 | tail -2 | head -1 > $OUTPUT_FILE
elif [ $4 = "string" ]; then
    if [ $2 != "tthsum" ]; then
        if [ $3 = "base64" ]; then
            echo -n "$TEST_STRING" | $2 | cut -d' ' -f1 | xxd -r -p | base64 > $TEST_FILE
        elif [ $3 = "hex" ]; then
            echo -n "$TEST_STRING" | $2 | cut -d' ' -f1 > $TEST_FILE
        fi
    else
        if [ $3 = "base64" ]; then
            echo -n "$TEST_STRING" | $2 | cut -d' ' -f1 | base32 -d | base64 > $TEST_FILE
        elif [ $3 = "hex" ]; then
            echo -n "$TEST_STRING" | $2 | cut -d' ' -f1 | base32 -d | xxd -p > $TEST_FILE
        fi
    fi
    echo "$1 \"$TEST_STRING\"" | $RHASHER 2>&1 | grep -v -E '^(Using|rhasher\>$)' | cut -d' ' -f2 | tail -2 | head -1 > $OUTPUT_FILE
fi

cmp $TEST_FILE $OUTPUT_FILE || exit $?

