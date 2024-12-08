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
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t vip_queue = PTHREAD_COND_INITIALIZER;
pthread_cond_t normal_queue = PTHREAD_COND_INITIALIZER;

int num_clients_inside = 0;
int num_vip_waiting = 0;
int num_normal_waiting = 0;

int ticket_normal = 0;
int ticket_vip = 0;
int orden_normal = 0;
int orden_vip = 0;

void enter_normal_client(int id)
{
	pthread_mutex_lock(&mutex);
	num_clients_inside++;
	num_normal_waiting--;
	pthread_mutex_unlock(&mutex);
}

void enter_vip_client(int id)
{
	pthread_mutex_lock(&mutex);

	num_vip_waiting++;
	ticket_vip++;
	int ticket = ticket_vip;

	while (num_clients_inside == CAPACITY && num_vip_waiting > 0 && ticket != orden_vip)
	{
		printf("Client %d, who is %s, is waiting to enter.\n", id, VIPSTR(1));
		pthread_cond_wait(&vip_queue, &mutex);
	}

	printf("Client %d, %s, has entered the disco.\n", id, VIPSTR(1));
	orden_vip++;
	num_clients_inside++;
	num_vip_waiting--;

	if (num_vip_waiting > 0)
		pthread_cond_broadcast(&vip_queue);
	else if (num_normal_waiting > 0)
		pthread_cond_broadcast(&normal_queue);

	pthread_mutex_unlock(&mutex);
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing at the disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void exit_client(int id, int isvip)
{
	pthread_mutex_lock(&mutex);

	printf("Client %d has exited the disco.\n", id);
	num_clients_inside--;

	if (num_vip_waiting > 0)
		pthread_cond_broadcast(&vip_queue);
	else if (num_normal_waiting > 0)
		pthread_cond_broadcast(&normal_queue);

	pthread_mutex_unlock(&mutex);
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

	/// parseo
	// int buff, m;
	// fscanf(option.input_file, "%d", &m);
	// int vip_list[m], i = 0;
	// while (fscanf(option.input_file, "%d", &vip_list[i]) == 1)
	// {
	// 	printf("Client number %d is %s\n", i, VIPSTR(vip_list[i]));
	// 	i++;
	// }

	int buff, M;
	fscanf(option.input_file, "%d", &M);
	struct client clients[M];
	for (int i = 0; i < M; i++)
	{
		clients[i].id = i;
		fscanf(option.input_file, "%d", &clients[i].vip);
		printf("Client number %d is %s\n", i, VIPSTR(clients[i].vip));
	};
	// ---

	srand(time(NULL)); // Seed random number generator for usleep

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

	// // Initialize mutexes for chopsticks
	// for (i = 0; i < N; i++)
	// {
	// 	pthread_mutex_init(&mutex, NULL);
	// }

	// // Destroy mutexes (though unreachable in this infinite loop)
	// for (i = 0; i < N; i++)
	// {
	// 	pthread_mutex_destroy(&mutex);
	// }

	// for (int i = 0; i < M; i++)
	// {
	// 	free(clients[i]);
	// }

	fclose(option.input_file);

	return 0;
}
