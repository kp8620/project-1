#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "queue.h"

/*

void quantum_timer(int ms);
void times_up(int signal);

int main(void)
{




    if (signal(SIGALRM, times_up) == SIG_ERR)
    {
        perror("Failed to set signal handler\n");
        exit(EXIT_FAILURE);
    }
    quantum_timer(10000);
    printf("Timer started\n");
    // Setup timer to send an Alarm
    pause(); // Why??

    printf("Now back to your regularly scheduled program\n");



return 0;
}




void quantum_timer(int ms)
{
    struct itimerval timer;
    int seconds = ms / 1000;
    int usec = (ms % 1000) * 1000;
    timer.it_value.tv_sec = seconds;
    timer.it_value.tv_usec = usec;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;


    if(setitimer(ITIMER_REAL, &timer, NULL) == -1){
        perror("Error setting timer\n");
        exit(EXIT_FAILURE);
    }
}

void times_up(int signal)
{

    // print Timer ran out for (current pid)
    printf("Times Up\n");
}
*/

#define MAX_PROCESSES 120
#define MAX_ARGUEMENTS 10

typedef struct
{
    char *program_id;
    char *args[MAX_ARGUEMENTS + 1];
    int num_args;
} Program;

Program programs[MAX_PROCESSES];
int prgnum = 0;

void add_program(char *name, char **args, int num_args)
{
    if (prgnum >= MAX_PROCESSES)
    {
        printf("Too many Programs\n");
        exit(EXIT_FAILURE);
    }

    Program *program_list = &programs[prgnum++];
    program_list->program_id = strdup(name);
    program_list->num_args = num_args;

    for (int i = 0; i < num_args; i++)
    {
        program_list->args[i] = strdup(args[i]);
    }

    program_list->args[num_args] = NULL;
}

int arguement_counter(int argc, char *argv[])
{
    char *program_name = NULL;
    char *args[MAX_ARGUEMENTS];
    int argument_number = 0;
    int arg_index = 0;
    int program_counter = 0;

    program_name = argv[2];
    int Programflag = 1;
    // printf("Quantum: %d\n", quantum);

    for (int i = 2; i < argc; i++) // Start at 2 since first two parts of arg do not count
    {

        if (strcmp(argv[i], ":") == 0) // skip : sperator
        {
            add_program(program_name, args, arg_index);
            printf("Program: %s\n", program_name);
            for (int j = 0; j < arg_index; j++)
            {
                printf("Arg %d: %s\n", j, args[j]);
            }
            arg_index = 0;
            Programflag = 1;
            // printf("\n");
        }

        else
        {
            if (Programflag)
            {
                // If it is a Program
                // printf("Program: %s\n", argv[i]);
                program_name = argv[i];
                program_counter++;
                Programflag = 0;
            }
            else
            {
                // If it is an Arguement
                args[arg_index] = argv[i];
                // printf("Argument: %s\n", argv[i]);
                arg_index++;
            }
        }

        if (i == argc-1)
        {
            add_program(program_name, args, arg_index);
            printf("Program: %s\n", program_name);
            for (int j = 0; j < arg_index; j++)
            {
                printf("Arg %d: %s\n", j, args[j]);
            }

            program_counter ++;
            prgnum = program_counter;
        }
    }
}

int main(int argc, char *argv[])
{

    arguement_counter(argc, argv);

    return 0;
}







//Junk



// struct Program *line_parser(int argc, char *argv[])
// {
//     Program *programs = malloc(MAX_PRG * sizeof(Program));
//     int program_index = 0;
//     int Program_flag = 1;
//     for (int i = 2; i < argc; i++) // Start at 2 since first two parts of arg do not count
//     {

//         if (strcmp(argv[i], ":") == 0) // skip : sperator
//         {
//         }
//     }
// }