#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../include/book.h"
#include "../include/sort.h"
#include "../include/graph.h"

// AVL ve Trie dosyaların henüz hazır olmayabilir diye include'ları kapattım.
// Dosyaları oluşturunca açabilirsin:
// #include "../include/avl.h"
// #include "../include/trie.h"

// 1. AVL TESTİ (Hazır olunca açarsın)
void test_avl() {
    printf("Test 1: AVL Agaci Mantigi...");
    /*
    AVLNode* root = NULL;
    Book b1 = {50, "Sefiller", "Victor Hugo", "Klasik", "Genel", 1862, 4.8};
    Book b2 = {30, "Nutuk", "Ataturk", "Tarih", "Genel", 1927, 5.0};
    
    root = insert_avl(root, b1);
    root = insert_avl(root, b2);

    AVLNode* res = search_avl(root, 30);
    assert(res != NULL && res->data.id == 30);
    */
    printf(" [ATLANDI - Dosya Yok]\n");
}

// 2. TRIE TESTİ (Hazır olunca açarsın)
void test_trie() {
    printf("Test 2: Trie Arama Motoru...");
    /*
    TrieNode* tRoot = createTrieNode();
    insert_trie(tRoot, "sefiller", 101);
    assert(search_trie(tRoot, "sefiller") == 101);
    */
    printf(" [ATLANDI - Dosya Yok]\n");
}

// 3. SIRALAMA TESTİ (QuickSort)
void test_sort() {
    printf("Test 3: QuickSort Siralama...");
    // Subgenre alanlari eklendi
    Book b[3] = {
        {1, "KitapA", "YazarA", "Tur", "AltTur", 2000, 5.0},
        {2, "KitapB", "YazarB", "Tur", "AltTur", 2005, 2.0},
        {3, "KitapC", "YazarC", "Tur", "AltTur", 2010, 4.0}
    };
    
    // sort.c içindeki fonksiyon
    quickSort(b, 0, 2); 
    
    // Skor bazli artan siralama (2.0 < 4.0 < 5.0)
    if(b[0].score <= b[1].score && b[1].score <= b[2].score) {
        printf(" [BASARILI]\n");
    } else {
        printf(" [BASARISIZ!]\n");
    }
}

// 4. GRAF TESTİ (BFS / Hiyerarşi)
void test_graph() {
    printf("Test 4: Graf (Kitap->Tur->Merkez)...");
    
    // Test verisi oluştur
    Book testLib[2] = {
        {10, "Nutuk", "Ataturk", "Tarih", "Cumhuriyet", 1927, 5.0},
        {20, "Sefiller", "Hugo", "Roman", "Klasik", 1862, 4.8}
    };

    // Grafı başlat (Kitap -> Alt Tür -> Ana Tür -> Merkez)
    init_graph(testLib, 2);

    // Merkezden (0) kitaba (10) yol var mı?
    int path[100];
    int steps = get_shortest_path_bfs(0, 10, testLib, 2, path);
    
    // Yolun var olması lazım
    if(steps > 0) printf(" [BASARILI]\n");
    else printf(" [BASARISIZ! Yol Bulunamadi]\n");
}

// --- DÜZELTİLEN KISIM BURASI ---
// Artık 'main' değil, 'run_all_unit_tests' oldu.
// Main.c dosyası bu ismi çağırıyor.
void run_all_unit_tests() {
    printf("\n=========================================\n");
    printf("   KUTUPHANE SISTEMI - UNIT TESTS        \n");
    printf("=========================================\n");
    
    // test_avl(); 
    // test_trie(); 
    test_sort();
    test_graph();

    printf("\n[SONUC] Tum testler tamamlandi.\n");
    printf("=========================================\n\n");
}