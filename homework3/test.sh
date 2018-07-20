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
# its output and exit status against what's expected.
runtest() {
  TEST_NO=$1
  EX_STATUS=$2

  rm -f output.pgm stderr.txt stdout.txt
  # Handle the last test case as a special case.  This is kind
  # of ugly.
  if [ $TEST_NO -ne 11 ]
  then
      ./drawing input_$TEST_NO.txt output.pgm > stdout.txt 2> stderr.txt
  else
      ./drawing bad command-line arguments > stdout.txt 2> stderr.txt
  fi
      
  STATUS=$?

  if [ $EX_STATUS -eq 0 ]
  then
      # Program should have succeeded
      if [ $STATUS -ne 0 ]
      then
	  echo "**** Test $TEST_NO FAILED - incorrect exit status. Expected: $EX_STATUS Got: $STATUS"
	  FAIL=1
	  return 1
      fi

      # Terminal output should be empty
      if [ -s stdout.txt ]
      then
	  echo "**** Test $TEST_NO FAILED - shouldn't be any output on standard out"
	  FAIL=1
	  return 1
      fi

      if [ -s stderr.txt ]
      then
	  echo "**** Test $TEST_NO FAILED - shouldn't be any output on standard err"
	  FAIL=1
	  return 1
      fi

      # Make sure the output image matches.
      diff -q expected_$TEST_NO.pgm output.pgm >/dev/null 2>&1
      if [ $? -ne 0 ]
      then
	  echo "**** Test $TEST_NO FAILED - output image doesn't match expected image"
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

      # Terminal output should be empty
      if [ -s stdout.txt ]
      then
	  echo "**** Test $TEST_NO FAILED - shouldn't be any output on standard out"
	  FAIL=1
	  return 1
      fi

      # Check the error message (don't care about the image in this case)
      # Make sure the output image matches.
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
runtest 1 0
runtest 2 0
runtest 3 0
runtest 4 0
runtest 5 0
runtest 6 0
runtest 7 1
runtest 8 1
runtest 9 1
runtest 10 1
runtest 11 1

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  exit 0
fi
