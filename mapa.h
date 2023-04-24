#include "estado.h"

#ifndef MAPA_H
#define MAPA_H

void iniciarMapa(CASA **mapa, int yMAX, int xMAX);

int contarObstaculo(CASA **mapa, int y, int x, OBSTACULO obst);

int compactaMapa(CASA **mapa, int yMAX, int xMAX, int fase);

void verificaAcesso(CASA **mapa, int y, int x);

void escreveMapa(CASA **mapa, int yMAX , int xMAX);

int calcularVisivel(CASA **mapa, JOGADOR jogador, int yMAX, int xMAX);

void escreveVisivel (CASA **mapa, int yMAX, int xMAX);

#endif