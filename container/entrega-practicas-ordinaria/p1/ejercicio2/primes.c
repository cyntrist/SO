/**
 *  This program calculates the sum of the first n prime
 *  numbers. Optionally, it allows the user to provide as argument the
 *  value of n, which is 10 by default.
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * This function takes an array of integers and returns the sum of its n elements.
 */
int sum(int *arr, int n);

/**
 * This function fills an array with the first n prime numbers.
 */
void compute_primes(int* result, int n);

/**
 * This function returns 1 if the integer provided is a prime, 0 otherwise.
 */
int is_prime(int x);

int main(int argc, char **argv) {
  printf("ARGC %d\n", argc);
  int n = 10; // by default the first 10 primes
  if(argc = 2) {
    n = atoi(argv[1]);
  }
  int* primes = (int*)malloc(n*sizeof(int));
  compute_primes(primes, n);
  
  //int s = 0;
  int s = sum(primes, n);
  printf("The sum of the first %d primes is %d\n", n, s);

  free(primes);
  primes = NULL;
  
  return 0;
}

int sum(int *arr, int n) {
  int i;
  int total = 0;
  printf("TOTAL BEFORE: %d\n", total);
  for(i=0; i<n; i++) {
    total += arr[i];
    printf("TOTAL STEP: %d\n", total);
  }
  return total;
}

void compute_primes(int* result, int n) {
  int i = 0;
  int x = 2;
  while(i < n) {
    if(is_prime(x)) {
      printf("PRIME: %d\n", x);
      result[i] = x;
      i++;
    }
    x++;
  }
  return;
}

int is_prime(int x) {
  if (x == 0 || x == 1)
    return 0;
  if (x == 2)
    return 1;
  int i;
  for (i = 2; i <= x / 2; ++i) {
    if (x % i == 0) {
      return 0;
    }
  }
  return 1;
}
