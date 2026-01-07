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
   SIRADAKİNİ İADE ET (DEQUEUE - FIFO)
   (Bu fonksiyon sadece en baştakini siler)
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

/* --------------------------------------------------
   BELİRLİ BİR KİTABI KUYRUKTAN SİL (Ortadan Silme)
   (GUI'deki 'İade Et' butonu için gereklidir)
-------------------------------------------------- */
int remove_specific_loan(Queue* q, const char* bookTitle) {
    if (!q || !q->front) return 0;

    QueueNode *current = q->front;
    QueueNode *prev = NULL;

    // Listeyi dolaş
    while (current != NULL) {
        if (strcmp(current->bookTitle, bookTitle) == 0) {
            // Eşleşme bulundu, silme işlemi başlıyor:
            
            // 1. Eğer silinecek eleman en baştaysa (Head)
            if (prev == NULL) {
                q->front = current->next;
                // Eğer liste tamamen boşaldıysa rear'ı da sıfırla
                if (q->front == NULL) {
                    q->rear = NULL;
                }
            } 
            // 2. Eğer eleman arada veya sondaysa
            else {
                prev->next = current->next;
                // Eğer sildiğimiz eleman en sondaysa (Tail), rear'ı güncelle
                if (prev->next == NULL) {
                    q->rear = prev;
                }
            }

            free(current);
            return 1; // Başarıyla silindi
        }
        
        // Bir sonraki düğüme geç
        prev = current;
        current = current->next;
    }
    return 0; // Kitap bulunamadı
}