if [ -n "$1" ]; then
	TEST=$1
else
	exit 1
fi

if [ -n "$2" ]; then
	PROG=$2
else
	exit 1
fi

if [ -n "$3" ]; then
	TMP=$3
else
	exit 1
fi

while IFS= read -r line || [ -n "$line" ]; do
    printf "%s\n" "$line" > $TMP
    xargs < $TMP $PROG

done < $TEST