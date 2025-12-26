#ifndef IO_H
#define IO_H

#include "book.h"

// Bu satırı ekleyerek derleyiciye fonksiyonun varlığını haber veriyoruz
void load_books_from_json(const char* filename, Book* bookList, int* bookCount);

#endif