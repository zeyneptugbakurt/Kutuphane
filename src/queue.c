#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/queue.h"

/* --------------------------------------------------
   QUEUE OLUŞTUR
-------------------------------------------------- */
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

/* --------------------------------------------------
   ÖDÜNÇ AL (ENQUEUE)
-------------------------------------------------- */
void enqueue_loan(Queue* q, const char* userName, const char* bookTitle)
{
    if (!q) return;

    QueueNode* newNode = malloc(sizeof(QueueNode));
    if (!newNode) return;

    strncpy(newNode->userName, userName, MAX_STR - 1);
    newNode->userName[MAX_STR - 1] = '\0';

    strncpy(newNode->bookTitle, bookTitle, MAX_STR - 1);
    newNode->bookTitle[MAX_STR - 1] = '\0';

    /* 📅 BORROW DATE = BUGÜN */
    time_t now = time(NULL);
    struct tm borrow = *localtime(&now);
    strftime(newNode->borrowDate, sizeof(newNode->borrowDate),
             "%Y-%m-%d", &borrow);

    /* ⏳ DUE DATE = BUGÜN + 14 GÜN */
    borrow.tm_mday += 14;
    mktime(&borrow); // tarih taşmalarını düzeltir
    strftime(newNode->dueDate, sizeof(newNode->dueDate),
             "%Y-%m-%d", &borrow);

    newNode->next = NULL;

    if (!q->rear) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}


/* --------------------------------------------------
   İADE ET (DEQUEUE - FIFO)
-------------------------------------------------- */
void dequeue_return(Queue* q) {
    if (q->front == NULL) {
        printf("\n[UYARI] Odunc verilmis kitap yok.\n");
        return;
    }

    QueueNode* temp = q->front;

    printf("\n[IADE] %s, '%s' kitabini iade etti.\n",
           temp->userName, temp->bookTitle);
    printf("Alim: %s | Son Teslim: %s\n",
           temp->borrowDate, temp->dueDate);

    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;

    free(temp);
}

/* --------------------------------------------------
   ÖDÜNÇ LİSTESİNİ GÖSTER
-------------------------------------------------- */
void displayLoans(Queue* q) {
    if (q->front == NULL) {
        printf("\n[BILGI] Su anda odunc alinmis kitap yok.\n");
        return;
    }

    QueueNode* curr = q->front;

    printf("\n--- MEVCUT ODUNC LISTESI (FIFO) ---\n");
    printf("%-15s | %-20s | %-12s | %-12s\n",
           "Kullanici", "Kitap", "Alim", "Son Teslim");
    printf("--------------------------------------------------------------\n");

    while (curr != NULL) {
        printf("%-15s | %-20s | %-12s | %-12s\n",
               curr->userName,
               curr->bookTitle,
               curr->borrowDate,
               curr->dueDate);
        curr = curr->next;
    }
}
