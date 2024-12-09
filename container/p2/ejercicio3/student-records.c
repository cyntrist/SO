#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"
#include "ctype.h"
#include <getopt.h>

/* Assume lines in the text file are no larger than 100 chars */
#define MAXLEN_LINE_FILE 100

void print_student(int *i, student_t *student)
{
	printf("[Entry #%d]\n", *i);
	printf("    student_id = %d\n", student->student_id);
	printf("    NIF = %s\n", student->NIF);
	printf("    first_name = %s\n", student->first_name);
	printf("    last_name = %s\n", student->last_name);
	free(student->first_name);
	free(student->last_name);
}

/// @brief Método auxiliar a la PARTE A
/// @param file fichero de estudiantes
/// @param num num de estudiantes a leer
/// @return array de estudiantes leidos
student_t *read_student_text_file(FILE *file, int *num)
{
	// Read the number of records
	if (fscanf(file, "%d", num) != 1 || *num < 0)
	{
		fprintf(stderr, "Error reading number of records\n");
		return NULL;
	}

	// Allocate memory for student array
	student_t *students_array = malloc(*num * sizeof(student_t));
	if (students_array == NULL)
	{
		fprintf(stderr, "Memory allocation error\n");
		return NULL;
	}

	// Read each student record
	for (int i = 0; i < *num; ++i)
	{
		if (fscanf(file, "%d:%9[^:]:%m[^:]:%m[^\n]", &students_array[i].student_id, students_array[i].NIF, &students_array[i].first_name, &students_array[i].last_name) != 4)
		{
			fprintf(stderr, "Error reading student record\n");
			free(students_array); // Free memory allocated so far
			return NULL;
		}
	}

	return students_array;
}

/// @brief PART A
/// @param path - dirección al directorio
/// @return código de salida
int print_text_file(char *path)
{
	FILE *file = fopen(path, "r");
	if (!file)
	{
		printf("Could not open file at %s\n", path);
		exit(EXIT_FAILURE);
	}

	int num_records;
	student_t *students = read_student_text_file(file, &num_records);
	if (students == NULL)
	{
		fclose(file);
		return EXIT_FAILURE;
	}

	for (int i = 0; i < num_records; ++i)
	{
		print_student(&i, &students[i]);
		// printf("[Entry #%d]\n", i);
		// printf("    student_id = %d\n", students[i].student_id);
		// printf("    NIF = %s\n", students[i].NIF);
		// printf("    first_name = %s\n", students[i].first_name);
		// printf("    last_name = %s\n", students[i].last_name);
		// free(students[i].first_name);
		// free(students[i].last_name);
	}

	free(students);
	fclose(file);
	return 0;
}






/// @brief Método auxiliar a la PARTE C
/// @param file -> fichero de estudiantes
/// @param num -> número de estudiantes
/// @return array de estudiantes leidos
student_t *read_student_binary_file(FILE *file, int *num)
{
	if (fread(num, sizeof(int), 1, file) != 1 || *num <= 0)
	{
		fprintf(stderr, "Error reading number of records\n");
		return NULL;
	}

	student_t *students = malloc(sizeof(student_t) * *num);
	if (students == NULL)
	{
		fprintf(stderr, "Memory allocation error\n");
		return NULL;
	}

	for (int i = 0; i < *num; i++)
	{
		// ID
		if (fread(&students[i].student_id, sizeof(int), 1, file) != 1)
		{
			fprintf(stderr, "Could not read student id.\n");
			free(students);
			return NULL;
		}

		char buff;
		int j = 0;
		// NIF
		while ((buff = fgetc(file)) != '\0' && j < MAX_CHARS_NIF)
			students[i].NIF[j++] = buff;
		students[i].NIF[j] = '\0';

		j = 0;
		// NOMBRE Y APELLIDO
		students[i].first_name = malloc(sizeof(char) * MAXLEN_LINE_FILE);
		while ((buff = fgetc(file)) != '\0' && j < MAXLEN_LINE_FILE)
			students[i].first_name[j++] = buff;
		students[i].first_name[j] = '\0';

		j = 0;
		students[i].last_name = malloc(sizeof(char) * MAXLEN_LINE_FILE);
		while ((buff = fgetc(file)) != '\0' && j < MAXLEN_LINE_FILE)
			students[i].last_name[j++] = buff;
		students[i].last_name[j] = '\0';
	}

	return students;
}

