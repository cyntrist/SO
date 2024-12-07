#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <err.h>

#define NTH 10

void change_clothes(int id)
{
	printf("Runner %d is changing clothes\n", id);
	sleep(1 + (rand() % 3));
}

void move_position(int id, int pos)
{
	sleep(1 + (rand() % (1 + pos)));
	printf("Runner %d reaches its starting position %d\n", id, pos);
}

void run(int id, int pos)
{
	printf("Runner %d running from position %d\n", id, pos);
	sleep(1 + (rand() % 3));
}

/* apartado b */
int get_position(int id)
{
	int pos;



	return pos;
}

/* apartado c */
void wait_all_ready(int id)
{

}

/* apartado d */
void wait_my_turn(int id, int pos)
{

}

/* apartado e */
void pass_on_relay(int id)
{

}

void *thmain(void *arg)
{
	int id = (int) (long long) arg;
	int pos;

	change_clothes(id);
	pos = get_position(id);
	move_position(id, pos);
	wait_all_ready(id);
	wait_my_turn(id, pos);
	run(id, pos);
	pass_on_relay(id);
	
	return NULL;
}

int main(int argc, char *argv[])
{
	int i;

	/* Apartado a */


	return EXIT_SUCCESS;
}
