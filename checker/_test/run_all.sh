#!/bin/bash

export SERVER="../../server"
export CLIENT="../client"
export GATEWAY="../../gateway"

export MQCHAR="/"

export PASS="--{ PASSED }--"
export FAIL="--{ FAILED }--"

n_passed=0
n_failed=0
score=0

n_test=9

#for i in `seq 1 $n_test` ; do
for i in `seq 1 $n_test` ; do
	echo "Running test $i"
	./test$i.sh $i
	
	ret=$?
	if [ $ret -eq 0 ]; then
		n_failed=$(($n_failed+1))
	else
		n_passed=$(($n_passed+1))
		score=$(($score+$ret))
	fi
	
	echo ""
done

echo 'Passed = '$n_passed
echo 'Failed = '$n_failed
echo '-------------'
echo 'Final score  = '$score

unset SERVER
unset CLIENT
unset GATEWAY
unset MQCHAR

unset PASS
unset FAIL


