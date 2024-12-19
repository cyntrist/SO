#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 3
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

/// STRUCTS
/// @brief parámetros de las opciones del programa
struct options
{
    FILE *input_file;
};

/// @brief estructura para los datos de un cliente de la discoteca
struct client
{
    int id;
    int vip;
    pthread_t thread;
};

/// SHARED DATA
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t vip_queue = PTHREAD_COND_INITIALIZER;
pthread_cond_t normal_queue = PTHREAD_COND_INITIALIZER;
pthread_cond_t bar_queue = PTHREAD_COND_INITIALIZER;

int num_clients_inside = 0;
int num_vip_waiting = 0;
int num_normal_waiting = 0;
int num_clients_in_bar = 0;

int normal_ticket = 0; // al más puro estilo carnicería
int vip_ticket = 0;
int normal_order = 1;
int vip_order = 1;

int bar_capacity = 0; // capacidad del bar

void print_disco_data()
{
    printf(">> Number of people inside the disco: %d\n", num_clients_inside);
    printf(">> Number of vip clients waiting: %d\n", num_vip_waiting);
    printf(">> Number of normal clients waiting: %d\n", num_normal_waiting);
    printf(">> Number of clients in the bar: %d\n", num_clients_in_bar);
}

void broadcast()
{
    print_disco_data();
    if (num_vip_waiting > 0)
        pthread_cond_broadcast(&vip_queue); // primero VIPs
    else if (num_normal_waiting > 0)
        pthread_cond_broadcast(&normal_queue); // después normales
    pthread_cond_broadcast(&bar_queue); // liberar clientes en el bar
}

void enter_normal_client(int id)
{
    pthread_mutex_lock(&mutex);

    num_normal_waiting++;
    normal_ticket++;
    int ticket = normal_ticket;

    while (num_clients_inside >= CAPACITY || num_vip_waiting > 0 || ticket != normal_order)
    {
        printf("Client %d, who is %s, is waiting to enter the disco.\n", id, VIPSTR(0));
        pthread_cond_wait(&normal_queue, &mutex);
    }

    printf("Client %d, %s, has entered the disco.\n", id, VIPSTR(0));
    normal_order++;
    num_clients_inside++;
    num_normal_waiting--;

    broadcast();
    pthread_mutex_unlock(&mutex);
}

void enter_vip_client(int id)
{
    pthread_mutex_lock(&mutex);

    num_vip_waiting++;
    vip_ticket++;
    int ticket = vip_ticket;

    while (num_clients_inside >= CAPACITY || ticket != vip_order)
    {
        printf("Client %d, who is %s, is waiting to enter the disco.\n", id, VIPSTR(1));
        pthread_cond_wait(&vip_queue, &mutex);
    }

    printf("Client %d, %s, has entered the disco.\n", id, VIPSTR(1));
    vip_order++;
    num_clients_inside++;
    num_vip_waiting--;

    broadcast();
    pthread_mutex_unlock(&mutex);
}

void go_to_bar(int id)
{
    pthread_mutex_lock(&mutex);

    while (num_clients_in_bar >= bar_capacity)
    {
        printf("Client %d is waiting for a seat in the bar.\n", id);
        pthread_cond_wait(&bar_queue, &mutex);
    }

    num_clients_in_bar++;
    printf("Client %d is now in the bar.\n", id);
    pthread_mutex_unlock(&mutex);

    sleep((rand() % 3) + 1); // Simula el tiempo de emborracharse

    pthread_mutex_lock(&mutex);
    num_clients_in_bar--;
    printf("Client %d has left the bar.\n", id);
    broadcast();
    pthread_mutex_unlock(&mutex);

    sleep((rand() % 3) + 1); // Simula el tiempo antes de volver a intentar
}

void dance(int id, int isvip)
{
    printf("Client %d (%s) dancing at the disco\n", id, VIPSTR(isvip));
    sleep((rand() % 3) + 1);
}

void exit_client(int id, int isvip)
{
    pthread_mutex_lock(&mutex);

    printf("Client %d has exited the disco.\n", id);
    num_clients_inside--;

    broadcast();
    pthread_mutex_unlock(&mutex);
}

void *client(void *arg)
{
    struct client *this = (struct client *)arg;
    int id = this->id;
    int isvip = this->vip;

    while (1)
    {
        if (isvip)
            enter_vip_client(id);
        else
            enter_normal_client(id);
        dance(id, isvip);
        exit_client(id, isvip);
        go_to_bar(id);
    }
}

int main(int argc, char *argv[])
{
    int opt;
    struct options option;
    option.input_file = fopen("ejemplo.txt", "r");

    while ((opt = getopt(argc, argv, "hi:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            fprintf(stderr, "Usage: %s [ -h | -i file ]\n", argv[0]);
            exit(EXIT_SUCCESS);
        case 'i':
            option.input_file = fopen(optarg, "r");
            if (!option.input_file)
            {
                fprintf(stderr, "File does not exist.\n");
                exit(EXIT_FAILURE);
            }
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    int M;
    fscanf(option.input_file, "%d %d", &M, &bar_capacity);
    struct client clients[M];
    for (int i = 0; i < M; i++)
    {
        clients[i].id = i;
        fscanf(option.input_file, "%d", &clients[i].vip);
        printf("Client number %d is %s\n", i, VIPSTR(clients[i].vip));
    }

    for (int i = 0; i < M; i++)
        pthread_create(&clients[i].thread, NULL, client, (void *)&clients[i]);

    for (int i = 0; i < M; i++)
        pthread_join(clients[i].thread, NULL);

    pthread_cond_destroy(&vip_queue);
    pthread_cond_destroy(&normal_queue);
    pthread_cond_destroy(&bar_queue);
    pthread_mutex_destroy(&mutex);
    fclose(option.input_file);
    return 0;
}
