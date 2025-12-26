#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/stack.h"

// Arkadaşının book.h içindeki tanımına uygun olarak:
// char last_search[MAX_STR] ismini kullanıyoruz.

void push(StackNode** top, const char* title) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) return;

    strncpy(newNode->last_search, title, MAX_STR);
    newNode->last_search[MAX_STR - 1] = '\0';

    newNode->next = *top;
    *top = newNode;
}

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

void freeStack(StackNode* top) {
    StackNode* temp;
    while (top != NULL) {
        temp = top;
        top = top->next;
        free(temp);
    }
}