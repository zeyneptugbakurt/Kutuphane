#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "book.h"
#include "avl.h"
#include "trie.h"

void run_performance_tests(Book *bookList, int bookCount, AVLNode *root, TrieNode *trieRoot);

#endif