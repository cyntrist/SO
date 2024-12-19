/*


#include <stdio.h>
#include <unistd.h> // for getopt() 
#include <getopt.h> // para el optarg
#include <stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE 
#include <string.h>
#include "defs.h"


// Assume lines in the text file are no larger than 100 chars 
#define MAXLEN_LINE_FILE 100

int print_text_file(char *path)
{
	FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", path);
        return EXIT_FAILURE;
    }

    int num_records;
    if (fscanf(file, "%d", &num_records) != 1 || num_records < 0) { // Check if fscanf fails or num_records is negative
        fprintf(stderr, "Error reading number of records\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < num_records; ++i) {
        student_t student;
        fscanf(file, "%d:%9[^:]:%m[^:]:%m[^\n]", &student.student_id, student.NIF, &student.first_name, &student.last_name); // se puede cambiar el %9 por %m pero necesitas un puntero a puntero char por lo que seria &students.NIF
        printf("[Entry #%d]\n", i);
        printf("    student_id = %d\n", student.student_id);
        printf("    NIF = %s\n", student.NIF);
        printf("    first_name = %s\n", student.first_name);
        printf("    last_name = %s\n", student.last_name);
        free(student.first_name); //%m lo que hace es reservar memoria dinamicamente 
        free(student.last_name); //%m lo que hace es reservar memoria dinamicamente 
    }

    fclose(file);
    return EXIT_SUCCESS;
}

// Otra version
int print_text_file(char *path)
{
	// To be completed (part A) 
	FILE *file = fopen(path, "r"); // Abre el archivo para lectura
	if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
	int students;
	if(fread(students,sizeof(int), 1, file) != 4){
		 err(EXIT_FAILURE, "fread failed!!");
	}

	for (int i = 0; i < students; i++)
	{
		char *buffer = NULL;
		char ch;
		int size = 1;
		buffer = malloc(size);

		while(fread(ch,sizeof(char), 1, file) == 1 && ch != ':'){
			buffer[size - 1] = ch;
			size++;
			buffer = realloc(buffer, size); // Ampliar el búfer para que quepan mas caracteres
		}
		printf("%s", buffer);
		printf("%c\n", ':');
		free(buffer);
	}
	
}*/

/*
int print_binary_file(char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", path);
        return EXIT_FAILURE;
    }

    int num_records;
    if (fread(&num_records, sizeof(int), 1, file) != 1 || num_records < 0) {
        fprintf(stderr, "Error reading number of records\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < num_records; ++i) {
        student_t student;
        if (fread(&student.student_id, sizeof(int), 1, file) != 1) {
            fprintf(stderr, "Error reading student ID\n");
            fclose(file);
            return EXIT_FAILURE;
        }
        // Leer NIF, first_name y last_name caracter por caracter hasta encontrar un caracter nulo
        char ch;
        int j = 0;
        while ((ch = fgetc(file)) != '\0' && j < MAX_CHARS_NIF) {
            student.NIF[j++] = ch;
        }
        student.NIF[j] = '\0';

        j = 0;
        student.first_name = malloc(MAXLEN_LINE_FILE * sizeof(char));
        while ((ch = fgetc(file)) != '\0' && j < MAXLEN_LINE_FILE - 1) {
            student.first_name[j++] = ch;
        }
        student.first_name[j] = '\0';

        j = 0;
        student.last_name = malloc(MAXLEN_LINE_FILE * sizeof(char));
        while ((ch = fgetc(file)) != '\0' && j < MAXLEN_LINE_FILE - 1) {
            student.last_name[j++] = ch;
        }
        student.last_name[j] = '\0';

        printf("[Entry #%d]\n", i);
        printf("    student_id = %d\n", student.student_id);
        printf("    NIF = %s\n", student.NIF);
        printf("    first_name = %s\n", student.first_name);
        printf("    last_name = %s\n", student.last_name);

        // Liberar la memoria asignada
        free(student.first_name);
        free(student.last_name);
    }

    fclose(file);
    return EXIT_SUCCESS;
}*/



/*
podríamos haber utilizado fscanf para leer directamente las cadenas de caracteres del archivo binario, 
pero hay un problema. La especificación de formato %m de fscanf solo funciona con archivos de texto, 
no con archivos binarios. La razón es que %m espera leer una cadena de caracteres desde un archivo de 
texto hasta que encuentre un delimitador de espacio en blanco, lo cual no funciona bien con archivos 
binarios que pueden contener cualquier valor de byte, incluidos los valores de byte que se interpretan
 como espacios en blanco.
*/



