#include "estado.h"

#ifndef MONSTROS_H
#define MONSTROS_H

int monstrosPerto(MONSTRO *listaMontros, int y, int x, int nMonstros);

int iniciaMonstros(CASA **mapa, MONSTRO **listaMonstros, int nivel, int yMAX, int xMAX);

//void moveMonstros(CASA **mapa, JOGADOR jogador, int yMAX , int xMAX);

#endif