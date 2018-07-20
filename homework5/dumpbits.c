/** Helpful program to report the value in every byte of stdin, in binary. */
#include <stdio.h>

int main()
{
  int idx = 0;
  int ch;

  // Read the contents of input.
  while ( ( ch = getchar() ) != EOF ) {
    // Print byte index, followed by its binary representation.
    printf( "%04x ", idx );
    // Start printing bits from the high-order one.
    for ( int i = 7; i >= 0; i-- )
      printf( "%c", ch & ( 0x01 << i ) ? '1' : '0' );
    printf( "\n" );
    idx++;
  }
}
