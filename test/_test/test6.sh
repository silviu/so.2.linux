#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(dirname `pwd`)
	
function f()
{
	($CLIENT "$basemsg$1" "$MQCHAR$basemq$1" ${enc[$1]} -1 > __stdout$1) &
	pids[$i]=$!
}

rm -f __stdout*
#./clean.sh mem
basemsg="ODRVBLACSSI"
basemq="mq$1_"
enc=(
		'1 2 3 4 3 2 1'
		'2 3 4 1 4 3 2'
		'3 4 1 2 1 4 3'
		'4 3 2 1 2 3 4'
)

($SERVER 1 2>&1 > /dev/null ) &
($SERVER 2 2>&1 > /dev/null ) &
($SERVER 3 2>&1 > /dev/null ) &
($SERVER 4 2>&1 > /dev/null ) &
($GATEWAY  2>&1 > /dev/null ) &
for i in ${!enc[*]};do
	del=$del$basemq$i" "
done
./clean.sh "$del"

for i in ${!enc[*]};do
	f $i
done

fail=0

for i in ${!pids[*]};do
	wait ${pids[$i]}
	x=$(cat __stdout$i)
	#echo $i $x 
	if [ "$x" != "$basemsg$i" ]; then
		fail=1
	fi
done

$CLIENT "exit" "exit" -1
ret=$?

rm -f __stdout*

if [ $fail -eq 1 ] ; then
	ret=1
fi

if [ $ret -eq 0 ] ; then
	echo $PASS
	exit 1
else
	echo $FAIL
fi
