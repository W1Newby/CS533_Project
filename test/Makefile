CC = gcc
CFLAGS = -Wall -Wextra

SRCS = test.c
OBJS = $(SRCS:.c=.o)
TARGET = test

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
