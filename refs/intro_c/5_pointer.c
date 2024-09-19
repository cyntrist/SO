#include <stdio.h>
void dosomething(int *ptr);

void main(void)
{
	int *p;
	dosomething(p)
	printf(" %d\n", *p);
	 /* will this work ? */
}

/* passed and returned by reference */
void dosomething(int *ptr)
{
	int temp=32+12;
	*ptr = temp;
}
