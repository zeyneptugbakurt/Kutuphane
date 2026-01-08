#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/graph.h"

static int adjMatrix[MAX_NODES][MAX_NODES];

// Tür İsimleri ve ID'lerini tutmak için
static char* subGenreNames[100]; int subGenreIds[100]; int subGenreCount = 0;
static char* mainGenreNames[10]; int mainGenreIds[10]; int mainGenreCount = 0;

// Yardımcı: İsimden ID bul (Yoksa oluştur)
int get_id_for_string(char* names[], int ids[], int* count, const char* target, int startID) {
    for(int i=0; i<*count; i++) {
        if(strcmp(names[i], target) == 0) return ids[i];
    }
    int newID = startID + (*count);
    if (newID >= MAX_NODES) return 0;
    names[*count] = strdup(target);
    ids[*count] = newID;
    (*count)++;
    return newID;
}

void init_graph(Book* library, int count) {
    // Temizlik
    for(int i=0; i<MAX_NODES; i++) for(int j=0; j<MAX_NODES; j++) adjMatrix[i][j] = 0;
    subGenreCount = 0;
    mainGenreCount = 0;

    for (int i = 0; i < count; i++) {
        int bookID = library[i].id;
        
        // 1. ID'leri Al
        int subID = get_id_for_string(subGenreNames, subGenreIds, &subGenreCount, library[i].subgenre, 501);
        int mainID = get_id_for_string(mainGenreNames, mainGenreIds, &mainGenreCount, library[i].genre, 601);

        // 2. Bağlantıları Kur
        // Kitap <-> Alt Tür
        if(bookID < MAX_NODES && subID > 0) {
            adjMatrix[bookID][subID] = 1;
            adjMatrix[subID][bookID] = 1;
        }

        // Alt Tür <-> Ana Tür
        if(subID > 0 && mainID > 0) {
            adjMatrix[subID][mainID] = 1;
            adjMatrix[mainID][subID] = 1;
        }

        // Ana Tür <-> Merkez (0)
        if(mainID > 0) {
            adjMatrix[mainID][0] = 1;
            adjMatrix[0][mainID] = 1;
        }
    }
    printf("[GRAF] Hiyerarsi kuruldu: Kitap > Alt Tur > Ana Tur > Merkez\n");
}

// BFS Algoritması (Adım adım yol bulur)
int get_shortest_path_bfs(int startID, int targetID, Book* library, int count, int* pathBuffer) {
    if(startID == targetID) return 0;
    
    int queue[MAX_NODES];
    int front = 0, rear = 0;
    bool visited[MAX_NODES] = { false };
    int parent[MAX_NODES]; 

    for(int i=0; i<MAX_NODES; i++) parent[i] = -1;

    visited[startID] = true;
    queue[rear++] = startID;

    bool found = false;

    while(front < rear) {
        int current = queue[front++];
        if(current == targetID) {
            found = true;
            break;
        }
        for(int i=0; i<MAX_NODES; i++) {
            if(adjMatrix[current][i] == 1 && !visited[i]) {
                visited[i] = true;
                parent[i] = current;
                queue[rear++] = i;
            }
        }
    }

    if(found) {
        int tempPath[MAX_NODES];
        int steps = 0;
        int curr = targetID;
        while(curr != -1) {
            tempPath[steps++] = curr;
            curr = parent[curr];
            if(steps >= MAX_NODES) break;
        }
        for(int i=0; i<steps; i++) pathBuffer[i] = tempPath[steps - 1 - i];
        return steps;
    }
    return 0;
}