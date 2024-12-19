
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
/*
 * Loads a string from a file.
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

// Función auxiliar para cargar una cadena de caracteres terminada en '\0' desde el archivo
char* loadstr(FILE* input) {
	/*
	Aqui hay dos opciones añadir al principio long start_position = ftell(input);
	y sustituir el fseek por este fseek(input, start_position, SEEK_SET);
	o utilizar la vesrsion actual, cuidado que en la version actual, si no haces 
	-length -1 no lo imprime bien, habria que hacer -1 tambien ya que se ha desplazado 1 mas 
	por el \0
	*/

    long length = 0;
    char ch;
    while ((ch = fgetc(input)) != EOF && ch != '\0' && ch != '\n') {
        length++;
    }

    // Si hemos llegado al final del archivo, devolvemos NULL
    if (ch == EOF || length == 0 || ch == '\n') {
        return NULL;
    }
	

    fseek(input, -length - 1, SEEK_CUR);

    char* str = (char*)malloc(length + 1);
    if (str == NULL) {
        fprintf(stderr, "Error: No se pudo reservar memoria para la cadena\\n");
        exit(EXIT_FAILURE);
    }

    fread(str, sizeof(char), length, input);
    str[length] = '\0';

    // Leer el siguiente carácter (que debería ser '\0', '\n' o EOF)
    fgetc(input);

    return str;
}


int main(int argc, char* argv[]) {
    FILE* input_file = NULL;

    // Verificar si se proporcionó el nombre del archivo como argumento
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Abrir el archivo para lectura
    if ((input_file = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Leer y mostrar las cadenas del archivo
    char* str;
    while ((str = loadstr(input_file)) != NULL) {
        printf("%s\n", str);
        free(str); // Liberar la memoria asignada para la cadena
    }

    // Cerrar el archivo
    fclose(input_file);

    return 0;
}



/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

 * Loads a string from a file.
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
/*
char *loadstr(FILE *file)
{
	char *buffer = NULL;
    int size = 1; // Lo pongo en 1 por el caracter '\0'
    char ch;

    // Leo caracteres del archivo hasta que se encuentre el carácter '\0'
    buffer = malloc(size);
    if(buffer == NULL){
        return NULL;
    }

    while (fread(&ch, sizeof(char), 1, file) == 1 && ch != '\0' && ch != '\n') {
        buffer[size - 1] = ch; //  guardo el carácter en el búfer
        size++; // Aumento el tamaño del búfer
        buffer = realloc(buffer, size); // Ampliar el búfer para que quepan mas caracteres
        if (buffer == NULL) {
            return NULL; // Error al asignar memoria
        }
    }

    buffer[size - 1] = '\0'; // El ultimo caracter es este
    return buffer;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argv[1], "r"); // Abre el archivo para lectura
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

   while (!feof(file)) { // Mientras no se llegue al final del archivo
    char *string = loadstr(file); // La función loadstr() se encarga de leer una cadena de caracteres terminada en '\0' desde un archivo y almacenarla dinámicamente en memoria utilizando malloc()
    if (string == NULL) {
        err(EXIT_FAILURE, "loadstr() failed!!");
    }
	if(strlen(string)>0){
		printf("%s\n", string);
	}
    
    free(string); // Liberar la memoria asignada por loadstr()
}

    fclose(file);
    return 0;
}*/
