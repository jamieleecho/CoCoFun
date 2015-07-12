/*
  Foo.c
  
  This software is for demonstration purposes only. Use at your own risk.

  This program demonstrates many of the CMOC compiler's capabilities.
  
  Among other things, this program plays a neat song named "Cadnza" that
  can be found in William Barden Junior's book "TRS-80 Color Computer
  and MC-10 Programs". The song was composed by Anthony (Craig) Verbeck.
  The book is now out of print, but was originally available at Radio Shack
  (26-3195). See http://williambardenjr.com for more information about
  the author.
*/

#include <cmoc.h>
#include <coco.h>

#define qsort(b, s, e) qsort2(b, s, (int)((e) - 1))

/**
 * Sort the contents of buffer.
 * https://www.codingbot.net/2013/01/quick-sort-algorithm-and-c-code.html
 * @param arr[in/out] input buffer
 * @param low[in] starting point of buffer
 * @param high[in] ending point of buffer
 */
void qsort2(byte *arr, int low, int high) {
  int pivot, j, i;
  byte temp;
  
  if (low<high) {
    pivot = low;
    i = low;
    j = high;
    
    while(i<j) {
      while((arr[i]<=arr[pivot])&&(i<high)) {
	i++;
      }
      
      while(arr[j]>arr[pivot]) {
	j--;
      }
      
      if (i<j) { 
	temp=arr[i];
	arr[i]=arr[j];
	arr[j]=temp;
      }
    }
 
    temp=arr[pivot];
    arr[pivot]=arr[j];
    arr[j]=temp;
    qsort2(arr, low, j - 1);
    qsort2(arr, j + 1, high);
  }
}


/**
 * Sort the contents of buffer.
 * @param buffer[in/out] input buffer
 * @param start[in] starting point of buffer
 * @param end[in] ending point of buffer
 */
void bsort(byte *buffer, int start, int end) {
  int ii, jj;
  for(ii=start; ii<end; ii++) {
    byte c1 = buffer[ii];
    for(jj=ii + 1; jj<end; jj++) {
      byte c2 = buffer[jj];
      if (c2 < c1) {
	buffer[ii] = c2;
	buffer[jj] = c1;
	c1 = c2;
      }
    }
  }
}


/**
 * Prints a message telling the user to press a key to continue.
 * Waits for a key to be pressed before returning.
 */
void anykey() {
  if (getTextMode() == 32)
    printf("PRESS ANY KEY TO CONTINUE... ");
  else
    printf("Press any key to continue... ");
  waitkey(TRUE);
}


/**
 * Data taken almost directly from the Cadnza program.
 */
byte data[] = {
  // Data
  125, 147, 170, 147,
  140, 159, 176, 159,
  147, 170, 185, 170,
  159, 176, 193, 176,
  78, 117, 140, 117,
  89, 125, 147, 125,
  108, 140, 159, 140,
  193, 204, 216, 204,
  
  // Melody
  204, 200, 193, 185,
  159, 170, 176, 185,
  204, 200, 193, 204,
  210, 204, 200, 216,
  218, 216, 210, 204,
  200, 204, 210, 216,
  210, 204, 200, 193,
  204, 200, 193, 185,
  
  // C Part
  176, 147, 125, 108,
  89, 125, 140, 147,
  159, 176, 170, 193,
  185, 200, 210, 216,
  218, 227, 232, 231,
  227, 218, 223, 210,
  204, 193, 185, 176,
  170, 147, 125, 89,
  
  0
};


/** chord data */
byte cd[8][4];

/** melody */
byte md[200];

/**
 * This function sets up the chord and melody
 * arrays.
 */
void readArrays() {
  int dataIndex = 0;
  for(int ii=0; ii<8; ii++) {
    for(int jj=0; jj<4; jj++, dataIndex++) {
      cd[ii][jj] = data[dataIndex];
    }
  }
  
  int ii = 0;
  do {
    md[ii] = data[dataIndex++];
  } while(md[ii++] > 0);
}

/**
 * Plays a chord.
 *
 * @param s[in] speed
 * @param n[in] number of times to play chord
 * @param c[in] chord to play on [0, 7]
 */
void playChord(int n, byte s, int c) {
  for(int n1 = 0; n1<n; n1++)
    for(int n2 = 0; n2<4; n2++)
      sound(cd[c][n2], s);
}

/**
 * Plays a melody and chord
 * Plays the melody as a first note
 * Uses the same parameters as playChord with the
 * addition of nt which is the note of the melody
 * to start play.
 *
 * @param s[in] speed
 * @param n[in] number of times to play chord
 * @param c[in] chord to play on [0, 7]
 * @param nt[in] note in melody to start play on [0, 199]
 */
void playMelodyAndChord(int n, byte s, int c, int nt) {
  if (md[nt] <= 0) return;
  for(int n1 = 0; n1<n; n1++) {
    for(int n2 = 0; n2<4; n2++) {
      sound(md[nt], s); nt++;
      sound(cd[c][n2], s);
      if (md[nt] <= 0) return;
    }
  }
}

/** Must be called before cadnza */
void setupCadnza() {
  // read arrays
  readArrays();
}


