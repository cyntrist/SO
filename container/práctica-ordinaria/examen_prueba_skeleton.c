#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 8192

// Function to copy a file from source path to destination path
void copy_file(const char *src_path, const char *dst_path) {
    // a completar utilizando las funciones read y write 
    // para leer del fichero fuente y escribir en el destino
    // la copia debe ser en bloques de BUFFER_SIZE, en ningún
    // caso con un buffer del tamaño total del archivo
}



#define MAX_PATH_LENGTH 512 //podemos asumir que la ruta no supera esta longitud

// Function to recursively copy directories and files
void copy_directory(const char *src_dir, const char *dst_dir) {
    // crear el directorio objetivo con mkdir
    // luego, recorrer el directorio (ignorando las entradas . y ..)
    // y copiar recursivamente todos los ficheros y directorios 
    // internos. 
    // se puede utilizar, por ejemplo, snprintf para generar las rutas 
    // a subarchivos y subdirectorios.
    // NOTA: no hay que copiar enlaces, dispositivos de caracteres, ni
    // ningún otro fichero especial, solo archivos y directorios
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_directory> <destination_directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *src_dir = argv[1];
    const char *dst_dir = argv[2];

    copy_directory(src_dir, dst_dir);

    return EXIT_SUCCESS;
}
