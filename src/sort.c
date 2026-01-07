#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sort.h"

static void swap(Book* a, Book* b) {
    Book temp = *a;
    *a = *b;
    *b = temp;
}

// --- QUICK SORT (Artan Puan) ---
int partition(Book* arr, int low, int high) {
    double pivot = arr[high].score;
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].score < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(Book* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// --- HEAP SORT (Azalan Puan) ---
void heapify(Book* arr, int n, int i) {
    int smallest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l].score < arr[smallest].score) smallest = l;
    if (r < n && arr[r].score < arr[smallest].score) smallest = r;
    if (smallest != i) {
        swap(&arr[i], &arr[smallest]);
        heapify(arr, n, smallest);
    }
}

void heapSort(Book* arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

// --- MERGE SORT (Alfabetik) ---
void merge(Book* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    Book *L = malloc(n1 * sizeof(Book));
    Book *R = malloc(n2 * sizeof(Book));
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (strcmp(L[i].title, R[j].title) <= 0) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L); free(R);
}

void mergeSort(Book* arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}