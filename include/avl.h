#ifndef AVL_H
#define AVL_H

#include "book.h" // AVLNode tanımı buradan gelecek

// BURADAKİ STRUCT TANIMINI SİLDİK

// Sadece fonksiyon prototipleri kalsın
int getHeight(AVLNode* n);
int getBalance(AVLNode* n);
int max(int a, int b);
AVLNode* rightRotate(AVLNode* y);
AVLNode* leftRotate(AVLNode* x);
AVLNode* insert_avl(AVLNode* node, Book data);

#endif