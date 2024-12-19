#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    NONE_ACT,
    X_ACT,
    S_ACT,
    BS_ACT
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
    pid_t pid = fork();

    if (pid == 0)
    {
        printf("@@ Running command #%d: %s %s\n", argv[0], argv[1]);
        if (execvp(argv[0], argv) == -1)
        {
            perror("execv");
            exit(EXIT_FAILURE);
        }
        exit(1);
    }

    return pid;
}

void exec_file(char *path, int *b)
{
    FILE *f ;
    fopen(f, "r");
    if (!f)
    {
        printf(stderr, "Error while opening input file.\n");
        return;
    }

    char command[256];
    char **cmd_argv;
    pid_t pidcmd;
    int cmd_argc, i = 0;
    process processes[256];

    if (b)
    {
        while (fgets(command, 256, f) != NULL)
        {
            cmd_argv = parse_command(command, &cmd_argc);
            pidcmd = launch_command(cmd_argv);
            processes[i].pid = pidcmd;
            processes[i].command_number = i;
            i++;
        }
        fclose(f);

        int proc = i, status;
        while (i > 0)
        {
            pidcmd = wait(&status);
            if (pidcmd > 0)
            {
                for (int j = 0; j < proc; j++)
                {
                    if (processes[j].pid == pidcmd)
                    {
                        printf("@@ Command #%d terminated (pid: %d, status: %d)\n", processes[j].command_number, pidcmd, WEXITSTATUS(status));
                        break;
                    }
                }
                i--;
            }
        }

        for (int j = 0; cmd_argv[j] != NULL; j++)
            free(cmd_argv[j]);
        free(cmd_argv);
        return;
    }
    else
    {
        while (fgets(command, 256, f) != NULL)
        {
            cmd_argv = parse_command(command, &cmd_argc);
            pidcmd = launch_command(cmd_argv);
            waitpid(pidcmd, NULL, 0);
            printf("PID Hijo: %d\n", pidcmd);
            i++;
        }
        fclose(f);
        for (int i = 0; cmd_argv[i] != NULL; i++)
            free(cmd_argv[i]); // Free individual argument
        free(cmd_argv);
    }
    return;
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
    int cmd_argc, i, opt, pid;
    struct options option;
    option.action = NONE_ACT;

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
            fprintf(stderr, "Usage: %s [ -x command | -s file | -b -s file ]\n", argv[0]);
            exit(EXIT_SUCCESS);
        case 'x':
            option.action = X_ACT;
            option.cmds = optarg;
            break;
        case 's':
            if (option.action == NONE_ACT)
                option.action = S_ACT;
            option.input_file = optarg;
            break;
        case 'b':
            option.action = BS_ACT;
            option.input_file = optarg;
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    switch (option.action)
    {
    case X_ACT:
        cmd_argv = parse_command(argv[1], &cmd_argc);
        pid = launch_command(cmd_argv);

        printf("PID: %d\n", getpid());
        printf("PID hijo: %d\n", pid);

        for (i = 0; cmd_argv[i] != NULL; i++)
            free(cmd_argv[i]); // Free individual argument
        free(cmd_argv);
        break;
    case S_ACT:
        exec_file(option.input_file, 0);
        break;
    case BS_ACT:
        exec_file(option.input_file, 1);
        break;
    case NONE_ACT:
        fprintf(stderr, "Usage: %s [ -x command | -s file | -b -s file ]\n");
        break;
    default:
        break;
    }

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