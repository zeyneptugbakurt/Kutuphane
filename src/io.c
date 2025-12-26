#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/io.h"

// JSON okuma fonksiyonunun gövdesi
void load_books_from_json(const char* filename, Book* bookList, int* bookCount) {
    // Gerçek bir JSON parse işlemi için cJSON gibi kütüphaneler gerekir.
    // Şimdilik test için manuel 2 kitap ekleyelim ki AVL'yi test edebilesin.
    
    printf("[IO] %s dosyasi okunuyor (Simülasyon)...\n", filename);

    // 1. Kitap
    bookList[0].id = 1;
    strcpy(bookList[0].title, "Nutuk");
    strcpy(bookList[0].author, "Mustafa Kemal Ataturk");
    bookList[0].score = 10.0;

    // 2. Kitap
    bookList[1].id = 2;
    strcpy(bookList[1].title, "Simyaci");
    strcpy(bookList[1].author, "Paulo Coelho");
    bookList[1].score = 8.5;

    *bookCount = 2; // Toplam 2 kitap yüklendiğini bildiriyoruz
}