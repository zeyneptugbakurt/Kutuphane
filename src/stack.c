#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/stack.h"
#include "../include/book.h" // StackNode ve MAX_STR tanımı buradan gelir

// Stack'e eleman ekle (En başa)
void push(StackNode** top, const char* title) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) return;

    strncpy(newNode->last_search, title, MAX_STR);
    newNode->last_search[MAX_STR - 1] = '\0';

    newNode->next = *top;
    *top = newNode;
}

// Stack'i ekrana yazdır (Debug amaçlı)
void displayStack(StackNode* top) {
    if (top == NULL) {
        printf("\n[BILGI] Henuz bir arama gecmisi bulunmuyor.\n");
        return;
    }

    printf("\n--- SON ARAMALARINIZ (Gecmis) ---\n");
    StackNode* current = top;
    int i = 1;
    while (current != NULL) {
        printf("%d. %s\n", i++, current->last_search);
        current = current->next;
    }
}

// Tüm Stack'i temizle
void freeStack(StackNode* top) {
    StackNode* temp;
    while (top != NULL) {
        temp = top;
        top = top->next;
        free(temp);
    }
}

// ---------------------------------------------------------
// YENİ EKLENEN FONKSİYONLAR (Undo ve Silme İşlemleri İçin)
// ---------------------------------------------------------

// Stack'in en tepesindeki elemanı çeker, listeden siler ve veriyi döndürür.
// (Geri alma işlemi ve toplu temizleme için kullanılır)
char* pop(StackNode** top) {
    if (*top == NULL) return NULL; // Stack boşsa NULL dön

    StackNode* temp = *top;
    
    // Veriyi kaybetmemek için kopyalıyoruz
    char* data = (char*)malloc(MAX_STR);
    if (data == NULL) return NULL;
    
    strcpy(data, temp->last_search);

    // Listeden çıkar
    *top = (*top)->next;
    
    // Eski düğümü hafızadan sil
    free(temp);
    
    return data; // Veriyi çağırana gönder (Çağıran kişi free etmeli!)
}

// Stack içinden belirli bir metni bulup aradan siler
// (Geçmiş ekranındaki 'X' butonları için)
void delete_search_node(StackNode** top, const char* target) {
    if (*top == NULL) return;

    StackNode *curr = *top;
    StackNode *prev = NULL;

    while (curr != NULL) {
        // Aranan kelime bulundu mu?
        if (strcmp(curr->last_search, target) == 0) {
            
            if (prev == NULL) {
                // Silinecek eleman en baştaysa (Top)
                *top = curr->next;
            } else {
                // Silinecek eleman aradaysa, bağı kopar ve sonrakine bağla
                prev->next = curr->next;
            }
            
            free(curr); // Hafızayı temizle
            return; // İşlem bitti, çık
        }
        
        // İlerlemeye devam et
        prev = curr;
        curr = curr->next;
    }
}