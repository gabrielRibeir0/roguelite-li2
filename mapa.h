#include "estado.h"

#ifndef MAPA_H
#define MAPA_H

void iniciarMapa(CASA mapa[yMAX][xMAX]);

int contarMuros(CASA mapa[yMAX][xMAX], int y, int x);

int compactaMapa(CASA mapa[yMAX][xMAX]);

void escreveMapa(CASA mapa[yMAX][xMAX]);

int calcularVisivel(CASA mapa[yMAX][xMAX], JOGADOR jogador);

#endif