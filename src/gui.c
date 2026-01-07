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

/* --------------------------------------------------
   GÖRSEL AYARLAR
-------------------------------------------------- */
#define COLOR_BG            (Color){255,250,250,255}
#define COLOR_PASTEL_PINK   (Color){255,182,193,255}
#define COLOR_DARK_PINK     (Color){255,140,157,255}
#define COLOR_TEXT          (Color){100,80,90,255}
#define COLOR_RED_ALERT     (Color){220, 80, 80, 255}

typedef enum { PAGE_HOME, PAGE_HISTORY, PAGE_LOANS } GuiPage;
typedef enum { FILTER_NONE, FILTER_SCORE_ASC, FILTER_SCORE_DESC, FILTER_ALPHABETIC } FilterType;

/* --------------------------------------------------
   GUI STATE
-------------------------------------------------- */
static StackNode* historyStack = NULL;
static Queue* loanQueue = NULL;
static bool borrowed[200] = { false }; // Kitapların ödünç durumunu tutar

// Ekranda gösterilecek mevcut kitap listesi
static Book displayList[200]; 
static int displayCount = 0;

static bool searchActive = false;
static int scrollOffset = 0;
static FilterType activeFilter = FILTER_NONE;

/* --------------------------------------------------
   YARDIMCI GÖRSEL FONKSİYONLAR
-------------------------------------------------- */
static void drawHeader(Font font, const char* title) {
    DrawRectangle(0, 0, GetScreenWidth(), 100, COLOR_PASTEL_PINK);
    DrawTextEx(font, title, (Vector2){40, 28}, 42, 1, WHITE);
}

// GUI'den bağımsız sadece sıralama tetikleyici
void apply_current_sorting() {
    if (displayCount <= 0) return;
    if (activeFilter == FILTER_SCORE_ASC) quickSort(displayList, 0, displayCount - 1);
    else if (activeFilter == FILTER_SCORE_DESC) heapSort(displayList, displayCount);
    else if (activeFilter == FILTER_ALPHABETIC) mergeSort(displayList, 0, displayCount - 1);
}

