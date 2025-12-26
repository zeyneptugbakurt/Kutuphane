#ifndef BOOK_H
#define BOOK_H

#define MAX_STR 100

// Kitap verisi için temel yapı
typedef struct {
    int id;                 // Kitap ID'si (AVL ağacında key olarak kullanılabilir)
    char title[MAX_STR];    // Kitap adı (Trie ağacı için kullanılacak)
    char author[MAX_STR];   // Yazar adı
    char genre[MAX_STR];    // Tür (Filtreleme için)
    int year;               // Basım yılı (Sıralama için)
    double score;           // Popülerlik/Skor (Sıralama için)
} Book;

// Arkadaşının yazacağı AVL Düğümü
typedef struct AVLNode {
    Book data;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

// Senin yazacağın Stack Yapısı (Undo için)
typedef struct StackNode {
    char last_search[MAX_STR]; 
    struct StackNode *next;
} StackNode;

// Arkadaşının yazacağı Trie Düğümü (Prefix arama için)
#define ALPHABET_SIZE 26 // Veya ASCII için 128
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
    int book_id; // Kelimenin bittiği yerdeki kitabın ID'si
} TrieNode;

#endif