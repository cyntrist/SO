#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

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
    if (bytes_leidos == -1) { // o bytes_leidos <0 mejor 
        perror("Error leyendo fichero origen");
        exit(EXIT_FAILURE);
    }
}
/*
Notas:
	 El número 0644 es un modo de archivo en octal utilizado en sistemas basados en UNIX/Linux para establecer los permisos de un archivo.
	 En este caso específico, 0644 se refiere a los permisos que se otorgarán al archivo de destino que se está creando en el programa copy.c.
	 Los permisos de archivo en UNIX/Linux están compuestos por tres grupos de usuarios: el propietario del archivo, el grupo al que pertenece 
	 el archivo y otros usuarios en general. Cada uno de estos grupos puede tener permisos de lectura (r), escritura (w) y ejecución (x). 
	 El número 0644 sigue la convención de asignación de permisos en octal en la que cada dígito representa los permisos para cada grupo de usuarios.
	 El primer dígito (0) indica atributos especiales del archivo, como el setuid, setgid y sticky bit, que no están establecidos en este caso.
	 Los siguientes tres dígitos (644) representan los permisos para el propietario del archivo (6), el grupo al que pertenece el archivo (4) y
	  otros usuarios en general (4).
	 Ahora, ¿qué significa cada dígito?
		El número 6 en binario es 110, lo que significa que el propietario del archivo tiene permisos de lectura y escritura (4 + 2 = 6).
		El número 4 en binario es 100, lo que significa que el grupo al que pertenece el archivo solo tiene permisos de lectura (4).
		El número 4 en binario es 100, lo que significa que otros usuarios en general también solo tienen permisos de lectura (4).
		(si fuera de ejecucuion)  
	Por lo tanto, en resumen, 0644 significa que el propietario del archivo puede leer y escribir en el archivo, mientras que el grupo al que pertenece 
	y otros usuarios solo pueden leer el archivo. Esto es una configuración común para archivos regulares en sistemas UNIX/Linux.

*/
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <fichero_origen> <fichero_destino>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *fichero_origen = argv[1];
    char *fichero_destino = argv[2];

    int fd_origen, fd_destino;

    // Abrir el fichero origen en modo lectura
    if ((fd_origen = open(fichero_origen, O_RDONLY)) == -1) {
        perror("Error abriendo fichero origen");
        exit(EXIT_FAILURE);
    }

    // Abrir o crear el fichero destino en modo escritura, truncándolo si ya existe
    if ((fd_destino = open(fichero_destino, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        perror("Error abriendo/creando fichero destino");
        exit(EXIT_FAILURE);
    }
	//si no pones TRUNC, te va sobreescribiendo lo que habia es decir, si el origen es 123 y el destino es ABDCEF pues despues de ejecutar el codigo
	// saldra 123CEF, por eso es importante poner el O_TRUNC, para que borre lo que habia en el fichero originalmente

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

    printf("Copia de %s a %s completada correctamente.\n", fichero_origen, fichero_destino);

    return 0;
}
