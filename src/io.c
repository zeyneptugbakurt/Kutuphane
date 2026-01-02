<<<<<<< HEAD
#include "../include/io.h"
#include "../include/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_books_from_json(const char *filename, Book *library, int *bookCount) {
    // 1. Dosyayı aç ve içeriğini metin (string) olarak oku
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Hata: %s dosyasi bulunamadi!\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(length + 1);
    fread(buffer, 1, length, file);
    fclose(file);
    buffer[length] = '\0';

    // 2. cJSON ile metni parçala
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        printf("JSON Ayristirma Hatasi!\n");
        free(buffer);
        return;
    }

    // 3. Her bir kitabı struct dizisine aktar
    int i = 0;
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, json) {
        library[i].id = cJSON_GetObjectItem(item, "id")->valueint;
        strcpy(library[i].title, cJSON_GetObjectItem(item, "title")->valuestring);
        strcpy(library[i].author, cJSON_GetObjectItem(item, "author")->valuestring);
        strcpy(library[i].genre, cJSON_GetObjectItem(item, "genre")->valuestring);
        library[i].year = cJSON_GetObjectItem(item, "year")->valueint;
        library[i].score = cJSON_GetObjectItem(item, "score")->valuedouble;
        i++;
    }

    *bookCount = i; // Toplam kaç kitap okunduğunu main'e haber ver
    cJSON_Delete(json);
    free(buffer);
    printf("%d kitap basariyla yuklendi.\n", *bookCount);
=======
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
>>>>>>> ruya_branch
}