/// @brief PART C
/// @param path - dirección al directorio
/// @return código de salida
int print_binary_file(char *path)
{
	FILE *file = fopen(path, "rb");
	if (!file)
	{
		printf("Could not open file at %s\n", path);
		exit(EXIT_FAILURE);
	}

	int num_records;
	student_t *students = read_student_binary_file(file, &num_records);
	if (students == NULL)
	{
		fclose(file);
		return EXIT_FAILURE;
	}

	for (int i = 0; i < num_records; ++i)
		print_student(&i, &students[i]);

	free(students);
	fclose(file);

	return 0;
}








/// @brief PART B
/// @param input_file
/// @param output_file
/// @return código de salida
int write_binary_file(char *input_file, char *output_file)
{
	FILE *input = fopen(input_file, "r");
	if (!input)
	{
		fprintf(stderr, "Could not open input file.\n");
		exit(EXIT_FAILURE);
	}
	FILE *output = fopen(output_file, "wb");
	if (!output)
	{
		fprintf(stderr, "Could not open output file.\n");
		fclose(input);
		exit(EXIT_FAILURE);
	}

	int n = 0;
	student_t *students;

	int c = fgetc(input);
	ungetc(c, input);

	if (isdigit(c))
		students = read_student_text_file(input, &n);
	else
		students = read_student_binary_file(input, &n);

	if (n <= 0)
	{
		fprintf(stderr, "Error while reading number of students\n");
		fclose(input);
		fclose(output);
		exit(EXIT_FAILURE);
	}

	// writing n
	fwrite(&n, sizeof(int), 1, output);

	// writing students
	for (int i = 0; i < n; i++)
	{
		fwrite(&students[i].student_id, sizeof(int), 1, output); // id

		fwrite(students[i].NIF, sizeof(char), strlen(students[i].NIF) + 1, output); // nif
		
		fwrite(students[i].first_name, sizeof(char), strlen(students[i].first_name) + 1, output); // nombre

		fwrite(students[i].last_name, sizeof(char), strlen(students[i].last_name) + 1, output); // apellido

		free(students[i].first_name);
		free(students[i].last_name);
	}

	printf("%d records successfully written to %s.\n", n, output_file);

	free(students);
	fclose(input);
	fclose(output);
	return 0;
}








