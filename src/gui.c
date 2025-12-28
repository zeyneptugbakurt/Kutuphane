#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raylib.h"
#include "../include/book.h"
#include "../include/gui.h"
#include "../include/stack.h" 

// Prototip Tanımlamaları
void push(StackNode** top, const char* title);
void freeStack(StackNode* top);

#define COLOR_BG            (Color){ 255, 250, 250, 255 }
#define COLOR_PASTEL_PINK   (Color){ 255, 182, 193, 255 }
#define COLOR_DARK_PINK     (Color){ 255, 140, 157, 255 }
#define COLOR_TEXT          (Color){ 100, 80, 90, 255 }

typedef enum { PAGE_HOME, PAGE_HISTORY } GuiPage;
StackNode* historyStack = NULL;

void start_gui(Book *library, int count)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT); 
    InitWindow(900, 700, "Kütüphane Yönetim Paneli");
    SetTargetFPS(60);

    // --- TÜRKÇE KARAKTER DESTEĞİ ---
    int codepoints[128];
    for (int i = 0; i < 95; i++) codepoints[i] = 32 + i;
    int tr_chars[] = { 0x0130, 0x0131, 0x015e, 0x015f, 0x011e, 0x011f,
                       0x00c7, 0x00e7, 0x00dc, 0x00fc, 0x00d6, 0x00f6 };
    for (int i = 0; i < 12; i++) codepoints[95 + i] = tr_chars[i];

    Font myFont = LoadFontEx("resources/arial.ttf", 40, codepoints, 107);
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);

    // 🔧 UTF-8 için buffer büyütüldü
    char searchText[128] = {0};
    int letterCount = 0;

    bool showFilterMenu = false;
    GuiPage currentPage = PAGE_HOME;

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();

        // --- 1. KLAVYE GİRDİSİ (UTF-8 DÜZELTİLDİ) ---
        if (currentPage == PAGE_HOME)
        {
            int key = GetCharPressed();
while (key > 0)
{
    if (letterCount < 120)
    {
        int bytes = 0;
        const char *utf8 = CodepointToUTF8(key, &bytes);

        if (utf8 && letterCount + bytes < 128)
        {
            memcpy(&searchText[letterCount], utf8, bytes);
            letterCount += bytes;
            searchText[letterCount] = '\0';
        }
    }
    key = GetCharPressed();
}


            // UTF-8 BACKSPACE
            if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0)
            {
                do {
                    letterCount--;
                } while ((searchText[letterCount] & 0xC0) == 0x80);
                searchText[letterCount] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER) && letterCount > 0)
            {
                push(&historyStack, searchText);
            }
        }

        // --- 2. TIKLAMA ALANLARI ---
        Rectangle btnFilter = { 620, 140, 120, 45 };
        Rectangle btnGoHistory = { 750, 140, 120, 45 };
        Rectangle searchBar = { 40, 140, 550, 45 };

        if (currentPage == PAGE_HOME)
        {
            if (CheckCollisionPointRec(mouse, btnFilter) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                showFilterMenu = !showFilterMenu;
            }
            else if (CheckCollisionPointRec(mouse, btnGoHistory) &&
                     IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentPage = PAGE_HISTORY;
                showFilterMenu = false;
            }
            else if (showFilterMenu && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mouse, (Rectangle){ 620, 185, 160, 40 }))
                {
                    for(int i=0;i<count-1;i++)
                        for(int j=0;j<count-i-1;j++)
                            if(library[j].score > library[j+1].score)
                            { Book t=library[j]; library[j]=library[j+1]; library[j+1]=t; }
                    showFilterMenu = false;
                }
                else if (CheckCollisionPointRec(mouse, (Rectangle){ 620, 225, 160, 40 }))
                {
                    for(int i=0;i<count-1;i++)
                        for(int j=0;j<count-i-1;j++)
                            if(library[j].score < library[j+1].score)
                            { Book t=library[j]; library[j]=library[j+1]; library[j+1]=t; }
                    showFilterMenu = false;
                }
                else if (CheckCollisionPointRec(mouse, (Rectangle){ 620, 265, 160, 40 }))
                {
                    for(int i=0;i<count-1;i++)
                        for(int j=0;j<count-i-1;j++)
                            if(strcmp(library[j].title, library[j+1].title) > 0)
                            { Book t=library[j]; library[j]=library[j+1]; library[j+1]=t; }
                    showFilterMenu = false;
                }
            }
        }

        BeginDrawing();
        ClearBackground(COLOR_BG);

        if (currentPage == PAGE_HOME)
        {
            DrawRectangle(0, 0, 900, 100, COLOR_PASTEL_PINK);
            DrawTextEx(myFont, "KÜTÜPHANE SİSTEMİ", (Vector2){ 40, 30 }, 45, 2, WHITE);

            DrawRectangleRounded(searchBar, 0.2, 10, WHITE);
            DrawRectangleLinesEx(searchBar, 2, COLOR_PASTEL_PINK);
            if (letterCount == 0)
                DrawTextEx(myFont, "Kitap / Yazar / Anahtar kelime ara...", (Vector2){ 55, 150 }, 22, 1, LIGHTGRAY);
            DrawTextEx(myFont, searchText, (Vector2){ 55, 150 }, 22, 1, COLOR_TEXT);

            DrawRectangleRounded(btnFilter, 0.3, 10,
                (showFilterMenu || CheckCollisionPointRec(mouse, btnFilter))
                ? COLOR_DARK_PINK : COLOR_PASTEL_PINK);
            DrawTextEx(myFont, "Filtreler", (Vector2){ 635, 152 }, 18, 1, WHITE);

            DrawRectangleRounded(btnGoHistory, 0.3, 10,
                CheckCollisionPointRec(mouse, btnGoHistory)
                ? COLOR_DARK_PINK : COLOR_PASTEL_PINK);
            DrawTextEx(myFont, "Geçmiş", (Vector2){ 775, 152 }, 18, 1, WHITE);

            DrawTextEx(myFont, "Kitap Listesi", (Vector2){ 40, 210 }, 28, 1, COLOR_PASTEL_PINK);

            int drawCount = 0;
            for (int i = 0; i < count; i++)
            {
                if (TextLength(searchText) == 0 ||
                    TextFindIndex(TextToLower(library[i].title),
                                  TextToLower(searchText)) != -1)
                {
                    float y = 260 + drawCount * 50;
                    DrawRectangleRounded((Rectangle){40,y,550,40},0.2,10,
                                         Fade(COLOR_PASTEL_PINK,0.15f));
                    DrawTextEx(myFont, library[i].title,
                               (Vector2){60,y+8},18,1,COLOR_TEXT);

                    DrawRectangleRounded((Rectangle){530,y+5,50,30},0.5,10,COLOR_PASTEL_PINK);
                    DrawTextEx(myFont, TextFormat("%.1f",library[i].score),
                               (Vector2){538,y+8},16,1,WHITE);
                    drawCount++;
                }
            }

            if (showFilterMenu)
            {
                DrawRectangleRec((Rectangle){620,185,120,90},WHITE);
                DrawRectangleLinesEx((Rectangle){620,185,120,90},1,COLOR_PASTEL_PINK);
                DrawTextEx(myFont,"Puan (Artan)",(Vector2){630,192},15,1,COLOR_TEXT);
                DrawTextEx(myFont,"Puan (Azalan)",(Vector2){630,222},15,1,COLOR_TEXT);
                DrawTextEx(myFont,"Alfabetik",(Vector2){630,252},15,1,COLOR_TEXT);
            }
        }
        else
        {
            DrawRectangle(0,0,900,100,COLOR_PASTEL_PINK);
            DrawTextEx(myFont,"ARAMA GEÇMİŞİ",(Vector2){320,35},35,2,WHITE);

            Rectangle btnBack={25,35,100,35};
            if(CheckCollisionPointRec(mouse,btnBack))
            {
                DrawRectangleRounded(btnBack,0.4,10,WHITE);
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    currentPage=PAGE_HOME;
            }
            else DrawRectangleRoundedLines(btnBack,0.4,10,WHITE);

            DrawTextEx(myFont,"< Geri",(Vector2){45,42},20,1,
                       CheckCollisionPointRec(mouse,btnBack)
                       ? COLOR_PASTEL_PINK : WHITE);

            StackNode* temp=historyStack;
            int i=0;
            while(temp && i<10)
            {
                float y=150+i*50;
                DrawRectangleRounded((Rectangle){100,y,700,40},0.2,10,WHITE);
                DrawTextEx(myFont,temp->last_search,(Vector2){130,y+10},20,1,COLOR_TEXT);
                temp=temp->next;
                i++;
            }
        }

        EndDrawing();
    }

    if (historyStack) freeStack(historyStack);
    UnloadFont(myFont);
    CloseWindow();
}
