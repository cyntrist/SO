#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 3
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

// MUTEX
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	/*
	Este es el mutex principal que se utiliza para garantizar la exclusión mutua en secciones críticas del código. 
	Se utiliza para proteger la sección crítica donde se modifican las variables compartidas como num_clients_inside, 
	num_vip_waiting y num_normal_waiting.
	*/

// VARIABLES DE CONDICIÓN
	pthread_cond_t vip_queue = PTHREAD_COND_INITIALIZER; // pongo el nombre de queue para hacerme una idea, pero no tiene internamente una cola el struct
	/*
	Esta es una variable de condición asociada a la cola de espera de los clientes VIP. Se utiliza para sincronizar 
	los hilos que representan a los clientes VIP que esperan entrar a la discoteca cuando la capacidad máxima se alcanza 
	o hay clientes normales dentro.
	*/
	pthread_cond_t normal_queue = PTHREAD_COND_INITIALIZER; // pongo el nombre de queue para hacerme una idea, pero no tiene internamente una cola el struct
	/*
	Esta es una variable de condición asociada a la cola de espera de los clientes normales. Se utiliza para sincronizar 
	los hilos que representan a los clientes normales que esperan entrar a la discoteca cuando la capacidad máxima se alcanza 
	o hay clientes VIP dentro.
	*/


// Variables Compartidas, las cuales son compartidas por los hilos al ser variables globales. Son tambien la SECCIÓN CRÍTICA del programa
	int num_clients_inside = 0;
	int num_vip_waiting = 0;
	int num_normal_waiting = 0;
    int ticket_n =0;
    int orden_n = 1;
    int ticket_v = 0;
    int orden_v = 1;


/*
En resumen, estas variables de condición se utilizan junto con los mutexes para permitir que los hilos esperen de manera eficiente. 
Los mutexes se bloquean y desbloquean en las diferentes secciones críticas del código para garantizar la exclusión mutua. Las variables 
de condición se utilizan en combinación con los mutexes para hacer esperar a los hilos de manera segura y eficiente hasta que se cumplan 
ciertas condiciones, momento en el cual son despertados por otros hilos que cambian el estado compartido y señalan las variables de condición.
*/



void enter_normal_client(int id) {
    pthread_mutex_lock(&mutex); //exclusion mutua
    ticket_n++;
    int myticket = ticket_n;
    printf("Initializing client %2d (%s)\n", id, VIPSTR(0));
    num_normal_waiting++;
    printf("ticket vale %d \n", ticket_n);
    printf("my ticket vale %d \n", myticket);

    while (num_clients_inside == CAPACITY || orden_n != myticket || num_vip_waiting > 0) {
        printf("Client %2d (%s) is waiting to enter\n", id, VIPSTR(0));
        pthread_cond_wait(&normal_queue, &mutex);
    }

    printf("Client %2d (%s) is entering\n", id, VIPSTR(0));
    num_clients_inside++;
    num_normal_waiting--;
    orden_n++;
    pthread_cond_broadcast(&normal_queue);
    pthread_mutex_unlock(&mutex);
}

/*
Se hace broadcast, para llamar a todos, entonces a alguno le tocara entrar; si se hace signal, eso solo despertara a uno de la cola 
(puede estar en cualquier orden, ser el primero, segundo,..., ultimo), pero puede ser que al que despierte de la cola no le toque el turno 
en ese momento o la capacidad esté llena, entonces puede producir deadlock.
Cuando se hace el broadcast, aunque solo uno de ellos podrá continuar, los demás volverán a bloquearse pero al menos se ha dado la 
oportunidad a todos los hilos de intentar continuar.
*/

void enter_vip_client(int id) {
    pthread_mutex_lock(&mutex); //exclusion mutua
    ticket_v++;
    int myticket = ticket_v;
    printf("Initializing client %2d (%s)\n", id, VIPSTR(1));
    printf("ticket_v vale %d \n", ticket_v);
    printf("my ticket vale %d \n", myticket);
    num_vip_waiting++;

    while (num_clients_inside == CAPACITY || orden_v != myticket) {
        printf("Client %2d (%s) is waiting to enter\n", id, VIPSTR(1));
        printf("Mientras espera orden_v vale %2d\n", orden_v);
        printf("Mientras espera myticket vale %2d\n", myticket);
        pthread_cond_wait(&vip_queue, &mutex);
    }

    printf("Client %2d (%s) is entering\n", id, VIPSTR(1));
    num_clients_inside++;
    num_vip_waiting--;
    orden_v++;
    if(num_vip_waiting>0){
        pthread_cond_broadcast(&vip_queue);
    }else{
        pthread_cond_broadcast(&normal_queue);
    }
    pthread_mutex_unlock(&mutex);
}

void dance(int id, int isvip) {
    printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
    sleep((rand() % 3) + 1);
}

void exit_client(int id, int isvip) {
    pthread_mutex_lock(&mutex); //exclusion mutua
    printf("Client %2d (%s) is exiting\n", id, VIPSTR(isvip));
    num_clients_inside--;

    if (num_vip_waiting > 0) {
        pthread_cond_broadcast(&vip_queue);
    } else if (num_normal_waiting > 0) {
        pthread_cond_broadcast(&normal_queue);
    }

    pthread_mutex_unlock(&mutex);
}

void *client(void *arg) {
    int *args = (int *)arg;
    int id = args[0];
    int isvip = args[1];
    free(arg);

    if (isvip)
        enter_vip_client(id);
    else
        enter_normal_client(id);

    dance(id, isvip);
    exit_client(id, isvip);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror("Error opening file");
        return 1;
    }

    int M;
    fscanf(input_file, "%d", &M);

    pthread_t *threads = malloc(M * sizeof(pthread_t));
    if (!threads) {
        perror("Error allocating memory");
        return 1;
    }

    for (int i = 0; i < M; i++) {
        int *args = malloc(2 * sizeof(int));
        if (!args) {
            perror("Error allocating memory");
            return 1;
        }
		args[0] = i + 1;
        fscanf(input_file, "%d", &args[1]);

        if (pthread_create(&threads[i], NULL, client, args) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }

    for (int i = 0; i < M; i++) { // bucle para esperar a que todos los hilos creados finalicen su ejecución antes de que el programa principal continúe
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            return 1;
        }
    }

    fclose(input_file);
    free(threads);

    return 0;
}
