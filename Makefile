CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = memory_manager
SOURCES = main.c mymemory.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

.PHONY: clean run test