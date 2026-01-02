#ifndef IO_H
#define IO_H

#include "book.h"

<<<<<<< HEAD
// JSON dosyasını okuyup library dizisine dolduran fonksiyon
void load_books_from_json(const char *filename, Book *library, int *bookCount);
=======
// Bu satırı ekleyerek derleyiciye fonksiyonun varlığını haber veriyoruz
void load_books_from_json(const char* filename, Book* bookList, int* bookCount);
>>>>>>> ruya_branch

#endif