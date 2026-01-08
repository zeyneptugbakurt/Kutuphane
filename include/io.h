#ifndef IO_H
#define IO_H

#include "book.h"

// JSON dosyasını okuyup library dizisine dolduran fonksiyon
void load_books_from_json(const char *filename, Book *library, int *bookCount);

#endif