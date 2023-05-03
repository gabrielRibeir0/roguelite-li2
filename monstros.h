#include "estado.h"

#ifndef MONSTROS_H
#define MONSTROS_H

int monstrosPerto(CASA **mapa, int y, int x, int yMAX, int xMAX);

void iniciaMonstros(CASA **mapa, int nivel, int nrows, int ncols);

#endif