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

// --- GÜNCELLENDİ: Anahtar Kelime Mantığı ---
// Kitap ismini kelimelere böler ve her kelimeyi Trie'ye ekler
void insert_trie(TrieNode* root, const char* title, int book_id) {
    char tempTitle[MAX_STR];
    strncpy(tempTitle, title, MAX_STR);
    tempTitle[MAX_STR - 1] = '\0';

    // Kitap ismini boşluklara göre parçala (Örn: "Suc ve Ceza")
    char* word = strtok(tempTitle, " ");
    while (word != NULL) {
        TrieNode* curr = root;
        for (int i = 0; word[i] != '\0'; i++) {
            if (!isalpha((unsigned char)word[i])) continue;
            
            int index = charToIndex(word[i]);
            if (index < 0 || index >= ALPHABET_SIZE) continue;

            if (curr->children[index] == NULL) {
                curr->children[index] = createTrieNode();
            }
            curr = curr->children[index];
        }
        // Kelimenin bittiği yere kitabın ID'sini işaretle
        curr->isEndOfWord = 1;
        curr->book_id = book_id;
        
        word = strtok(NULL, " "); // Bir sonraki kelimeye geç ("ve", sonra "Ceza")
    }
}

// YARDIMCI FONKSİYON: Recursive olarak tüm ihtimalleri dökür
void display_suggestions(TrieNode* curr, char* currentPrefix, int level, Book* bookList, int bookCount) {
    if (curr->isEndOfWord) {
        currentPrefix[level] = '\0';
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

// Önek veya Anahtar Kelime ile arama yapan fonksiyon
void search_prefix_trie(TrieNode* root, const char* prefix, Book* bookList, int bookCount) {
    TrieNode* curr = root;
    
    for (int i = 0; prefix[i] != '\0'; i++) {
        if (!isalpha((unsigned char)prefix[i])) continue;
        int index = charToIndex(prefix[i]);
        if (index < 0 || index >= ALPHABET_SIZE || curr->children[index] == NULL) {
            printf("\n[BILGI] '%s' ile eslesen bir anahtar kelime bulunamadi.\n", prefix);
            return;
        }
        curr = curr->children[index];
    }

    printf("\n--- '%s' iceren/baslayan sonuclar ---\n", prefix);
    char buffer[MAX_STR];
    strcpy(buffer, prefix);
    display_suggestions(curr, buffer, strlen(prefix), bookList, bookCount);
}

// Tam isim araması (Geriye dönük uyumluluk için duruyor)
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