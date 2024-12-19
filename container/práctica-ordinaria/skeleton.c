#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 8192
#define MAX_PATH_LENGTH 512 // Podemos asumir que la ruta no supera esta longitud

// Function to copy a file from source path to destination path
void copy_file(const char *src_path, const char *dst_path) {
    int src_fd, dst_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    // Abrir el archivo fuente en modo lectura
    src_fd = open(src_path, O_RDONLY);
    if (src_fd < 0) {
        perror("Error opening source file");
        return;
    }

    // Crear el archivo destino en modo escritura
    dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0) {
        perror("Error creating destination file");
        close(src_fd);
        return;
    }

    // Leer del archivo fuente y escribir en el destino en bloques de BUFFER_SIZE
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error writing to destination file");
            close(src_fd);
            close(dst_fd);
            return;
        }
    }

    if (bytes_read < 0) {
        perror("Error reading from source file");
    }

    // Cerrar los archivos
    close(src_fd);
    close(dst_fd);
}

// Function to recursively copy directories and files
void copy_directory(const char *src_dir, const char *dst_dir) {
    DIR *dir;
    struct dirent *entry;
    struct stat entry_stat;

    // Crear el directorio destino
    if (mkdir(dst_dir, 0755) < 0 && errno != EEXIST) {
        perror("Error creating destination directory");
        return;
    }

    // Abrir el directorio fuente
    dir = opendir(src_dir);
    if (!dir) {
        perror("Error opening source directory");
        return;
    }

    // Recorrer las entradas del directorio
    while ((entry = readdir(dir)) != NULL) {
        // Ignorar las entradas . y ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char src_path[MAX_PATH_LENGTH];
        char dst_path[MAX_PATH_LENGTH];

        // Construir las rutas completas para el archivo o directorio
        snprintf(src_path, MAX_PATH_LENGTH, "%s/%s", src_dir, entry->d_name);
        snprintf(dst_path, MAX_PATH_LENGTH, "%s/%s", dst_dir, entry->d_name);

        // Obtener información del archivo o directorio
        if (stat(src_path, &entry_stat) < 0) {
            perror("Error getting file status");
            continue;
        }

        // Si es un directorio, copiar recursivamente
        if (S_ISDIR(entry_stat.st_mode)) {
            copy_directory(src_path, dst_path);
        }
        // Si es un archivo regular, copiarlo
        else if (S_ISREG(entry_stat.st_mode)) {
            copy_file(src_path, dst_path);
        }
        // Ignorar otros tipos de archivos
        else {
            printf("Skipping special file: %s\n", src_path);
        }
    }

    // Cerrar el directorio
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_directory> <destination_directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *src_dir = argv[1];
    const char *dst_dir = argv[2];

    copy_directory(src_dir, dst_dir);

    return EXIT_SUCCESS;
}
