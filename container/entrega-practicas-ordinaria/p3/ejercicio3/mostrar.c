#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>			  // flags para arhcivos
#include <bits/getopt_core.h> // getopt

struct options
{
	FILE *input_file;
};

int main(int argc, char *argv[])
{
	int opt, N, b_read;
	struct options option;
	option.input_file = fopen("ejemplo.txt", "r");
	int end = 0;

	while ((opt = getopt(argc, argv, "n:e")) != -1)
	{
		switch (opt)
		{
		case 'n':
			N = atoi(optarg);
			break;
		case 'e':
			end = 1;
			break;
		default:
			fprintf(stderr, "Uso: %s [-n N] [-e] <fichero>\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (optind >= argc)
	{
		fprintf(stderr, "Se requiere el nombre del archivo como argumento\n");
		exit(EXIT_FAILURE);
	}

	option.input_file = open(argv[optind], O_RDONLY);
	if (option.input_file == -1)
	{
		perror("Error abriendo el archivo");
		exit(EXIT_FAILURE);
	}

	if (end)
	{
		int off = lseek(option.input_file, -N, SEEK_END);
		if (off == -1)
		{
			perror("Error moviendo el marcador de posición");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		int off = lseek(option.input_file, N, SEEK_SET);
		if (off == -1)
		{
			perror("Error moviendo el marcador de posición");
			exit(EXIT_FAILURE);
		}
	}

	char buffer[1];
	while ((b_read = read(option.input_file, buffer, sizeof(buffer))) > 0)
	{
		if (write(STDOUT_FILENO, buffer, b_read) == -1)
		{
			perror("Error escribiendo en la salida estándar");
			exit(EXIT_FAILURE);
		};
	}

	if (b_read == -1)
	{
		perror("Error leyendo del archivo");
		exit(EXIT_FAILURE);
	}

	if (close(option.input_file) == -1)
	{
		perror("Error cerrando el archivo");
		exit(EXIT_FAILURE);
	}

	return 0;
}