/*
int write_binary_file(char *input_file, char *output_file) {
    FILE *input = fopen(input_file, "r");
    if (input == NULL) {
        fprintf(stderr, "Error opening input file\n");
        return EXIT_FAILURE;
    }

    FILE *output = fopen(output_file, "wb");
    if (output == NULL) {
        fprintf(stderr, "Error opening output file\n");
        fclose(input);
        return EXIT_FAILURE;
    }

    int num_records = 0;
    student_t student;

    // Read number of records
    if (fscanf(input, "%d", &num_records) != 1) {
        fprintf(stderr, "Error reading number of records\n");
        fclose(input);
        fclose(output);
        return EXIT_FAILURE;
    }
    fwrite(&num_records, sizeof(int), 1, output); // Write number of records to binary file


    // Write student records to binary file
    for (int i = 0; i < num_records; ++i) {
        if (fscanf(input, "%d:%9[^:]:%m[^:]:%m[^\n]", &student.student_id, student.NIF, &student.first_name, &student.last_name) != 4) {
            fprintf(stderr, "Error reading student record\n");
            fclose(input);
            fclose(output);
            return EXIT_FAILURE;
        }
        fwrite(&student.student_id, sizeof(int), 1, output); // Write student ID
        fwrite(student.NIF, sizeof(char), strlen(student.NIF) + 1, output); // Write NIF
        fwrite(student.first_name, sizeof(char), strlen(student.first_name) + 1, output); // Write first name
        fwrite(student.last_name, sizeof(char), strlen(student.last_name) + 1, output); // Write last name
        free(student.first_name);
        free(student.last_name);
    }

	printf("%d student records written successfully to binary file %s\n", num_records, output_file);

    fclose(input);
    fclose(output);
    return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
	int ret_code, opt;
	struct options options;

	// Initialize default values for options 
	options.input_file = NULL;
	options.output_file = NULL;
	options.action = NONE_ACT;
	ret_code = 0;

	// Parse command-line options (incomplete code!) 
	while ((opt = getopt(argc, argv, "hi:po:b")) != -1)
	{
		switch (opt)
		{
		case 'h':
			fprintf(stderr, "Usage: %s [ -h | -i file | -p | -o <output_file> | -b ]\n", argv[0]);
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
		default:
			exit(EXIT_FAILURE);
		}
	}

	if (options.input_file == NULL)
	{
		fprintf(stderr, "Must specify one record file as an argument of -i\n");
		exit(EXIT_FAILURE);
	}

	switch (options.action)
	{
	case NONE_ACT:
		fprintf(stderr, "Must indicate one of the following options: -p, -o, -b \n");
		ret_code = EXIT_FAILURE;
		break;
	case PRINT_TEXT_ACT:
		// Part A 
		ret_code = print_text_file(options.input_file);
		break;
	case WRITE_BINARY_ACT:
		// Part B 
		ret_code = write_binary_file(options.input_file, options.output_file);
		break;
	case PRINT_BINARY_ACT:
		// Part C
		ret_code = print_binary_file(options.input_file);
		break;
	default:
		break;
	}
	exit(ret_code);
}
*/





#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

// Assume lines in the text file are no larger than 100 chars 
#define MAXLEN_LINE_FILE 100

