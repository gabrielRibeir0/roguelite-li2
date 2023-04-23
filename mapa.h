#include "estado.h"

#ifndef MAPA_H
#define MAPA_H

void iniciarMapa(CASA **mapa, int yMAX, int xMAX);

int contarMuros(CASA **mapa, int y, int x);

int compactaMapa(CASA **mapa, int yMAX, int xMAX);

void escreveMapa(CASA **mapa, int yMAX , int xMAX);

int calcularVisivel(CASA **mapa, JOGADOR jogador, int yMAX, int xMAX);

#endif