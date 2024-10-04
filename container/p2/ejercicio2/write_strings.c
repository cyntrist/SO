#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

// fopen(),fclose(), fread(), fwrite(), fseek() y malloc()

int main(int argc, char *argv[])
{
	FILE *file;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <file_name> <content> <content>\n", argv[0]);
		exit(1);
	}

	if ((file = fopen(argv[1], "w")) == NULL)
		err(2, "The input file %s could not be opened", argv[1]);







	while ((n = fread(&c, sizeof(c), 1, file)) == 1)
	{
		fwrite(&c, sizeof(c), 1, stdout);

		if (n == 0)
		{
			fclose(file);
			err(3, "fread() failed!!");
		}
	}
	fwrite(&"\0", 1, 1, stdout);






	fclose(file);
	return 0;
}
