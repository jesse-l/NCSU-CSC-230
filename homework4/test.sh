#!/bin/bash
FAIL=0

# make a fresh copy of the target program
make clean
make
if [ $? -ne 0 ]; then
  echo "**** Make (compilation) FAILED"
  FAIL=1
fi

# Function to run the program against a test case, checking
# its output, exit status and error output against what's expected.
runtest() {
  TEST_NO=$1
  EX_STATUS=$2
  TEST_MODE=$3

  rm -f trace.txt stderr.txt
  # Test mode tells us whether this is a test from a file given
  # on the command line, 0, a script from standard input, 1, or bad
  # command-line arguments, 2.  Kind of ugly but it helps avoid duplicating
  # code
  if [ $TEST_MODE -eq 0 ]
  then
      echo "Test $TEST_NO: ./explorer input_$TEST_NO.txt > trace.txt 2> stderr.txt"
      ./explorer input_$TEST_NO.txt > trace.txt 2> stderr.txt
  elif [ $TEST_MODE -eq 1 ]
  then
      echo "Test $TEST_NO: ./explorer < input_$TEST_NO.txt > trace.txt 2> stderr.txt"
      ./explorer < input_$TEST_NO.txt > trace.txt 2> stderr.txt
  else
      echo "Test $TEST_NO: ./explorer too many command-line arguments > trace.txt 2> stderr.txt"
      ./explorer too many command-line arguments > trace.txt 2> stderr.txt
  fi
      
  STATUS=$?

  # Make sure the output matches the expected output
  diff -q expected_$TEST_NO.txt trace.txt >/dev/null 2>&1
  if [ $? -ne 0 ]
  then
      echo "**** Test $TEST_NO FAILED - program output didin't match expected output"
      FAIL=1
      return 1
  fi

  if [ $EX_STATUS -eq 0 ]
  then
      # Program should have succeeded
      if [ $STATUS -ne 0 ]
      then
	  echo "**** Test $TEST_NO FAILED - incorrect exit status. Expected: $EX_STATUS Got: $STATUS"
	  FAIL=1
	  return 1
      fi

      if [ -s stderr.txt ]
      then
	  echo "**** Test $TEST_NO FAILED - shouldn't be any output on standard err"
	  FAIL=1
	  return 1
      fi
  else
      # Program should have exited unsuccessfully.
      if [ $STATUS -eq 0 ]
      then
	  echo "**** Test $TEST_NO FAILED - incorrect exit status. Expected unsuccessful exit status, got successful."
	  FAIL=1
	  return 1
      fi

      # Check for the right error message.
      diff -q expected_err_$TEST_NO.txt stderr.txt >/dev/null 2>&1
      if [ $? -ne 0 ]
      then
	  echo "**** Test $TEST_NO FAILED - didn't print exactly the right error message"
	  FAIL=1
	  return 1
      fi
  fi

  echo "Test $TEST_NO PASS"
  return 0
}

# Run each of the test cases
runtest 1 0 0
runtest 2 0 0
runtest 3 0 1
runtest 4 0 0
runtest 5 0 0
runtest 6 0 0
runtest 7 1 0
runtest 8 1 0
runtest 9 1 0
runtest 10 1 2

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  exit 0
fi
