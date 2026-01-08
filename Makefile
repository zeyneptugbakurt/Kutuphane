CC = gcc

TARGET = kutuphane_gui

CFLAGS = -Iinclude -I/opt/homebrew/include -Wall -O2

LDFLAGS = -L/opt/homebrew/lib -lraylib \
          -framework OpenGL -framework Cocoa -framework IOKit

SOURCES = \
    src/main.c \
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
    src/performance.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
