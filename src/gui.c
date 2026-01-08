#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

#include "../include/book.h"
#include "../include/gui.h"
#include "../include/stack.h"
#include "../include/sort.h"
#include "../include/search.h"
#include "../include/queue.h"
#include "../include/graph.h" 

/* --- RENK PALETİ --- */
#define COLOR_BG            (Color){255,250,250,255}
#define COLOR_PASTEL_PINK   (Color){255,182,193,255}
#define COLOR_DARK_PINK     (Color){255,140,157,255}
#define COLOR_TEXT          (Color){80, 60, 70, 255}
#define COLOR_RED_ALERT     (Color){220, 80, 80, 255}
#define COLOR_MENU_BG       (Color){255, 255, 255, 255}
#define COLOR_MENU_BORDER   (Color){200, 150, 160, 255}
#define COLOR_MENU_HOVER    (Color){255, 230, 235, 255}

// MENÜ AYARLARI
#define MAX_ITEMS_PER_COL   10  // Bir sütunda en fazla 10 tür olsun (Taşmayı engeller)

typedef enum { PAGE_HOME, PAGE_HISTORY, PAGE_LOANS, PAGE_PATH_FINDER } GuiPage;
typedef enum { FILTER_NONE, FILTER_SCORE_ASC, FILTER_SCORE_DESC, FILTER_ALPHABETIC } FilterType;

/* --- GLOBAL DEĞİŞKENLER --- */
static StackNode* historyStack = NULL;
static StackNode* undoStack = NULL;    
static Queue* loanQueue = NULL;        
static bool borrowed[500] = { false }; 

static Book displayList[500]; 
static int displayCount = 0;
static int scrollOffset = 0;
static FilterType activeFilter = FILTER_NONE;

// Menü Kontrol
static bool isBooksMenuOpen = false;
static char hoveredMainGenre[50] = {0};

// Harita / BFS
static int selectedStartID = -1;
static int selectedTargetID = -1;
static int calculatedPath[600];
static int pathLength = 0;

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

bool is_string_in_array(char arr[][50], int count, const char* target) {
    for(int i=0; i<count; i++) if(strcmp(arr[i], target) == 0) return true;
    return false;
}

