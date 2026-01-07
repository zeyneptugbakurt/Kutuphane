#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/search.h"
#include "../include/trie.h"

static TrieNode* trieRoot = NULL;
static int trieInitialized = 0;

static int find_book_by_id(Book* library, int count, int id) {
    for (int i = 0; i < count; i++) {
        if (library[i].id == id) return i;
    }
    return -1;
}

// --- GÜNCELLENDİ: ID Listesini Gezme ---
static void dfs_trie(TrieNode* node, Book* library, int count, Book* results, int* found) {
    if (node->isEndOfWord) {
        // Liste üzerindeki tüm kitapları topla
        IdNode* currentId = node->ids;
        while (currentId != NULL) {
            
            int idx = find_book_by_id(library, count, currentId->id);
            if (idx != -1) {
                // Aynısı eklenmiş mi kontrol et (Duplicate önleme)
                int alreadyAdded = 0;
                for(int k=0; k<*found; k++) {
                    if(results[k].id == library[idx].id) {
                        alreadyAdded = 1;
                        break;
                    }
                }
                if(!alreadyAdded) {
                    results[(*found)++] = library[idx];
                }
            }
            currentId = currentId->next;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            dfs_trie(node->children[i], library, count, results, found);
        }
    }
}

static void init_trie(Book* library, int count) {
    trieRoot = createTrieNode();
    for (int i = 0; i < count; i++) {
        insert_trie(trieRoot, library[i].title, library[i].id);
        insert_trie(trieRoot, library[i].author, library[i].id);
    }
    trieInitialized = 1;
}

int search_books_by_prefix(Book* library, int count, const char* prefix, Book* results) {
    if (!trieInitialized) init_trie(library, count);

    TrieNode* curr = trieRoot;
    
    // Normalizasyon ve Trie üzerinde ilerleme
    for (int i = 0; prefix[i] != '\0'; i++) {
        char charToProcess = 0;
        unsigned char c = (unsigned char)prefix[i];

        if (c > 127) { 
             unsigned char next = (unsigned char)prefix[i+1];
             if (c == 0xC3) {
                 if (next == 0xA7 || next == 0x87) { charToProcess = 'c'; i++; }
                 else if (next == 0xBC || next == 0x9C) { charToProcess = 'u'; i++; }
                 else if (next == 0xB6 || next == 0x96) { charToProcess = 'o'; i++; }
             } else if (c == 0xC4) {
                 if (next == 0x9F || next == 0x9E) { charToProcess = 'g'; i++; }
                 else if (next == 0xB1 || next == 0xB0) { charToProcess = 'i'; i++; }
             } else if (c == 0xC5) {
                 if (next == 0x9F || next == 0x9E) { charToProcess = 's'; i++; }
             } else { i++; continue; }
        } else if (isalnum(c)) { 
            charToProcess = tolower(c);
        } else { continue; }

        if (charToProcess != 0) {
            int idx = charToIndex(charToProcess);
            if (idx < 0 || idx >= ALPHABET_SIZE || !curr->children[idx]) return 0;
            curr = curr->children[idx];
        }
    }

    int found = 0;
    dfs_trie(curr, library, count, results, &found);
    return found;
}