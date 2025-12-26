#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/book.h"
#include "../include/io.h"     
#include "../include/avl.h"    
#include "../include/gui.h"
#include "../include/stack.h"
#include "../include/trie.h"

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
        printf("2. Kitaplari Sirala (Puan/Yil)\n");
        printf("3. Arama Gecmisi (Stack)\n");
        printf("4. Kitap Ara (Isim/On Ek ile - Trie)\n"); 
        printf("0. Cikis\n");
        printf("Seciminiz: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
            case 1: { // ID ARAMA
                int arananID;
                printf("\nID girin: ");
                scanf("%d", &arananID);
                AVLNode* sonuc = search_avl(root, arananID);
                if (sonuc) {
                    printf("\n-----------------------------------\n");
                    printf("   KITAP BULUNDU!\n");
                    printf("ID:     %d\n", sonuc->data.id);
                    printf("Ad:     %s\n", sonuc->data.title);
                    printf("-----------------------------------\n");
                    push(&searchHistory, sonuc->data.title);
                } else printf("\n[HATA] ID bulunamadi.\n");
                break;
            }
            case 2: // SIRALAMA
                printf("\n[BILGI] Siralama (Quick/Merge Sort) yakinda eklenecek.\n");
                break;
            case 3: // GECMİS
                displayStack(searchHistory);
                break;
            case 4: { // İSİM ÖNEK ARAMA (TRIE)
                char arananOnEk[MAX_STR];
                printf("\nKİtap Ara..");
                scanf(" %s", arananOnEk);

                // Bu fonksiyon girilen harflerle baslayan tüm kitapları listeleyecektir
                search_prefix_trie(trieRoot, arananOnEk, bookList, bookCount);
                
                // Aramayı genel geçmişe (Stack) eklemek istersen, 
                // Öneki eklemek yerine tam sonuçları göstermek daha iyidir.
                break;
            }
            default:
                printf("\nGecersiz secim.\n");
        }
    }

    free(bookList);
    freeStack(searchHistory);
    // free_trie(trieRoot); // Gerekirse eklenebilir
    return 0;
}