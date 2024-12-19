#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct ThreadArgs {
    int thread_num;
    char priority;
};

void *thread_usuario(void *arg) {
    struct ThreadArgs *args = (struct ThreadArgs *)arg;
    
    int thread_num = args->thread_num;
    char priority = args->priority;
    
    printf("Hilo %ld: Número de hilo: %d, Prioridad: %c\n", pthread_self(), args->thread_num, args->priority);
    
    free(arg); // Liberar la memoria dinámica
    
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <numero de hilos>\n", argv[0]);
        return -1;
    }
    
    int num_threads = atoi(argv[1]);
    pthread_t threads[num_threads];
    
    for (int i = 0; i < num_threads; i++) {
        struct ThreadArgs *args = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
        
        args->thread_num = i;
        args->priority = (i % 2 == 0) ? 'P' : 'N'; // Par: Prioritario, Impar: No prioritario
        
        pthread_create(&threads[i], NULL, thread_usuario, (void *)args);
    }
    
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}

/*
si comentas free(arg); y en el for del main no haces mallos sino que haces 
struct ThreadArgs args;
        
        args.thread_num = i;
        args.priority = (i % 2 == 0) ? 'P' : 'N'; // Par: Prioritario, Impar: No prioritario
        
        pthread_create(&threads[i], NULL, thread_usuario, (void *)&args); // al ser una variable no puntero, necesoitas pasar el & de args

lo que estaria pasando es que en cada iteracion se redefine la variable args, pero la direccion de memoria es la misma en cada iteracion
por lo que todos los hilos ven la misma variable, comparten la misma variable, lo que provoca comportamientos inesperados, ya que todos
ven thread_num y priority. Si ejecutas el programa todo el rato veras que todas las ejecuciones son distintas.

La llamada a pthread_create en sí misma puede ser bastante rápida, ya que simplemente inicia la ejecución del hilo y luego retorna. 
Sin embargo, la función que se pasa como parámetro (thread_usuario en este caso) puede tardar más en ejecutarse, 
especialmente si realiza operaciones que consumen mucho tiempo. 

En resumen, haces pthread_create, eso crea un hilo que ejecutara (solo ese hilo) la funcion de thread_usuario, pasas a la siguiente iteracion,
cambias los valores del struct y cuando el anterior hilo que iba es escribir correctos pues ahora se los ha cambiado y escribe los no correctos 
porque ha ido mas lento

*/