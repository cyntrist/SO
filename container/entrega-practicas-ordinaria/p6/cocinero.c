#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#define M 3

int finish = 0;
sem_t *sem_cook; // semaforo para indicar si el cocinero tiene que cocinar
sem_t *sem_savage; // semaforo para indicar si de los salvajes alguno puedo tomar una racion
sem_t *sem_pot; // Nuevo semáforo para controlar el acceso al caldero entre diversos salvajes
int *pot;

void putServingsInPot(int servings)
{
    sem_wait(sem_cook);
    printf("Cocinero: Reabasteciendo el caldero con %d raciones\n", servings);
    *pot = servings;
    sem_post(sem_pot);
}

void cook(void)
{
    while (!finish) {
        putServingsInPot(M);
    }
}

void handler(int signo)
{
    finish = 1;
}

int main(int argc, char *argv[])
{
    signal(SIGTERM, handler);
    signal(SIGINT, handler);

    sem_cook = sem_open("/sem_cook", O_CREAT, 0644, 0);
    if (sem_cook == SEM_FAILED) {
        perror("Error al abrir semáforo sem_cook");
        return EXIT_FAILURE;
    }

    sem_savage = sem_open("/sem_savage", O_CREAT, 0644, 1);
    if (sem_savage == SEM_FAILED) {
        perror("Error al abrir semáforo sem_savage");
        return EXIT_FAILURE;
    }
    
    sem_pot = sem_open("/sem_pot", O_CREAT, 0644, 1); // Crear e inicializar el semáforo sem_pot
    if (sem_pot == SEM_FAILED) {
        perror("Error al abrir semáforo sem_pot");
        return EXIT_FAILURE;
    }

    int fd = shm_open("/shm_pot", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("Error al abrir memoria compartida");
        return EXIT_FAILURE;
    }

    ftruncate(fd, sizeof(int));
    pot = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pot == MAP_FAILED) {
        perror("Error al mapear memoria compartida");
        return EXIT_FAILURE;
    }


    *pot = M;
    cook();

    // Liberar recursos compartidos
    sem_close(sem_cook);
    sem_unlink("/sem_cook");
    sem_close(sem_savage);
    sem_unlink("/sem_savage");
    //sem_close(sem_pot); // Cerrar el semáforo del caldero
    sem_unlink("/sem_pot"); // Desvincular el semáforo del caldero
    munmap(pot, sizeof(int));
    shm_unlink("/shm_pot");

    return EXIT_SUCCESS;
}


/**
 * 
 *  Esto puede suceder si todos los salvajes están esperando que el cocinero rellene el caldero, 
 * pero el cocinero está esperando que los salvajes consuman todas las raciones antes de poder rellenar el caldero
*/
