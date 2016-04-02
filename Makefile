
SRC_ROOT = "src"

CC = gcc
CFLAGS = -std=gnu11 -pedantic -Werror -Wall -g3
CLIBS = -lncurses
SRCS = $(shell find $(SRC_ROOT) -type f \( -iname "*.c" \))
OBJS = $(SRCS:.c=.o)

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJS)
	$(CC) $^ $(CLIBS) -o pumpkin-mines

clean:
	$(RM) $(shell find . | grep -F .o)
	$(RM) mines
