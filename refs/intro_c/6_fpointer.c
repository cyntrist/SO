#include <stdio.h>

void myproc (int d);

void main(void) {
	myproc(10);		/*call myproc with parameter 10*/
}

void myproc (int d){
	printf("D vale %d\n", d);
}
