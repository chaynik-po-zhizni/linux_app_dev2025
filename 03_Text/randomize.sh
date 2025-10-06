#!/bin/sh
exit_handler() {
  trap - EXIT
  test -r "$TMPFILE" && rm -f "$TMPFILE"
  test -r "$TMPFILE1" && rm -f "$TMPFILE1"
  test -r "$TMPFILE2" && rm -f "$TMPFILE2"
}

NEWLINE=`echo -n "\n"| od -An -b -v -w1`
SPACE=`echo -n " "| od -An -b -v -w1`

trap exit_handler EXIT HUP INT QUIT PIPE TERM
TMPFILE="`mktemp`"
TMPFILE1="`mktemp`"
TMPFILE2="`mktemp`"

X=0
Y=0
if [ -n "$1" ]; then
	DELAY=$1
else
	DELAY=0.1
fi

od -An -b -v -w1 >> $TMPFILE
while read SYMBOL
do
    if [ "$SYMBOL" -ne "$NEWLINE" ]; then  
        if [ "$SYMBOL" -ne "$SPACE" ]; then  
            echo $SYMBOL $X $Y >> $TMPFILE1
        fi
        X=$((X+1))
    else
        Y=$((Y+1))
        X=0
    fi
done < $TMPFILE

cat $TMPFILE1
shuf $TMPFILE1 -o $TMPFILE2
clear
while read var1 var2 var3 
do 
	tput cup $var3 $var2
	echo -n "\\0$var1"
	sleep $DELAY

done < $TMPFILE2
tput cup $Y $X
test -r "$TMPFILE" && rm -f "$TMPFILE"
test -r "$TMPFILE1" && rm -f "$TMPFILE1"
test -r "$TMPFILE2" && rm -f "$TMPFILE2"