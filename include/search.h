#ifndef SEARCH_H
#define SEARCH_H

#include "book.h"

// Arkadaşının yazacağı algoritma: 
// Yazılan metni alır ve kütüphaneden uygun kitapları 'results' içine doldurur.
int search_books_by_prefix(Book* library, int count, const char* prefix, Book* results);

#endif