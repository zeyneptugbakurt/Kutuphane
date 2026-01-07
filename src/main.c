#include <stdio.h>
#include <stdlib.h>
#include "../include/book.h"
#include "../include/io.h"     
#include "../include/gui.h" 

int main() {
    int bookCount = 0;
    Book* bookList = (Book*)malloc(sizeof(Book) * 100); 

    if (bookList == NULL) {
        printf("Bellek ayirma hatasi!\n");
        return 1;
    }

    // JSON'dan verileri yükle
    load_books_from_json("data/books.json", bookList, &bookCount);

    if (bookCount > 0) {
        printf("Veriler yuklendi. GUI Penceresi tetikleniyor...\n");
        // Terminal menüsünü sildiğin için program artık burada durmayacak
        // Doğrudan Raylib penceresini açacak
        start_gui(bookList, bookCount); 
    }

    free(bookList);
    return 0;
}