#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "../include/book.h"
#include "../include/gui.h"

#define COLOR_BG            (Color){ 255, 250, 250, 255 }
#define COLOR_PASTEL_PINK   (Color){ 255, 182, 193, 255 }
#define COLOR_DARK_PINK     (Color){ 255, 140, 157, 255 }
#define COLOR_TEXT          (Color){ 100, 80, 90, 255 }

// Sıralama türleri için yardımcı yapı
typedef enum { NONE, SCORE_ASC, SCORE_DESC, ALPHABETICAL } SortType;

void start_gui(Book *library, int count) {
    SetConfigFlags(FLAG_MSAA_4X_HINT); 
    InitWindow(900, 700, "Kütüphane Yönetim Paneli");
    SetTargetFPS(60);

    // Türkçe Karakter Desteği
    int codepoints[128];
    for (int i = 0; i < 95; i++) codepoints[i] = 32 + i;
    int tr_chars[] = { 0x0130, 0x0131, 0x015e, 0x015f, 0x011e, 0x011f, 0x00c7, 0x00e7, 0x00dc, 0x00fc, 0x00d6, 0x00f6 };
    for (int i = 0; i < 12; i++) codepoints[95 + i] = tr_chars[i];

    Font myFont = LoadFontEx("resources/arial.ttf", 40, codepoints, 107);
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);

    char searchText[50] = "\0";
    int letterCount = 0;
    
    // Filtreleme Durumları
    bool showFilterMenu = false;
    SortType currentSort = NONE;

    while (!WindowShouldClose()) {
        // --- Girdi Yönetimi ---
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (letterCount < 49)) {
                searchText[letterCount] = (char)key;
                searchText[letterCount+1] = '\0';
                letterCount++;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
            letterCount--;
            searchText[letterCount] = '\0';
        }

        Vector2 mouse = GetMousePosition();
        Rectangle btnFilter = { 620, 140, 150, 45 };
        Rectangle opt1 = { 620, 185, 150, 40 }; // Artan
        Rectangle opt2 = { 620, 225, 150, 40 }; // Azalan
        Rectangle opt3 = { 620, 265, 150, 40 }; // Alfabetik

        // Filtreler Butonuna Tıklama
        if (CheckCollisionPointRec(mouse, btnFilter) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            showFilterMenu = !showFilterMenu;
        }

        // Seçeneklere Tıklama ve Sıralama Mantığı
        if (showFilterMenu && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, opt1)) { // Artan Puan
                for(int i=0; i<count-1; i++) 
                    for(int j=0; j<count-i-1; j++)
                        if(library[j].score > library[j+1].score) { Book temp = library[j]; library[j] = library[j+1]; library[j+1] = temp; }
                showFilterMenu = false;
            }
            else if (CheckCollisionPointRec(mouse, opt2)) { // Azalan Puan
                for(int i=0; i<count-1; i++) 
                    for(int j=0; j<count-i-1; j++)
                        if(library[j].score < library[j+1].score) { Book temp = library[j]; library[j] = library[j+1]; library[j+1] = temp; }
                showFilterMenu = false;
            }
            else if (CheckCollisionPointRec(mouse, opt3)) { // Alfabetik
                for(int i=0; i<count-1; i++) 
                    for(int j=0; j<count-i-1; j++)
                        if(strcmp(library[j].title, library[j+1].title) > 0) { Book temp = library[j]; library[j] = library[j+1]; library[j+1] = temp; }
                showFilterMenu = false;
            }
        }

        BeginDrawing();
        ClearBackground(COLOR_BG);

        // Header
        DrawRectangle(0, 0, 900, 100, COLOR_PASTEL_PINK);
        DrawTextEx(myFont, "KÜTÜPHANE SİSTEMİ", (Vector2){ 40, 30 }, 45, 2, WHITE);
        
        // Arama Çubuğu
        DrawRectangleRounded((Rectangle){ 40, 140, 550, 45 }, 0.2, 10, WHITE);
        DrawRectangleLinesEx((Rectangle){ 40, 140, 550, 45 }, 2, COLOR_PASTEL_PINK);
        if (letterCount == 0) DrawTextEx(myFont, "Kitap / Yazar / Anahtar kelime ara...", (Vector2){ 55, 150 }, 22, 1, LIGHTGRAY);
        DrawTextEx(myFont, searchText, (Vector2){ 55, 150 }, 22, 1, COLOR_TEXT);

        // --- FİLTRELER BUTONU VE MENÜSÜ ---
        Color bColor = (showFilterMenu || CheckCollisionPointRec(mouse, btnFilter)) ? COLOR_DARK_PINK : COLOR_PASTEL_PINK;
        DrawRectangleRounded(btnFilter, 0.3, 10, bColor);
        DrawTextEx(myFont, "Filtreler", (Vector2){ 663, 152 }, 20, 1, WHITE);

        if (showFilterMenu) {
            DrawRectangleRec((Rectangle){ 620, 185, 230, 120 }, WHITE);
            DrawRectangleLinesEx((Rectangle){ 620, 185, 150, 120 }, 1, COLOR_PASTEL_PINK);

            // Seçenek 1: Artan Puan
            if (CheckCollisionPointRec(mouse, opt1)) DrawRectangleRec(opt1, Fade(COLOR_PASTEL_PINK, 0.3f));
            DrawTextEx(myFont, "Puan (Artan)", (Vector2){ 635, 195 }, 18, 1, COLOR_TEXT);

            // Seçenek 2: Azalan Puan
            if (CheckCollisionPointRec(mouse, opt2)) DrawRectangleRec(opt2, Fade(COLOR_PASTEL_PINK, 0.3f));
            DrawTextEx(myFont, "Puan (Azalan)", (Vector2){ 635, 235 }, 18, 1, COLOR_TEXT);

            // Seçenek 3: Alfabetik
            if (CheckCollisionPointRec(mouse, opt3)) DrawRectangleRec(opt3, Fade(COLOR_PASTEL_PINK, 0.3f));
            DrawTextEx(myFont, "Alfabetik (A-Z)", (Vector2){ 635, 275 }, 18, 1, COLOR_TEXT);
        }

        // Kitap Listesi
        DrawTextEx(myFont, "Kitap Listesi", (Vector2){ 40, 210 }, 28, 1, COLOR_PASTEL_PINK);

        int drawCount = 0;
        for (int i = 0; i < count; i++) {
            if (TextLength(searchText) == 0 || TextFindIndex(TextToLower(library[i].title), TextToLower(searchText)) != -1) {
                float yPos = 260 + (drawCount * 50);
                DrawRectangleRounded((Rectangle){ 40, yPos, 550, 40 }, 0.2, 10, Fade(COLOR_PASTEL_PINK, 0.15f));
                DrawTextEx(myFont, library[i].title, (Vector2){ 60, yPos + 8 }, 20, 1, COLOR_TEXT);
                
                DrawRectangleRounded((Rectangle){ 530, yPos + 5, 50, 30 }, 0.5, 10, COLOR_PASTEL_PINK);
                DrawTextEx(myFont, TextFormat("%.1f", library[i].score), (Vector2){ 538, yPos + 8 }, 18, 1, WHITE);
                drawCount++;
            }
        }

        EndDrawing();
    }

    UnloadFont(myFont);
    CloseWindow();
}