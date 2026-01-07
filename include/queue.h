#ifndef QUEUE_H
#define QUEUE_H

#include "book.h"
#include <time.h>

typedef struct QueueNode {
    char userName[MAX_STR];
    char bookTitle[MAX_STR];
    char borrowDate[20]; // YYYY-MM-DD formatı için
    char dueDate[20]; // son teslim tarihi
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

Queue* createQueue();
void enqueue_loan(Queue* q, const char* userName, const char* bookTitle);
void dequeue_return(Queue* q);
void displayLoans(Queue* q);
int remove_specific_loan(Queue* q, const char* bookTitle);

#endif