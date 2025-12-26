#include <stdio.h>
#include <stdlib.h>
#include "../include/avl.h"

// Düğüm yüksekliğini döndürür
int getHeight(AVLNode* n) {
    if (n == NULL) return 0;
    return n->height;
}

// İki sayıdan büyük olanı bulur
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Denge faktörünü hesaplar
int getBalance(AVLNode* n) {
    if (n == NULL) return 0;
    return getHeight(n->left) - getHeight(n->right);
}

// Sağa Döndürme (Right Rotate)
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Sola Döndürme (Left Rotate)
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// AVL Ağacına Yeni Kitap Ekleme
AVLNode* insert_avl(AVLNode* node, Book data) {
    if (node == NULL) {
        AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
        newNode->data = data;
        newNode->left = newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }

    if (data.id < node->data.id)
        node->left = insert_avl(node->left, data);
    else if (data.id > node->data.id)
        node->right = insert_avl(node->right, data);
    else
        return node; // Aynı ID eklenmez

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    // Sol Sol Durumu
    if (balance > 1 && data.id < node->left->data.id)
        return rightRotate(node);

    // Sağ Sağ Durumu
    if (balance < -1 && data.id > node->right->data.id)
        return leftRotate(node);

    // Sol Sağ Durumu
    if (balance > 1 && data.id > node->left->data.id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Sağ Sol Durumu
    if (balance < -1 && data.id < node->right->data.id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}
