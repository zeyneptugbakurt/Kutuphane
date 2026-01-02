#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../include/book.h"
#include "../include/avl.h"
#include "../include/trie.h"
#include "../include/sort.h"
#include "../include/graph.h"

// 1. AVL TESTİ (ID Bazlı Arama ve Dengeleme)
void test_avl() {
    printf("Test 1: AVL Agaci Mantigi...");
    AVLNode* root = NULL;
    // book.h siralamasi: id, title, author, genre, year, score
    Book b1 = {50, "Sefiller", "Victor Hugo", "Klasik", 1862, 4.8};
    Book b2 = {30, "Nutuk", "Ataturk", "Tarih", 1927, 5.0};
    Book b3 = {70, "Suclular", "Yazar", "Polisiye", 2020, 3.5};

    root = insert_avl(root, b1);
    root = insert_avl(root, b2);
    root = insert_avl(root, b3);

    AVLNode* res = search_avl(root, 30);
    assert(res != NULL && res->data.id == 30);
    assert(search_avl(root, 999) == NULL); // Olmayan ID
    printf(" [BASARILI]\n");
}

// 2. TRIE TESTİ (Küçük Harf ve Boşluksuz Güvenli Mod)
void test_trie() {
    printf("Test 2: Trie Arama Motoru...");
    TrieNode* tRoot = createTrieNode();
    
    // Alfabe sinirina (26) takilmamak icin kucuk harf ve tek kelime kullaniyoruz
    insert_trie(tRoot, "sefiller", 101);
    insert_trie(tRoot, "nutuk", 102);
    insert_trie(tRoot, "calikusu", 103);

    assert(search_trie(tRoot, "sefiller") == 101);
    assert(search_trie(tRoot, "nutuk") == 102);
    assert(search_trie(tRoot, "calikusu") == 103);
    assert(search_trie(tRoot, "olmayan") == -1);
    printf(" [BASARILI]\n");
}

// 3. SIRALAMA TESTİ (QuickSort Skor Analizi)
void test_sort() {
    printf("Test 3: QuickSort Siralama...");
    // book.h siralamasi: id, title, author, genre, year, score
    Book b[3] = {
        {1, "KitapA", "YazarA", "Tur", 2000, 5.0},
        {2, "KitapB", "YazarB", "Tur", 2005, 2.0},
        {3, "KitapC", "YazarC", "Tur", 2010, 4.0}
    };
    
    quickSort(b, 0, 2); 
    
    // Skor bazli artan siralama kontrolü (2.0 < 4.0 < 5.0)
    assert(b[0].score <= b[1].score);
    assert(b[1].score <= b[2].score);
    printf(" [BASARILI]\n");
}

// 4. DIJKSTRA / GRAF TESTİ
void test_graph() {
    printf("Test 4: Dijkstra Graf Yapisi...");
    Graph* g = createGraph();
    assert(g != NULL);
    // Giris (0) ile Tarih (2) arasi mesafe 2 birim mi?
    assert(g->weightMatrix[0][2] == 2);
    printf(" [BASARILI]\n");
}

int main() {
    printf("\n=========================================\n");
    printf("   KUTUPHANE SISTEMI - FULL UNIT TESTS   \n");
    printf("=========================================\n");
    
    test_avl();
    test_trie();
    test_sort();
    test_graph();

    printf("\n[SONUC] Tum kritik moduller dogru calisiyor!\n");
    printf("=========================================\n\n");
    return 0;
}