#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	FILE* input_file = NULL;
    FILE* output_file = NULL;
    unsigned char *buffer = NULL;
    long file_size;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Abrir archivo de entrada
    if ((input_file = fopen(argv[1], "rb")) == NULL) {
        err(2, "No se pudo abrir el archivo de entrada %s", argv[1]);
    }

    // Obtener tamaño del archivo
    fseek(input_file, 0, SEEK_END);
    file_size = ftell(input_file);
    rewind(input_file); // equivalente a fseek(input_file, 0, SEEK_SET);

    // Asignar memoria para el búfer
    buffer = (unsigned char *)malloc(file_size);
    if (buffer == NULL) {
        fclose(input_file);
        err(3, "No se pudo asignar memoria para el búfer");
    }

    // Leer datos del archivo de entrada
    size_t bytes_read = fread(buffer, 1, file_size, input_file);
    if (bytes_read != file_size) {
        fclose(input_file);
        free(buffer);
        err(4, "Error al leer el archivo de entrada");
    }

    // Abrir archivo de salida
    if ((output_file = fopen(argv[2], "wb")) == NULL) {
        fclose(input_file);
        free(buffer);
        err(5, "No se pudo abrir el archivo de salida %s", argv[2]);
    }

    // Escribir datos en el archivo de salida
    size_t bytes_written = fwrite(buffer, 1, bytes_read, output_file);
    if (bytes_written != bytes_read) {
        fclose(input_file);
        fclose(output_file);
        free(buffer);
        err(6, "Error al escribir en el archivo de salida");
    }

	/* Read file byte by byte */
	/*
	while ((c = getc(file)) != EOF) {
		// Print byte to stdout 
		
		ret=putc((unsigned char) c, stdout);
		
		El casting a unsigned char en la llamada a putc se realiza para asegurarse de que el valor pasado a putc 
		esté en el rango válido de un unsigned char. Esto es importante porque putc espera un valor de tipo int, 
		pero interpreta este valor como un unsigned char cuando lo escribe en el flujo de salida especificado.
		En resumen, el casting a unsigned char se realiza para garantizar que el valor pasado a putc esté en el 
		rango válido de un byte sin signo, evitando posibles problemas de desbordamiento o comportamiento indefinido.
		
		*/
		/*fprintf(stderr, "Valor de ret: %d\n", ret);
		if (ret != EOF) {
			fprintf(stderr, "Carácter correspondiente: %c ", (char)ret);
		} else {
			fprintf(stderr, "Se produjo un error al escribir en stdout\n");
		}
		if (ret==EOF){
			fclose(file);
			err(3,"putc() failed!!");
		}
	}*/
	
	// Cerrar archivos y liberar memoria
    free(buffer);
    fclose(input_file);
    fclose(output_file);
	return 0;
}



/*
otra forma mas sencilla

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	unsigned char buffer[1]; // añadimos el buffer de 1 byte porque leemos de 1 en uno
	size_t ret;

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	//Open file 
	if ((file = fopen(argv[1], "r")) == NULL) // le indicamos que es lectura unicamente y le pasamos el nombre del archivo
		err(2,"The input file %s could not be opened",argv[1]);

	// Read file byte by byte 
	while ((ret = fread(buffer, sizeof(unsigned char), 1, file)) > 0) { // Mientras que se haya leido un byte
		// Print byte to stdout 
		ret=fwrite(buffer, sizeof(unsigned char), 1, stdout); // mostramos el byte que hemos guardado en el buffer con fread

		if (ret != 1){ // Si no se ha escrito un byte hay error
			fclose(file);
			err(3,"fwrite() failed!!");
		}
	}

	fclose(file);
	return 0;
}
*/