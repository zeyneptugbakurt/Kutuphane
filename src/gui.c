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
static bool borrowed[200] = { false };

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
    InitWindow(1200, 800, "Kütüphane Yönetim Paneli");
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
        scrollOffset -= GetMouseWheelMove() * 35;
        if (scrollOffset < 0) scrollOffset = 0;

        /* ---------- INPUT VE ARAMA (TRIE KULLANIMI) ---------- */
        if (page == PAGE_HOME) 
        {
            // Klavye girişlerini al
            int key = GetCharPressed();
            while (key > 0) {
                if (letterCount < 126) {
                    searchText[letterCount++] = (char)key;
                    searchText[letterCount] = '\0';
                }
                key = GetCharPressed();
            }

            // Silme işlemi (Backspace)
            if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
                searchText[--letterCount] = '\0';
                if (letterCount == 0) {
                    searchActive = false;
                    memcpy(displayList, library, sizeof(Book) * count);
                    displayCount = count;
                    apply_current_sorting();
                }
            }

            // ARAMA TETİKLEME (Enter tuşu ile Trie üzerinden arama yapar)
            if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                push(&historyStack, searchText);
                displayCount = search_books_by_prefix(library, count, searchText, displayList);
                searchActive = true;
                apply_current_sorting();
                scrollOffset = 0;
            }
        }

        /* ---------- GÖRSEL BUTONLAR VE ETKİLEŞİM ---------- */
        Rectangle searchBar  = {40,140,550,45};
        Rectangle btnFilter  = {620,140,140,45};
        Rectangle btnHistory = {775,140,120,45};
        Rectangle btnLoans   = {910,140,220,45};
        Rectangle btnBack    = {40,140,120,45};

        if (page == PAGE_HOME) {
            // FİLTRE BUTONU MANTIĞI
            if (CheckCollisionPointRec(mouse, btnFilter) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showFilterMenu = !showFilterMenu;
            }
            
            // Eğer Filtre menüsü açıksa butonları kontrol et
            if (showFilterMenu) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    bool clickedOption = false;
                    if (CheckCollisionPointRec(mouse, (Rectangle){620, 190, 200, 30})) {
                        activeFilter = FILTER_SCORE_ASC; clickedOption = true;
                    } 
                    else if (CheckCollisionPointRec(mouse, (Rectangle){620, 225, 200, 30})) {
                        activeFilter = FILTER_SCORE_DESC; clickedOption = true;
                    } 
                    else if (CheckCollisionPointRec(mouse, (Rectangle){620, 260, 200, 30})) {
                        activeFilter = FILTER_ALPHABETIC; clickedOption = true;
                    }

                    if (clickedOption) {
                        apply_current_sorting();
                        showFilterMenu = false; // Seçim yapılınca menüyü kapat
                    } else if (!CheckCollisionPointRec(mouse, btnFilter)) {
                        // Menü dışına ve filtre butonuna basılmadıysa menüyü kapat
                        showFilterMenu = false; 
                    }
                }
            } 
            else {
                // Filtre menüsü kapalıysa diğer butonlara basılabilir
                if (CheckCollisionPointRec(mouse, btnHistory) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    page = PAGE_HISTORY;
                else if (CheckCollisionPointRec(mouse, btnLoans) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    page = PAGE_LOANS;
            }
        } 
        else if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            page = PAGE_HOME;
        }

        /* ---------- ÇİZİM ---------- */
        BeginDrawing();
        ClearBackground(COLOR_BG);

        if (page == PAGE_HOME) {
            drawHeader(font, "KÜTÜPHANE SİSTEMİ");
            DrawRectangleRounded(searchBar, 0.2, 10, WHITE);
            DrawRectangleLinesEx(searchBar, 2, COLOR_PASTEL_PINK);
            DrawTextEx(font, letterCount ? searchText : "Ara (Trie): Enter'a bas...", (Vector2){55,152}, 20, 1, letterCount ? COLOR_TEXT : LIGHTGRAY);

            // Filtreler Butonu
            DrawRectangleRounded(btnFilter, 0.3, 10, COLOR_PASTEL_PINK);
            DrawTextEx(font, "Filtreler", (Vector2){655, 152}, 18, 1, WHITE);

            DrawRectangleRounded(btnHistory, 0.3, 10, COLOR_PASTEL_PINK);
            DrawTextEx(font, "Geçmiş", (Vector2){795, 152}, 18, 1, WHITE);
            DrawRectangleRounded(btnLoans, 0.3, 10, COLOR_PASTEL_PINK);
            DrawTextEx(font, "Ödünç Alınanlar", (Vector2){935, 152}, 18, 1, WHITE);

            // --- 1. ADIM: ÖNCE KİTAP LİSTESİNİ ÇİZ (Altta kalsın) ---
            int visibleIdx = 0;
            for (int i = 0; i < displayCount; i++) {
                if (borrowed[i]) continue;
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
                    // Eğer menü açıksa arkadaki butona tıklanmasını engellemek için basit bir kontrol
                    if (!showFilterMenu) {
                        enqueue_loan(loanQueue, "Zeynep", displayList[i].title);
                        borrowed[i] = true;
                    }
                }
                visibleIdx++;
            }

            // --- 2. ADIM: SONRA FİLTRE MENÜSÜNÜ ÇİZ (Üstte kalsın) ---
            // Bu kod bloğunu kitap döngüsünün altına aldık, böylece menü kitapların üstüne çizilecek.
            if (showFilterMenu) {
                // Menünün altına hafif bir gölge veya arka plan ekleyerek okunabilirliği artırıyoruz
                DrawRectangle(620, 185, 220, 110, WHITE);
                DrawRectangleLines(620, 185, 220, 110, COLOR_PASTEL_PINK);
                DrawTextEx(font, "Puan: Düşük -> Yüksek", (Vector2){630, 195}, 16, 1, COLOR_TEXT);
                DrawTextEx(font, "Puan: Yüksek -> Düşük", (Vector2){630, 230}, 16, 1, COLOR_TEXT);
                DrawTextEx(font, "Alfabetik (A-Z)", (Vector2){630, 265}, 16, 1, COLOR_TEXT);
            }
        } 
        else if (page == PAGE_HISTORY) {
            drawHeader(font, "ARAMA GEÇMİŞİ");
            DrawRectangleRounded(btnBack, 0.3, 10, COLOR_PASTEL_PINK);
            DrawTextEx(font, "Geri", (Vector2){75, 152}, 18, 1, WHITE);
            
            // GEÇMİŞ LİSTESİ
            StackNode* t = historyStack;
            for (int i = 0; t && i < 15; i++, t = t->next)
                DrawTextEx(font, t->last_search, (Vector2){100, 200 + i * 40}, 24, 1, COLOR_TEXT);
        }
        else if (page == PAGE_LOANS) {
            drawHeader(font, "ÖDÜNÇ ALINANLAR");
            DrawRectangleRounded(btnBack, 0.3, 10, COLOR_PASTEL_PINK);
            DrawTextEx(font, "Geri", (Vector2){75, 152}, 18, 1, WHITE);
            
            // ÖDÜNÇ LİSTESİ
            QueueNode* curr = loanQueue->front;
            int idx = 0;
            while (curr) {
                DrawRectangleRounded((Rectangle){100, 200 + idx * 80, 1000, 70}, 0.2, 10, Fade(COLOR_PASTEL_PINK, 0.1f));
                
                // Kitap ve Kullanıcı
                DrawTextEx(font, curr->bookTitle, (Vector2){120, 210 + idx * 80}, 20, 1, COLOR_TEXT);
                DrawTextEx(font, curr->userName, (Vector2){120, 240 + idx * 80}, 16, 1, GRAY);
                
                // Tarihler
                DrawTextEx(font, TextFormat("Verilis: %s", curr->borrowDate), (Vector2){800, 215 + idx * 80}, 16, 1, GRAY);
                DrawTextEx(font, TextFormat("Son Teslim: %s", curr->dueDate), (Vector2){800, 240 + idx * 80}, 16, 1, COLOR_RED_ALERT);
                
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