void start_gui(Book *library, int count)
{
    InitWindow(1400, 750, "Kütüphane Yönetim Paneli");
    SetTargetFPS(60);
    loanQueue = createQueue();
    
    memcpy(displayList, library, sizeof(Book) * count);
    displayCount = count;

    // FONT YÜKLEME
    int codepoints[512] = { 0 };
    int cpCount = 0;
    for (int i = 32; i <= 126; i++) codepoints[cpCount++] = i;
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
        
        // Scroll Mantığı
        if (page == PAGE_HOME || page == PAGE_LOANS || page == PAGE_PATH_FINDER) {
            scrollOffset -= GetMouseWheelMove() * 35;
            if (scrollOffset < 0) scrollOffset = 0;
        } else scrollOffset = 0;

        // --- KLAVYE & ARAMA ---
        if (page == PAGE_HOME && !isBooksMenuOpen) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (letterCount < 250)) {
                    int byteSize = 0; const char* c = CodepointToUTF8(key, &byteSize);
                    for(int i = 0; i < byteSize; i++) searchText[letterCount++] = c[i];
                    searchText[letterCount] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
                letterCount--;
                while (letterCount > 0 && (searchText[letterCount] & 0xC0) == 0x80) letterCount--;
                searchText[letterCount] = '\0';
                if (letterCount == 0) {
                    memcpy(displayList, library, sizeof(Book) * count); displayCount = count; apply_current_sorting();
                }
            }
            if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                push(&historyStack, searchText);
                displayCount = search_books_by_prefix(library, count, searchText, displayList);
                apply_current_sorting(); scrollOffset = 0;
            }
        }

        // --- BUTON TANIMLARI ---
        Rectangle btnBooksMenu = {40, 140, 160, 45};
        Rectangle searchBar  = {220, 140, 380, 45};
        Rectangle btnFilter  = {620, 140, 120, 45}; 
        Rectangle btnHistory = {750, 140, 100, 45};
        Rectangle btnLoans   = {860, 140, 160, 45};
        Rectangle btnGraph   = {1030, 140, 150, 45}; 
        Rectangle btnBack    = {40, 140, 120, 45};

        // --- TIKLAMA MANTIKLARI ---
        if (page == PAGE_HOME) {
            if (CheckCollisionPointRec(mouse, btnBooksMenu) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isBooksMenuOpen = !isBooksMenuOpen; showFilterMenu = false;
                if(!isBooksMenuOpen) strcpy(hoveredMainGenre, ""); 
            }
            if (CheckCollisionPointRec(mouse, btnFilter) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showFilterMenu = !showFilterMenu; isBooksMenuOpen = false;
            }
            if (showFilterMenu && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                bool clicked = false;
                if (CheckCollisionPointRec(mouse, (Rectangle){620, 185, 120, 45})) { activeFilter=FILTER_SCORE_ASC; clicked=true; }
                else if (CheckCollisionPointRec(mouse, (Rectangle){620, 230, 120, 45})) { activeFilter=FILTER_SCORE_DESC; clicked=true; }
                else if (CheckCollisionPointRec(mouse, (Rectangle){620, 275, 120, 45})) { activeFilter=FILTER_ALPHABETIC; clicked=true; }
                if (clicked) { apply_current_sorting(); showFilterMenu = false; }
            }
            if (!isBooksMenuOpen && !showFilterMenu) {
                if (CheckCollisionPointRec(mouse, btnHistory) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) page = PAGE_HISTORY;
                else if (CheckCollisionPointRec(mouse, btnLoans) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { page = PAGE_LOANS; scrollOffset = 0; }
                else if (CheckCollisionPointRec(mouse, btnGraph) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    page = PAGE_PATH_FINDER; scrollOffset = 0; selectedStartID = -1; selectedTargetID = -1; pathLength = 0;
                }
            }
        } else if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            page = PAGE_HOME; scrollOffset = 0;
        }

        BeginDrawing();
        ClearBackground(COLOR_BG);

        /* ====================================================================
           SAYFA: ANASAYFA
           ==================================================================== */
        if (page == PAGE_HOME) {
            drawHeader(font, "KÜTÜPHANE SİSTEMİ");
            DrawRectangleRounded(btnBooksMenu, 0.3, 10, isBooksMenuOpen ? COLOR_DARK_PINK : COLOR_PASTEL_PINK);
            DrawTextEx(font, "Kitaplar", (Vector2){70, 152}, 20, 1, WHITE);
            DrawTextEx(font, isBooksMenuOpen ? "^" : "v", (Vector2){170, 152}, 15, 1, WHITE);

            DrawRectangleRounded(searchBar, 0.2, 10, WHITE);
            DrawRectangleLinesEx(searchBar, 2, COLOR_PASTEL_PINK);
            DrawTextEx(font, letterCount ? searchText : "Ara...", (Vector2){230, 148}, 28, 1, letterCount ? COLOR_TEXT : Fade(LIGHTGRAY, 0.7f));

            DrawRectangleRounded(btnFilter, 0.3, 10, COLOR_PASTEL_PINK); DrawTextEx(font, "Filtreler", (Vector2){640, 152}, 18, 1, WHITE);
            DrawRectangleRounded(btnHistory, 0.3, 10, COLOR_PASTEL_PINK); DrawTextEx(font, "Geçmiş", (Vector2){770, 152}, 18, 1, WHITE);
            DrawRectangleRounded(btnLoans, 0.3, 10, COLOR_PASTEL_PINK); DrawTextEx(font, "Ödünç Alınanlar", (Vector2){877, 152}, 18, 1, WHITE);
            DrawRectangleRounded(btnGraph, 0.3, 10, COLOR_PASTEL_PINK); DrawTextEx(font, "Ağ Analizi", (Vector2){1060, 152}, 18, 1, WHITE);

            int visibleIdx = 0;
            for (int i = 0; i < displayCount; i++) {
                int originalIndex = -1;
                for(int k=0; k<count; k++) { if(displayList[i].id == library[k].id) { originalIndex = k; break; } }
                
                if (originalIndex != -1 && borrowed[originalIndex]) continue;

                float yPos = 240 + (visibleIdx * 75) - scrollOffset;
                if (yPos < 180 || yPos > GetScreenHeight()) { visibleIdx++; continue; }

                DrawRectangleRounded((Rectangle){40, yPos, 800, 65}, 0.15, 10, Fade(COLOR_PASTEL_PINK, 0.1f));
                DrawTextEx(font, displayList[i].title, (Vector2){60, yPos + 10}, 20, 1, COLOR_TEXT);
                char genreStr[100]; sprintf(genreStr, "%s  >  %s", displayList[i].genre, displayList[i].subgenre);
                DrawTextEx(font, genreStr, (Vector2){400, yPos + 22}, 16, 1, GRAY);
                DrawTextEx(font, displayList[i].author, (Vector2){60, yPos + 35}, 15, 1, GRAY);
                DrawTextEx(font, TextFormat("%.1f", displayList[i].score), (Vector2){780, yPos + 22}, 20, 1, COLOR_DARK_PINK);

                Rectangle btnBorrow = {860, yPos + 15, 120, 35};
                DrawRectangleRounded(btnBorrow, 0.3, 8, COLOR_PASTEL_PINK);
                DrawTextEx(font, "Ödünç Al", (Vector2){875, yPos + 22}, 15, 1, WHITE);

                if (!isBooksMenuOpen && !showFilterMenu && CheckCollisionPointRec(mouse, btnBorrow) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    enqueue_loan(loanQueue, "Zeynep", displayList[i].title);
                    push(&undoStack, displayList[i].title);
                    if (originalIndex != -1) borrowed[originalIndex] = true;
                }
                visibleIdx++;
            }

            if (showFilterMenu) {
                DrawRectangle(620, 185, 120, 135, WHITE);
                DrawRectangleLines(620, 185, 120, 135, COLOR_MENU_BORDER);
                DrawTextEx(font, "Artan Puan", (Vector2){630, 198}, 16, 1, COLOR_TEXT);
                DrawTextEx(font, "Azalan Puan", (Vector2){630, 243}, 16, 1, COLOR_TEXT);
                DrawTextEx(font, "Alfabetik", (Vector2){630, 288}, 16, 1, COLOR_TEXT);
            }

            // -------------------------------------------------------------
            // DÜZELTME 1: ÇOKLU SÜTUN (SCROLL YERİNE YAN YANA AÇILAN MENÜ)
            // -------------------------------------------------------------
            if (isBooksMenuOpen) {
                char uniqueMain[20][50]; int mainCount = 0;
                for(int i=0; i<count; i++) { if(!is_string_in_array(uniqueMain, mainCount, library[i].genre)) { strcpy(uniqueMain[mainCount], library[i].genre); mainCount++; } }

                float menuX = 40, menuY = 185, itemW = 160, itemH = 40;
                DrawRectangle(menuX, menuY, itemW, mainCount * itemH, COLOR_MENU_BG);
                DrawRectangleLines(menuX, menuY, itemW, mainCount * itemH, COLOR_MENU_BORDER);

                for(int i=0; i<mainCount; i++) {
                    Rectangle itemRect = {menuX, menuY + (i * itemH), itemW, itemH};
                    if (CheckCollisionPointRec(mouse, itemRect)) {
                        DrawRectangleRec(itemRect, COLOR_MENU_HOVER);
                        strcpy(hoveredMainGenre, uniqueMain[i]);
                    }
                    DrawTextEx(font, TextFormat("v %s", uniqueMain[i]), (Vector2){menuX + 15, menuY + (i*itemH) + 10}, 18, 1, COLOR_TEXT);
                }

                if (strlen(hoveredMainGenre) > 0) {
                    char uniqueSub[50][50]; int subCount = 0;
                    for(int i=0; i<count; i++) {
                        if (strcmp(library[i].genre, hoveredMainGenre) == 0 && !is_string_in_array(uniqueSub, subCount, library[i].subgenre)) {
                            strcpy(uniqueSub[subCount], library[i].subgenre); subCount++;
                        }
                    }
                    if (subCount > 0) {
                        float subX = menuX + itemW, subY = menuY;
                        
                        // Sütun Hesaplama: Eğer 10'dan fazla ise yeni sütun aç
                        int numCols = (subCount + MAX_ITEMS_PER_COL - 1) / MAX_ITEMS_PER_COL;
                        int totalWidth = numCols * itemW;
                        int currentHeightRows = (subCount > MAX_ITEMS_PER_COL) ? MAX_ITEMS_PER_COL : subCount;

                        DrawRectangle(subX, subY, totalWidth, currentHeightRows * itemH, COLOR_MENU_BG);
                        DrawRectangleLines(subX, subY, totalWidth, currentHeightRows * itemH, COLOR_MENU_BORDER);

                        for(int j=0; j<subCount; j++) {
                            // Hangi sütun ve hangi satır?
                            int col = j / MAX_ITEMS_PER_COL;
                            int row = j % MAX_ITEMS_PER_COL;
                            
                            Rectangle subRect = {subX + (col * itemW), subY + (row * itemH), itemW, itemH};
                            bool subHover = CheckCollisionPointRec(mouse, subRect);
                            
                            if (subHover) DrawRectangleRec(subRect, COLOR_MENU_HOVER);
                            DrawTextEx(font, TextFormat("> %s", uniqueSub[j]), (Vector2){subRect.x + 15, subRect.y + 10}, 18, 1, COLOR_TEXT);
                            
                            if (subHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                                displayCount = 0;
                                for(int k=0; k<count; k++) {
                                    if(strcmp(library[k].genre, hoveredMainGenre) == 0 && strcmp(library[k].subgenre, uniqueSub[j]) == 0) displayList[displayCount++] = library[k];
                                }
                                activeFilter = FILTER_ALPHABETIC; apply_current_sorting();
                                isBooksMenuOpen = false; scrollOffset = 0;
                            }
                        }
                    }
                }
            }
        } 
        /* ====================================================================
           SAYFA: AĞ ANALİZİ
           ==================================================================== */
        else if (page == PAGE_PATH_FINDER) {
             drawHeader(font, "AĞ ANALİZİ");
             DrawRectangleRounded(btnBack, 0.3, 10, COLOR_PASTEL_PINK); DrawTextEx(font, "Geri", (Vector2){75, 152}, 18, 1, WHITE);
             DrawTextEx(font, "Bir kitaba tıklayın, hiyerarşik yolunu görün.", (Vector2){250, 155}, 20, 1, GRAY);

             int visibleIdx = 0;
             for (int i = 0; i < count; i++) {
                float yPos = 200 + (visibleIdx * 50) - scrollOffset;
                if (yPos > GetScreenHeight() - 150) { visibleIdx++; continue; }
                if (yPos < 180) { visibleIdx++; continue; }
                Color boxColor = Fade(LIGHTGRAY, 0.3f);
                if (library[i].id == selectedTargetID) boxColor = COLOR_DARK_PINK;
                
                Rectangle rowRect = {40, yPos, 1300, 45};
                DrawRectangleRounded(rowRect, 0.2, 5, boxColor);
                DrawTextEx(font, library[i].title, (Vector2){60, yPos + 10}, 20, 1, COLOR_TEXT);
                DrawTextEx(font, TextFormat("[%s > %s]", library[i].genre, library[i].subgenre), (Vector2){600, yPos + 12}, 16, 1, GRAY);
                
                if (CheckCollisionPointRec(mouse, rowRect)) {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        selectedStartID = 0; selectedTargetID = library[i].id;
                        pathLength = get_shortest_path_bfs(selectedStartID, selectedTargetID, library, count, calculatedPath);
                    }
                }
                visibleIdx++;
             }
             DrawRectangle(0, GetScreenHeight() - 120, GetScreenWidth(), 120, WHITE);
             DrawRectangleLines(0, GetScreenHeight() - 120, GetScreenWidth(), 120, COLOR_PASTEL_PINK);
             if (pathLength > 0) {
                 int xPos = 40;
                 for(int i=0; i<pathLength; i++) {
                     char nameBuffer[100]; int nodeID = calculatedPath[i];
                     if(nodeID == 0) strcpy(nameBuffer, "TUM KITAPLAR");
                     else if(nodeID >= 601) { if(i==1) { for(int k=0; k<count; k++) { if(library[k].id == selectedTargetID) { strcpy(nameBuffer, library[k].genre); break; } } } else { sprintf(nameBuffer, "ANA TUR (%d)", nodeID); } } 
                     else if(nodeID >= 501) { if(i==2) { for(int k=0; k<count; k++) { if(library[k].id == selectedTargetID) { strcpy(nameBuffer, library[k].subgenre); break; } } } else { sprintf(nameBuffer, "ALT TUR (%d)", nodeID); } }
                     else { for(int k=0; k<count; k++) if(library[k].id == nodeID) { strncpy(nameBuffer, library[k].title, 99); break;} }
                     DrawTextEx(font, nameBuffer, (Vector2){xPos, GetScreenHeight()-70}, 20, 1, COLOR_TEXT);
                     if(i < pathLength-1) { int textW = MeasureTextEx(font, nameBuffer, 20, 1).x; DrawTextEx(font, " -> ", (Vector2){xPos + textW, GetScreenHeight()-70}, 20, 1, COLOR_RED_ALERT); xPos += textW + 40; }
                 }
             }
        }
        /* ====================================================================
           SAYFA: ARAMA GEÇMİŞİ
           ==================================================================== */
        else if (page == PAGE_HISTORY) {
            drawHeader(font, "ARAMA GEÇMİŞİ");
            DrawRectangleRounded(btnBack, 0.3, 10, COLOR_PASTEL_PINK); DrawTextEx(font, "Geri", (Vector2){75, 152}, 18, 1, WHITE);
            Rectangle btnClearAll = {1100, 140, 200, 45};
            DrawRectangleRounded(btnClearAll, 0.3, 10, COLOR_RED_ALERT); DrawTextEx(font, "Tümünü Temizle", (Vector2){1125, 152}, 18, 1, WHITE);
            
            if (CheckCollisionPointRec(mouse, btnClearAll) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                while (historyStack != NULL) { char* d = pop(&historyStack); if(d) free(d); }
            }
            StackNode* t = historyStack; int i = 0; bool actionTaken = false; 
            while(t && i < 15) {
                float yPos = 220 + i*50;
                DrawTextEx(font, TextFormat("%d. %s", i+1, t->last_search), (Vector2){100, yPos}, 24, 1, COLOR_TEXT);
                Rectangle btnDel = {400, yPos, 30, 30}; DrawRectangleRounded(btnDel, 0.2, 5, COLOR_RED_ALERT); DrawTextEx(font, "X", (Vector2){408, yPos+2}, 20, 1, WHITE);
                if (!actionTaken && CheckCollisionPointRec(mouse, btnDel) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { delete_search_node(&historyStack, t->last_search); actionTaken = true; }
                if (actionTaken) break; t=t->next; i++; 
            }
        }
        /* ====================================================================
           SAYFA: ÖDÜNÇLER (DÜZELTME 2: DETAYLI BİLGİ EKLENDİ)
           ==================================================================== */
        else if (page == PAGE_LOANS) {
            drawHeader(font, "ÖDÜNÇ ALINANLAR");
            DrawRectangleRounded(btnBack, 0.3, 10, COLOR_PASTEL_PINK); DrawTextEx(font, "Geri", (Vector2){75, 152}, 18, 1, WHITE);
            
            QueueNode* curr = loanQueue->front; 
            int idx = 0; bool actionTaken = false;

            while(curr) {
                float yPos = 200 + idx*90 - scrollOffset; // Aralık arttırıldı (Detaylar sığsın diye)
                
                if (yPos > 100 && yPos < GetScreenHeight()) {
                    DrawRectangleRounded((Rectangle){100, yPos, 1100, 80}, 0.2, 10, Fade(COLOR_PASTEL_PINK, 0.1f));
                    
                    // 1. Kitap Adı
                    DrawTextEx(font, TextFormat("Kitap: %s", curr->bookTitle), (Vector2){120, yPos+10}, 20, 1, COLOR_TEXT);
                    
                    // 2. Alan Kişi
                    DrawTextEx(font, TextFormat("Alan: %s", curr->userName), (Vector2){120, yPos+45}, 16, 1, GRAY);

                    // 3. Tarih Bilgileri (Sağ tarafa hizalı)
                    DrawTextEx(font, TextFormat("Alım Tarihi: %s", curr->borrowDate), (Vector2){500, yPos+15}, 16, 1, GRAY);
                    DrawTextEx(font, TextFormat("Son Teslim Tarihi: %s", curr->dueDate), (Vector2){500, yPos+45}, 16, 1, COLOR_RED_ALERT);

                    // İade Butonu
                    Rectangle btnReturn = {960, yPos+25, 120, 30};
                    DrawRectangleRounded(btnReturn, 0.3, 8, COLOR_PASTEL_PINK); DrawTextEx(font, "İade Et", (Vector2){985, yPos+30}, 16, 1, WHITE);
                    
                    if(!actionTaken && CheckCollisionPointRec(mouse, btnReturn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                         for(int k=0; k<count; k++) { if(strcmp(library[k].title, curr->bookTitle) == 0) { borrowed[k] = false; break; } }
                         char tmp[100]; strcpy(tmp, curr->bookTitle); remove_specific_loan(loanQueue, tmp); actionTaken = true;
                    }
                }
                if(actionTaken) break;
                curr = curr->next; idx++;
            }
        }

        EndDrawing();
    }
    
    // Temizlik
    freeStack(historyStack); freeStack(undoStack);
    while(loanQueue->front) dequeue_return(loanQueue);
    free(loanQueue);
    UnloadFont(font); CloseWindow();
}