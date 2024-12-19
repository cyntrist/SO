#define _POSIX_C_SOURCE 200809L
/*El estándar POSIX (Portable Operating System Interface) define una serie de características opcionales que pueden 
o no estar disponibles en un sistema operativo en particular. Algunas de estas características están desactivadas 
por defecto en ciertas bibliotecas de C, como glibc, y deben ser habilitadas explícitamente.

La macro _POSIX_C_SOURCE es una forma de decirle al compilador que quieres habilitar las características definidas 
en una versión específica del estándar POSIX. Al definir _POSIX_C_SOURCE con el valor 200809L, estás indicando que 
quieres habilitar las características que se añadieron en la versión de 2008 del estándar POSIX.

La estructura sigaction y la función sigaction(), que estás utilizando en tu código, son parte de estas 
características opcionales de POSIX. Por lo tanto, necesitas definir _POSIX_C_SOURCE antes de incluir <signal.h> 
para que estas definiciones estén disponibles.*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

pid_t child_pid;

void kill_child(int sig) {
    kill(child_pid, SIGKILL);
}

void ignore_signal(int sig) {}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Por favor, proporciona el nombre del programa a ejecutar como argumento.\n");
        return 1;
    }

    child_pid = fork();

    if (child_pid == 0) {
        // Este es el proceso hijo
        signal(SIGINT, SIG_IGN);  // Ignora la señal SIGINT
        execvp(argv[1], &argv[1]);
        perror("execvp");
        return 1;
    } else if (child_pid < 0) {
        perror("fork");
        return 1;
    }

    // Este es el proceso padre
    struct sigaction sa;
    sa.sa_handler = kill_child;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);

    sa.sa_handler = ignore_signal;
    sigaction(SIGINT, &sa, NULL);

    alarm(5);

    int status;
    waitpid(child_pid, &status, 0);

    if (WIFEXITED(status)) {
        printf("El proceso hijo terminó normalmente.\n");
    } else if (WIFSIGNALED(status)) {
        int signal_number = WTERMSIG(status);
        char* signal_name = strsignal(signal_number);
        printf("El proceso hijo fue terminado por la señal %d, %s.\n", signal_number, signal_name);
    }

    return 0;
}