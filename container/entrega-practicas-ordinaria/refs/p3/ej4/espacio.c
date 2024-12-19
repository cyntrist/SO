#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

int get_size_dir(char *fname, size_t *blocks);
int get_size(char *fname, size_t *blocks);

int get_size(char *fname, size_t *blocks) {
    struct stat st;
    if (lstat(fname, &st) == -1) {
        perror("Error getting file information");
        return -1;
    }

    if (S_ISDIR(st.st_mode)) {
        return get_size_dir(fname, blocks);
    } else {
        *blocks = (st.st_blocks * 512) / 1024;
        return 0;
    }
}

int get_size_dir(char *dname, size_t *blocks) {
    DIR *dir;
    struct dirent *entry;
    size_t total_blocks = 0;

    if ((dir = opendir(dname)) == NULL) {
        perror("Error opening directory");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; 
        }

        char entry_path[1024];
        snprintf(entry_path, sizeof(entry_path), "%s/%s", dname, entry->d_name);

        size_t entry_blocks;
        if (get_size(entry_path, &entry_blocks) == -1) {
            closedir(dir);
            return -1;
        }

        total_blocks += entry_blocks;
    }

    *blocks = total_blocks;
    closedir(dir);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [<file2> ...]\n", argv[0]);
        return 1;
    }

    size_t total_blocks = 0;
    for (int i = 1; i < argc; i++) {
        size_t blocks;
        if (get_size(argv[i], &blocks) == -1) {
            return 1;
        }
        total_blocks += blocks;
        printf("%zuK %s\n", blocks, argv[i]);
    }

    return 0;
}
