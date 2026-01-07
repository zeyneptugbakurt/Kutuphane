#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raylib.h"

#include "../include/book.h"
#include "../include/gui.h"
#include "../include/stack.h"
#include "../include/sort.h"
#include "../include/search.h"
#include "../include/queue.h"

#define COLOR_BG            (Color){255,250,250,255}
#define COLOR_PASTEL_PINK   (Color){255,182,193,255}
#define COLOR_DARK_PINK     (Color){255,140,157,255}
#define COLOR_TEXT          (Color){100,80,90,255}
#define COLOR_RED_ALERT     (Color){220, 80, 80, 255}

typedef enum { PAGE_HOME, PAGE_HISTORY, PAGE_LOANS } GuiPage;
typedef enum { FILTER_NONE, FILTER_SCORE_ASC, FILTER_SCORE_DESC, FILTER_ALPHABETIC } FilterType;

static StackNode* historyStack = NULL;
static Queue* loanQueue = NULL;
static bool borrowed[200] = { false }; 
static Book displayList[200]; 
static int displayCount = 0;
static bool searchActive = false;
static int scrollOffset = 0;
static FilterType activeFilter = FILTER_NONE;

static void drawHeader(Font font, const char* title) {
    DrawRectangle(0, 0, GetScreenWidth(), 100, COLOR_PASTEL_PINK);
    DrawTextEx(font, title, (Vector2){40, 28}, 42, 1, WHITE);
}

void apply_current_sorting() {
    if (displayCount <= 0) return;
    if (activeFilter == FILTER_SCORE_ASC) quickSort(displayList, 0, displayCount - 1);
    else if (activeFilter == FILTER_SCORE_DESC) heapSort(displayList, displayCount);
    else if (activeFilter == FILTER_ALPHABETIC) mergeSort(displayList, 0, displayCount - 1);
}

