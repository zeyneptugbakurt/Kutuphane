#ifndef GRAPH_H
#define GRAPH_H

#include "book.h"

// ID PLANLAMASI:
// 0: MERKEZ (TÜM KİTAPLAR)
// 1-500: KİTAPLAR
// 501-600: ALT TÜRLER (Macera, Tarih, Aşk...)
// 601-610: ANA TÜRLER (Roman, Şiir, Akademik)
#define MAX_NODES 700 

// Grafı Kurar (Kitap -> Alt Tür -> Ana Tür -> Merkez)
void init_graph(Book* library, int count);

// İki düğüm arasındaki ilişki zincirini bulur (BFS)
int get_shortest_path_bfs(int startID, int targetID, Book* library, int count, int* pathBuffer);

#endif