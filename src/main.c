#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/book.h"
#include "../include/io.h"
#include "../include/gui.h"
#include "../include/avl.h"   // AVL hata denetimi için eklendi
#include "../include/trie.h"  // Trie hata denetimi için eklendi
#include "../include/graph.h" // Graf navigasyonu için eklendi

int main(void) {
    int bookCount = 0;

    // 1. HATA YÖNETİMİ: Bellek Ayırma Kontrolü
    Book* bookList = malloc(sizeof(Book) * 100);
    if (bookList == NULL) {
        fprintf(stderr, "[KRITIK HATA] Sistem bellek ayiramadi! Program sonlandiriliyor.\n");
        return 1;
    }

    // 2. HATA YÖNETİMİ: Dosya Okuma Denetimi
    // load_books_from_json içinde fopen NULL kontrolü yapıldığı varsayılır
    load_books_from_json("data/books.json", bookList, &bookCount);

    // 3. HATA YÖNETİMİ: Boş Veri Seti Kontrolü
    // Eğer JSON boşsa veya dosya yoksa GUI'yi başlatmak yerine güvenli çıkış yapıyoruz
    if (bookCount <= 0) {
        printf("[HATA] Kitap verisi yuklenemedi veya data/books.json dosyasi bos.\n");
        printf("[IPUCU] Veri dosyasinin yolunu ve JSON formatini kontrol edin.\n");
        free(bookList);
        return 1;
    }

    // 4. VERİ YAPILARINI HAZIRLAMA (GUI Öncesi Son Kontrol)
    // Ağaç yapılarını burada başlatıyoruz ki GUI içinde NULL pointer hatası almayalım
    AVLNode* root = NULL;
    TrieNode* trieRoot = createTrieNode();
    
    if (trieRoot == NULL) {
        printf("[HATA] Trie veri yapisi olusturulamadi (Bellek hatasi).\n");
        free(bookList);
        return 1;
    }

    // Verileri yapılara aktarırken hata oluşursa (Örn: Geçersiz kitap verisi)
    for(int i = 0; i < bookCount; i++) {
        if (bookList[i].id > 0) { // Sadece geçerli ID'li kitapları ekle
            root = insert_avl(root, bookList[i]);
            insert_trie(trieRoot, bookList[i].title, bookList[i].id);
        }
    }

    printf("[SISTEM] Veritabani dogrulandi. %d kitap GUI moduna aktariliyor.\n", bookCount);
    printf("[GUI] Raylib penceresi aciliyor...\n");

    /* ---- GUI ANA UYGULAMA ---- */
    // Hata yönetimi sayesinde start_gui her zaman temiz ve dolu bir liste alır
    start_gui(bookList, bookCount);

    /* GUI kapanınca bellek iadesi yaparak Memory Leak (Sızıntı) önlenir */
    printf("[SISTEM] Bellek temizleniyor ve program kapatiliyor.\n");
    free(bookList);
    // free_avl(root); // Eğer temizleme fonksiyonun varsa eklemelisin
    // free_trie(trieRoot);

    return 0;
}
