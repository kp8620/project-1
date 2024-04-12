#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "queue.h"
#define MAX_PROCESSES 120
#define MAX_ARGUEMENTS 10

void arguement_counter(int argc, char *argv[]);
void quantum_timer(int ms);
void times_up(int signal);
int process_status(pid_t current_pid);
void add_program(char *name, char **args, int num_args);

typedef struct
{
    char *program_id;
    char *args[MAX_ARGUEMENTS + 1];
    int num_args;
} Program;

Program programs[MAX_PROCESSES];
int prgnum = 0;

// Make sure execvp runs on unix server as well

int main(int argc, char *argv[])
{
    //printf("HELLO\n");
    int quantum = atoi(argv[1]); // Convert quantum from string to integer from command line
    // int queue_size = arguement_counter(argc, argv);
    arguement_counter(argc, argv);
    Queue *process_queue = queueInit(); // initialize queue

    // for (int i = 0; i < prgnum-1; i++)
    // {                                                          // Iterate over each program
    //     printf("Program %d: %s\n", i, programs[i].program_id); // Print the program's name

    //     for (int j = 0; programs[i].args[j] != NULL; j++)
    //     { // Iterate over each argument of the program

    //         printf("  Arg %d: %s\n", j, programs[i].args[j]); // Print each argument
    //     }
    // }

    for (int i = 0; i < prgnum-1; i++)
    {

        pid_t pid = fork();

        if (pid == 0)
        {
            // child
            // Immedietly send SIGSTOP

            //printf("Starting program: %s\n", programs[i].program_id);
            if ((execv(programs[i].program_id, programs[i].args)) == -1)
            {
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
            //printf("got to 65\n");

            
        }

        else if (pid > 0)
        {
            // Parent process
            // Parent handles the enqueuing of all children

            queue(process_queue, pid);
            //printf("Enqueued child PID: %d\n", pid);
            kill(pid, SIGSTOP);
        }
        else
        {
            // Fork failed
            perror("fork");
            return -1;
        }
    }

    while (!isEmpty(process_queue))
    {
        pid_t current_pid = dequeue(process_queue);
        //printf("Dequeued child PID: %d\n", current_pid);

        kill(current_pid, SIGCONT);

        if (signal(SIGALRM, times_up) == SIG_ERR)
        {
            perror("Failed to set signal handler\n");
            exit(EXIT_FAILURE);
        }

        quantum_timer(quantum); // Setup timer to send an Alarm
        //printf("Timer started\n");

        pause(); // Why??

        // We need to pause the whole program until alarm goes off then it will get to the Kill current_pid
        kill(current_pid, SIGSTOP);
        //printf("Current Program has stopped\n"); // TODO

        if (process_status(current_pid))
        {

            queue(process_queue, current_pid);
        }
    }

    list_free(process_queue);



    for(int ind =0; ind < prgnum-1; ind++){
        free(programs[ind].program_id);

        for(int index = 0; index < programs[ind].num_args; index ++){
            free(programs[ind].args[index]);
        }
    }


    return 0;
}

void arguement_counter(int argc, char *argv[])
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
            /*
            printf("Program: %s\n", program_name);
            for (int j = 0; j < arg_index; j++)
            {
                printf("Arg %d: %s\n", j, args[j]);
            }
            */
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

        if (i == argc - 1)
        {
            add_program(program_name, args, arg_index);
            /*  printf("Program: %s\n", program_name);
              for (int j = 0; j < arg_index; j++)
              {
                  printf("Arg %d: %s\n", j, args[j]);
              }
              */
            program_counter++;
            prgnum = program_counter;
        }
    }
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

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
    {
        perror("Error setting timer\n");
        exit(EXIT_FAILURE);
    }
}

void times_up(int signal)
{

    // print Timer ran out for (current pid)
    printf("");
}

int process_status(pid_t current_pid)
{
    int status;
    pid_t process_status = waitpid(current_pid, &status, WNOHANG);

    if (process_status == 0)
    {
        // Process is not done
        return 1;
    }

    if (process_status == current_pid)
    {
        return 0;
    }

    else
    {

        perror("waitpid");
        printf("Process Errored: %d\n", current_pid);
        return -1;
    }
}

void add_program(char *name, char **args, int num_args)
{
    if (prgnum >= MAX_PROCESSES)
    {
        printf("Too many Programs\n");
        exit(EXIT_FAILURE);
    }

    Program *program_list = &programs[prgnum++];
    program_list->program_id = strdup(name);
    program_list->args[0] = strdup(name);
    program_list->num_args = num_args + 1;

    for (int i = 0; i < num_args; i++)
    {
        program_list->args[i + 1] = strdup(args[i]);
    }

    program_list->args[num_args+1] = NULL;
}

/*
    Round Robin est 3-4hours+ (Depending on Skill)

    Ingredients
    -Queue
    -Number Of Programs
    -Number of Arguements
    -Quantum
    -Pids
    -SIGSTOP and SIGCONT


    To prepare:

    Initialize a Queue
    Allow each Number of Programs to create a Fork Process with a PID
    create the flow of the Queue with the pid process
    Repeat(
    If program does not stop by the Quantum, SIGSTOP, back of the line
    Next one up: SIGCONT
    If program ends before quantum: leave it do not enqueue it
    )

    */