/// @brief esto es una locura
/// @param file_path
/// @param new_records
/// @param num_new_records
/// @return
int add_records_to_file(char *file_path, char **new_records, int num_new_records)
{
	FILE *file = fopen(file_path, "r");
	if (file == NULL)
	{
		fprintf(stderr, "Error opening file %s\n", file_path);
		return EXIT_FAILURE;
	}

	// Leer el número actual de registros
	int num_records;
	if (fscanf(file, "%d\n", &num_records) != 1)
	{
		fprintf(stderr, "Error reading number of records\n");
		fclose(file);
		return EXIT_FAILURE;
	}

	// Crear un archivo temporal
	char temp_path[] = "/tmp/tempfileXXXXXX";
	int temp_fd = mkstemp(temp_path);

	if (temp_fd == -1)
	{
		fprintf(stderr, "Error creating temporary file\n");
		fclose(file);
		return EXIT_FAILURE;
	}
	FILE *temp_file = fdopen(temp_fd, "w");
	if (temp_file == NULL)
	{
		fprintf(stderr, "Error opening temporary file\n");
		fclose(file);
		return EXIT_FAILURE;
	}

	// Escribir el número actualizado de registros en el archivo temporal
	fprintf(temp_file, "%d\n", num_records + num_new_records);

	// Copiar el resto del archivo original al archivo temporal
	char buffer[1]; // 1 o max_record_length
	size_t bytes_read;
	while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
	{
		fwrite(buffer, 1, bytes_read, temp_file);
	}

	// Verificar si hay una línea vacía al final del archivo original
	fseek(file, -1, SEEK_END);
	int last_char = fgetc(file);
	if (last_char != '\n')
	{
		// No hay una línea vacía al final, agregar un salto de línea
		fprintf(temp_file, "\n");
	}

	// Escribir los nuevos registros en el archivo temporal
	for (int i = 0; i < num_new_records; ++i)
	{
		fprintf(temp_file, "%s\n", new_records[i]);
	}

	// Cerrar el archivo original y el archivo temporal
	fclose(file);
	fclose(temp_file);

	// Reabrir el archivo original en modo de escritura para borrar su contenido
	file = fopen(file_path, "w");
	if (file == NULL)
	{
		fprintf(stderr, "Error reopening file %s\n", file_path);
		return EXIT_FAILURE;
	}

	// Reabrir el archivo temporal en modo de lectura
	temp_file = fopen(temp_path, "r");
	if (temp_file == NULL)
	{
		fprintf(stderr, "Error reopening temporary file\n");
		fclose(file);
		return EXIT_FAILURE;
	}

	// Copiar el contenido del archivo temporal al archivo original
	int ch;
	while ((ch = fgetc(temp_file)) != EOF)
	{
		fputc(ch, file);
	}

	// Cerrar los archivos y eliminar el archivo temporal
	fclose(file);
	fclose(temp_file);
	remove(temp_path);

	printf("%d records written successfully to file %s\n", num_new_records, file_path);
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	int ret_code, opt;
	struct options options;

	/* Initialize default values for options */
	options.input_file = "students-db.txt";
	options.output_file = "stdout";
	options.action = NONE_ACT;
	ret_code = 0;

	/* Parse command-line options (incomplete code!) */
	while ((opt = getopt(argc, argv, "hi:po:ba")) != -1)
	{
		switch (opt)
		{
		case 'h':
			fprintf(stderr, "Usage: %s [ -h | -p | -i file ]\n", argv[0]);
			exit(EXIT_SUCCESS);
		case 'i':
			options.input_file = optarg;
			break;
		case 'p':
			options.action = PRINT_TEXT_ACT;
			break;
		case 'o':
			options.action = WRITE_BINARY_ACT;
			options.output_file = optarg;
			break;
		case 'b':
			options.action = PRINT_BINARY_ACT;
			break;
		case 'a':
			options.action = ADD_RECORD_ACT;
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}

	if (options.input_file == NULL)
	{
		printf("holaa");
		fprintf(stderr, "aaaaaaMust specify one record file as an argument of -i\n");
		exit(EXIT_FAILURE);
	}

	switch (options.action)
	{
	case NONE_ACT:
		fprintf(stderr, "bbbbbbbMust indicate one of the following options: -p, -o, -b \n");
		ret_code = EXIT_FAILURE;
		break;
	case PRINT_TEXT_ACT:
		/* Part A */
		ret_code = print_text_file(options.input_file);
		break;
	case WRITE_BINARY_ACT:
		/* Part B */
		ret_code = write_binary_file(options.input_file, options.output_file);
		break;
	case PRINT_BINARY_ACT:
		/* Part C */
		ret_code = print_binary_file(options.input_file);
		break;
	case ADD_RECORD_ACT:
		/* Part Optional 2 */
		if (argc - optind < 1)
		{
			fprintf(stderr, "No records provided to add\n");
			exit(EXIT_FAILURE);
		}
		ret_code = add_records_to_file(options.input_file, &argv[optind], argc - optind);
		break;
	default:
		break;
	}
	exit(ret_code);
}
