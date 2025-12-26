#include <stdio.h>
#include <stdlib.h>
#include "../include/book.h"
#include "../include/io.h"     
#include "../include/avl.h"    
#include "../include/gui.h"    

int main() {
    // 1. ADIM: Başlatma (Senin Görevin)
    // Kitaplar için bellekte yer açıyoruz (Örn: 100 kitaplık kapasite)
    int bookCount = 0;
    Book* bookList = (Book*)malloc(sizeof(Book) * 100); 

    if (bookList == NULL) {
        printf("Bellek ayirma hatasi!\n");
        return 1;
    }

    // JSON dosyasından verileri oku ve bookList dizisine doldur
    load_books_from_json("data/books.json", bookList, &bookCount);

    // --- TEST KODU (İstediğin test kısmı burası) ---
    if (bookCount > 0) {
        printf("\n--- TEST: Veriler Basariyla Yuklendi ---\n");
        printf("Ilk Kitap: %s | Yazar: %s | Puan: %.1f\n", 
                bookList[0].title, bookList[0].author, bookList[0].score);
        printf("Toplam Kitap Sayisi: %d\n", bookCount);
        printf("---------------------------------------\n\n");
    } else {
        printf("Uyari: Hic kitap yuklenemedi. JSON dosyasini kontrol et!\n");
    }
    // --- TEST KODU SONU ---

    // 2. ADIM: Veri Yapılarını Kurma (Arkadaşının Görevi)
    AVLNode* root = NULL;
    for(int i = 0; i < bookCount; i++) {
        // Arkadaşın insert_avl fonksiyonunu yazdığında burası aktif olacak
        // root = insert_avl(root, bookList[i]);
    }

    int choice;
    while (1) {
        // 3. ADIM: Arayüzü Göster (Senin Görevin)
        // show_menu(); // gui.c içinde tanımlayacağın fonksiyon
        printf("1. Arama\n2. Siralama\n3. Gecmis\n0. Cikis\nSecim: ");
        if (scanf("%d", &choice) != 1) break;

        if (choice == 0) break;

        switch (choice) {
            case 1:
                printf("Arama yakinda eklenecek (Trie/AVL)...\n");
                break;
            case 2:
                printf("Siralama yakinda eklenecek (Quick/Merge)...\n");
                break;
            case 3:
                printf("Gecmis yakinda eklenecek (Stack)...\n");
                break;
            default:
                printf("Gecersiz secim!\n");
        }
    }

    // 4. ADIM: Bellek Temizliği (Ortak)
    free(bookList);
    // free_tree(root); // Arkadaşın ağaç temizleme fonksiyonunu yazdığında açılacak
    return 0;
}