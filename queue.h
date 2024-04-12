#ifndef QUEUE_H
#define QUEUE_H

#define MAX_SIZE 100 // Maximum size of the Queue

typedef struct {
    int items[MAX_SIZE];
    int front;
    int rear;
} Queue;

Queue* queueInit();
int checkFull(Queue* q);
int isEmpty(Queue* q);
void queue(Queue* q, int value);
int dequeue(Queue* q);
#endif
