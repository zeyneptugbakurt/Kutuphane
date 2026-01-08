#include <stdio.h>
#include <stdlib.h>
#include "../include/book.h"
#include "../include/io.h"     
#include "../include/gui.h" 
#include "../include/graph.h"

// Bu fonksiyonların tanımları src/performance.c ve src/unit_tests.c içinde olmalı
void run_performance_tests(Book* library, int count);
void run_all_unit_tests(); 

int main() {
    int bookCount = 0;
    
    // GÜVENLİK ÖNLEMİ: Kitap sayısı 200 olsa bile, 
    // belleği biraz fazla (500) ayırıyoruz ki taşma olmasın.
    Book* bookList = (Book*)malloc(sizeof(Book) * 500); 

    if (bookList == NULL) {
        printf("Bellek ayirma hatasi!\n");
        return 1;
    }

    // JSON'dan verileri yükle
    load_books_from_json("data/books.json", bookList, &bookCount);

    if (bookCount > 0) {
        printf("Veriler yuklendi (%d Kitap). Sistem baslatiliyor...\n", bookCount);
    
        // 1. Testleri Çalıştır
        // NOT: Bu fonksiyonların çalışması için src/unit_tests.c ve src/performance.c 
        // dosyalarının Makefile'a ekli olması ve içlerindeki fonksiyon isimlerinin tutması gerekir.
        run_all_unit_tests();
        run_performance_tests(bookList, bookCount);
        
        // 2. Graf Yapısını Kur (Ağ Analizi için)
        init_graph(bookList, bookCount);

        // 3. Arayüzü Başlat
        start_gui(bookList, bookCount); 
    } else {
        printf("Hata: Hic kitap yuklenemedi!\n");
    }

    free(bookList);
    return 0;
}