#!/bin/bash
FAIL=0

# make a fresh copy of the target program
make clean
make
if [ $? -ne 0 ]; then
  echo "**** Make (compilation) FAILED"
  FAIL=1
fi

# Function to run the program against a (successful) test case.
runtest() {
  TEST_NO=$1

  rm -f output.txt stderr.txt

  echo "Test $TEST_NO: ./interpreter prog_$TEST_NO.txt > output.txt 2> stderr.txt"
  ./interpreter prog_$TEST_NO.txt > output.txt 2> stderr.txt
  STATUS=$?

  # Program should have succeeded.
  if [ $STATUS -ne 0 ]
  then
      echo "**** Test $TEST_NO FAILED - incorrect exit status. Expected: 0 Got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure the output matches the expected output
  diff -q expected_$TEST_NO.txt output.txt >/dev/null 2>&1
  if [ $? -ne 0 ]
  then
      echo "**** Test $TEST_NO FAILED - program output didn't match expected output."
      FAIL=1
      return 1
  fi

  # And it shouldn't have printed any error messages.
  if [ -s stderr.txt ]
  then
      echo "**** Test $TEST_NO FAILED - shouldn't print anything to stderr"
      FAIL=1
      return 1
  fi

  echo "Test $TEST_NO PASS"
  return 0
}

# Check the output of a bad test case, to make sure we get the right error message.
checkerror() {
  TEST_NO=$1
  STATUS=$2

  # Make sure we get the right error message
  diff -q expected_err_${TEST_NO}.txt stderr.txt >/dev/null 2>&1
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

  # The test may be expected to print some output before it fails.
  diff -q expected_$TEST_NO.txt output.txt >/dev/null 2>&1
  if [ $? -ne 0 ]
  then
      echo "**** Test $TEST_NO FAILED - program output didn't match expected output."
      FAIL=1
      return 1
  fi

  echo "Test $TEST_NO PASS"
  return 0
}


# Run successfule test cases
runtest 01
runtest 02
runtest 03
runtest 04
runtest 05
runtest 06
runtest 07
runtest 08
runtest 09
runtest 10
runtest 11

# There's a test_12.txt, but it's too slow to test with every time.

# Tests for error cases.
rm -f output.txt stderr.txt
echo "Test 20: ./interpreter prog_20.txt > output.txt 2> stderr.txt"
./interpreter prog_20.txt > output.txt 2> stderr.txt
STATUS=$?
checkerror 20 $STATUS

rm -f output.txt stderr.txt
echo "Test 21: ./interpreter prog_21.txt > output.txt 2> stderr.txt"
./interpreter prog_21.txt > output.txt 2> stderr.txt
STATUS=$?
checkerror 21 $STATUS

rm -f output.txt stderr.txt
echo "Test 22: ./interpreter prog_22.txt > output.txt 2> stderr.txt"
./interpreter prog_22.txt > output.txt 2> stderr.txt
STATUS=$?
checkerror 22 $STATUS

# missing program input file.
rm -f output.txt stderr.txt
echo "Test 23: ./interpreter prog_23.txt > output.txt 2> stderr.txt"
./interpreter prog_23.txt > output.txt 2> stderr.txt
STATUS=$?
checkerror 23 $STATUS

rm -f output.txt stderr.txt
echo "Test 24: ./interpreter prog_24.txt > output.txt 2> stderr.txt"
./interpreter prog_24.txt > output.txt 2> stderr.txt
STATUS=$?
checkerror 24 $STATUS

rm -f output.txt stderr.txt
echo "Test 25: ./interpreter prog_25.txt > output.txt 2> stderr.txt"
./interpreter prog_25.txt > output.txt 2> stderr.txt
STATUS=$?
checkerror 25 $STATUS

rm -f output.txt stderr.txt
echo "Test 26: ./interpreter prog_26.txt > output.txt 2> stderr.txt"
./interpreter prog_26.txt > output.txt 2> stderr.txt
STATUS=$?
checkerror 26 $STATUS

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  exit 0
fi
