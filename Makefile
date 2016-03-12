
SRC_ROOT = "src"

CC = clang
CFLAGS = -std=gnu11 -pedantic -Werror -Wall -g3
SRCS = $(shell find $(SRC_ROOT) | grep -F .c)
OBJS = $(SRCS:.c=.o)

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJS)
	$(CC) $^ -o mines

clean:
	$(RM) $(shell find . | grep -F .o)
	$(RM) mines
