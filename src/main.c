#include <stdio.h>
#include <stdlib.h>
#include "../include/book.h"   // ../ bir üst klasöre çıkar, sonra include'a girer
#include "../include/io.h"     
#include "../include/avl.h"    
#include "../include/gui.h"    

int main() {
    // 1. ADIM: Başlatma (Senin Görevin)
    // JSON dosyasından verileri oku ve bir diziye (array) aktar
    int bookCount = 0;
    Book* bookList = load_books_from_json("data/books.json", &bookCount);

    // 2. ADIM: Veri Yapılarını Kurma (Arkadaşının Görevin)
    // Okunan kitapları hızlı arama için AVL ve Trie ağaçlarına yerleştir
    AVLNode* root = NULL;
    for(int i = 0; i < bookCount; i++) {
        root = insert_avl(root, bookList[i]);
        // insert_trie(trieRoot, bookList[i].title);
    }

    int choice;
    while (1) {
        // 3. ADIM: Arayüzü Göster (Senin Görevin)
        show_menu();
        scanf("%d", &choice);

        if (choice == 0) break; // Çıkış

        switch (choice) {
            case 1: // Arama (Arkadaşının Algoritması Çalışır)
                // search_book_ui(); -> Bu fonksiyon içinden trie_search veya avl_search çağrılır
                break;
            case 2: // Sıralama (Arkadaşının Algoritması Çalışır)
                // sort_books_ui(bookList, bookCount); -> QuickSort veya MergeSort çağrılır
                break;
            case 3: // Geçmişi Gör (Senin Stack yapın)
                // show_history();
                break;
        }
    }

    // 4. ADIM: Bellek Temizliği (Ortak)
    free(bookList);
    // free_tree(root);
    return 0;
}