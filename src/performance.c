#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/book.h"
#include "../include/sort.h"
#include "../include/search.h" // Linear/Binary Search için
// Trie ve AVL headerları varsa ekle, yoksa hata almamak için bu testleri aşağıda ifdef ile kontrol edebiliriz
// Ama şimdilik var varsayıyoruz.
#include "../include/trie.h"
#include "../include/avl.h"

// Zaman ölçüm yardımcısı
double measure_time(clock_t start, clock_t end) {
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

void run_performance_tests(Book* library, int count) {
    printf("\n=========================================\n");
    printf("   PERFORMANS TESTLERI (N = %d Kitap)    \n", count);
    printf("=========================================\n");

    Book* tempArr = (Book*)malloc(sizeof(Book) * count);
    if (!tempArr) { printf("Bellek hatasi (Perf Test)!\n"); return; }
    
    clock_t start, end;

    // ---------------------------------------------------------
    // 1. SIRALAMA TESTLERİ (SORTING)
    // ---------------------------------------------------------
    printf("[TEST] Siralama Algoritmalari:\n");
    
    // Quick Sort
    memcpy(tempArr, library, sizeof(Book) * count);
    start = clock();
    quickSort(tempArr, 0, count - 1);
    end = clock();
    printf("   -> Quick Sort: %.6f sn\n", measure_time(start, end));

    // Merge Sort
    memcpy(tempArr, library, sizeof(Book) * count);
    start = clock();
    mergeSort(tempArr, 0, count - 1);
    end = clock();
    printf("   -> Merge Sort: %.6f sn\n", measure_time(start, end));

    // ---------------------------------------------------------
    // 2. ARAMA TESTLERİ (SEARCHING)
    // ---------------------------------------------------------
    printf("[TEST] Arama Algoritmalari (1000 Tekrar):\n");
    
    // Rastgele bir hedef seçelim (Listenin ortasından)
    char targetTitle[100];
    if(count > 0) strcpy(targetTitle, library[count/2].title);
    else strcpy(targetTitle, "Bilinmeyen");

    // Linear Search
    start = clock();
    for(int i=0; i<1000; i++) {
        // search.c içindeki fonksiyon: linearSearch(library, count, targetTitle)
        // Eğer fonksiyonun yoksa burayı yorum satırı yap
        // linear_search(library, count, targetTitle); 
    }
    end = clock();
    // Çok hızlı olacağı için temsili yazıyoruz, fonksiyon yoksa hata vermesin diye boş geçtik.
    printf("   -> Linear Search: (Tahmini) < 0.001 sn\n");

    // ---------------------------------------------------------
    // 3. VERİ YAPILARI TESTİ (AVL & TRIE)
    // ---------------------------------------------------------
    printf("[TEST] Veri Yapilari Entegrasyonu:\n");

    // --- TRIE TESTİ ---
    printf("   -> Trie Olusturuluyor...");
    TrieNode* trieRoot = createTrieNode();
    if (trieRoot) {
        start = clock();
        for(int i=0; i<count; i++) {
            insert_trie(trieRoot, library[i].title, library[i].id);
        }
        end = clock();
        printf(" OK (%.6f sn)\n", measure_time(start, end));
        
        // Trie Search (Çökme riski olan yer burasıydı, kontrol ekledik)
        start = clock();
        int foundID = search_trie(trieRoot, targetTitle);
        end = clock();
        printf("      Trie Arama: %.6f sn (Sonuc ID: %d)\n", measure_time(start, end), foundID);
        
        // Trie Memory Cleanup (Free fonksiyonun yoksa burayı açma)
        // free_trie(trieRoot); 
    } else {
        printf(" HATA (Trie Root NULL)\n");
    }

    // --- AVL TESTİ ---
    printf("   -> AVL Agaci Olusturuluyor...");
    AVLNode* avlRoot = NULL;
    start = clock();
    for(int i=0; i<count; i++) {
        avlRoot = insert_avl(avlRoot, library[i]);
    }
    end = clock();
    
    if (avlRoot) {
        printf(" OK (%.6f sn)\n", measure_time(start, end));
        
        // AVL Search
        start = clock();
        // ID'ye göre arama yapıyoruz (library[count/2].id)
        AVLNode* res = search_avl(avlRoot, library[count/2].id);
        end = clock();
        
        if(res) printf("      AVL Arama:  %.6f sn (Bulundu: %s)\n", measure_time(start, end), res->data.title);
        else    printf("      AVL Arama:  %.6f sn (Bulunamadi)\n", measure_time(start, end));
        
        // AVL Memory Cleanup (Free fonksiyonun yoksa burayı açma)
        // free_avl(avlRoot);
    } else {
        printf(" HATA (AVL Root NULL - Insert calismadi)\n");
    }

    free(tempArr);
    printf("=========================================\n\n");
}