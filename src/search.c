#include <string.h>
#include <ctype.h>
#include "../include/search.h"
#include "../include/trie.h"

static TrieNode* trieRoot = NULL;
static int trieInitialized = 0;

/* --------------------------------------------------
   YARDIMCI: book_id -> Book index
-------------------------------------------------- */
static int find_book_by_id(Book* library, int count, int id)
{
    for (int i = 0; i < count; i++) {
        if (library[i].id == id)
            return i;
    }
    return -1;
}

/* --------------------------------------------------
   DFS (TRIE GEZME) — C'DE DIŞARIDA OLMAK ZORUNDA
-------------------------------------------------- */
static void dfs_trie(TrieNode* node,
                     Book* library, int count,
                     Book* results, int* found)
{
    if (node->isEndOfWord) {
        int idx = find_book_by_id(library, count, node->book_id);
        if (idx != -1) {
            results[(*found)++] = library[idx];
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            dfs_trie(node->children[i], library, count, results, found);
        }
    }
}

/* --------------------------------------------------
   TRIE'YI BİR KERE OLUŞTUR
-------------------------------------------------- */
static void init_trie(Book* library, int count)
{
    trieRoot = createTrieNode();

    for (int i = 0; i < count; i++) {
        insert_trie(trieRoot, library[i].title, library[i].id);
    }

    trieInitialized = 1;
}

/* --------------------------------------------------
   PREFIX ARAMA (TRIE TABANLI)
-------------------------------------------------- */
int search_books_by_prefix(Book* library, int count,
                           const char* prefix, Book* results)
{
    if (!trieInitialized)
        init_trie(library, count);

    TrieNode* curr = trieRoot;

    for (int i = 0; prefix[i]; i++) {
        if (!isalpha((unsigned char)prefix[i])) continue;

        int idx = charToIndex(prefix[i]);
        if (idx < 0 || idx >= ALPHABET_SIZE || !curr->children[idx])
            return 0;

        curr = curr->children[idx];
    }

    int found = 0;
    dfs_trie(curr, library, count, results, &found);
    return found;
}
