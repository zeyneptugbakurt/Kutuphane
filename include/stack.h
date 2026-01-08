#ifndef STACK_H
#define STACK_H

#include "book.h" 

void push(StackNode** top, const char* title);
char* pop(StackNode** top); // Stack'ten eleman çeker ve siler (Undo/Clear için)
void delete_search_node(StackNode** top, const char* target); // Stack içinden belirli bir elemanı siler
void displayStack(StackNode* top); // Konsol çıktısı için (Opsiyonel)
void freeStack(StackNode* top); // Tüm stack'i temizler

#endif