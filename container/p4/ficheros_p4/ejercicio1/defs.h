#ifndef DEFS_H
#define DEFS_H

/**
 * Available actions supported by the program
 */
typedef enum {
	NONE_ACT,
	X_CMD_ACT,
	FILE_X_ACT,
	FILE_BS_ACT,
} action_t;

/**
 * Structure to hold the "variables" associated with
 * command-line options
 */
struct options {
	char* input_file;
    char* cmds;
	action_t action;
};


#endif