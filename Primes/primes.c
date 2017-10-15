#if 1
// Based on Basic09 code by James Jones on Facebook
#include <cmoc.h>
#include <coco.h>
typedef byte bool;
#else
#include <stdio.h>
typedef unsigned short uint16_t;
#define TRUE 1
#define FALSE 0
uint16_t getTimer() { return 0; }
void setTimer(uint16_t t) { }
typedef unsigned char bool;
#endif

#define NUM_PRIMES 4000
uint32_t primes[NUM_PRIMES];


bool primetest(uint32_t *primes, uint32_t *endprime, uint32_t candidate) {
  for(uint32_t *ii=primes; ii<endprime; ++ii) {
    uint32_t prime = *ii;
    uint32_t quot = candidate / prime;
    if (quot < prime)
      return TRUE; 
    if (quot * prime == candidate)
      return FALSE;
  }
  return TRUE;
}


void primecalc(uint32_t *primes, uint32_t num) {
  if (num == 0) {
    return;
  }

  *primes = 2;
  uint32_t *maxprime = primes + num;
  uint32_t *current_prime = primes + 1;
  uint32_t *primes_plus_1 = current_prime;
  uint32_t candidate = 3;
  uint32_t nprimes = 1;

  while(current_prime < maxprime) {
    if (primetest(primes_plus_1, current_prime, candidate)) {
      *current_prime = candidate;
      ++current_prime;
      ++nprimes;
      if ((nprimes & 3) == 0) {
        printf("%lu     %lu     %lu     %lu     %u\n",
               *(current_prime - 4), *(current_prime - 3),
               *(current_prime - 2), *(current_prime - 1),
               getTimer());
      }
    }
    candidate = candidate + 2;
  }
}


int main() {
  setTimer(0);
  primecalc(primes, sizeof(primes)/sizeof(primes[0]));
  printf("Final time: %u\n", getTimer()); 
  return 0;
}
