#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define N 10

void print_array(int *array, int size) {
	for (int i = 0; i < size; i++)
		printf("[ %d] = %d, ", i, array[i]);
	printf("\n-------------------------\n");
}

void copy_array(int src[],int dst[],int size) {
	dst = src;
}

int main() {
	int A[N] = {4,3,8,5,6,9,0,1,7,2};
	int B[N];
	copy_array(A, B, N);
	print_array(B, N);
}
