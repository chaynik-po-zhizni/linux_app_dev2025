RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' 

# Функции для вывода
pass() { echo -e "${GREEN}PASS${NC}: $1"; }
fail() { echo -e "${RED}FAIL${NC}: $1"; }

TOTAL_TESTS=0
PASSED_TESTS=0
TEST="./tests/test.sh  "
ERR_TEST="./tests/test_err.sh"

add_test() {    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    $TEST $1 $2 $3 $4
    RETURN_CODE=$?
    
    if [ $RETURN_CODE -eq 0 ]; then
        pass "$5"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        return 0
    else
        fail "$5"
        return 1
    fi
}

add_test md5 md5sum base64 file "test md5sum base64 file"
add_test mD5 md5sum base64 string "test md5sum base64 string"
add_test Md5 md5sum hex file "test md5sum hex file"
add_test MD5 md5sum hex string "test md5sum hex string"

add_test sha1 sha1sum base64 file "test sha1sum base64 file"
add_test shA1 sha1sum base64 string "test sha1sum base64 string"
add_test Sha1 sha1sum hex file "test sha1sum hex file"
add_test SHA1 sha1sum hex string "test sha1sum hex string"

add_test tth tthsum base64 file "test tthsum base64 file"
add_test ttH tthsum base64 string "test tthsum base64 string"
add_test Tth tthsum hex file "test tthsum hex file"
add_test TTH tthsum hex string "test tthsum hex string"

TOTAL_TESTS=$((TOTAL_TESTS + 1))
$ERR_TEST
RETURN_CODE=$?
if [ $RETURN_CODE -eq 0 ]; then
    pass "Error test"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    fail "Error test"
fi

echo "Test Results: $PASSED_TESTS/$TOTAL_TESTS passed"