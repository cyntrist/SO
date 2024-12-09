#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    NONE_ACT,
    X_ACT,
    S_ACT,
    B_ACT
} action_t;

typedef struct
{
    pid_t pid;
    int command_number;
} process;

struct options
{
    char *input_file;
    char *cmds;
    action_t action;
};

pid_t launch_command(char **argv)
{

    return 0;
}

char **parse_command(const char *cmd, int *argc)
{
    // Allocate space for the argv array (initially with space for 10 args)
    size_t argv_size = 10;
    const char *end;
    size_t arg_len;
    int arg_count = 0;
    const char *start = cmd;
    char **argv = malloc(argv_size * sizeof(char *));

    if (argv == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (*start && isspace(*start))
        start++; // Skip leading spaces

    while (*start)
    {
        // Reallocate more space if needed
        if (arg_count >= argv_size - 1)
        { // Reserve space for the NULL at the end
            argv_size *= 2;
            argv = realloc(argv, argv_size * sizeof(char *));
            if (argv == NULL)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        // Find the start of the next argument
        end = start;
        while (*end && !isspace(*end))
            end++;

        // Allocate space and copy the argument
        arg_len = end - start;
        argv[arg_count] = malloc(arg_len + 1);

        if (argv[arg_count] == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strncpy(argv[arg_count], start, arg_len);
        argv[arg_count][arg_len] = '\0'; // Null-terminate the argument
        arg_count++;

        // Move to the next argument, skipping spaces
        start = end;
        while (*start && isspace(*start))
            start++;
    }

    argv[arg_count] = NULL; // Null-terminate the array

    (*argc) = arg_count; // Return argc

    return argv;
}

int main(int argc, char *argv[])
{
    char **cmd_argv;
    int cmd_argc, i, opt;
    struct options option;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s \"command\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Parse command-line options (incomplete code!) */
    while ((opt = getopt(argc, argv, "x:s:b")) != -1)
    {
        switch (opt)
        {
        case 'h':
            fprintf(stderr, "Usage: %s [ -x command | -s file | -s file -b ]\n", argv[0]);
            exit(EXIT_SUCCESS);
        case 'x':
            option.action = X_ACT;
            option.cmds = optarg;
            break;
        case 's':
            option.action = S_ACT;
            option.input_file = optarg;
            break;
        case 'b':
            option.action = B_ACT;
            option.cmds[0] = argv[2];
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    cmd_argv = parse_command(argv[1], &cmd_argc);

    // Print parsed arguments
    printf("argc: %d\n", cmd_argc);
    for (i = 0; cmd_argv[i] != NULL; i++)
    {
        printf("argv[%d]: %s\n", i, cmd_argv[i]);
        free(cmd_argv[i]); // Free individual argument
    }

    free(cmd_argv); // Free the cmd_argv array

    return EXIT_SUCCESS;
}