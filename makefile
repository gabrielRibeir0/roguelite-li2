CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99
LDFLAGS = -lncurses -lm

SRCS = main.c jogador.c mapa.c
OBJS = $(SRCS:.c=.o)
DEPS = estado.h jogador.h mapa.h

.PHONY: all clean

all: jogo

jogo: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f jogo $(OBJS)
