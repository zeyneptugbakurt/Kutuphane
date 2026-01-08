# --- Ayarlar ---
CC = gcc
TARGET = kutuphane_gui

# Derleme Bayrakları
CFLAGS = -Iinclude -I/opt/homebrew/include

# Linkleme Bayrakları
LDFLAGS = -L/opt/homebrew/lib -lraylib \
          -framework IOKit -framework Cocoa -framework OpenGL

# Kaynak Dosyalar (GÖRSELDEKİ İSİMLERE GÖRE DÜZELTİLDİ)
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
          src/graph.c \
          src/performance.c \
          src/unit_tests.c

# --- Kurallar ---

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)