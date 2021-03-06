#!/bin/bash
# Test for race conditions

echo
echo
cat /proc/driver/booga
echo
echo

for i in  1 2 3 4 5 6 7 8 9 10
do
./test-booga 0 64 r  >& /dev/null &
echo "First read";
./test-booga 0 64 w >& /dev/null &
echo "First write";
./test-booga 0 64 r >& /dev/null  &
echo "Second read";
./test-booga 0 64 w >& /dev/null &
echo "Second write";
./test-booga 0 64 r >& /dev/null  &
echo "Third read";
./test-booga 0 64 w >& /dev/null &
echo "Third write";
./test-booga 0 64 r  >& /dev/null &
echo "Fourth read";
./test-booga 0 64 w >& /dev/null &
echo "Fourth write";
echo "Finished iteration $i";
done

wait

echo
echo
cat /proc/driver/booga
echo
echo
