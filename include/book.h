#ifndef BOOK_H
#define BOOK_H

// Sabitler
#define MAX_STR 256
#define ALPHABET_SIZE 36 // 26 Harf + 10 Rakam

// Kitap Yapısı
typedef struct Book {
    int id;
    char title[MAX_STR];
    char author[MAX_STR];
    char genre[MAX_STR];    // io.c 'genre' aradığı için ismini düzelttik
    int year;
    float score;
} Book;

// StackNode: Diğer dosyaları değiştirmemek için buraya geri ekledik
typedef struct StackNode {
    char last_search[MAX_STR]; 
    struct StackNode *next;
} StackNode;

// AVLNode: Diğer dosyaları değiştirmemek için buraya geri ekledik
typedef struct AVLNode {
    Book data;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

/* NOT: TrieNode yapısını buraya EKLEMİYORUZ. 
   O sadece trie.h içinde kalacak, yoksa "redefinition" hatası alırsın.
*/

#endif