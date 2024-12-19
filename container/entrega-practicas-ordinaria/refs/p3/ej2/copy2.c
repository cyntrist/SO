#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#define BUFFER_SIZE 512

void copy(int fdo, int fdd) {
    ssize_t bytes_leidos, bytes_escritos;
    char buffer[BUFFER_SIZE];

    // Leer y escribir en bloques de 512 bytes
    while ((bytes_leidos = read(fdo, buffer, BUFFER_SIZE)) > 0) {
        bytes_escritos = write(fdd, buffer, bytes_leidos);
        if (bytes_escritos != bytes_leidos) {
            perror("Error escribiendo en fichero destino");
            exit(EXIT_FAILURE);
        }
    }

    // Comprobar si hubo error de lectura
    if (bytes_leidos == -1) {
        perror("Error leyendo fichero origen");
        exit(EXIT_FAILURE);
    }
}

void copy_regular(char *orig, char *dest) {
    // Abrir el fichero origen en modo lectura
    int fd_origen = open(orig, O_RDONLY);
    if (fd_origen == -1) {
        perror("Error abriendo fichero origen");
        exit(EXIT_FAILURE);
    }

    // Abrir o crear el fichero destino en modo escritura, truncándolo si ya existe
    int fd_destino = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_destino == -1) {
        perror("Error abriendo/creando fichero destino");
        exit(EXIT_FAILURE);
    }

    // Llamar a la función copy para realizar la copia del fichero
    copy(fd_origen, fd_destino);

    // Cerrar ficheros
    if (close(fd_origen) == -1) {
        perror("Error cerrando fichero origen");
        exit(EXIT_FAILURE);
    }
    if (close(fd_destino) == -1) {
        perror("Error cerrando fichero destino");
        exit(EXIT_FAILURE);
    }
}

void copy_link(char *orig, char *dest)
{
    struct stat st;
    size_t bytes_leidos; // La razón principal para utilizar size_t en lugar de un tipo de datos firmado como int o long para representar tamaños de memoria
                        //  es que garantiza que el tipo tendrá un tamaño suficiente para contener cualquier tamaño de objeto válido en el sistema en el que se esté ejecutando el programa.
    char *ruta_enlace;

    // Llamo a lstat para obtener información sobre el enlace simbólico original y concretamente su tamaño (numero de bytes que ocupa el enlace simbólico)
    if (lstat(orig, &st) == -1) {
        perror("Error obteniendo información del enlace simbólico");
        exit(EXIT_FAILURE);
    }

    // Reservo memoria para almacenar la ruta del enlace simbólico
    ruta_enlace = malloc(st.st_size + 1); // +1 por '/0'
    if (ruta_enlace == NULL) {
        perror("Error reservando memoria");
        exit(EXIT_FAILURE);
    }

    // Leo la ruta del enlace simbólico
    bytes_leidos = readlink(orig, ruta_enlace, st.st_size);
    if (bytes_leidos == -1) {
        perror("Error leyendo el enlace simbólico");
        free(ruta_enlace);
        exit(EXIT_FAILURE);
    }
    ruta_enlace[bytes_leidos] = '\0'; // Añado el caracter null al final de la cadena

    if (symlink(ruta_enlace, dest) == -1) { // creo un nuevo enlace simbólico que apunte a la ruta especificada por dest.
        perror("Error creando el enlace simbólico de destino");
        free(ruta_enlace);
        exit(EXIT_FAILURE);
    }
	
    printf("Enlace simbólico copiado de %s a %s\n", orig, dest);

    free(ruta_enlace);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <fichero_origen> <fichero_destino>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *fichero_origen = argv[1];
    char *fichero_destino = argv[2];

    // Obtener información sobre el fichero origen
    struct stat info;
    if (lstat(fichero_origen, &info) == -1) {
        perror("Error obteniendo información del fichero origen");
        exit(EXIT_FAILURE);
    }
	/*
	lstat es una llamada al sistema que obtiene información sobre un archivo o un enlace simbólico.
		Recibe dos argumentos:
		El nombre del archivo o enlace simbólico del cual se desea obtener información.
		Un puntero a una estructura stat donde se almacenará la información obtenida.
	*/

    // Realizar la copia adecuada según el tipo de fichero origen
    if (S_ISREG(info.st_mode)) {  //S_ISREG es una macro que se utiliza para determinar si un archivo es regular o no
        // Fichero regular
        copy_regular(fichero_origen, fichero_destino);
    } else if (S_ISLNK(info.st_mode)) { //S_ISLNK es una macro similar a S_ISREG, pero en este caso se utiliza para determinar si el archivo es un enlace simbólico.
        // Enlace simbólico
        copy_link(fichero_origen, fichero_destino);
    } else {
        // Otro tipo de fichero
        fprintf(stderr, "%s no es un fichero regular ni un enlace simbólico\n", fichero_origen);
        exit(EXIT_FAILURE);
    }

    printf("Copia de %s a %s completada correctamente.\n", fichero_origen, fichero_destino);

    return 0;
}
