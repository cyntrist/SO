#include <fcntl.h>     // for open
#include <unistd.h>    // for read, write, close, lseek
#include <stdlib.h>    // for malloc, free
#include <stdio.h>     // for perror
#include <sys/stat.h>  // for open modes

void copy_file_contents(const char* input_file, const char* output_file) {
    int input_fd, output_fd;
    off_t file_size;
    ssize_t bytes_read, bytes_written;
    char *buffer;

    // Open the input file for reading
    input_fd = open(input_file, O_RDONLY);

    // Use lseek to determine the size of the file
    file_size = lseek(input_fd, 0, SEEK_END);

    // Move the file offset back to the beginning of the file
    lseek(input_fd, 0, SEEK_SET);

    // Allocate memory for the buffer to store the entire file contents
    buffer = (char*)malloc(file_size);

    // Read the file contents into the buffer
    bytes_read = read(input_fd, buffer, file_size);

    // Open the output file for writing (create if it doesn't exist, truncate if it does)
    output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    // Write the buffer contents to the output file
    bytes_written = write(output_fd, buffer, bytes_read);

    // Clean up: free the buffer and close both file descriptors
    free(buffer);
    close(input_fd);
    close(output_fd);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        // Ensure that the program is called with two arguments (input and output file)
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // Copy file contents from input_file to output_file
    copy_file_contents(argv[1], argv[2]);

    return 0;
}
