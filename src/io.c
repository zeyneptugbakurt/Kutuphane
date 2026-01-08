#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/book.h"
#include "../include/io.h"
#include "../include/cJSON.h"

// Dosya okuma yardımcısı (Internal)
char* read_file(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;
    
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char* data = (char*)malloc(length + 1);
    if (!data) {
        fclose(f);
        return NULL;
    }
    
    fread(data, 1, length, f);
    fclose(f);
    data[length] = '\0';
    return data;
}

// Senin istediğin fonksiyon yapısı:
void load_books_from_json(const char *filename, Book *library, int *bookCount) {
    char* json_data = read_file(filename);
    if (!json_data) {
        printf("Hata: Dosya okunamadi (%s)!\n", filename);
        *bookCount = 0;
        return;
    }

    cJSON* json = cJSON_Parse(json_data);
    if (!json) {
        printf("Hata: JSON parse edilemedi!\n");
        free(json_data);
        *bookCount = 0;
        return;
    }

    int i = 0;
    cJSON* item = NULL;
    
    cJSON_ArrayForEach(item, json) {
        // ID
        cJSON* idItem = cJSON_GetObjectItem(item, "id");
        if (idItem) library[i].id = idItem->valueint;

        // Title
        cJSON* titleItem = cJSON_GetObjectItem(item, "title");
        if (titleItem) strcpy(library[i].title, titleItem->valuestring);

        // Author
        cJSON* authorItem = cJSON_GetObjectItem(item, "author");
        if (authorItem) strcpy(library[i].author, authorItem->valuestring);
        
        // Genre (Ana Tür)
        cJSON* genreItem = cJSON_GetObjectItem(item, "genre");
        if(genreItem) strcpy(library[i].genre, genreItem->valuestring);
        else strcpy(library[i].genre, "Genel");

        // --- YENİ: Subgenre (Alt Tür) ---
        cJSON* subGenreItem = cJSON_GetObjectItem(item, "subgenre");
        if(subGenreItem) strcpy(library[i].subgenre, subGenreItem->valuestring);
        else strcpy(library[i].subgenre, "Genel");

        // Year
        cJSON* yearItem = cJSON_GetObjectItem(item, "year");
        if (yearItem) library[i].year = yearItem->valueint;

        // Score
        cJSON* scoreItem = cJSON_GetObjectItem(item, "score");
        if (scoreItem) library[i].score = (float)scoreItem->valuedouble;

        i++;
    }

    *bookCount = i; // Main'deki sayacı güncelle
    cJSON_Delete(json);
    free(json_data);
    
    printf("%d kitap basariyla yuklendi.\n", *bookCount);
}