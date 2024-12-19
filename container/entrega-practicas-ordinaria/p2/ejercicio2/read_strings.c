#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor
 *
 * The loadstr() function must allocate memory from the heap to store
 * the contents of the string read from the FILE.
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc())
 *
 * Returns: !=NULL if success, NULL if error
 */
char *loadstr(FILE *file)
{
	long ini = ftell(file); // posicion inicial de contador del archivo

	int length = 0;
	int c;
	while ((c =fgetc(file)) != '\0' && c != EOF)
		length++; // longitud de la palabra en caracteres aka bytes

	fseek(file, ini, SEEK_SET); // vuelve al principio del archivo y limpia el EOF si procede

	char *string;
	if (!(string = (char*) malloc(length + 1)))
	{ // https://www.ibm.com/docs/en/xl-c-and-cpp-aix/16.1?topic=promotions-boolean-conversions
		perror("Memory allocation error");
		exit(EXIT_FAILURE);
	}

	if ((fread(string, 1, length + 1, file)) != length + 1)
		return NULL;

	return string; // devuelve un puntero al string leido
}

int main(int argc, char *argv[])
{
	FILE *file;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
		exit(1);
	}

	if ((file = fopen(argv[1], "rb")) == NULL)
		err(2, "The input file %s could not be opened", argv[1]);



	// he estado leyendo que hacer while(!feof()) es mala practica y 
	// encima no estoy haciendo uso del retorno NULL de la funcion loadstr()
	// while (!feof(file))
	// {
	// 	char *line = loadstr(file);
	// 	printf("%s\n", line);
	// 	free(line);
	// 	if (feof(file))
	// 		break;
	// }

	// Esto no me funciona, no imprime infinitamente pero el programa nunca acaba 
	// entiendo que es porque libero line cada vez
	// while (line != NULL)
	// {
	// 	line = loadstr(file);
	// 	printf("%s\n", line);
	// 	free(line);
	// }


	char * line;
	while ((line = loadstr(file)) != NULL)
	{
		printf("%s\n", line);
		free(line);
	}


	fclose(file);
	printf("Read of file: %s\n", argv[1]);
	return 0;
}
