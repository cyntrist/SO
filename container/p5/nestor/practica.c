#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#define CAPACITY 5 //aforo maximo
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip") //sustituye 1 por vip y 0 por no vip

//variables globales compartidas entre hilos
pthread_mutex_t mutex;
pthread_cond_t normal_queue;
pthread_cond_t vip_queue;

int num_clients = 0;
int vips_waiting = 0;
int normals_waiting = 0;

int normal_order = 0;
int normal_ticket = 0;

int vip_order = 0;
int vip_ticket = 0;

struct hilos_param {
    int id;
    int vip;
};

void enter_normal_client(int id) {
	pthread_mutex_lock(&mutex); //se bloquea
	
	normals_waiting++;
	int myticket = normal_ticket;
	normal_ticket++;
	
	while (num_clients == CAPACITY || normal_order < myticket || vips_waiting > 0){
		printf("Client %2d (%s) is waiting\n", id, VIPSTR(0));
		pthread_cond_wait(&normal_queue, &mutex);
	}
	
	printf("Client %2d (%s) is coming in\n", id, VIPSTR(0));
	num_clients++;
	normals_waiting--;
	normal_order++;
	
	pthread_cond_broadcast(&normal_queue);
	
	pthread_mutex_unlock(&mutex);
}

void enter_vip_client(int id) {
	pthread_mutex_lock(&mutex); //se bloquea
	
	vips_waiting++;
	int myticket = vip_ticket;
	vip_ticket++;
	
	while (num_clients == CAPACITY || vip_order < myticket){
		printf("Client %2d (%s) is waiting\n", id, VIPSTR(1));
		pthread_cond_wait(&vip_queue, &mutex);
	}
	
	printf("Client %2d (%s) is coming in\n", id, VIPSTR(1));
	num_clients++;
	vips_waiting--;
	vip_order++;
	
	if (vips_waiting > 0)
		pthread_cond_broadcast(&vip_queue);
	else if (normals_waiting > 0)
		pthread_cond_broadcast(&normal_queue);
		
	pthread_mutex_unlock(&mutex);
}

void dance(int id, int isvip) {
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip) {
	pthread_mutex_lock(&mutex);
	printf("Client %2d (%s) is coming out\n", id, VIPSTR(isvip));
	num_clients--;
	
	if (vips_waiting > 0)
		pthread_cond_broadcast(&vip_queue);
	else if (normals_waiting > 0)
		pthread_cond_broadcast(&normal_queue);
		
	pthread_mutex_unlock(&mutex);
}

void* client(void *arg) {
	struct hilos_param* x = (struct hilos_param *)arg;
	int id = x->id;
	int vip = x->vip;
	
	if (vip)
		enter_vip_client(id);
	else
		enter_normal_client(id);
		
	dance(id, vip);
	disco_exit(id, vip);
	return NULL;
}

int main(int argc, char *argv[]) {

	//da error si no añades el parametro con el nombre del archivo
	if (argc < 2){
	    //equivalente a printf, pero tambien se usa para escribir en un fichero
	    fprintf(stderr, "Se necesita un archivo de entrada\n");
	    return 1;
	}
	
	//da error si no puede abrir el archivo
	FILE *input_file = fopen(argv[1], "r");
	if (input_file == NULL) {
	    	fprintf(stderr, "No se pudo abrir el archivo: '%s'\n",argv[1]);
	    	return 1;
	}
	
	//saco del archivo el numero de clientes
	int M;
	fscanf(input_file, "%d", &M);
	
	//creo los hilos y las estructuras para cada cliente
	pthread_t thread[M];
	//struct hilos_param* param = malloc(M * sizeof(struct hilos_param));
	struct hilos_param param[M];
	for (int i = 0; i < M; i++){
	
		param[i].id = i;
		int vip;
		fscanf(input_file, "%d", &vip);
		param[i].vip = vip; 
		
		pthread_create(&thread[i], NULL, client, (void *)&param[i]);
	}
	
	// bucle para esperar a que todos los hilos creados finalicen su ejecución antes de que el programa principal continue
	for (int i = 0; i < M; i++) {
		if (pthread_join(thread[i], NULL) != 0) {
		    	fprintf(stderr, "Error joining thread %d\n", i);
		    	return 1;
        	}
    	}
	
	
	return 0;
}

