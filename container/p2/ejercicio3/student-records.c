#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"

/* Assume lines in the text file are no larger than 100 chars */
#define MAXLEN_LINE_FILE 100

int print_text_file(char *path)
{
	FILE *file;
	char line[MAXLEN_LINE_FILE + 1];
	student_t *entries;
	student_t *cur_entry;
	char *token;
	char *lineptr;
	token_id_t token_id;
	int entry_idx;
	int cur_line;

	if ((file = fopen(path, "r")) == NULL)
	{
		fprintf(stderr, "%s could not be opened: ", path);
		perror(NULL);
		return -1;
	}

	// first char is no of lines
	int n_lines = fgetc(file);

	if (fscanf(file, "%d\n", &n_lines) != 1)
	{
		fprintf(stderr, "Error al leer el número de registros\n");
		fclose(file);
		return -1;
	}

	// /* Figure out number of lines */
	// while (fgets(line, MAXLEN_LINE_FILE + 1, path) != NULL)
	// {
	// 	line_count++;
	// 	/* Discard lines that begin with # */
	// 	if (line[0] != '#')
	// 		entry_count++;
	// }

	/* Rewind position indicator*/
	fseek(file, 0, SEEK_SET);

	entries = malloc(sizeof(student_t) * n_lines);
	/* zero fill the array of structures */
	memset(entries, 0, sizeof(student_t) * n_lines);

	/* Parse file */
	entry_idx = 0;
	cur_line = 1;
	while (fgets(line, MAX_PASSWD_LINE + 1, file) != NULL)
	{

		/* Discard lines that begin with # */
		// if (line[0] == '#')
		// {
		// 	cur_line++;
		// 	continue;
		// }

		/* Point to the beginning of the line */
		lineptr = line;
		token_id = STUDENT_ID_IDX;
		cur_entry = &entries[entry_idx];

		while ((token = strsep(&lineptr, ":")) != NULL)
		{
			switch (token_id)
			{
			case STUDENT_ID_IDX:
				strcpy(cur_entry->student_id, token);
				break;
			case NIF_IDX:
				strcpy(cur_entry->NIF, token);
				break;
			case FIRST_NAME_IDX:
				strcpy(cur_entry->first_name, token);
				break;
			case LAST_NAME_IDX:
				strcpy(cur_entry->last_name, token);
				break;
			default:
				break;
			}
			token_id++;
		}

		if (token_id != NR_FIELDS_STUDENT)
		{
			fprintf(stderr, "Could not process all tokens from line %d of /etc/passwd\n", entry_idx + 1);
			return -1;
		}
		cur_line++;
		entry_idx++;
	}
	//(*nr_entries) = entry_count;

	// printing
	for (int i = 0; i < n_lines; i++)
	{
		student_t *e = &entries[i]; /* Point to current entry */
		fprintf(stdout, "[Entry #%d]\n", i);
		fprintf(stdout, "\tID=%d\n\tNIF=%s\n\t"
						"First Name=%s\n\tLast Name=%s\n\t",
				e->student_id, e->NIF, e->first_name, e->last_name);
	}

	return 0;
}

int print_binary_file(char *path)
{
	/* To be completed  (part B) */
	return 0;
}

int write_binary_file(char *input_file, char *output_file)
{
	/* To be completed  (part C) */
	return 0;
}

int main(int argc, char *argv[])
{
	int ret_code, opt;
	struct options options;

	/* Initialize default values for options */
	options.input_file = "students-db.txt";
	options.output_file = "stdout";
	options.action = NONE_ACT;
	ret_code = 0;

	/* Parse command-line options (incomplete code!) */
	while ((opt = getopt(argc, argv, "hi:p")) != -1)
	{
		switch (opt)
		{
		case 'h':
			fprintf(stderr, "Usage: %s [ -h ]\n", argv[0]);
			exit(EXIT_SUCCESS);
		case 'i':
			options.input_file = optarg;
			break;
		case 'p':
			options.action = PRINT_TEXT_ACT;
			break;

		default:
			exit(EXIT_FAILURE);
		}
	}

	if (options.input_file == NULL)
	{
		fprintf(stderr, "Must specify one record file as an argument of -i\n");
		exit(EXIT_FAILURE);
	}

	switch (options.action)
	{
	case NONE_ACT:
		fprintf(stderr, "Must indicate one of the following options: -p, -o, -b \n");
		ret_code = EXIT_FAILURE;
		break;
	case PRINT_TEXT_ACT:
		/* Part A */
		ret_code = print_text_file(options.input_file);
		break;
	case WRITE_BINARY_ACT:
		/* Part B */
		ret_code = write_binary_file(options.input_file, options.output_file);
		break;
	case PRINT_BINARY_ACT:
		/* Part C */
		ret_code = print_binary_file(options.input_file);
		break;
	default:
		break;
	}
	exit(ret_code);
}
