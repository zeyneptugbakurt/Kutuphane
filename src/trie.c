#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/trie.h"

// Yeni bir Trie düğümü oluşturur
TrieNode* createTrieNode() {
    TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
    newNode->isEndOfWord = 0;
    newNode->book_id = -1;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Karakteri 0-25 arası indekse çevirir
int charToIndex(char c) {
    return tolower((unsigned char)c) - 'a';
}

// Trie ağacına kitap ismini ekler
void insert_trie(TrieNode* root, const char* title, int book_id) {
    TrieNode* curr = root;
    for (int i = 0; title[i] != '\0'; i++) {
        // Sadece harfleri işleme alıyoruz
        if (!isalpha((unsigned char)title[i])) continue; 
        
        int index = charToIndex(title[i]);
        if (index < 0 || index >= ALPHABET_SIZE) continue;

        if (curr->children[index] == NULL) {
            curr->children[index] = createTrieNode();
        }
        curr = curr->children[index];
    }
    curr->isEndOfWord = 1;
    curr->book_id = book_id;
}

// YARDIMCI FONKSİYON: Bir düğümden aşağı doğru tüm kitapları bulup yazdırır (Recursive)
void display_suggestions(TrieNode* curr, char* currentPrefix, int level, Book* bookList, int bookCount) {
    if (curr->isEndOfWord) {
        currentPrefix[level] = '\0';
        // Kitap listesinde ID'yi bulup detayları yazdırıyoruz
        for(int i = 0; i < bookCount; i++) {
            if(bookList[i].id == curr->book_id) {
                printf("  [>] %s (ID: %d)\n", bookList[i].title, bookList[i].id);
            }
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (curr->children[i]) {
            currentPrefix[level] = i + 'a';
            display_suggestions(curr->children[i], currentPrefix, level + 1, bookList, bookCount);
        }
    }
}

// Önek (Prefix) ile arama yapan ana fonksiyon
void search_prefix_trie(TrieNode* root, const char* prefix, Book* bookList, int bookCount) {
    TrieNode* curr = root;
    
    // Önce aranan önekin sonuna kadar ağaçta git
    for (int i = 0; prefix[i] != '\0'; i++) {
        if (!isalpha((unsigned char)prefix[i])) continue;
        int index = charToIndex(prefix[i]);
        if (index < 0 || index >= ALPHABET_SIZE || curr->children[index] == NULL) {
            printf("\n[BILGI] '%s' ile baslayan bir kitap bulunamadi.\n", prefix);
            return;
        }
        curr = curr->children[index];
    }

    // Önekin bittiği yerden itibaren tüm alt dalları (kelimeleri) yazdır
    printf("\n--- '%s' ile baslayan sonuclar ---\n", prefix);
    char buffer[MAX_STR];
    strcpy(buffer, prefix);
    display_suggestions(curr, buffer, strlen(prefix), bookList, bookCount);
}

// Tam isim araması (Daha önce yazdığın fonksiyon)
int search_trie(TrieNode* root, const char* title) {
    TrieNode* curr = root;
    for (int i = 0; title[i] != '\0'; i++) {
        if (!isalpha((unsigned char)title[i])) continue;
        int index = charToIndex(title[i]);
        if (index < 0 || index >= ALPHABET_SIZE || curr->children[index] == NULL) return -1;
        curr = curr->children[index];
    }
    return (curr != NULL && curr->isEndOfWord) ? curr->book_id : -1;
}