/** Each loop plays the song once */
void playCadnza() {
  // Start playing the song
  int n = 4;
  byte s = 4;
  byte c = 0;
  playChord(n, s, c);
  c = 5; playChord(n, s, c);
  c = 4; n = 2; playChord(n, s, c);
  sound(78, 4); sound(125, 4);
  sound(147, 4); sound(159, 4);
  c = 2; n = 1; playChord(n, s, c);
  
  // Counterpoint
  n = 4;
  c = 0; s = 2; int nt = 0; playMelodyAndChord(n, s, c, nt);
  c = 5; nt = 0; playMelodyAndChord(n, s, c, nt);
  c = 0; nt = 0; playMelodyAndChord(n, s, c, nt);
  c = 5; nt = 0; playMelodyAndChord(n, s, c, nt);
  c = 4; n = 2; nt = 7; playMelodyAndChord(n, s, c, nt);
  sound(78, 2); sound(170, 2);
  sound(125, 2); sound(193, 2);
  sound(147, 2); sound(204, 2);
  sound(159, 2); sound(210, 2);
  nt = 7; c = 2; n = 1; playMelodyAndChord(n, s, c, nt);
  s = 1;
  n = 8; nt = 0; c = 0;
  playMelodyAndChord(n, s, c, nt);
  c = 5; playMelodyAndChord(n, s, c, nt);
  nt = 0;
  c = 0; playMelodyAndChord(n, s, c, nt);
  c = 5; playMelodyAndChord(n, s, c, nt);
  nt = 0;
  c = 4; playMelodyAndChord(n, s, c, nt);
  nt = 0;
  playMelodyAndChord(n, s, c, nt);
  c = 0; n = 4; nt = 0; playMelodyAndChord(n, s, c, nt);
  c = 3; n = 2; playMelodyAndChord(n, s, c, nt);
  c = 4; playMelodyAndChord(n, s, c, nt);
  c = 0; n = 4; nt = 0; playMelodyAndChord(n, s, c, nt);
  c = 3; n = 2; playMelodyAndChord(n, s, c, nt);
  c = 4; playMelodyAndChord(n, s, c, nt);
  c = 5; playMelodyAndChord(n, s, c, nt);
  c = 6; playMelodyAndChord(n, s, c, nt);
  nt = 0;
  c = 7; n = 8; playMelodyAndChord(n, s, c, nt);
  nt = 0;
  s = 8; n = 1; playMelodyAndChord(n, s, c, nt);
  sound(125, 32);
}


int main() {
  // Do stuff required to initialize CoCo
  initCoCoSupport();
  setHighSpeed(TRUE);
  setupCadnza();

  // Show a title screen
  width(32);
  printf("********************************");
  printf("*** WELCOME TO THE CMOC DEMO ***");
  printf("********************************\n");
  anykey();

  // Show 40 and 80 column displays  
  for(byte ii=1; ii<=2; ii++) {
    width(40 * ii);
    printf("*************************************\n");
    printf("*** CMOC Supports %u column text! ***\n", 40 * ii);
    printf("*************************************\n\n");
    printf("With fancy attributes such as ");
    attr(1, 2, FALSE, FALSE);
    printf("C");
    attr(2, 3, FALSE, FALSE);
    printf("O");
    attr(3, 4, FALSE, FALSE);
    printf("L");
    attr(4, 5, FALSE, FALSE);
    printf("O");
    attr(5, 6, FALSE, FALSE);
    printf("R");
    attr(0, 0, FALSE, FALSE);
    printf(",\n");
    attr(0, 0, FALSE, TRUE);
    printf("Underline");
    attr(0, 0, FALSE, FALSE);
    printf(" and ");
    attr(0, 0, TRUE, FALSE);
    printf("Blink");
    attr(0, 0, FALSE, FALSE);
    printf("!\n\n");
    printf("Combine attributes like so... ");
    attr(1, 2, TRUE, FALSE);
    printf("U");
    attr(2, 3, FALSE, TRUE);
    printf("G");
    attr(3, 4, TRUE, TRUE);
    printf("L");
    attr(4, 5, TRUE, FALSE);
    printf("Y");
    attr(5, 6, FALSE, FALSE);
    printf("!");
    attr(0, 0, FALSE, FALSE);
    printf("\nBut don't feel obligated to do that!\n\n");
    anykey();
  }

  // Sorting demo
  width(32);
  for(byte ii=0; ii<2; ii++) {
    // Nice little title message
    cls(1);
    printf("********************************");
    printf("*** BUBBLE SORT VERSUS QSORT ***");
    printf("********************************\n");

    // Make it user interactive
    if (ii == 0)
      printf("PRESS A KEY TO SEE BUBBLE SORT ");
    else
      printf("PRESS A KEY TO SEE QSORT ");
    waitkey(TRUE);
    
    // More text to make sort interesting
    printf("LOREM IPSUM DOLOR SIT AMET... ");
    printf("THE QUICK BROWN FOX JUMPED OVER\n");
    printf("THE LAZY TYPIST\n\n");

    // Do the sort
    byte *buffer = 0x400;  
    if (ii == 0)
      bsort(buffer, 0, (int)512);
    else
      qsort(buffer, 0, 512);
    anykey();
  }

  // Show high-res screen
  cls(1);
  printf("********************************");
  printf("***  HIRES GRAPHICS SCREENS  ***");
  printf("********************************\n");
  anykey();
  hscreen(4);
  word xx, yy;
  for(xx=0; xx<640; xx++) {
    hset(xx, 0, (byte)(xx & 3));
    hset(xx, 191, (byte)(xx & 3));
  }
  for(yy=0; yy<192; yy++) {
    hset(0, yy, (byte)(yy & 3));
    hset(639, yy, (byte)(yy & 3));
  }
  for(xx=0; xx<640; xx++) {
    hset(xx, ((xx+1)*100)/333 - 1, (byte)(xx & 3));
    hset(xx, 191 - (((xx+1)*100)/333 - 1), (byte)(xx & 3));
  }
  anykey();
  hscreen(0);

  // All done
  cls(1);
  printf("********************************");
  printf("***    PLAY AWESOME SONGS    ***");
  printf("********************************\n");
  setHighSpeed(FALSE);
  playCadnza();

  // All done
  cls(1);
  printf("********************************");
  printf("***   HAVE FUN USING CMOC!   ***");
  printf("********************************\n");

  return 0;
}
