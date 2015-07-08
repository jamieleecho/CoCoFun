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
    qsort2(arr, low, (int)(j - 1));
    qsort2(arr, (int)(j + 1), high);
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


int main() {
  // Do stuff required to initialize CoCo
  initCoCoSupport();
  setHighSpeed(TRUE);

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
  printf("***   HAVE FUN USING CMOC!   ***");
  printf("********************************\n");

  return 0;
}
