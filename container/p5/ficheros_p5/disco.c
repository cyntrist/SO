#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

struct options
{
	FILE *input_file;
};

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;

void enter_normal_client(int id)
{
}

void enter_vip_client(int id)
{
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip)
{
}

void *client(void *arg)
{
	// ...

	// 	if (isvip)
	// 		enter_vip_client(...);
	// else enter_normal_client(...);
	// dance(...);
	// exit_client(...);

	// ...
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
	int buff, m;
	fscanf(option.input_file, "%d", &m);
	int clients[m], i = 0;
	while (fscanf(option.input_file, "%d", &clients[i]) == 1) 
	{
		printf("Client number %d is %s\n", i, VIPSTR(clients[i]));
		i++;
	}
	// ---




	return 0;
}
