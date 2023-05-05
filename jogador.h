#include "estado.h"

#ifndef JOGADOR_H
#define JOGADOR_H

int fazJogador(CASA **mapa, JOGADOR jogador, int yMAX, int xMAX);

void moverJogador(JOGADOR jogador, int dx, int dy, CASA destino);

void escreveJogador(JOGADOR jogador);

void abreBau(CASA **mapa, JOGADOR jogador, int yMAX);

#endif