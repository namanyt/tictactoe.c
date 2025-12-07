CC = gcc
CFLAGS = -Wall -Wextra -std=c2x
SRCS = main.c game.c ai.c utils.c ui.c
OBJS = $(SRCS:.c=.o)
TARGET = tictactoe

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) $(OBJS)
