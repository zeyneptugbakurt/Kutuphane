#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raylib.h"

#include "../include/book.h"
#include "../include/gui.h"
#include "../include/stack.h"  // Stack işlemleri (Geçmiş, Undo)
#include "../include/sort.h"   // Sıralama algoritmaları
#include "../include/search.h" // Arama algoritmaları (Trie)
#include "../include/queue.h"  // Kuyruk işlemleri (Ödünç alma)

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
   GLOBAL STATE (Veri Yapıları)
-------------------------------------------------- */
static StackNode* historyStack = NULL; // Arama Geçmişi (LIFO)
static StackNode* undoStack = NULL;    // Geri Alma İşlemleri (LIFO)
static Queue* loanQueue = NULL;        // Ödünç Kuyruğu (FIFO)

// Kitapların ödünç alınıp alınmadığını tutan basit dizi
static bool borrowed[200] = { false }; 

// Ekranda o an gösterilecek kitapların listesi (Filtrelenmiş/Sıralanmış)
static Book displayList[200]; 
static int displayCount = 0;

static int scrollOffset = 0;
static FilterType activeFilter = FILTER_NONE;

// Yardımcı Fonksiyon: Başlık Çizimi
static void drawHeader(Font font, const char* title) {
    DrawRectangle(0, 0, GetScreenWidth(), 100, COLOR_PASTEL_PINK);
    DrawTextEx(font, title, (Vector2){40, 28}, 42, 1, WHITE);
}

// Yardımcı Fonksiyon: Seçili sıralamayı uygula (SORT Modülünü Kullanır)
void apply_current_sorting() {
    if (displayCount <= 0) return;
    
    // Sort.c modülündeki algoritmaları çağırıyoruz
    if (activeFilter == FILTER_SCORE_ASC) quickSort(displayList, 0, displayCount - 1);
    else if (activeFilter == FILTER_SCORE_DESC) heapSort(displayList, displayCount);
    else if (activeFilter == FILTER_ALPHABETIC) mergeSort(displayList, 0, displayCount - 1);
}

