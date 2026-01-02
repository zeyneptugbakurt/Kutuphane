#ifndef SORT_H
#define SORT_H

#include "book.h"

// Sıralama Fonksiyonları
void quickSort(Book* arr, int low, int high);
void heapSort(Book* arr, int n);
void mergeSort(Book* arr, int l, int r);

// Yardımcı Fonksiyonlar
void swap(Book* a, Book* b);

#endif