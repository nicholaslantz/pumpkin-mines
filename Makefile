
SRC_ROOT = "src"

CC = gcc
CFLAGS = -std=gnu11 -pedantic -Werror -Wall
CLIBS = -lncurses
SRCS = $(shell find $(SRC_ROOT) -type f \( -iname "*.c" \))
OBJS = $(SRCS:.c=.o)
NAME = pumpkin-mines

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJS)
	$(CC) $^ $(CLIBS) -o $(NAME)

install: all
	cp -p $(NAME) ~/bin/$(NAME)

clean:
	$(RM) $(shell find . | grep -F .o)
	$(RM) $(NAME)