void start_gui(Book *library, int count)
{
    InitWindow(1400, 750, "Kütüphane Yönetim Paneli");
    SetTargetFPS(60);
    
    // Kuyruğu başlat (Queue Modülü)
    loanQueue = createQueue();
    
    // Başlangıçta tüm kitapları göster
    memcpy(displayList, library, sizeof(Book) * count);
    displayCount = count;

    /* ----------------------------------------------------------------
       FONT YÜKLEME (Türkçe Karakter Desteği)
    ---------------------------------------------------------------- */
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
        
        // Scroll İşlemi
        if (page == PAGE_HOME || page == PAGE_LOANS) {
            scrollOffset -= GetMouseWheelMove() * 35;
            if (scrollOffset < 0) scrollOffset = 0;
        } else {
            scrollOffset = 0;
        }

        /* ---------- INPUT İŞLEMLERİ ---------- */
        if (page == PAGE_HOME) 
        {
            // Klavye Girişi (UTF-8 Destekli)
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

            // Silme İşlemi (Backspace)
            if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
                letterCount--;
                while (letterCount > 0 && (searchText[letterCount] & 0xC0) == 0x80) letterCount--;
                searchText[letterCount] = '\0';

                // Yazı tamamen silindiyse listeyi sıfırla (Tüm kitapları geri getir)
                if (letterCount == 0) {
                    memcpy(displayList, library, sizeof(Book) * count);
                    displayCount = count;
                    apply_current_sorting();
                }
            }

            // ARAMA İŞLEMİ (SADECE ENTER TUŞUNA BASINCA)
            if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                // 1. Geçmişe Kaydet (Stack Modülü)
                push(&historyStack, searchText);
                
                // 2. Arama Yap (Search Modülü / Trie)
                displayCount = search_books_by_prefix(library, count, searchText, displayList);
                
                // 3. Varsa Sıralamayı Uygula (Sort Modülü)
                apply_current_sorting();
                
                scrollOffset = 0; // Sayfayı başa sar
            }
        }

        /* ---------- BUTON TANIMLARI ---------- */
        Rectangle searchBar  = {40,140,550,45};
        Rectangle btnFilter  = {620,140,140,45}; 
        Rectangle btnHistory = {775,140,120,45};
        Rectangle btnLoans   = {910,140,220,45};
        Rectangle btnBack    = {40,140,120,45};

        /* ---------- TIKLAMA MANTIKLARI ---------- */
        if (page == PAGE_HOME) {
            // Filtre Menüsü Aç/Kapa
            if (CheckCollisionPointRec(mouse, btnFilter) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showFilterMenu = !showFilterMenu;
            }
            // Filtre Seçimi (Sort Modülünü Tetikler)
            if (showFilterMenu && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                bool clicked = false;
                if (CheckCollisionPointRec(mouse, (Rectangle){620, 185, 140, 45})) { activeFilter=FILTER_SCORE_ASC; clicked=true; }
                else if (CheckCollisionPointRec(mouse, (Rectangle){620, 230, 140, 45})) { activeFilter=FILTER_SCORE_DESC; clicked=true; }
                else if (CheckCollisionPointRec(mouse, (Rectangle){620, 275, 140, 45})) { activeFilter=FILTER_ALPHABETIC; clicked=true; }
                
                if (clicked) { 
                    apply_current_sorting(); 
                    showFilterMenu = false; 
                }
                else if (!CheckCollisionPointRec(mouse, btnFilter)) showFilterMenu = false;
            } 
            // Sayfa Değişimi
            else if (!showFilterMenu) {
                if (CheckCollisionPointRec(mouse, btnHistory) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) page = PAGE_HISTORY;
                else if (CheckCollisionPointRec(mouse, btnLoans) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { page = PAGE_LOANS; scrollOffset = 0; }
            }
        } else if (CheckCollisionPointRec(mouse, btnBack) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            page = PAGE_HOME; scrollOffset = 0;
        }

        /* ---------- ÇİZİM (RENDER) ---------- */
        BeginDrawing();
        ClearBackground(COLOR_BG);

        /* --- SAYFA: ANASAYFA --- */
        if (page == PAGE_HOME) {
            drawHeader(font, "KÜTÜPHANE SİSTEMİ");
            
            // Arama Çubuğu
            DrawRectangleRounded(searchBar, 0.2, 10, WHITE);
            DrawRectangleLinesEx(searchBar, 2, COLOR_PASTEL_PINK);
            DrawTextEx(font, letterCount ? searchText : "Ara (Sonuc icin ENTER basin)...", (Vector2){55,148}, 28, 1, letterCount ? COLOR_TEXT : Fade(LIGHTGRAY, 0.7f));

            // Butonlar
            DrawRectangleRounded(btnFilter, 0.3, 10, COLOR_PASTEL_PINK); DrawTextEx(font, "Filtreler", (Vector2){655, 152}, 18, 1, WHITE);
            DrawRectangleRounded(btnHistory, 0.3, 10, COLOR_PASTEL_PINK); DrawTextEx(font, "Geçmiş", (Vector2){795, 152}, 18, 1, WHITE);
            DrawRectangleRounded(btnLoans, 0.3, 10, COLOR_PASTEL_PINK); DrawTextEx(font, "Ödünç Alınanlar", (Vector2){935, 152}, 18, 1, WHITE);

            // Kitap Listesi Çizimi
            int visibleIdx = 0;
            for (int i = 0; i < displayCount; i++) {
                // Orijinal indexi bul (Ödünç durumu kontrolü için)
                int originalIndex = -1;
                for(int k=0; k<count; k++) { if(displayList[i].id == library[k].id) { originalIndex = k; break; } }
                
                // Eğer kitap ödünç alınmışsa listede gösterme
                if (originalIndex != -1 && borrowed[originalIndex]) continue;

                float yPos = 240 + (visibleIdx * 75) - scrollOffset;
                
                // Ekran dışındakileri çizme (Performans optimizasyonu)
                if (yPos < 180 || yPos > GetScreenHeight()) { visibleIdx++; continue; }

                // Kart Çizimi
                DrawRectangleRounded((Rectangle){40, yPos, 800, 65}, 0.15, 10, Fade(COLOR_PASTEL_PINK, 0.1f));
                DrawTextEx(font, displayList[i].title, (Vector2){60, yPos + 10}, 20, 1, COLOR_TEXT);
                DrawTextEx(font, displayList[i].author, (Vector2){60, yPos + 35}, 15, 1, GRAY);
                DrawTextEx(font, TextFormat("%.1f", displayList[i].score), (Vector2){780, yPos + 22}, 20, 1, COLOR_DARK_PINK);

                // Ödünç Al Butonu
                Rectangle btnBorrow = {860, yPos + 15, 120, 35};
                DrawRectangleRounded(btnBorrow, 0.3, 8, COLOR_PASTEL_PINK);
                DrawTextEx(font, "Ödünç Al", (Vector2){875, yPos + 22}, 15, 1, WHITE);

                // Ödünç Alma Mantığı (Queue ve Stack Modülleri)
                if (CheckCollisionPointRec(mouse, btnBorrow) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (!showFilterMenu) {
                        // 1. Kuyruğa Ekle (Queue Modülü)
                        enqueue_loan(loanQueue, "Zeynep", displayList[i].title);
                        
                        // 2. Undo Stack'e Ekle (Stack Modülü)
                        push(&undoStack, displayList[i].title);

                        if (originalIndex != -1) borrowed[originalIndex] = true;
                    }
                }
                visibleIdx++;
            }

            // Filtre Menüsü Çizimi
            if (showFilterMenu) {
                DrawRectangle(620, 185, 140, 135, WHITE);
                DrawRectangleLines(620, 185, 140, 135, COLOR_PASTEL_PINK);
                DrawTextEx(font, "Artan Puan", (Vector2){635, 198}, 18, 1, COLOR_TEXT);
                DrawTextEx(font, "Azalan Puan", (Vector2){635, 243}, 18, 1, COLOR_TEXT);
                DrawTextEx(font, "Alfabetik", (Vector2){635, 288}, 18, 1, COLOR_TEXT);
            }
        } 
        /* --- SAYFA: GEÇMİŞ --- */
        else if (page == PAGE_HISTORY) {
            drawHeader(font, "ARAMA GEÇMİŞİ");
            DrawRectangleRounded(btnBack, 0.3, 10, COLOR_PASTEL_PINK);
            DrawTextEx(font, "Geri", (Vector2){75, 152}, 18, 1, WHITE);
            
            // "Tümünü Temizle" Butonu
            Rectangle btnClearAll = {1100, 140, 200, 45};
            DrawRectangleRounded(btnClearAll, 0.3, 10, COLOR_RED_ALERT);
            DrawTextEx(font, "Tümünü Temizle", (Vector2){1125, 152}, 18, 1, WHITE);

            // Temizleme Mantığı (Stack Modülü: Sürekli Pop)
            if (CheckCollisionPointRec(mouse, btnClearAll) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                while (historyStack != NULL) {
                    char* deleted = pop(&historyStack); // Stack.c'den çağır
                    if(deleted) free(deleted);
                }
            }

            // Geçmiş Listesi
            StackNode* t = historyStack;
            int i = 0;
            bool itemDeleted = false;
            while (t != NULL && i < 15) {
                float yPos = 220 + i * 50;
                DrawTextEx(font, TextFormat("%d. %s", i + 1, t->last_search), (Vector2){100, yPos}, 24, 1, COLOR_TEXT);

                // Tekil Silme (X) Butonu
                Rectangle btnSingleDel = {500, yPos, 30, 30};
                DrawRectangleRounded(btnSingleDel, 0.2, 5, Fade(COLOR_RED_ALERT, 0.8f));
                DrawTextEx(font, "X", (Vector2){508, yPos + 2}, 20, 1, WHITE);

                // Silme Mantığı (Stack Modülü: Aradan Silme)
                if (!itemDeleted && CheckCollisionPointRec(mouse, btnSingleDel) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    delete_search_node(&historyStack, t->last_search); // Stack.c'den çağır
                    itemDeleted = true;
                }
                if (itemDeleted) break;

                t = t->next;
                i++;
            }
        }
        /* --- SAYFA: ÖDÜNÇ ALINANLAR --- */
        else if (page == PAGE_LOANS) {
            drawHeader(font, "ÖDÜNÇ ALINANLAR");
            DrawRectangleRounded(btnBack, 0.3, 10, COLOR_PASTEL_PINK);
            DrawTextEx(font, "Geri", (Vector2){75, 152}, 18, 1, WHITE);
            
            // Kuyruk Listesi
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

                    // İade Mantığı (Queue Modülü: Aradan/Sıradan Silme)
                    if (!actionTaken && CheckCollisionPointRec(mouse, btnReturn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        for(int k = 0; k < count; k++) { if(strcmp(library[k].title, curr->bookTitle) == 0) { borrowed[k] = false; break; } }
                        
                        char titleToDel[200]; 
                        strcpy(titleToDel, curr->bookTitle);
                        remove_specific_loan(loanQueue, titleToDel); // Queue.c'den çağır
                        
                        actionTaken = true; 
                    }
                }
                if (actionTaken) break; 
                curr = curr->next; idx++;
            }
        }
        EndDrawing();
    }
    
    // Temizlik ve Kapanış
    freeStack(historyStack);
    freeStack(undoStack);
    while (loanQueue->front) dequeue_return(loanQueue);
    free(loanQueue);
    UnloadFont(font);
    CloseWindow();
}