#ifndef AVL_H
#define AVL_H

#include "book.h"

// Eğer book.h içinde tanımlı değilse struct buraya gelmeli
// typedef struct AVLNode { ... } AVLNode;

int getHeight(AVLNode* n);
int getBalance(AVLNode* n);
int max(int a, int b);
AVLNode* rightRotate(AVLNode* y);
AVLNode* leftRotate(AVLNode* x);
AVLNode* insert_avl(AVLNode* node, Book data);

// BU SATIRIN OLDUĞUNDAN EMİN OL:
AVLNode* search_avl(AVLNode* root, int id);

#endif