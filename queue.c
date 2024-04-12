#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#define MAX_SIZE 130

typedef struct{

    pid_t* items;
    int capacity;
    int size;
    int top;
    int end;



} Queue;




Queue* queueInit(){

    Queue* q = malloc(sizeof(Queue));
    q->items = malloc(MAX_SIZE*sizeof(pid_t));
    q->size =0;
    q->top = -1;
    q->end = -1;
    return q;

}

int checkfull(Queue* q){
    if ((q->end + 1) % MAX_SIZE == q->top) {
        return 1;
    } else {
        return 0;
    }


}


int isEmpty(Queue* q) {
    if (q->top == -1)
        return 1;
    else
        return 0;
}

void queue(Queue* q, int process){//Change this to take in processess later

   if (checkfull(q))
        printf("Queue is full\n");
    else {
        if (q->top == -1)
            q->top = 0;
        q->end = (q->end + 1) % MAX_SIZE;
        q->items[q->end] = process;
    }

}


int dequeue(Queue* q) {
    int item;
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        item = -1;
    } else {
        item = q->items[q->top];
        if (q->top == q->end) {
            q->top = -1;
            q->end = -1;
        } else {
            q->top = (q->top + 1) % MAX_SIZE;
        }
    }
    return item;
}



void list_free(Queue* q){
    free(q->items);
    free(q);


}