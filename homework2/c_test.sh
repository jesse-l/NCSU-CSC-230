#!/bin/bash
FAIL=0

# make a fresh copy of the target program
make clean
make comments
if [ $? -ne 0 ]; then
  echo "**** Make (compilation) FAILED"
  FAIL=1
fi

# Function to run the program against a test case, checking
# its output and exit status against what's expected.
runtest() {
  TEST_NO=$1
  EX_STATUS=$2
  LOCALFAIL=0

  rm -f output.txt
  ./comments < c_input_$TEST_NO.txt > output.txt
  STATUS=$?

  if [ $STATUS -ne $EX_STATUS ]; then
    echo "**** Test $TEST_NO FAILED - incorrect exit status. Expected: $EX_STATUS Got: $STATUS"
    FAIL=1
    LOCALFAIL=1
  fi
  
  DIFFREPORT=$(diff -q c_expected_$TEST_NO.txt output.txt)
  if [ $? -ne 0 ]
  then
    echo "**** Test $TEST_NO FAILED - program output didn't match expected output: $DIFFREPORT"
    FAIL=1
    LOCALFAIL=1
  fi

  if [ $LOCALFAIL -eq 0 ]; then
    echo "Test $TEST_NO PASS"
  fi

}

runtest 1 0
runtest 2 0
runtest 3 0
runtest 4 0
runtest 5 101
runtest 6 100

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  exit 0
fi

