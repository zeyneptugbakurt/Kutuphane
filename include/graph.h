#ifndef GRAPH_H
#define GRAPH_H

#define MAX_NODES 6
#define INF 9999

typedef struct {
    int weightMatrix[MAX_NODES][MAX_NODES];
    char nodeNames[MAX_NODES][20];
} Graph;

Graph* createGraph();
void dijkstra(Graph* g, int startNode, int targetNode);

#endif