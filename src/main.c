#include <stdio.h>
#include <stdlib.h>

#include "../include/book.h"
#include "../include/io.h"
#include "../include/gui.h"

int main(void) {
    int bookCount = 0;

    /* Kitap listesi */
    Book* bookList = malloc(sizeof(Book) * 100);
    if (bookList == NULL) {
        printf("Bellek ayirma hatasi!\n");
        return 1;
    }

    /* JSON'dan kitaplari yukle */
    load_books_from_json("data/books.json", bookList, &bookCount);

    if (bookCount == 0) {
        printf("Kitap verisi yuklenemedi.\n");
        free(bookList);
        return 1;
    }

    printf("[SISTEM] Veritabani hazir. %d kitap yuklendi.\n", bookCount);
    printf("[GUI] Raylib penceresi aciliyor...\n");

    /* ---- GUI ANA UYGULAMA ---- */
    start_gui(bookList, bookCount);

    /* GUI kapaninca buraya gelinir */
    free(bookList);

    return 0;
}
