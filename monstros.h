#include "estado.h"

#ifndef MONSTROS_H
#define MONSTROS_H

int monstrosPerto(CASA **mapa, int y, int x, int yMAX, int xMAX);

void iniciaMonstros(CASA **mapa, int nivel, int yMAX, int xMAX);

void moveMonstros(CASA **mapa, JOGADOR jogador, int yMAX , int xMAX);

#endif