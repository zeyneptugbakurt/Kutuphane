#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/book.h"
#include "../include/io.h"
#include "../include/gui.h"
#include "../include/avl.h"
#include "../include/trie.h"
#include "../include/graph.h"
#include "../include/performance.h"   // ✅ PERFORMANS TESTLERI EKLENDI

int main(void) {
    int bookCount = 0;

    /* 1. HATA YÖNETİMİ: Bellek Ayırma Kontrolü */
    Book* bookList = malloc(sizeof(Book) * 100);
    if (bookList == NULL) {
        fprintf(stderr, "[KRITIK HATA] Sistem bellek ayiramadi! Program sonlandiriliyor.\n");
        return 1;
    }

    /* 2. HATA YÖNETİMİ: Dosya Okuma Denetimi */
    load_books_from_json("data/books.json", bookList, &bookCount);

    /* 3. BOŞ VERİ SETİ KONTROLÜ */
    if (bookCount <= 0) {
        printf("[HATA] Kitap verisi yuklenemedi veya data/books.json dosyasi bos.\n");
        printf("[IPUCU] Veri dosyasinin yolunu ve JSON formatini kontrol edin.\n");
        free(bookList);
        return 1;
    }

    /* 4. VERİ YAPILARINI HAZIRLAMA */
    AVLNode* root = NULL;
    TrieNode* trieRoot = createTrieNode();

    if (trieRoot == NULL) {
        printf("[HATA] Trie veri yapisi olusturulamadi (Bellek hatasi).\n");
        free(bookList);
        return 1;
    }

    /* 5. VERILERI AVL ve TRIE YAPILARINA AKTARMA */
    for (int i = 0; i < bookCount; i++) {
        if (bookList[i].id > 0) {
            root = insert_avl(root, bookList[i]);
            insert_trie(trieRoot, bookList[i].title, bookList[i].id);
        }
    }

    printf("[SISTEM] Veritabani dogrulandi. %d kitap yuklendi.\n", bookCount);

    /* ================= PERFORMANS TESTLERI ================= */
    run_performance_tests(bookList, bookCount, root, trieRoot);
    /* ======================================================= */

    printf("[GUI] Raylib penceresi aciliyor...\n");

    /* ---- GUI ANA UYGULAMA ---- */
    start_gui(bookList, bookCount);

    /* 6. BELLEK TEMIZLEME */
    printf("[SISTEM] Bellek temizleniyor ve program kapatiliyor.\n");
    free(bookList);
    // free_avl(root);
    // free_trie(trieRoot);

    return 0;
}
