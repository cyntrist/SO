#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

int main(int argc, char* argv[])
{
	FILE* output_file = NULL;

	if (argc <3) {
        fprintf(stderr, "Usage: %s <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

	// Abrir archivo de salida
    if ((output_file = fopen(argv[1], "wb")) == NULL) {
        err(5, "No se pudo abrir el archivo de salida %s", argv[2]);
    }

	for (int i = 2; i < argc; i++) {
        // Escribir el argumento actual en el archivo
        size_t arg_length = strlen(argv[i]);
        size_t bytes_written = fwrite(argv[i], sizeof(char), arg_length, output_file);
			// escribo de argv[i], byte a byte (caracter a caracter) el contenido de argv[i] en output_file

        // Verificar si la escritura fue exitosa
        if (bytes_written != arg_length) {
            fprintf(stderr, "Error al escribir en el archivo de salida\n");
            fclose(output_file);
            exit(EXIT_FAILURE);
        }

		// Escribir el carácter terminador '\0'
        if (fwrite("\0", sizeof(char), 1, output_file) != 1) {
            fclose(output_file);
            err(EXIT_FAILURE, "fwrite() failed!!");
        }
		/*
		fwrite no agrega automáticamente el carácter nulo de terminación '\0' al final de la cadena escrita. 
		Por lo tanto, si necesitas que el contenido del archivo sea una serie de strings concatenados, 
		necesitas agregar manualmente el carácter nulo de terminación al final de cada string.

		El carácter nulo de terminación '\0' es importante para marcar el final de una cadena de caracteres en C. 
		Sin él, las funciones de manipulación de strings de C no podrán determinar el final de la cadena y pueden 
		comportarse de manera impredecible o incluso causar errores.
		*/

        
		// Escribir un espacio adicional después de cada string, excepto después del último
        if (i < argc - 1) {
            /*
            char space = '\n';
            size_t space_written = fwrite(&space, sizeof(char), 1, output_file);
            if (space_written != 1) {
                fprintf(stderr, "Error al escribir en el archivo de salida\n");
                fclose(output_file);
                exit(EXIT_FAILURE);
            }*/
        } else { // Escribir un salto de línea después del último string
      
            char newline = '\n';
            size_t newline_written = fwrite(&newline, sizeof(char), 1, output_file);
            if (newline_written != 1) {
                fprintf(stderr, "Error al escribir en el archivo de salida\n");
                fclose(output_file);
                exit(EXIT_FAILURE);
            }
		}
    }
	fclose(output_file);
	return 0;
}
