#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 512

struct options
{
	FILE *src_file;
	FILE *dest_file;
};

void copy(int fdo, int fdd)
{
	char buffer[BUFFER_SIZE];
	ssize_t b_read, b_written; // signed size para devolver -1;

	while ((b_read = read(fdo, buffer, BUFFER_SIZE)) > 0)
	{
		b_written = write(fdd, buffer, b_read);
		if (b_written != b_read)
		{
			fprintf(stderr, "Error while writting.\n");
			exit(EXIT_FAILURE);
		}
	}

	if (b_read == -1)
	{
		perror("Error leyendo fichero origen");
		exit(EXIT_FAILURE);
	}
}

void copy_regular(struct options option, char *argv[])
{
	printf("COPYING REGULAR\n");

	if ((option.src_file = open(argv[1], O_RDONLY)) == -1)
	{
		fprintf(stderr, "Could not read file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if ((option.dest_file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
	{
		fprintf(stderr, "Could not write file %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	copy(option.src_file, option.dest_file);

	if (close(option.src_file) == -1)
	{
		fprintf(stderr, "Could not write file %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	if (close(option.dest_file) == -1)
	{
		fprintf(stderr, "Could not write file %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}
}

void copy_link(char *src, char *dest)
{
	printf("COPYING LINK\n");

	char *ruta;
	ssize_t b_read;
	struct stat st;

	if (lstat(src, &st) == -1)
	{
		perror("Error obteniendo información del enlace simbólico");
		exit(EXIT_FAILURE);
	}
	ruta = malloc(st.st_size + 1);
	b_read = readlink(src, ruta, st.st_size);
	if (b_read == -1)
	{
		perror("Error leyendo el enlace simbólico");
		free(ruta);
		exit(EXIT_FAILURE);
	}
	ruta[b_read] = '\0';
	if (symlink(ruta, dest) == -1)
	{
		perror("Error creando el enlace simbólico de destino");
		free(ruta);
		exit(EXIT_FAILURE);
	}

	free(ruta);
}

int main(int argc, char *argv[])
{
	int opt;
	struct options option;
	struct stat st;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	lstat(argv[1], &st);
	if (S_ISREG(st.st_mode))
		copy_regular(option, argv);
	else if (S_ISLNK(st.st_mode))
		copy_link(argv[1], argv[2]);

	printf("%s copied successfully to %s\n", argv[1], argv[2]);

	return 0;
}
