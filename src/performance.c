#include <stdio.h>
#include <time.h>
#include "../include/performance.h"

void run_performance_tests(Book *bookList, int bookCount, AVLNode *root, TrieNode *trieRoot) {
    clock_t start, end;
    double time_spent;

    printf("\n===== PERFORMANS TESTLERI =====\n");

    /* Trie Search Testi */
    start = clock();
    for (int i = 0; i < 1000; i++) {
        search_trie(trieRoot, bookList[0].title);
    }
    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Trie Search (1000 tekrar): %f sn\n", time_spent);

    /* AVL Insert Testi */
    start = clock();
    for (int i = 0; i < 1000; i++) {
        root = insert_avl(root, bookList[0]);
    }
    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("AVL Insert (1000 tekrar): %f sn\n", time_spent);

    printf("================================\n\n");
}