student_t* read_student_text_file(FILE* students, int* nr_entries) {
    // Read the number of records
    if (fscanf(students, "%d", nr_entries) != 1 || *nr_entries < 0) {
        fprintf(stderr, "Error reading number of records\n");
        return NULL;
    }

    // Allocate memory for student array
    student_t* students_array = malloc(*nr_entries * sizeof(student_t));
    if (students_array == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    // Read each student record
    for (int i = 0; i < *nr_entries; ++i) {
        if (fscanf(students, "%d:%9[^:]:%m[^:]:%m[^\n]", &students_array[i].student_id, students_array[i].NIF, &students_array[i].first_name, &students_array[i].last_name) != 4) {
            fprintf(stderr, "Error reading student record\n");
            free(students_array); // Free memory allocated so far
            return NULL;
        }
    }

    return students_array;
}

student_t* read_student_binary_file(FILE* students, int* nr_entries) {
    // Read the number of records
    if (fread(nr_entries, sizeof(int), 1, students) != 1 || *nr_entries < 0) {
        fprintf(stderr, "Error reading number of records\n");
        return NULL;
    }

    // Allocate memory for student array
    student_t* students_array = malloc(*nr_entries * sizeof(student_t));
    if (students_array == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    // Read each student record
    for (int i = 0; i < *nr_entries; ++i) {
        if (fread(&students_array[i].student_id, sizeof(int), 1, students) != 1) {
            fprintf(stderr, "Error reading student ID\n");
            free(students_array); // Free memory allocated so far
            return NULL;
        }
        // Leer NIF, first_name y last_name caracter por caracter hasta encontrar un caracter nulo
        char ch;
        int j = 0;
        while ((ch = fgetc(students)) != '\0' && j < MAX_CHARS_NIF) {
            students_array[i].NIF[j++] = ch;
        }
        students_array[i].NIF[j] = '\0';

        j = 0;
        students_array[i].first_name = malloc(MAXLEN_LINE_FILE * sizeof(char));
        while ((ch = fgetc(students)) != '\0' && j < MAXLEN_LINE_FILE - 1) {
            students_array[i].first_name[j++] = ch;
        }
        students_array[i].first_name[j] = '\0';

        j = 0;
        students_array[i].last_name = malloc(MAXLEN_LINE_FILE * sizeof(char));
        while ((ch = fgetc(students)) != '\0' && j < MAXLEN_LINE_FILE - 1) {
            students_array[i].last_name[j++] = ch;
        }
        students_array[i].last_name[j] = '\0';
    }

    return students_array;
}

int print_text_file(char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", path);
        return EXIT_FAILURE;
    }

    int num_records;
    student_t* students = read_student_text_file(file, &num_records);
    if (students == NULL) {
        fclose(file);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < num_records; ++i) {
        printf("[Entry #%d]\n", i);
        printf("    student_id = %d\n", students[i].student_id);
        printf("    NIF = %s\n", students[i].NIF);
        printf("    first_name = %s\n", students[i].first_name);
        printf("    last_name = %s\n", students[i].last_name);
        free(students[i].first_name);
        free(students[i].last_name);
    }

    free(students);
    fclose(file);
    return EXIT_SUCCESS;
}

int print_binary_file(char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", path);
        return EXIT_FAILURE;
    }

    int num_records;
    student_t* students = read_student_binary_file(file, &num_records);
    if (students == NULL) {
        fclose(file);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < num_records; ++i) {
        printf("[Entry #%d]\n", i);
        printf("    student_id = %d\n", students[i].student_id);
        printf("    NIF = %s\n", students[i].NIF);
        printf("    first_name = %s\n", students[i].first_name);
        printf("    last_name = %s\n", students[i].last_name);
        free(students[i].first_name);
        free(students[i].last_name);
    }

    free(students);
    fclose(file);
    return EXIT_SUCCESS;
}
/*
int write_binary_file(char *input_file, char *output_file) {
    FILE *input = fopen(input_file, "r");
    if (input == NULL) {
        fprintf(stderr, "Error opening input file\n");
        return EXIT_FAILURE;
    }

    FILE *output = fopen(output_file, "wb");
    if (output == NULL) {
        fprintf(stderr, "Error opening output file\n");
        fclose(input);
        return EXIT_FAILURE;
    }

    int num_records = 0;
    student_t* students = read_student_text_file(input, &num_records);
    if (students == NULL) {
        fclose(input);
        fclose(output);
        return EXIT_FAILURE;
    }

    fwrite(&num_records, sizeof(int), 1, output); // Write number of records to binary file

    for (int i = 0; i < num_records; ++i) {
        fwrite(&students[i].student_id, sizeof(int), 1, output); // Write student ID
        fwrite(students[i].NIF, sizeof(char), strlen(students[i].NIF) + 1, output); // Write NIF
        fwrite(students[i].first_name, sizeof(char), strlen(students[i].first_name) + 1, output); // Write first name
        fwrite(students[i].last_name, sizeof(char), strlen(students[i].last_name) + 1, output); // Write last name
        free(students[i].first_name);
        free(students[i].last_name);
    }

    free(students);
    fclose(input);
    fclose(output);

    printf("%d student records written successfully to binary file %s\n", num_records, output_file);

    return EXIT_SUCCESS;
}*/
int write_binary_file(char *input_file, char *output_file) {
    FILE *input = fopen(input_file, "r");
    if (input == NULL) {
        fprintf(stderr, "Error opening input file\n");
        return EXIT_FAILURE;
    }

    FILE *output = fopen(output_file, "wb");
    if (output == NULL) {
        fprintf(stderr, "Error opening output file\n");
        fclose(input);
        return EXIT_FAILURE;
    }

    int num_records = 0;
    student_t* students = NULL;

    // Determine if the input file is text or binary
    int c = fgetc(input);
    ungetc(c, input);

    if (isdigit(c)) {
        // Text file
        students = read_student_text_file(input, &num_records);
    } else {
        // Binary file
        students = read_student_binary_file(input, &num_records);
    }

    if (num_records <= 0) {
        fprintf(stderr, "Error: Invalid number of records\n");
        fclose(input);
        fclose(output);
        return EXIT_FAILURE;
    }

    fwrite(&num_records, sizeof(int), 1, output); // Write number of records to binary file

    for (int i = 0; i < num_records; ++i) {
        fwrite(&students[i].student_id, sizeof(int), 1, output); // Write student ID
        fwrite(students[i].NIF, sizeof(char), strlen(students[i].NIF) + 1, output); // Write NIF
        fwrite(students[i].first_name, sizeof(char), strlen(students[i].first_name) + 1, output); // Write first name
        fwrite(students[i].last_name, sizeof(char), strlen(students[i].last_name) + 1, output); // Write last name
        free(students[i].first_name);
        free(students[i].last_name);
    }

    free(students);
    fclose(input);
    fclose(output);

    printf("%d student records written successfully to binary file %s\n", num_records, output_file);

    return EXIT_SUCCESS;
}

