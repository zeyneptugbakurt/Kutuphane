# --- Ayarlar ---
CC = gcc
TARGET = kutuphane_gui

# Derleme Bayrakları (Include klasörleri)
CFLAGS = -Iinclude -I/opt/homebrew/include

# Linkleme Bayrakları (Kütüphaneler ve Framework'ler)
LDFLAGS = -L/opt/homebrew/lib -lraylib \
          -framework IOKit -framework Cocoa -framework OpenGL

# Kaynak Dosyalar (Hepsini buraya ekledik)
SOURCES = src/main.c \
          src/gui.c \
          src/io.c \
          src/cJSON.c \
          src/stack.c \
          src/sort.c \
          src/search.c \
          src/trie.c \
          src/avl.c \
          src/queue.c \
          src/graph.c

# --- Kurallar ---

# Varsayılan hedef: Sadece 'make' yazınca çalışır
all: $(TARGET)

# Derleme işlemi
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

# Hem derle hem çalıştır: 'make run' yazınca çalışır
run: all
	./$(TARGET)

# Temizlik: 'make clean' yazınca oluşan dosyayı siler
clean:
	rm -f $(TARGET)