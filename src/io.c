#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/io.h"

void load_books_from_json(const char* filename, Book* bookList, int* bookCount) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("[HATA] %s dosyasi acilamadi!\n", filename);
        return;
    }

    char line[256];
    int count = 0;

    // Basit bir JSON temizleyici/okuyucu (Gerçek kütüphane yerine manuel okuma)
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "\"id\":")) {
            sscanf(strstr(line, ":"), ": %d,", &bookList[count].id);
        } else if (strstr(line, "\"title\":")) {
            char* start = strchr(line, '\"') + 9; // "title": " kısmını atla
            char* end = strrchr(line, '\"');
            if (start && end) {
                *end = '\0';
                strncpy(bookList[count].title, start, MAX_STR);
            }
        } else if (strstr(line, "\"author\":")) {
            char* start = strchr(line, '\"') + 10;
            char* end = strrchr(line, '\"');
            if (start && end) {
                *end = '\0';
                strncpy(bookList[count].author, start, MAX_STR);
            }
        } else if (strstr(line, "\"score\":")) {
            sscanf(strstr(line, ":"), ": %lf", &bookList[count].score);
            count++; // Bir kitap tamamlandı
        }
    }

    *bookCount = count;
    fclose(file);
    printf("[IO] %d adet kitap JSON'dan basariyla okundu.\n", count);
}