#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/graph.h"

Graph* createGraph() {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    
    // Bölüm İsimleri
    strcpy(g->nodeNames[0], "Giris");
    strcpy(g->nodeNames[1], "Roman");
    strcpy(g->nodeNames[2], "Tarih");
    strcpy(g->nodeNames[3], "Bilim");
    strcpy(g->nodeNames[4], "Yazilim");
    strcpy(g->nodeNames[5], "Sanat");

    // Mesafeler (Örn: Girişten Romana 4 metre, Girişten Tarihe 2 metre vb.)
    // 0:Giris, 1:Roman, 2:Tarih, 3:Bilim, 4:Yazilim, 5:Sanat
    int initialMatrix[MAX_NODES][MAX_NODES] = {
        {0, 4, 2, 0, 0, 0},
        {4, 0, 1, 5, 0, 0},
        {2, 1, 0, 8, 10, 0},
        {0, 5, 8, 0, 2, 6},
        {0, 0, 10, 2, 0, 3},
        {0, 0, 0, 6, 3, 0}
    };

    for(int i=0; i<MAX_NODES; i++)
        for(int j=0; j<MAX_NODES; j++)
            g->weightMatrix[i][j] = (initialMatrix[i][j] == 0 && i != j) ? INF : initialMatrix[i][j];

    return g;
}

void dijkstra(Graph* g, int start, int target) {
    int dist[MAX_NODES], visited[MAX_NODES], parent[MAX_NODES];

    for (int i = 0; i < MAX_NODES; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[start] = 0;

    for (int count = 0; count < MAX_NODES - 1; count++) {
        int min = INF, u;
        for (int v = 0; v < MAX_NODES; v++)
            if (!visited[v] && dist[v] <= min) { min = dist[v]; u = v; }

        visited[u] = 1;

        for (int v = 0; v < MAX_NODES; v++)
            if (!visited[v] && g->weightMatrix[u][v] != INF && dist[u] + g->weightMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + g->weightMatrix[u][v];
                parent[v] = u;
            }
    }

    printf("\n[NAVIGASYON] %s -> %s arasi en kisa yol:\n", g->nodeNames[start], g->nodeNames[target]);
    printf("Toplam Mesafe: %d metre\n", dist[target]);
    printf("Rota: ");
    
    // Yolu tersten yazdırmak için basit bir recursive yapı simülasyonu
    int path[MAX_NODES], count = 0;
    int curr = target;
    while(curr != -1) {
        path[count++] = curr;
        curr = parent[curr];
    }
    for(int i=count-1; i>=0; i--) {
        printf("%s%s", g->nodeNames[path[i]], (i==0 ? "" : " -> "));
    }
    printf("\n");
}