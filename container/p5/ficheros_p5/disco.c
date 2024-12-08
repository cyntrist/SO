#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

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
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // con esto ya no hace falta hacer pthread_mutex_init entiendo
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t vip_queue = PTHREAD_COND_INITIALIZER;
pthread_cond_t normal_queue = PTHREAD_COND_INITIALIZER;

int num_clients_inside = 0;
int num_vip_waiting = 0;
int num_normal_waiting = 0;

int normal_ticket = 0; // al mas puro estilo carnicería
int vip_ticket = 0;
int normal_order = 0;
int vip_order = 0;

void print_disco_data()
{
	printf(">> Number of people inside the disco: %d\n", num_clients_inside);
	printf(">> Number of vip clients waiting: %d\n", num_vip_waiting);
	printf(">> Number of normal clients waiting: %d\n", num_normal_waiting);
}

void print_queue_data()
{
	printf(">> Normal ticket: %d\n", normal_ticket);
	printf(">> VIP ticket: %d\n", vip_ticket);
	printf(">> Normal order: %d\n", normal_order);
	printf(">> VIP order: %d\n", vip_order);
}

void broadcast()
{
	print_disco_data();
	if (num_vip_waiting > 0)
		pthread_cond_broadcast(&vip_queue); // primero vips
	else if (num_normal_waiting > 0)
		pthread_cond_broadcast(&normal_queue); // despues normal
}

void enter_normal_client(int id)
{
	pthread_mutex_lock(&mutex); // exclusion

	// gestion de datos compartidos
	num_normal_waiting++;
	normal_ticket++;
	int ticket = normal_ticket;

	// espera de la variable condicional hasta que se pueda continuar
	while (num_clients_inside >= CAPACITY && num_normal_waiting > 0 && ticket != normal_order)
	{
		printf("Client %d, who is %s, is waiting to enter.\n", id, VIPSTR(0));
		pthread_cond_wait(&normal_queue, &mutex);
	}

	printf("Client %d, %s, has entered the disco.\n", id, VIPSTR(0));

	// gestion de datos compartidos
	normal_order++;
	num_clients_inside++;
	num_normal_waiting--;

	broadcast(); // llamar al resto de la cola

	pthread_mutex_unlock(&mutex); // desbloqueo
}

void enter_vip_client(int id)
{
	pthread_mutex_lock(&mutex); // bloqueo

	// gestion de datos compartidos
	num_vip_waiting++;
	vip_ticket++;
	int ticket = vip_ticket;

	// espera de la variable condicional hasta que se pueda continuar
	while (num_clients_inside >= CAPACITY && num_vip_waiting > 0 && ticket != vip_order)
	{
		printf("Client %d, who is %s, is waiting to enter.\n", id, VIPSTR(1));
		pthread_cond_wait(&vip_queue, &mutex);
	}

	printf("Client %d, %s, has entered the disco.\n", id, VIPSTR(1));

	// gestion de datos compartidos
	vip_order++;
	num_clients_inside++;
	num_vip_waiting--;

	broadcast(); // llamada al resto de colas

	pthread_mutex_unlock(&mutex); // desbloqueo
}

void dance(int id, int isvip)
{
	printf("Client %d (%s) dancing at the disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void exit_client(int id, int isvip)
{
	pthread_mutex_lock(&mutex); // bloqueo

	printf("Client %d has exited the disco.\n", id);
	num_clients_inside--;

	broadcast(); // llamada al resto de colas

	pthread_mutex_unlock(&mutex); // desbloqueo
}

void *client(void *arg)
{
	struct client *this = (struct client *)arg;
	int id = this->id;
	int isvip = this->vip;

	if (isvip)
		enter_vip_client(id);
	else
		enter_normal_client(id);
	dance(id, isvip);
	exit_client(id, isvip);
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

	// parseo
	int buff, M;
	fscanf(option.input_file, "%d", &M);
	struct client clients[M];
	for (int i = 0; i < M; i++)
	{
		clients[i].id = i;
		fscanf(option.input_file, "%d", &clients[i].vip);
		printf("Client number %d is %s\n", i, VIPSTR(clients[i].vip));
	};

	for (int i = 0; i < M; i++)
	{
		if (pthread_create(&clients[i].thread, NULL, client, (void *)&clients[i]) == 0)
			;
		// printf("Thread %d created successfully.\n", i);
	}

	for (int i = 0; i < M; i++)
	{
		if (pthread_join(clients[i].thread, NULL) == 0)
			;
		// printf("Thread %d joined successfully.\n", i);
	}

	pthread_mutex_destroy(&mutex);
	fclose(option.input_file);
	return 0;
}