#define MAX_RECORD_LENGTH 255

int add_records_to_file(char *file_path, char **new_records, int num_new_records) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", file_path);
        return EXIT_FAILURE;
    }

    // Leer el número actual de registros
    int num_records;
    if (fscanf(file, "%d\n", &num_records) != 1) {
        fprintf(stderr, "Error reading number of records\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Crear un archivo temporal
    char temp_path[] = "/tmp/tempfileXXXXXX";
    int temp_fd = mkstemp(temp_path);
    /*
    char temp_path[] = "/tmp/tempfileXXXXXX";: En esta línea, declaro una matriz de caracteres llamada temp_path. 
    Esta cadena contiene la ruta al archivo temporal que queremos crear. La ruta típica de los archivos temporales en sistemas 
    Unix-like es /tmp/, pero esta ubicación puede variar según la configuración del sistema. La parte tempfileXXXXXX se utiliza 
    como un patrón para generar un nombre de archivo único. La función mkstemp() reemplazará los X con caracteres aleatorios para 
    garantizar la unicidad del nombre de archivo.

    int temp_fd = mkstemp(temp_path);: Aquí, invoco la función mkstemp(), que crea un archivo temporal único. Toma como 
    argumento la ruta al archivo temporal. La función devuelve un descriptor de archivo (temp_fd) que se puede utilizar para realizar
    operaciones de lectura y escritura en el archivo. Este descriptor de archivo es un identificador único que representa el archivo 
    abierto en el sistema operativo.
    */


    if (temp_fd == -1) {
        fprintf(stderr, "Error creating temporary file\n");
        fclose(file);
        return EXIT_FAILURE;
    }
    FILE *temp_file = fdopen(temp_fd, "w");
    if (temp_file == NULL) {
        fprintf(stderr, "Error opening temporary file\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Escribir el número actualizado de registros en el archivo temporal
    fprintf(temp_file, "%d\n", num_records + num_new_records);

    // Copiar el resto del archivo original al archivo temporal
    char buffer[1]; // 1 o max_record_length
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        fwrite(buffer, 1, bytes_read, temp_file);
    }

    // Verificar si hay una línea vacía al final del archivo original
    fseek(file, -1, SEEK_END);
    int last_char = fgetc(file);
    if (last_char != '\n') {
        // No hay una línea vacía al final, agregar un salto de línea
        fprintf(temp_file, "\n");
    }

    // Escribir los nuevos registros en el archivo temporal
    for (int i = 0; i < num_new_records; ++i) {
        fprintf(temp_file, "%s\n", new_records[i]);
    }

    // Cerrar el archivo original y el archivo temporal
    fclose(file);
    fclose(temp_file);

    // Reabrir el archivo original en modo de escritura para borrar su contenido
    file = fopen(file_path, "w");
    if (file == NULL) {
        fprintf(stderr, "Error reopening file %s\n", file_path);
        return EXIT_FAILURE;
    }

    // Reabrir el archivo temporal en modo de lectura
    temp_file = fopen(temp_path, "r");
    if (temp_file == NULL) {
        fprintf(stderr, "Error reopening temporary file\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Copiar el contenido del archivo temporal al archivo original
    int ch;
    while ((ch = fgetc(temp_file)) != EOF) {
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
	options.input_file = NULL;
	options.output_file = NULL;
	options.action = NONE_ACT;
	ret_code = 0;

	/* Parse command-line options (incomplete code!) */
	while ((opt = getopt(argc, argv, "hi:po:b:a")) != -1)
	{
		switch (opt)
		{
		case 'h':
			fprintf(stderr, "Usage: %s [ -h | -i file | -p | -o <output_file> | -b | -a ]\n", argv[0]);
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
		fprintf(stderr, "Must specify one record file as an argument of -i\n");
		exit(EXIT_FAILURE);
	}

	switch (options.action)
	{
	case NONE_ACT:
		fprintf(stderr, "Must indicate one of the following options: -p, -o, -b \n");
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
        if (argc - optind < 1) {
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