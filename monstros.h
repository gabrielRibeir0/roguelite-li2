#include "estado.h"

#ifndef MONSTROS_H
#define MONSTROS_H

int monstrosPerto(MONSTRO *listaMontros, int y, int x, int nMonstros);

int iniciaMonstros(CASA **mapa, MONSTRO **listaMonstros, int nivel, int yMAX, int xMAX);

void moveMonstros(CASA **mapa, MONSTRO *listaMonstros, JOGADOR jogador, int nMonstros, double *ultimoTempo);

int visaoMonstro(CASA **mapa, int xMonstro, int yMonstro, int xJogador, int yJogador, int *newPosX, int *newPosY);

#endif