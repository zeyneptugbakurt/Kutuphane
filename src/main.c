#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
=======
#include <string.h>
>>>>>>> ruya_branch
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
<<<<<<< HEAD
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
=======
    int bookCount = 0;
    Book* bookList = (Book*)malloc(sizeof(Book) * 100); 
    StackNode* searchHistory = NULL; 
>>>>>>> ruya_branch
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
<<<<<<< HEAD
        // Arkadaşın insert_avl fonksiyonunu yazdığında burası aktif olacak
        // root = insert_avl(root, bookList[i]);
=======
        root = insert_avl(root, bookList[i]);
        insert_trie(trieRoot, bookList[i].title, bookList[i].id);
>>>>>>> ruya_branch
    }
    
    printf("\n[SISTEM] Veritabani hazir. %d kitap yuklendi.\n", bookCount);

    int choice;
    while (1) {
<<<<<<< HEAD
        // 3. ADIM: Arayüzü Göster (Senin Görevin)
        // show_menu(); // gui.c içinde tanımlayacağın fonksiyon
        printf("1. Arama\n2. Siralama\n3. Gecmis\n0. Cikis\nSecim: ");
        if (scanf("%d", &choice) != 1) break;
=======
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
>>>>>>> ruya_branch

        if (choice == 0) break;

        switch (choice) {
<<<<<<< HEAD
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
=======
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
>>>>>>> ruya_branch
        }
    }

    free(bookList);
<<<<<<< HEAD
    // free_tree(root); // Arkadaşın ağaç temizleme fonksiyonunu yazdığında açılacak
=======
    freeStack(searchHistory);
>>>>>>> ruya_branch
    return 0;
}