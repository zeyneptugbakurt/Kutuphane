#ifndef TRIE_H
#define TRIE_H

#include "book.h"

// Temel Trie Fonksiyonları
TrieNode* createTrieNode();
void insert_trie(TrieNode* root, const char* title, int book_id);
int search_trie(TrieNode* root, const char* title);
int charToIndex(char c);

// --- YENI: Önek (Prefix) Arama Fonksiyonları ---

// "nu" yazınca "nutuk" gibi başlayanları bulur
void search_prefix_trie(TrieNode* root, const char* prefix, Book* bookList, int bookCount);

// Verilen düğümden aşağıya doğru tüm kelimeleri gezer (Yardımcı Fonksiyon)
void display_suggestions(TrieNode* curr, char* currentPrefix, int level, Book* bookList, int bookCount);

#endif