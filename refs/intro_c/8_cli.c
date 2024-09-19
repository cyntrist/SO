#include <stdio.h>
/* program called with cmd line parameters */
void main(int argc, char *argv[])
{
	int i;
	for (i = 0; i < argc; i++)
		printf("Argument # %d-> %s\n", i, argv[i]);
	/* ex., argv[0] == the name of the program */
}
