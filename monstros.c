#include <stdlib.h>
#include "estado.h"
#include "monstros.h"

int monstrosPerto(CASA **mapa, int y, int x, int yMAX, int xMAX){
    for(int i = y-5 > 0 ? y-5 : 0; i <= y+5 < yMAX ? y+5 : yMAX; i++){
        for(int j = x-5 > 0 ? x-5 : 0; j <= x+5 < xMAX ? x+5 : xMAX; j++){
            if(mapa[i][j].temMonstro == 1)
                return 1;
        }
    }

    return 0;
}

void iniciaMonstros(CASA **mapa, int nivel, int nrows, int ncols){
    int nMonstros, yRand, xRand;
    switch(nivel){
        case 1:
            nMonstros = rand() % 3 + 5;
            break;
        case 2:
            nMonstros = rand() % 4 + 8;
            break;
        case 3:
            nMonstros = rand() % 3 + 12;
            break;
        default:
            break;
    }

    for(int i = 0; i <= nMonstros; i++){
        do{
            yRand = rand() % nrows;
            xRand = rand() % ncols;
        }while(mapa[yRand][xRand].obs != VAZIO || monstrosPerto(mapa, yRand, xRand, nrows, ncols));

        mapa[yRand][xRand].temMonstro = 1;
        mapa[yRand][xRand].monstro = malloc(sizeof(struct monst));
        //inicializar stats do monstros
    }
}