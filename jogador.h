#include "estado.h"

#ifndef JOGADOR_H
#define JOGADOR_H

void iniciaJogador(JOGADOR jogador);

void posicaoJogador(CASA **mapa, MONSTRO *listaMonstros, JOGADOR jogador, int yMAX, int xMAX, int nMonstros);

void moverJogador(JOGADOR jogador, int dx, int dy, CASA destino);

void escreveJogador(JOGADOR jogador);

void danoTrap(CASA **mapa, JOGADOR jogador, int yMAX);

void danoLava(CASA** mapa,JOGADOR jogador,int yMAX, double *ultimoTempo);

void abreBau(CASA **mapa, JOGADOR jogador, int yMAX);

#endif