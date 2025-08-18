CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -lcurl

SRC = dirleaks.c $(wildcard src/*.c)

TARGET = dirleaks

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o
