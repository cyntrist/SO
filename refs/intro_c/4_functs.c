#include <stdio.h>
void swap(int, int);

void main(void) {
	int num1 = 5, num2 = 10;
	printf("num1 = %d and num2 = %d\n", num1, num2);
	swap(num1, num2);
	printf("num1 = %d and num2 = %d\n", num1, num2);
}

void swap(int n1, int n2) {
	int temp;
	temp = n1;
	n1 = n2;
	n2 = temp;
}
