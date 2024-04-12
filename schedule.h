#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <sys/types.h> 
#include <unistd.h>

#define MAX_PROCESSES 120
#define MAX_ARGUEMENTS 10

typedef struct {
    char *program_id;
    char *args[MAX_ARGUEMENTS + 1];
    int num_args;
} Program;

void arguement_counter(int argc, char *argv[]);
void quantum_timer(int ms);
void times_up(int signal);
int process_status(pid_t current_pid);
void add_program(char *name, char **args, int num_args);

#endif // SCHEDULE_H
