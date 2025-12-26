#include <stdio.h>
#include <stdlib.h>
#include "../include/book.h"
#include "../include/io.h"     
#include "../include/avl.h"    
#include "../include/gui.h"    

int main() {
    // 1. ADIM: Verileri Yükleme
    int bookCount = 0;
    Book* bookList = (Book*)malloc(sizeof(Book) * 100); 

    if (bookList == NULL) {
        printf("Bellek ayirma hatasi!\n");
        return 1;
    }

    load_books_from_json("data/books.json", bookList, &bookCount);

    // 2. ADIM: AVL Ağacını Oluşturma
    AVLNode* root = NULL;
    for(int i = 0; i < bookCount; i++) {
        root = insert_avl(root, bookList[i]);
    }
    
    printf("\n[SISTEM] Veritabani hazir. %d kitap yuklendi.\n", bookCount);

    int choice;
    while (1) {
        printf("\n=== KUTUPHANE YONETIM SISTEMI ===\n");
        printf("1. Kitap Ara (ID ile)\n");
        printf("2. Kitaplari Sirala (Puan/Yil)\n");
        printf("3. Arama Gecmisi (Stack)\n");
        printf("0. Cikis\n");
        printf("Seciminiz: ");
        
        if (scanf("%d", &choice) != 1) break;

        if (choice == 0) {
            printf("Programdan cikiliyor...\n");
            break;
        }

        switch (choice) {
            case 1: {
                // ARAMA MENÜSÜ AKTİF
                int arananID;
                printf("\nAramak istediginiz Kitap ID'sini girin: ");
                scanf("%d", &arananID);

                printf("[SISTEM] AVL agacinda sorgulaniyor...\n");
                AVLNode* sonuc = search_avl(root, arananID);

                if (sonuc != NULL) {
                    printf("\n-----------------------------------\n");
                    printf("   KITAP BULUNDU!\n");
                    printf("-----------------------------------\n");
                    printf("ID:     %d\n", sonuc->data.id);
                    printf("Ad:     %s\n", sonuc->data.title);
                    printf("Yazar:  %s\n", sonuc->data.author);
                    printf("Puan:   %.1f\n", sonuc->data.score);
                    printf("-----------------------------------\n");
                } else {
                    printf("\n[HATA] ID %d olan bir kitap bulunamadi.\n", arananID);
                }
                break;
            }
            case 2:
                printf("\n[BILGI] Siralama algoritmasi (QuickSort/MergeSort) henüz entegre edilmedi.\n");
                break;
            case 3:
                printf("\n[BILGI] Stack yapisi (Gecmis) henüz entegre edilmedi.\n");
                break;
            default:
                printf("\n[!] Gecersiz secim, tekrar deneyin.\n");
        }
    }

    // 4. ADIM: Bellek Temizliği
    free(bookList);
    // free_tree(root); // İleride eklenecek
    return 0;
}