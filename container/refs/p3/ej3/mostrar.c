#include <stdio.h> // scanf printf
#include <stdlib.h> // malloc...
#include <unistd.h> //open ,close read
#include <fcntl.h> // flags para arhcivos
#include <bits/getopt_core.h> // getopt


int main(int argc, char *argv[])
{
	int fd, N, bytes_leidos;
    off_t offset = 0;
    int opt;
   	int end = 0;
    /*
    optarg: Esta variable se utiliza para almacenar el argumento asociado con la opción que se está procesando actualmente.
    Por ejemplo, si tienes una opción que requiere un valor, como -n N, donde N es un número, optarg contendrá ese número después de que se procese la opción -n.
    */
    // proceso los argumentos por linea de comandos el -1 lo devuelve hasta que no haya mas opciones por procesar
    while ((opt = getopt(argc, argv, "n:e")) != -1) { // se pone : al final de la primera opcion para indicar que tiene argumento
        switch (opt) {
            case 'n':
                N = atoi(optarg); // numero de bytes que se quieren saltar
				//atoi(optarg) convierte la cadena apuntada por optarg a un número entero (int) y devuelve ese valor entero.
                break;
            case 'e':
                end = 1; // si hay que empezar a contar N desde el final o no
                break;
            default:
                fprintf(stderr, "Uso: %s [-n N] [-e] <fichero>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    // optind: Esta variable se utiliza para mantener el índice del próximo argumento que debe ser procesado en el arreglo argv después de que se hayan procesado todas las opciones.
    // Después de que getopt haya procesado todas las opciones, optind indicará la posición en argv del primer argumento que no es una opción.
	/*
	Cuando getopt() procesa los argumentos de línea de comandos, modifica argv y argc para reorganizar los argumentos, 
	moviendo los argumentos de opciones y sus argumentos relacionados al principio de argv y eliminándolos de argc, 
	dejando los argumentos no relacionados con las opciones al final.
	 optind se utiliza para indicar el índice del próximo argumento en argv que no ha sido procesado todavía.
	*/


	/*
	ejemplo  "./mostrar -n 10 archivo.txt -e"
	En C, los índices de los argumentos en argv comienzan en 0, mientras que optind comienza en 1. Por lo tanto, optind apunta al primer elemento no procesado en argv.

	Por ejemplo, puede pasar que optind apuntará al primer argumento que no sea una opción, es decir, al primer argumento que no comience con un guion ("-").
	En este caso, optind apuntaría a archivo.txt, ya que es el primer argumento que no es una opción. 
	El argumento ./mostrar no se considera una opción, sino que es el nombre del programa en sí y no es procesado por getopt()
	
	
	otro ejemplo
	Si tienes ./mostrar -h -e -j, entonces optind tendría un valor de 2.

	El primer elemento en argv (argv[0]) sería ./mostrar, que es el nombre del programa y no se cuenta como una opción.
	El siguiente elemento en argv (argv[1]) sería -h, que es la primera opción.
	Luego, optind apuntaría al siguiente elemento, que sería -e
	
	
	*/
    // Si optind es mayor o igual que argc, significa que no hay más argumentos disponibles, lo que sugiere que no se proporcionó el nombre del archivo que se esperaba como argumento.
    //optind empieza en 1, porque no trata el nombre del ejecutable ./mostrar
	if (optind >= argc) {
        fprintf(stderr, "Se requiere el nombre del archivo como argumento\n");
        exit(EXIT_FAILURE);
    }
	//argc: Esta variable indica la cantidad total de argumentos pasados al programa en la línea de comandos, incluyendo el nombre del programa mismo. 
	//Por lo tanto, argc es siempre al menos 1.
	/*
	Ejemplo ./programa -a -b valor -c argumento1 argumento2
	Antes de llamar a getopt(), optind es 1.
	getopt() procesa -a, incrementa optind a 2.
	getopt() procesa -b y su argumento valor, incrementa optind a 4 (porque -b y valor ocupan dos posiciones en argv).
	getopt() procesa -c, incrementa optind a 5.
	Después de procesar todas las opciones, optind apunta al primer argumento que no es una opción, que es argumento1
	*/

    // Abro el archivo para lectura
    fd = open(argv[optind], O_RDONLY);
    if (fd == -1) {
        perror("Error abriendo el archivo");
        exit(EXIT_FAILURE);
    }

    // Desplazo el marcador de posición según la opción -n
    if (end) {
        offset = lseek(fd, -N, SEEK_END); // SEEK_END empieza desde atrás y con el -N le digo que retroceda
        if (offset == -1) {
            perror("Error moviendo el marcador de posición");
            exit(EXIT_FAILURE);
        }
    } else {
        offset = lseek(fd, N, SEEK_SET); // SEEK_SET empieza desde el principio del fichero
        if (offset == -1) {
            perror("Error moviendo el marcador de posición");
            exit(EXIT_FAILURE);
        }
    }

    // Leo y escribo byte a byte hasta el final del archivo
    char buffer[1];
    while ((bytes_leidos = read(fd, buffer, sizeof(buffer))) > 0) { // Si ha leido algo continua en el bucle
        if (write(STDOUT_FILENO, buffer, bytes_leidos) == -1) { // Escribo exactamente lo que se lee
            /*
            STDOUT_FILENO es una constante que representa el descriptor de archivo
            estándar para la salida estándar, típicamente asociada con la pantalla o la consola del usuario en sistemas Unix-like.
            */
            perror("Error escribiendo en la salida estándar");
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_leidos == -1) {
        perror("Error leyendo del archivo");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("Error cerrando el archivo");
        exit(EXIT_FAILURE);
    }

    return 0;
}