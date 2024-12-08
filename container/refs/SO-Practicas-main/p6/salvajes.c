#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#define NUMITER 3

sem_t *sem_cook; // semaforo para indicar si el cocinero tiene que cocinar
sem_t *sem_savage; // semaforo para indicar si de los salvajes alguno puedo tomar una racion
sem_t *sem_pot; // Nuevo semáforo para controlar el acceso al caldero entre diversos salvajes
int *pot;

int getServingsFromPot(void)
{
    sem_wait(sem_savage);
    printf("Hay %u raciones\n", *pot);
    if (*pot == 0) {
        printf("Salvaje %lu: LLama al cocinero\n", (unsigned long)getpid());
        sem_post(sem_cook);
        //sleep(1);
        sem_post(sem_pot);
    }

    (*pot)--;
    printf("Salvaje %lu: Tomó una ración del caldero, quedan %u raciones\n", (unsigned long)getpid(), *pot);

    sem_post(sem_savage);
}
void eat(int i)
{
    printf("Salvaje %lu: Comiendo\n", (unsigned long)getpid());
    printf("Salvaje %lu: LLEVA %u de %u turnos por comer\n", (unsigned long)getpid(), i+1,NUMITER);
    sleep(rand() % 5);
}

void savages(void)
{
    for (int i = 0; i < NUMITER; i++) {
        getServingsFromPot();
        eat(i);
    }
}

int main(int argc, char *argv[])
{
    // Abrir los semáforos sin la bandera O_CREAT
    sem_cook = sem_open("/sem_cook", 0);//se abre en modo lectura

    if (sem_cook == SEM_FAILED) {
        perror("Error al abrir semáforo sem_cook. ¿Has ejecutado el programa cocinero?");
        return EXIT_FAILURE;
    }
    /*
    Este semáforo se utiliza para notificar al cocinero para rellenar el caldero cuando está vacío. 
    Se inicializa a 0 porque al principio, el caldero está vacío y el cocinero necesita rellenarlo. 
    Cuando un salvaje encuentra el caldero vacío, notifica al cocinero liberando este semáforo 
    (llamando a sem_post(sem_cook))
    */

    sem_savage = sem_open("/sem_savage", 1); // se abre en modode lectura y escritura
    if (sem_savage == SEM_FAILED) {
        perror("Error al abrir semáforo sem_savage. ¿Has ejecutado el programa cocinero?");
        return EXIT_FAILURE;
    }
    /*
    Este semáforo se utiliza para sincronizar el acceso al caldero entre los salvajes y el cocinero. 
    Se inicializa a 1 para permitir que los salvajes accedan al caldero. Cuando el caldero está vacío, 
    los salvajes se bloquean (llamando a sem_wait(sem_savage)) y el cocinero es notificado para rellenar el caldero.
    */

   
    sem_pot = sem_open("/sem_pot", 1); // se abre en modode lectura y escritura
    if (sem_pot == SEM_FAILED) {
        perror("Error al abrir semáforo sem_pot. ¿Has ejecutado el programa cocinero?");
        return EXIT_FAILURE;
    }
    /*
    sem_pot: Este semáforo se utiliza para asegurar que sólo un salvaje pueda tomar una ración 
    del caldero a la vez. Se inicializa a 1 para indicar que el caldero está disponible para ser 
    accedido por un salvaje. Cuando un salvaje va a tomar una ración, adquiere este semáforo 
    (llamando a sem_wait(sem_pot)) y cuando termina de tomar la ración, libera el semáforo 
    (llamando a sem_post(sem_pot)).
    */
    // Abrir la memoria compartida sólo con la bandera O_RDWR
    int fd = shm_open("/shm_pot", O_RDWR, 0);
    if (fd == -1) {
        perror("Error al abrir memoria compartida. ¿Has ejecutado el programa cocinero?");
        return EXIT_FAILURE;
    }

    pot = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pot == MAP_FAILED) {
        perror("Error al mapear memoria compartida. ¿Has ejecutado el programa cocinero?");
        return EXIT_FAILURE;
    }
    savages();

    // Cerrar los semáforos y desmapear la memoria compartida
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