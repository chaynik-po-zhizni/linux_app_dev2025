#!/bin/sh

exit_handler() {
  trap - EXIT
  test -r "$TMPFILE" && rm -vf "$TMPFILE" 
  test -r "$TMPFILE1" && rm -vf "$TMPFILE1"
  test -r "$TMPFILE2" && rm -vf "$TMPFILE2"
  test -r "$TMPFILE3" && rm -vf "$TMPFILE3"
  exit 1
}

NEWLINE=`echo -n "\n" | iconv -t UCS2 | od -An -b -v -w2`
SPACE=`echo -n " " | iconv -t UCS2 | od -An -b -v -w2`
NEWLINE=${NEWLINE%" "*}${NEWLINE##*" "} 
SPACE=${SPACE%" "*}${SPACE##*" "} 

TOTAL_SIZE=`stty size < /dev/tty`
TOTAL_H=${TOTAL_SIZE%" "*}
TOTAL_W=${TOTAL_SIZE##*" "} 


trap exit_handler EXIT HUP INT QUIT PIPE TERM
TMPFILE="`mktemp`"
TMPFILE1="`mktemp`"
TMPFILE2="`mktemp`"
TMPFILE3="`mktemp`"


if [ -n "$1" ]; then
	DELAY=$1
else
	DELAY=0.1
fi

tee $TMPFILE3 | iconv -t UCS2 | od -An -b -v -w2 > $TMPFILE

SIZE=`cat $TMPFILE3 | wc -L -l`
H=${SIZE%" "*}
W=${SIZE##*" "} 

INIT_X=$((TOTAL_W-W))
INIT_X=$((INIT_X/2))
X=$INIT_X
Y=$((TOTAL_H-H))
Y=$((Y/2))

while read var1 var2 
do
    if [ "$var1$var2" -ne "$NEWLINE" ]; then  
        if [ "$var1$var2" -ne "$SPACE" ]; then  
            echo $var1 $var2 $X $Y >> $TMPFILE1
        fi
        X=$((X+1))
    else
        Y=$((Y+1))
        X=$INIT_X
    fi
done < $TMPFILE

shuf $TMPFILE1 -o $TMPFILE2
clear
while read var1 var4 var2 var3 
do 
	tput cup $var3 $var2
	echo -n "\\$var1\\$var4" | iconv -f UCS2
	sleep $DELAY

done < $TMPFILE2
tput cup $Y 0

test -r "$TMPFILE" && rm -vf "$TMPFILE"
test -r "$TMPFILE1" && rm -vf "$TMPFILE1"
test -r "$TMPFILE2" && rm -vf "$TMPFILE2"
test -r "$TMPFILE3" && rm -vf "$TMPFILE3"