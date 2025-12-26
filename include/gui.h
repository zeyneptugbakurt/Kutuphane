#ifndef GUI_H
#define GUI_H

#include "book.h"

// Menüyü ve arama çubuğunu ekrana basar
void draw_interface();

// Kullanıcının seçtiği filtreye göre listeyi günceller
void apply_filter(Book* library, int count, int filter_type);

// Arama sonuçlarını anlık olarak gösterir
void display_search_results(Book* results, int count);

#endif