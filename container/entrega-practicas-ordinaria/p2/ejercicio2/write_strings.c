#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

// fopen(),fclose(), fread(), fwrite(), fseek() y malloc()

int main(int argc, char *argv[])
{
	FILE *file;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s <file_name> <content> ...\n", argv[0]);
		exit(1);
	}

	if ((file = fopen(argv[1], "wb")) == NULL)
		err(2, "The input file %s could not be opened", argv[1]);

	int i;
	for (i = 2; i < argc; i++) 
		fwrite(argv[i],  strlen(argv[i]) + 1, 1, file);

	fclose(file);
	printf("Written to file: %s\n", argv[1]);
	return 0;
}
