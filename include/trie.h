#ifndef TRIE_H
#define TRIE_H

#include "book.h"

// --- YENİ: ID Listesi Yapısı ---
// Bir kelime (örneğin yazar adı) birden fazla kitaba ait olabilir.
typedef struct IdNode {
    int id;
    struct IdNode* next;
} IdNode;

// Trie Düğümü
typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
    struct IdNode* ids; // YENİ: Tek bir ID yerine ID listesi tutuyoruz
} TrieNode;

// Fonksiyonlar
TrieNode* createTrieNode();
void insert_trie(TrieNode* root, const char* title, int book_id);
int search_trie(TrieNode* root, const char* title);
int charToIndex(char c);
void normalize_string(const char* src, char* dest);
void search_prefix_trie(TrieNode* root, const char* prefix, Book* bookList, int bookCount);
void display_suggestions(TrieNode* curr, char* currentPrefix, int level, Book* bookList, int bookCount);

#endif