CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2
LIBS= -lcurses -lm

SRCS = main.c jogador.c mapa.c monstros.c combate.c
OBJS = main.o jogador.o mapa.o monstros.o combate.o
DEPS = estado.h jogador.h mapa.h monstros.h combate.h

jogo: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f jogo $(OBJS)