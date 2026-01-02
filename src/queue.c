#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/queue.h"

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

// Ödünç Al (Kuyruğa Ekle)
void enqueue_loan(Queue* q, const char* userName, const char* bookTitle) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    strncpy(newNode->userName, userName, MAX_STR);
    strncpy(newNode->bookTitle, bookTitle, MAX_STR);

    // Otomatik Tarih Alımı
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(newNode->borrowDate, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    printf("\n[BASARILI] %s, '%s' kitabini odunc aldi. Tarih: %s\n", userName, bookTitle, newNode->borrowDate);
}

// İade Et (Kuyruktan Çıkar - FIFO)
void dequeue_return(Queue* q) {
    if (q->front == NULL) {
        printf("\n[UYARI] Odunc verilmis kitap bulunmuyor.\n");
        return;
    }

    QueueNode* temp = q->front;
    printf("\n[IADE] %s isimli kullanici '%s' kitabini iade etti.\n", temp->userName, temp->bookTitle);
    printf("(Bu kitap %s tarihinde alinmisti ve ilk iade edilmesi gerekendi.)\n", temp->borrowDate);

    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
}

void displayLoans(Queue* q) {
    if (q->front == NULL) {
        printf("\n[BILGI] Su an odunc verilmis kitap yok.\n");
        return;
    }
    QueueNode* curr = q->front;
    printf("\n--- MEVCUT ODUNC LISTESI (FIFO SIRALAMASI) ---\n");
    printf("%-15s | %-20s | %-12s\n", "Kullanici", "Kitap", "Alim Tarihi");
    printf("----------------------------------------------------------\n");
    while (curr != NULL) {
        printf("%-15s | %-20s | %-12s\n", curr->userName, curr->bookTitle, curr->borrowDate);
        curr = curr->next;
    }
}