/* --------------------------------------------------
   ANA GUI FONKSİYONU
-------------------------------------------------- */
void start_gui(Book *library, int count)
{
    InitWindow(1400, 750, "Kütüphane Yönetim Paneli");
    SetTargetFPS(60);
    loanQueue = createQueue();
    
    // İlk açılışta tüm kütüphaneyi yükle
    memcpy(displayList, library, sizeof(Book) * count);
    displayCount = count;

    /* --- FONT YÜKLEME --- */
    int codepoints[128];
    for (int i = 0; i < 95; i++) codepoints[i] = 32 + i;
    int tr[] = {0x0130,0x0131,0x015E,0x015F,0x011E,0x011F,0x00C7,0x00E7,0x00DC,0x00FC,0x00D6,0x00F6};
    for (int i = 0; i < 12; i++) codepoints[95+i] = tr[i];
    Font font = LoadFontEx("resources/arial.ttf", 40, codepoints, 107);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    char searchText[128] = {0};
    int letterCount = 0;
    bool showFilterMenu = false;
    GuiPage page = PAGE_HOME;

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();
        
        // Scroll işlemi (Sadece listelerde aktif)
        if (page == PAGE_HOME || page == PAGE_LOANS) {
            scrollOffset -= GetMouseWheelMove() * 35;
            if (scrollOffset < 0) scrollOffset = 0;
        } else {
            scrollOffset = 0;
        }

        /* ---------- INPUT VE ARAMA (TRIE KULLANIMI) ---------- */
        if (page == PAGE_HOME) 
        {
            int key = GetCharPressed();
            while (key > 0) {
                if (letterCount < 126) {
                    searchText[letterCount++] = (char)key;
                    searchText[letterCount] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
                searchText[--letterCount] = '\0';
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
            }
        }

        /* ---------- TIKLAMA VE MENÜ ETKİLEŞİMİ ---------- */
        Rectangle searchBar  = {40,140,550,45};
        // Filtre butonu genişliği 140 (Yükseklik 45)
        Rectangle btnFilter  = {620,140,140,45}; 
        Rectangle btnHistory = {775,140,120,45};
        Rectangle btnLoans   = {910,140,220,45};
        Rectangle btnBack    = {40,140,120,45};

        if (page == PAGE_HOME) {
            // FİLTRE BUTONU
            if (CheckCollisionPointRec(mouse, btnFilter) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showFilterMenu = !showFilterMenu;
            }
            
            // Filtre Menüsü Seçimleri
            if (showFilterMenu) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    bool clickedOption = false;
                    // Her bir seçenek butonuyla aynı boyutta: 140x45
                    // 1. Seçenek (185 - 230)
                    if (CheckCollisionPointRec(mouse, (Rectangle){620, 185, 140, 45})) {
                        activeFilter = FILTER_SCORE_ASC; clickedOption = true;
                    } 
                    // 2. Seçenek (230 - 275)
                    else if (CheckCollisionPointRec(mouse, (Rectangle){620, 230, 140, 45})) {
                        activeFilter = FILTER_SCORE_DESC; clickedOption = true;
                    } 
                    // 3. Seçenek (275 - 320)
                    else if (CheckCollisionPointRec(mouse, (Rectangle){620, 275, 140, 45})) {
                        activeFilter = FILTER_ALPHABETIC; clickedOption = true;
                    }

                    if (clickedOption) {
                        apply_current_sorting();
                        showFilterMenu = false;
                    } else if (!CheckCollisionPointRec(mouse, btnFilter)) {
                        showFilterMenu = false; 
                    }
                }
            } 
            else {
                // Diğer Sayfa Geçişleri
                if (CheckCollisionPointRec(mouse, btnHistory) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    page = PAGE_HISTORY;
                else if (CheckCollisionPointRec(mouse, btnLoans) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    page = PAGE_LOANS;
                    scrollOffset = 0;
                }
            }
        } 
        else if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            page = PAGE_HOME;
            scrollOffset = 0;
        }

        /* ---------- ÇİZİM ---------- */
        BeginDrawing();
        ClearBackground(COLOR_BG);

        if (page == PAGE_HOME) {
            drawHeader(font, "KÜTÜPHANE SİSTEMİ");
            DrawRectangleRounded(searchBar, 0.2, 10, WHITE);
            DrawRectangleLinesEx(searchBar, 2, COLOR_PASTEL_PINK);
            DrawTextEx(font, letterCount ? searchText : "Kitap ara...", (Vector2){55,152}, 20, 1, letterCount ? COLOR_TEXT : LIGHTGRAY);

            DrawRectangleRounded(btnFilter, 0.3, 10, COLOR_PASTEL_PINK);
            DrawTextEx(font, "Filtreler", (Vector2){655, 152}, 18, 1, WHITE);

            DrawRectangleRounded(btnHistory, 0.3, 10, COLOR_PASTEL_PINK);
            DrawTextEx(font, "Geçmiş", (Vector2){795, 152}, 18, 1, WHITE);
            DrawRectangleRounded(btnLoans, 0.3, 10, COLOR_PASTEL_PINK);
            DrawTextEx(font, "Ödünç Alınanlar", (Vector2){935, 152}, 18, 1, WHITE);

            // --- 1. ADIM: KİTAP LİSTESİ (ALT KATMAN) ---
            int visibleIdx = 0;
            for (int i = 0; i < displayCount; i++) {
                
                int originalIndex = -1;
                for(int k=0; k<count; k++) {
                    if(strcmp(library[k].title, displayList[i].title) == 0) {
                        originalIndex = k;
                        break;
                    }
                }

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

            // --- 2. ADIM: FİLTRE MENÜSÜ (ÜST KATMAN) ---
            if (showFilterMenu) {
                // Menü kutusu: Genişlik 140, Yükseklik 135 (3x45)
                DrawRectangle(620, 185, 140, 135, WHITE);
                DrawRectangleLines(620, 185, 140, 135, COLOR_PASTEL_PINK);
                
                // Seçenekler (Font boyutu 18, Yükseklik 45px aralıklı)
                DrawTextEx(font, "Artan Puan", (Vector2){635, 198}, 18, 1, COLOR_TEXT);
                DrawTextEx(font, "Azalan Puan", (Vector2){635, 243}, 18, 1, COLOR_TEXT);
                DrawTextEx(font, "Alfabetik", (Vector2){635, 288}, 18, 1, COLOR_TEXT);
                
                // Ayırıcı Çizgiler (Opsiyonel ama şık durur)
                DrawLine(620, 230, 760, 230, Fade(COLOR_PASTEL_PINK, 0.5f));
                DrawLine(620, 275, 760, 275, Fade(COLOR_PASTEL_PINK, 0.5f));
            }
        } 
        else if (page == PAGE_HISTORY) {
            drawHeader(font, "ARAMA GEÇMİŞİ");
            DrawRectangleRounded(btnBack, 0.3, 10, COLOR_PASTEL_PINK);
            DrawTextEx(font, "Geri", (Vector2){75, 152}, 18, 1, WHITE);
            
            StackNode* t = historyStack;
            for (int i = 0; t && i < 15; i++, t = t->next)
                DrawTextEx(font, t->last_search, (Vector2){100, 200 + i * 40}, 24, 1, COLOR_TEXT);
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

                // Sadece ekrana sığanları çiz
                if (yPos > 100 && yPos < GetScreenHeight()) {
                    
                    DrawRectangleRounded((Rectangle){100, yPos, 1000, 70}, 0.2, 10, Fade(COLOR_PASTEL_PINK, 0.1f));
                    
                    // Kitap Bilgileri
                    DrawTextEx(font, curr->bookTitle, (Vector2){120, yPos + 10}, 20, 1, COLOR_TEXT);
                    DrawTextEx(font, curr->userName, (Vector2){120, yPos + 40}, 16, 1, GRAY);
                    DrawTextEx(font, TextFormat("Verilis: %s", curr->borrowDate), (Vector2){720, yPos + 15}, 16, 1, GRAY);
                    DrawTextEx(font, TextFormat("Son Teslim: %s", curr->dueDate), (Vector2){720, yPos + 40}, 16, 1, COLOR_RED_ALERT);

                    // --- İADE ET BUTONU (Pastel Pembe) ---
                    Rectangle btnReturn = {960, yPos + 20, 120, 30};
                    DrawRectangleRounded(btnReturn, 0.3, 8, COLOR_PASTEL_PINK); 
                    DrawTextEx(font, "Iade Et", (Vector2){985, yPos + 25}, 16, 1, WHITE);

                    // Tıklama Kontrolü
                    if (!actionTaken && CheckCollisionPointRec(mouse, btnReturn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        
                        // 1. Kitabı ana kütüphanede bul ve borrowed işaretini kaldır
                        for(int k = 0; k < count; k++) {
                            if(strcmp(library[k].title, curr->bookTitle) == 0) {
                                borrowed[k] = false;
                                break;
                            }
                        }

                        // 2. Kitabı kuyruktan sil
                        char titleToDel[200];
                        strcpy(titleToDel, curr->bookTitle);
                        remove_specific_loan(loanQueue, titleToDel);
                        
                        actionTaken = true; 
                    }
                }
                
                if (actionTaken) break; 
                curr = curr->next; 
                idx++;
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