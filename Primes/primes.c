// Based on Basic09 code by James Jones on Facebook
#include <cmoc.h>
#include <coco.h>
typedef byte bool;

#define NUM_PRIMES 4000
uint16_t primes[NUM_PRIMES];


bool primetest(uint16_t *primes, uint16_t nprimes, uint16_t candidate) {
  for(uint16_t ii=0; ii<nprimes; ii++) {
    uint16_t quot = candidate / primes[ii];
    if (quot < primes[ii])
      return TRUE; 
    if (candidate % primes[ii] == 0)
      return FALSE;
  }
  return FALSE; // Should not get here
}


void primecalc(uint16_t *primes, uint16_t num) {
  primes[0] = 2;
  uint16_t candidate = 3;
  uint16_t nprimes = 1;

  while(nprimes < num) {
    if (primetest(primes, nprimes, candidate)) {
      primes[nprimes++] = candidate;
      if ((nprimes & 3) == 0) {
        printf("%u     %u     %u     %u     %u\n",
               primes[nprimes-4], primes[nprimes-3],
               primes[nprimes-2], primes[nprimes-1],
               getTimer());
      }
    }
    candidate = candidate + 1;
  }
}


int main() {
  setTimer(0);
  primecalc(primes, sizeof(primes)/sizeof(primes[0]));
  printf("Final time: %u\n", getTimer()); 
  return 0;
}
