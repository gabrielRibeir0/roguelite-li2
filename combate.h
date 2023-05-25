#include "estado.h"

#ifndef COMBATE_H
#define COMBATE_H

int modoCombate(JOGADOR jogador, MONSTRO *monstro, int yMAX);

void verificaCombate(JOGADOR jogador, MONSTRO *listaMonstros, int *nMonstros, int yMAX, double *delayFugir);

#endif