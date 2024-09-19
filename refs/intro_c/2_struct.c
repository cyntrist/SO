#include <stdio.h>
#include <string.h>

struct alumno {
	int id;
	char nombre[100];
};


int main(int argc, char * argv[]) {
	struct alumno mialumno;
	mialumno.id = 100;
	strncpy(mialumno.nombre, "Zacarías Zeferino", 100);
	printf("El alumno con id %d se llama %s\n", mialumno.id, mialumno.nombre);

	struct alumno * palumno;
	palumno = &mialumno;
	printf("El alumno con id %d se llama %s\n", palumno->id, palumno->nombre);
}
