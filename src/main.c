#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/book.h"
#include "../include/io.h"     
#include "../include/avl.h"    
#include "../include/gui.h"
#include "../include/stack.h"
#include "../include/trie.h"
#include "../include/sort.h"
#include "../include/queue.h"
#include "../include/graph.h" // Navigasyon için yeni eklendi

int main() {
    int bookCount = 0;
    Book* bookList = (Book*)malloc(sizeof(Book) * 100); 
    StackNode* searchHistory = NULL; 
    AVLNode* root = NULL;
    TrieNode* trieRoot = createTrieNode(); 
    Queue* loanQueue = createQueue(); 
    Graph* libraryMap = createGraph(); // Graf haritası başlatıldı

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

    if (bookCount > 0) {
        printf("Veriler yuklendi. GUI Penceresi tetikleniyor...\n");
        // Terminal menüsünü sildiğin için program artık burada durmayacak
        // Doğrudan Raylib penceresini açacak
        start_gui(bookList, bookCount); 
    }

    free(bookList);
    int choice;
    while (1) {
        printf("\n=== KUTUPHANE YONETIM SISTEMI ===\n");
        printf("1. Kitap Ara (ID ile - AVL)\n");
        printf("2. Kitaplari Sirala (Puan/Alfabetik)\n");
        printf("3. Arama Gecmisi (Stack)\n");
        printf("4. Akilli Arama (Isim/Anahtar Kelime - Trie)\n"); 
        printf("5. Odunc Al / Iade Et (Queue - FIFO)\n");
        printf("6. Kitap Bolumune Yol Tarifi (Dijkstra - Graph)\n"); // Yeni Seçenek
        printf("0. Cikis\n");
        printf("Seciminiz: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
            case 1: { // ID ARAMA
                int id; printf("\nID girin: "); scanf("%d", &id);
                AVLNode* s = search_avl(root, id);
                if (s) {
                    printf("\n[BULUNDU] %s\n", s->data.title);
                    push(&searchHistory, s->data.title);
                } else printf("\n[HATA] Bulunamadi.\n");
                break;
            }
            case 2: { // SIRALAMA
                int sc; printf("\n1.Artan 2.Azalan 3.Alfabetik: "); scanf("%d", &sc);
                if (sc == 1) quickSort(bookList, 0, bookCount - 1);
                else if (sc == 2) heapSort(bookList, bookCount);
                else if (sc == 3) mergeSort(bookList, 0, bookCount - 1);
                
                printf("\n%-5s | %-25s | %-5s\n", "ID", "Kitap Adi", "Puan");
                for (int i = 0; i < bookCount; i++)
                    printf("%-5d | %-25s | %.1f\n", bookList[i].id, bookList[i].title, bookList[i].score);
                break;
            }
            case 3: displayStack(searchHistory); break;
            case 4: { // TRIE ARAMA
                char kelime[MAX_STR]; printf("\nKitap Ara.. "); scanf("%s", kelime);
                search_prefix_trie(trieRoot, kelime, bookList, bookCount);
                break;
            }
            case 5: { // QUEUE
                int qc; printf("\n1.Odunc Al 2.Iade Et 3.Liste: "); scanf("%d", &qc);
                if (qc == 1) {
                    char u[MAX_STR], b[MAX_STR];
                    printf("Adiniz: "); scanf(" %[^\n]s", u);
                    printf("Kitap: "); scanf(" %[^\n]s", b);
                    enqueue_loan(loanQueue, u, b);
                } else if (qc == 2) dequeue_return(loanQueue);
                else displayLoans(loanQueue);
                break;
            }
            case 6: { // DIJKSTRA NAVIGASYON
                int hedef;
                printf("\n--- BOLUM SECIN ---\n");
                printf("1. Roman  2. Tarih  3. Bilim  4. Yazilim  5. Sanat\n");
                printf("Hedef bolum no: "); scanf("%d", &hedef);
                if(hedef >= 1 && hedef <= 5)
                    dijkstra(libraryMap, 0, hedef); // Giris'ten (0) hedefe
                else printf("\nGecersiz bolum.\n");
                break;
            }
            default: printf("\nGecersiz secim.\n");
        }
    }

    free(bookList);
    freeStack(searchHistory);
    return 0;
}