#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/book.h"
#include "../include/io.h"     
#include "../include/avl.h"    
#include "../include/gui.h"
#include "../include/stack.h"
#include "../include/trie.h"
#include "../include/sort.h" // Sıralama başlık dosyası eklendi

int main() {
    int bookCount = 0;
    Book* bookList = (Book*)malloc(sizeof(Book) * 100); 
    StackNode* searchHistory = NULL; 
    AVLNode* root = NULL;
    TrieNode* trieRoot = createTrieNode(); 

    if (bookList == NULL) {
        printf("Bellek ayirma hatasi!\n");
        return 1;
    }

    load_books_from_json("data/books.json", bookList, &bookCount);

    for(int i = 0; i < bookCount; i++) {
        root = insert_avl(root, bookList[i]);
        insert_trie(trieRoot, bookList[i].title, bookList[i].id);
    }
    
    printf("\n[SISTEM] Veritabani hazir. %d kitap yuklendi.\n", bookCount);

    int choice;
    while (1) {
        printf("\n=== KUTUPHANE YONETIM SISTEMI ===\n");
        printf("1. Kitap Ara (ID ile - AVL)\n");
        printf("2. Kitaplari Sirala (Puan/Alfabetik)\n");
        printf("3. Arama Gecmisi (Stack)\n");
        printf("4. Kitap Ara (Isim/Anahtar Kelime - Trie)\n"); 
        printf("0. Cikis\n");
        printf("Seciminiz: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
            case 1: { // ID ARAMA (AVL)
                int arananID;
                printf("\nID girin: ");
                scanf("%d", &arananID);
                AVLNode* sonuc = search_avl(root, arananID);
                if (sonuc) {
                    printf("\n-----------------------------------\n");
                    printf("   KITAP BULUNDU!\n");
                    printf("ID:     %d | Ad: %s\n", sonuc->data.id, sonuc->data.title);
                    printf("-----------------------------------\n");
                    push(&searchHistory, sonuc->data.title);
                } else printf("\n[HATA] ID bulunamadi.\n");
                break;
            }
            case 2: { // SIRALAMA MENÜSÜ
                int sortChoice;
                printf("\n--- SIRALAMA YONTEMI SECIN ---\n");
                printf("1. Artan Puan (QuickSort)\n");
                printf("2. Azalan Puan (HeapSort)\n");
                printf("3. Alfabetik (MergeSort)\n");
                printf("Seciminiz: ");
                scanf("%d", &sortChoice);

                if (sortChoice == 1) {
                    quickSort(bookList, 0, bookCount - 1);
                    printf("\n[OK] QuickSort ile puana gore (Artan) siralandi.\n");
                } else if (sortChoice == 2) {
                    heapSort(bookList, bookCount);
                    printf("\n[OK] HeapSort ile puana gore (Azalan) siralandi.\n");
                } else if (sortChoice == 3) {
                    mergeSort(bookList, 0, bookCount - 1);
                    printf("\n[OK] MergeSort ile alfabetik siralandi.\n");
                }

                // Sıralanmış Listeyi Yazdır
                printf("\n%-5s | %-25s | %-5s\n", "ID", "Kitap Adi", "Puan");
                printf("--------------------------------------------\n");
                for (int i = 0; i < bookCount; i++) {
                    printf("%-5d | %-25s | %.1f\n", bookList[i].id, bookList[i].title, bookList[i].score);
                }
                break;
            }
            case 3:
                displayStack(searchHistory);
                break;
            case 4: { // AKILLI ARAMA (TRIE)
                char arananKelime[MAX_STR];
                printf("\nKitap Ara.. ");
                scanf(" %s", arananKelime);
                search_prefix_trie(trieRoot, arananKelime, bookList, bookCount);
                break;
            }
            default:
                printf("\nGecersiz secim.\n");
        }
    }

    free(bookList);
    freeStack(searchHistory);
    return 0;
}
