CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = sistema

all:
	$(CC) $(CFLAGS) -o $(TARGET) main.c core.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.bin *.csv *.html
