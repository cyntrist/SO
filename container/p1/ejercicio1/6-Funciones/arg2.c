#include <stdio.h>

/* Structured data type */
struct _complex_ {
	float re;
	float im;
};

/* Forward declaration */
int sum(int *pa, int *pb);
void * sumC( struct _complex_ *c, struct _complex_ *a,  struct _complex_ *b);

int main(void)
{
	int x = 4, y = 5;
	int* ptr = &y;
	struct _complex_ xc = {.re = 1.0, .im = 2.0};
	struct _complex_ yc = {.re = 3.0, .im = 1.0};
	struct _complex_ *zc;


	printf("Complex numbers addition. (%f,%f i) + (%f,%f i) = ",
			xc.re, xc.im, yc.re, yc.im);

	sumC(zc, &xc, &yc);
	printf("(%f,%f i)\n", zc->re, zc->im);

	int total = sum(&x,ptr);
	printf("Integer addition:  x +y = %d + %d = %d \n", x, y, total);
	printf("xc = (%f,%f i)  yc = (%f,%f i) zc = (%f,%f i)\n",
			xc.re, xc.im, yc.re, yc.im, zc->re, zc->im);
	return 0;
}

int sum(int *pa, int *pb)
{
	/* args passed by reference */
	int c = *pa + *pb;

	*pa = 7;
	*pb = 8;
	return c;  /* return by value */
}

void * sumC(struct _complex_ *r, struct _complex_* a,  struct _complex_* b)
{
	r->re = a->re + b->re;
	r->im = a->im + b->im;

	// We modify the first argument
	a->re = 12.5;
	a->im = 13.4;
}
