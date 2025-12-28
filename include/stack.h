#ifndef STACK_H
#define STACK_H

#include "book.h"

// struct StackNode tanımını sildik, çünkü book.h içinde zaten var.

void push(StackNode** top, const char* title);
void displayStack(StackNode* top);
void freeStack(StackNode* top);

#endif