#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char *argv[])
{
	FILE *file = NULL;
	int c;
	size_t n = 1;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2, "The input file %s could not be opened", argv[1]);

	/* Read file byte by byte */
	// while ((c = getc(file)) != EOF) {
	// 	/* Print byte to stdout */
	// 	ret=putc((unsigned char) c, stdout);

	// 	if (ret==EOF){
	// 		fclose(file);
	// 		err(3,"putc() failed!!");
	// 	}
	// }

	while ((n = fread(&c, sizeof(c), 1, file)) == 1) {
		fwrite(&c, sizeof(c), 1, stdout);

		if (n == 0) {
			fclose(file);
			err(3,"fread() failed!!");
		}
	}
	fwrite(&"\n", 1, 1, stdout);

	fclose(file);
	return 0;
}
