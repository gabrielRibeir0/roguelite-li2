#include "estado.h"

#ifndef JOGADOR_H
#define JOGADOR_H

void fazJogador(CASA **mapa, MONSTRO *listaMonstros, JOGADOR jogador, int yMAX, int xMAX, int nMonstros);

void moverJogador(JOGADOR jogador, int dx, int dy, CASA destino);

void escreveJogador(JOGADOR jogador);

void danoTrap(CASA **mapa, JOGADOR jogador, int yMAX);

void abreBau(CASA **mapa, JOGADOR jogador, int yMAX);

//void initMapaProximidade(CASA **mapa, int **listaProximidade, int passos, int i, int j, int yMax , int xMax);

#endif