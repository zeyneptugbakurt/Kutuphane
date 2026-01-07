#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/trie.h"

TrieNode* createTrieNode() {
    TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
    newNode->isEndOfWord = 0;
    newNode->ids = NULL; // Liste başlangıcı boş
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

int charToIndex(char c) {
    if (isdigit((unsigned char)c)) return (c - '0') + 26;
    return tolower((unsigned char)c) - 'a';
}

void normalize_string(const char* src, char* dest) {
    int i = 0, j = 0;
    while (src[i] != '\0') {
        unsigned char c = (unsigned char)src[i];
        if (c > 127) {
            unsigned char next = (unsigned char)src[i+1];
            if (c == 0xC3) {
                if (next == 0xA7 || next == 0x87) { dest[j++] = 'c'; i += 2; continue; }
                if (next == 0xBC || next == 0x9C) { dest[j++] = 'u'; i += 2; continue; }
                if (next == 0xB6 || next == 0x96) { dest[j++] = 'o'; i += 2; continue; }
            } else if (c == 0xC4) {
                if (next == 0x9F || next == 0x9E) { dest[j++] = 'g'; i += 2; continue; }
                if (next == 0xB1 || next == 0xB0) { dest[j++] = 'i'; i += 2; continue; }
            } else if (c == 0xC5) {
                if (next == 0x9F || next == 0x9E) { dest[j++] = 's'; i += 2; continue; }
            }
            i += 2; 
        } else {
            dest[j++] = tolower(c);
            i++;
        }
    }
    dest[j] = '\0';
}

// --- GÜNCELLENDİ: Listeye Ekleme Mantığı ---
void insert_trie(TrieNode* root, const char* title, int book_id) {
    char cleanTitle[MAX_STR];
    normalize_string(title, cleanTitle);

    char tempTitle[MAX_STR];
    strncpy(tempTitle, cleanTitle, MAX_STR);
    tempTitle[MAX_STR - 1] = '\0';

    char* word = strtok(tempTitle, " ");
    while (word != NULL) {
        TrieNode* curr = root;
        for (int i = 0; word[i] != '\0'; i++) {
            if (!isalnum((unsigned char)word[i])) continue;
            int index = charToIndex(word[i]);
            if (index < 0 || index >= ALPHABET_SIZE) continue;

            if (curr->children[index] == NULL) {
                curr->children[index] = createTrieNode();
            }
            curr = curr->children[index];
        }
        
        // Kelime bitti, ID'yi listeye ekle
        curr->isEndOfWord = 1;
        
        // Yeni ID düğümü oluştur
        IdNode* newIdNode = (IdNode*)malloc(sizeof(IdNode));
        newIdNode->id = book_id;
        newIdNode->next = NULL;

        // Listeye ekle (Listenin başına eklemek en hızlısıdır)
        newIdNode->next = curr->ids;
        curr->ids = newIdNode;
        
        word = strtok(NULL, " "); 
    }
}

// Search fonksiyonları konsol testi içindi, GUI search.c'yi kullanıyor.
// Ancak derleme hatası olmaması için burayı da uyumlu bırakıyoruz.
void display_suggestions(TrieNode* curr, char* currentPrefix, int level, Book* bookList, int bookCount) {
    // Bu fonksiyon sadece debug amaçlıdır
}

void search_prefix_trie(TrieNode* root, const char* prefix, Book* bookList, int bookCount) {
    // Bu fonksiyon sadece debug amaçlıdır
}

int search_trie(TrieNode* root, const char* title) {
    // Basitleştirilmiş, sadece ilk bulduğunu döndürür
    return -1; 
}