void start_gui(Book *library, int count)
{
    InitWindow(1400, 750, "Kütüphane Yönetim Paneli");
    SetTargetFPS(60);
    loanQueue = createQueue();
    memcpy(displayList, library, sizeof(Book) * count);
    displayCount = count;

    // --- FONT YÜKLEME (Genişletilmiş) ---
    int codepoints[512] = { 0 };
    int cpCount = 0;
    // ASCII (32-126) -> Harf, Rakam, Tire, Nokta, Virgül hepsi burada
    for (int i = 32; i <= 126; i++) codepoints[cpCount++] = i;
    // Türkçe Karakterler
    int trIds[] = {0x0130, 0x0131, 0x015E, 0x015F, 0x011E, 0x011F, 0x00C7, 0x00E7, 0x00DC, 0x00FC, 0x00D6, 0x00F6};
    for(int i=0; i<12; i++) codepoints[cpCount++] = trIds[i];

    Font font = LoadFontEx("resources/arial.ttf", 40, codepoints, cpCount);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    char searchText[256] = {0};
    int letterCount = 0;
    bool showFilterMenu = false;
    GuiPage page = PAGE_HOME;

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();
        if (page == PAGE_HOME || page == PAGE_LOANS) {
            scrollOffset -= GetMouseWheelMove() * 35;
            if (scrollOffset < 0) scrollOffset = 0;
        } else scrollOffset = 0;

        /* INPUT */
        if (page == PAGE_HOME) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (letterCount < 250)) {
                    int byteSize = 0;
                    const char* charUtf8 = CodepointToUTF8(key, &byteSize);
                    for(int i = 0; i < byteSize; i++) searchText[letterCount++] = charUtf8[i];
                    searchText[letterCount] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
                letterCount--;
                while (letterCount > 0 && (searchText[letterCount] & 0xC0) == 0x80) letterCount--;
                searchText[letterCount] = '\0';
                if (letterCount == 0) {
                    searchActive = false;
                    memcpy(displayList, library, sizeof(Book) * count);
                    displayCount = count;
                    apply_current_sorting();
                }
            }
            if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                push(&historyStack, searchText);
                displayCount = search_books_by_prefix(library, count, searchText, displayList);
                searchActive = true;
                apply_current_sorting();
                scrollOffset = 0;
            } else if (letterCount > 0) {
                 displayCount = search_books_by_prefix(library, count, searchText, displayList);
                 apply_current_sorting();
            }
        }

        /* ETKİLEŞİM */
        Rectangle searchBar  = {40,140,550,45};
        Rectangle btnFilter  = {620,140,140,45}; 
        Rectangle btnHistory = {775,140,120,45};
        Rectangle btnLoans   = {910,140,220,45};
        Rectangle btnBack    = {40,140,120,45};

        if (page == PAGE_HOME) {
            if (CheckCollisionPointRec(mouse, btnFilter) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) showFilterMenu = !showFilterMenu;
            if (showFilterMenu && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                bool clicked = false;
                if (CheckCollisionPointRec(mouse, (Rectangle){620, 185, 140, 45})) { activeFilter=FILTER_SCORE_ASC; clicked=true; }
                else if (CheckCollisionPointRec(mouse, (Rectangle){620, 230, 140, 45})) { activeFilter=FILTER_SCORE_DESC; clicked=true; }
                else if (CheckCollisionPointRec(mouse, (Rectangle){620, 275, 140, 45})) { activeFilter=FILTER_ALPHABETIC; clicked=true; }
                if (clicked) { apply_current_sorting(); showFilterMenu = false; }
                else if (!CheckCollisionPointRec(mouse, btnFilter)) showFilterMenu = false;
            } else if (!showFilterMenu) {
                if (CheckCollisionPointRec(mouse, btnHistory) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) page = PAGE_HISTORY;
                else if (CheckCollisionPointRec(mouse, btnLoans) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { page = PAGE_LOANS; scrollOffset = 0; }
            }
        } else if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            page = PAGE_HOME; scrollOffset = 0;
        }

        /* ÇİZİM */
        BeginDrawing();
        ClearBackground(COLOR_BG);

        if (page == PAGE_HOME) {
            drawHeader(font, "KÜTÜPHANE SİSTEMİ");
            DrawRectangleRounded(searchBar, 0.2, 10, WHITE);
            DrawRectangleLinesEx(searchBar, 2, COLOR_PASTEL_PINK);
            DrawTextEx(font, letterCount ? searchText : "Kitap veya Yazar ara...", (Vector2){55,148}, 28, 1, letterCount ? COLOR_TEXT : Fade(LIGHTGRAY, 0.7f));
            DrawRectangleRounded(btnFilter, 0.3, 10, COLOR_PASTEL_PINK); DrawTextEx(font, "Filtreler", (Vector2){655, 152}, 18, 1, WHITE);
            DrawRectangleRounded(btnHistory, 0.3, 10, COLOR_PASTEL_PINK); DrawTextEx(font, "Geçmiş", (Vector2){795, 152}, 18, 1, WHITE);
            DrawRectangleRounded(btnLoans, 0.3, 10, COLOR_PASTEL_PINK); DrawTextEx(font, "Ödünç Alınanlar", (Vector2){935, 152}, 18, 1, WHITE);

            int visibleIdx = 0;
            for (int i = 0; i < displayCount; i++) {
                int originalIndex = -1;
                for(int k=0; k<count; k++) { if(displayList[i].id == library[k].id) { originalIndex = k; break; } }
                if (originalIndex != -1 && borrowed[originalIndex]) continue;

                float yPos = 240 + (visibleIdx * 75) - scrollOffset;
                if (yPos < 180 || yPos > GetScreenHeight()) { visibleIdx++; continue; }

                DrawRectangleRounded((Rectangle){40, yPos, 800, 65}, 0.15, 10, Fade(COLOR_PASTEL_PINK, 0.1f));
                DrawTextEx(font, displayList[i].title, (Vector2){60, yPos + 10}, 20, 1, COLOR_TEXT);
                DrawTextEx(font, displayList[i].author, (Vector2){60, yPos + 35}, 15, 1, GRAY);
                DrawTextEx(font, TextFormat("%.1f", displayList[i].score), (Vector2){780, yPos + 22}, 20, 1, COLOR_DARK_PINK);
                Rectangle btnBorrow = {860, yPos + 15, 120, 35};
                DrawRectangleRounded(btnBorrow, 0.3, 8, COLOR_PASTEL_PINK);
                DrawTextEx(font, "Ödünç Al", (Vector2){875, yPos + 22}, 15, 1, WHITE);

                if (CheckCollisionPointRec(mouse, btnBorrow) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (!showFilterMenu) {
                        enqueue_loan(loanQueue, "Zeynep", displayList[i].title);
                        if (originalIndex != -1) borrowed[originalIndex] = true;
                    }
                }
                visibleIdx++;
            }
            if (showFilterMenu) {
                DrawRectangle(620, 185, 140, 135, WHITE);
                DrawRectangleLines(620, 185, 140, 135, COLOR_PASTEL_PINK);
                DrawTextEx(font, "Artan Puan", (Vector2){635, 198}, 18, 1, COLOR_TEXT);
                DrawTextEx(font, "Azalan Puan", (Vector2){635, 243}, 18, 1, COLOR_TEXT);
                DrawTextEx(font, "Alfabetik", (Vector2){635, 288}, 18, 1, COLOR_TEXT);
            }
        } 
        else if (page == PAGE_HISTORY) {
            drawHeader(font, "ARAMA GEÇMİŞİ");
            DrawRectangleRounded(btnBack, 0.3, 10, COLOR_PASTEL_PINK);
            DrawTextEx(font, "Geri", (Vector2){75, 152}, 18, 1, WHITE);
            StackNode* t = historyStack;
            for (int i = 0; t && i < 15; i++, t = t->next) DrawTextEx(font, t->last_search, (Vector2){100, 200 + i * 40}, 24, 1, COLOR_TEXT);
        }
        else if (page == PAGE_LOANS) {
            drawHeader(font, "ÖDÜNÇ ALINANLAR");
            DrawRectangleRounded(btnBack, 0.3, 10, COLOR_PASTEL_PINK);
            DrawTextEx(font, "Geri", (Vector2){75, 152}, 18, 1, WHITE);
            QueueNode* curr = loanQueue->front;
            int idx = 0;
            bool actionTaken = false; 
            while (curr) {
                float yPos = 200 + idx * 80 - scrollOffset;
                if (yPos > 100 && yPos < GetScreenHeight()) {
                    DrawRectangleRounded((Rectangle){100, yPos, 1000, 70}, 0.2, 10, Fade(COLOR_PASTEL_PINK, 0.1f));
                    DrawTextEx(font, curr->bookTitle, (Vector2){120, yPos + 10}, 20, 1, COLOR_TEXT);
                    DrawTextEx(font, curr->userName, (Vector2){120, yPos + 40}, 16, 1, GRAY);
                    DrawTextEx(font, TextFormat("Verilis: %s", curr->borrowDate), (Vector2){720, yPos + 15}, 16, 1, GRAY);
                    DrawTextEx(font, TextFormat("Son Teslim: %s", curr->dueDate), (Vector2){720, yPos + 40}, 16, 1, COLOR_RED_ALERT);
                    Rectangle btnReturn = {960, yPos + 20, 120, 30};
                    DrawRectangleRounded(btnReturn, 0.3, 8, COLOR_PASTEL_PINK);
                    DrawTextEx(font, "Iade Et", (Vector2){985, yPos + 25}, 16, 1, WHITE);
                    if (!actionTaken && CheckCollisionPointRec(mouse, btnReturn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        for(int k = 0; k < count; k++) { if(strcmp(library[k].title, curr->bookTitle) == 0) { borrowed[k] = false; break; } }
                        char titleToDel[200]; strcpy(titleToDel, curr->bookTitle);
                        remove_specific_loan(loanQueue, titleToDel);
                        actionTaken = true; 
                    }
                }
                if (actionTaken) break; 
                curr = curr->next; idx++;
            }
        }
        EndDrawing();
    }
    freeStack(historyStack);
    while (loanQueue->front) dequeue_return(loanQueue);
    free(loanQueue);
    UnloadFont(font);
    CloseWindow();
}