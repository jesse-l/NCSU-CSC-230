#!/bin/bash
FAIL=0

# make a fresh copy of the target program
make clean
make
if [ $? -ne 0 ]; then
  echo "**** Make (compilation) FAILED"
  FAIL=1
fi

# Function to run the program against a (successful) test case, first compressing, checking
# its output and exit status, then uncompressing and checking the results.
runtest() {
  TEST_NO=$1
  ALT_WORDS=$2

  rm -f compressed.raw output.txt stdout.txt stderr.txt

  if [ "$ALT_WORDS" == "" ]
  then
      echo "Test $TEST_NO: ./pack input_$TEST_NO.txt compressed.raw > stdout.txt 2> stderr.txt"
      ./pack input_$TEST_NO.txt compressed.raw > stdout.txt 2> stderr.txt
      STATUS=$?
  else
      echo "Test $TEST_NO: ./pack input_$TEST_NO.txt compressed.raw $ALT_WORDS > stdout.txt 2> stderr.txt"
      ./pack input_$TEST_NO.txt compressed.raw $ALT_WORDS > stdout.txt 2> stderr.txt
      STATUS=$?
  fi

  # Program should have succeeded.
  if [ $STATUS -ne 0 ]
  then
      echo "**** Test $TEST_NO FAILED - incorrect exit status. Expected: 0 Got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure the compressed output matches the expected output
  diff -q expected_$TEST_NO.raw compressed.raw >/dev/null 2>&1
  if [ $? -ne 0 ]
  then
      echo "**** Test $TEST_NO FAILED - compressed output didin't match expected output"
      FAIL=1
      return 1
  fi

  # And it shouldn't have printed anything.
  if [ -s stderr.txt ] || [ -s stdout.txt ]
  then
      echo "**** Test $TEST_NO FAILED - shouldn't print anything to stdout or stderr"
      FAIL=1
      return 1
  fi

  # Now, see if it uncompresses back to the original.
  if [ "$ALT_WORDS" == "" ]
  then
      echo "Test $TEST_NO: ./unpack compressed.raw output.txt > stdout.txt 2> stderr.txt"
      ./unpack compressed.raw output.txt > stdout.txt 2> stderr.txt
      STATUS=$?
  else
      echo "Test $TEST_NO: ./unpack compressed.raw output.txt $ALT_WORDS > stdout.txt 2> stderr.txt"
      ./unpack compressed.raw output.txt > stdout.txt $ALT_WORDS 2> stderr.txt
      STATUS=$?
  fi

  # Make sure the uncompressed output matches the original input
  diff -q output.txt input_$TEST_NO.txt >/dev/null 2>&1
  if [ $? -ne 0 ]
  then
      echo "**** Test $TEST_NO FAILED - uncompressed output didin't match original input"
      FAIL=1
      return 1
  fi

  # Program should have succeeded.
  if [ $STATUS -ne 0 ]
  then
      echo "**** Test $TEST_NO FAILED - incorrect exit status. Expected: 0 Got: $STATUS"
      FAIL=1
      return 1
  fi

  # And it shouldn't have printed anything.
  if [ -s stderr.txt ] || [ -s stdout.txt ]
  then
      echo "**** Test $TEST_NO FAILED - shouldn't print anything to stdout or stderr"
      FAIL=1
      return 1
  fi

  echo "Test $TEST_NO PASS"
  return 0
}

# Run the checke the output of a bad test case, to make sure we get the right error message.
checkerror() {
  TEST_NO=$1
  STATUS=$2

  # Make sure we get the right error message
  diff -q expected_${TEST_NO}_err.txt stderr.txt >/dev/null 2>&1
  if [ $? -ne 0 ]
  then
      echo "**** Test $TEST_NO FAILED - incorrect error message"
      FAIL=1
      return 1
  fi

  # Program should not have succeeded.
  if [ $STATUS -eq 0 ]
  then
      echo "**** Test $TEST_NO FAILED - should have exited unsuccessfully."
      FAIL=1
      return 1
  fi

  # And it shouldn't have printed anything to standard output.
  if [ -s stdout.txt ]
  then
      echo "**** Test $TEST_NO FAILED - shouldn't print anything to stdout"
      FAIL=1
      return 1
  fi

  echo "Test $TEST_NO PASS"
  return 0
}


# Run successfule test cases
runtest 1 ""
runtest 2 ""
runtest 3 ""
runtest 4 ""
runtest 5 ""
runtest 6 "altwords.txt"

# Tests for bad input.
rm -f compressed.raw output.txt stdout.txt stderr.txt
echo "Test 7: ./pack input_7.txt compressed.raw > stdout.txt 2> stderr.txt"
./pack input_7.txt compressed.raw > stdout.txt 2> stderr.txt
STATUS=$?
checkerror 7 $STATUS

rm -f compressed.raw output.txt stdout.txt stderr.txt
echo "Test 8: ./pack input_8.txt compressed.raw badlist_8.txt > stdout.txt 2> stderr.txt"
./pack input_8.txt compressed.raw badlist_8.txt > stdout.txt 2> stderr.txt
STATUS=$?
checkerror 8 $STATUS

rm -f compressed.raw output.txt stdout.txt stderr.txt
echo "Test 9: ./unpack input_9.txt output.txt badlist_9.txt > stdout.txt 2> stderr.txt"
./unpack input_9.txt output.txt badlist_9.txt > stdout.txt 2> stderr.txt
STATUS=$?
checkerror 9 $STATUS

rm -f compressed.raw output.txt stdout.txt stderr.txt
echo "Test 10: ./pack input_10.txt compressed.raw > stdout.txt 2> stderr.txt"
./pack input_10.txt compressed.raw > stdout.txt 2> stderr.txt
STATUS=$?
checkerror 10 $STATUS

rm -f compressed.raw output.txt stdout.txt stderr.txt
echo "Test 11: ./unpack not-enough-arguments > stdout.txt 2> stderr.txt"
./unpack not-enough-arguments > stdout.txt 2> stderr.txt
STATUS=$?
checkerror 11 $STATUS

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  exit 0
fi
