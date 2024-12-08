#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <linux/limits.h>
#include <bits/getopt_core.h>
#include <sys/stat.h>


struct options {
	char *progname;
	int recurse;
};

struct options opt;


void list_dir(char *name);
void process_recurse(char *dirname, char *name);
void list_dir_recurse(char *name);

void usage(void)
{
	printf("%s [options] [dirname]\n\n", opt.progname);
	printf("lists the contents of dirname (default .)\n");
	printf("options:\n"
		"\t-h:\tshow this help\n"
		"\t-R:\trecursive\n"
	);
}

void list_dir(char *name)
{
	DIR *d;
	struct dirent *de;

	d = opendir(name);
	if (d == NULL) {
		perror("opendir");
		return;
	}

	while ((de = readdir(d)) != NULL) {
		printf("%s\n", de->d_name);
	}

	closedir(d);
}

void process_recurse(char *dirname, char *name) {
    pid_t pid;
    char path[PATH_MAX];

    snprintf(path, sizeof(path), "%s/%s", dirname, name);

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) { // Child process
        printf("%s:\n", path);
        list_dir(path);
        list_dir_recurse(path); // Aquí debería ser llamado con la ruta completa del subdirectorio
        exit(EXIT_SUCCESS);
    } else { // Parent process
        wait(NULL);
    }
}

void list_dir_recurse(char *name) {
    DIR *d;
    struct dirent *de;
    char sub_path[PATH_MAX]; // Variable para almacenar la ruta completa del subdirectorio

    d = opendir(name);
    if (d == NULL) {
        perror("opendir");
        return;
    }
    while ((de = readdir(d)) != NULL) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
			continue;
		
        if (de->d_type == DT_DIR) {
			printf("\n");
            process_recurse(name, de->d_name); // Llamar a process_recurse con la ruta completa del subdirectorio
        }
    }

    closedir(d);
}


int main(int argc, char **argv)
{
	char *dirname = ".";
	int o;
	opt.progname = argv[0];
	opt.recurse = 0;

	while ((o = getopt(argc, argv, "hR")) != -1) {
		switch (o) {
		case 'h':
			usage();
			exit(0);
		case 'R':
			opt.recurse = 1;
			break;
		default:
			usage();
			exit(1);
		}
	}

	if (optind < argc)
		dirname = argv[optind];

	if (opt.recurse)
		printf("%s:\n", dirname); // .:

	list_dir(dirname);

	
	if (opt.recurse)
		list_dir_recurse(dirname);
	
	return 0;
}