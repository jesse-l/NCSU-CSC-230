/**
    @file bits.c
    @author Jesse Liddle - jaliddl2

    This bit is responsible for converting a number into a 9bit
    binary value and writing 8bits to a file and sending the rest
    to be stored in a pendingbit which stores the values of the
    binary that could not be stored at the current time-.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bits.h"

#define NINE 9
#define EIGHT 8
#define SEVEN 7
#define SIX 6
#define FIVE 5
#define FOUR 4
#define THREE 3
#define TWO 2
#define ONE 1
#define ZERO 0

/**
  Write the 9 low-order bits from code to the given file.

  @param code bits to write out, a value betteen 0 and 2^9 - 1.

  @param pending pointer to storage for unwritten bits left over
  from the previous call to writeCode().  After this call, any bits
  that partially fill the next byte will be left in the pending
  struct, to be written in the next call.

  @param fp file we're writing to, opened for writing.
*/
void writeCode( int code, PendingBits *pending, FILE *fp )
{
  int mask1 = 0x0001;//0000 0000 0000 0001
  int mask8 = 0x00FF;//0000 0000 1111 1111
  unsigned int temp = 0;
  
  if ( pending->bits == ZERO ) {
    temp = code & mask8;
    //Writes the temp code out 
    fputc( temp, fp );
    //Mask the opposite bits of mask8 and shifts 8 
    pending->bits = ( code & (~mask8) ) >> EIGHT;
    //Sets number of bits in pending to 1
    pending->bitCount = ONE;
    
  } else {
    //Writes pending bits out if full
    if ( pending->bitCount == EIGHT ) {
      fputc( pending->bits, fp );
      pending->bitCount = ZERO;
    }
    
    int shift = pending->bitCount;
    
    temp = code << shift;
    temp = temp + pending->bits;
    
    fputc( (temp&mask8), fp );
    pending->bits = ( temp & (~mask8) ) >> EIGHT;
    
    int counter = ZERO;
    temp = temp >> EIGHT;
    for ( int i = ZERO; i > EIGHT; i++ ) {
      if ( (( temp >> ONE ) & mask1) == ONE ) 
        counter++;
    }
    
    pending->bitCount = counter;
    
  }
}

/**
  If there are any bits buffered in pending, write them out to the
  given file in the low-order bit positions of a byte, leaving zeros
  in the high-order bits.

  @param pending pointer to storage for unwritten bits left over
  from the most recent call to writeCode().

  @param fp file these bits are to be written to, opened for writing.
*/
void flushBits( PendingBits *pending, FILE *fp )
{
  if ( pending->bitCount != 0 ) {
    unsigned char byte = 0x00;

    byte = byte + pending->bits;

    fputc( byte, fp );
  }
}

/**
  Read and return the next 9-bit code from the given file.

  @param pending pointer to storage for left-over bits read during
  the last call to readCode().

  @param fp file bits are being read from, opened for reading.

  @return value of the 9-bit code read in, or -1 if we reach the
  end-of-file before getting 9 bits.
*/
int readCode( PendingBits *pending, FILE *fp )
{
  //Gets 1 bite from the file
  int byte = fgetc( fp );
#ifdef DEBUG
  printf("Byte is:  %x\n", (unsigned char)byte );
#endif
  
  if ( byte == EOF )
    return EOF;

  unsigned int mask = 0x01FF;
  int number = 0;
  
  if ( pending->bitCount == ZERO ) {
    int byte2 = fgetc( fp );
#ifdef DEBUG
  printf("Byte2 is:  %x\n", (unsigned char)byte2 );
#endif
    int store = (byte2 << EIGHT) + byte;
    number = store & mask;
    
    pending->bits = ( store & (~mask) ) >> NINE;
    pending->bitCount = SEVEN;
  } else {
    int shift = pending->bitCount;
 #ifdef DEBUG
  printf("Byte2 is:  %x\n", pending->bits );
#endif   
    int store = byte << shift;
    store = store + pending->bits;
    
    number = store & mask;
    
    pending->bits = ( store & (~mask) ) >> NINE;
    pending->bitCount--;
    
  }
  
  return number;
}
