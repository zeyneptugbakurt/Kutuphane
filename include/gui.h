#ifndef GUI_H
#define GUI_H

#include "book.h"

// Pencereyi başlatan ana fonksiyon
void start_gui(Book *library, int count);

// Bunlar daha sonra ihtiyaç duyacağın yardımcı fonksiyonlar olabilir
void draw_interface();
void apply_filter(Book* library, int count, int filter_type);

#endif