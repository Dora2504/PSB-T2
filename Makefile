CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = memory_manager
SOURCES = main.c mymemory.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run