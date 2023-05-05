#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "estado.h"
#include "monstros.h"

int monstrosPerto(CASA **mapa, int y, int x, int yMAX, int xMAX){
    //ajustar raio de procura de monstros
    int yIni = y-5 > 0 ? y-5 : 0;
    int yFim = y+5 < yMAX ? y+5 : yMAX - 1;
    int xIni = x-5 > 0 ? x-5 : 0;
    int xFim = x+5 < xMAX ? x+5 : xMAX - 1;

    for(int i = yIni; i <= yFim; i++){
        for(int j = xIni; j <= xFim; j++){
            if(mapa[i][j].temMonstro == 1)
                return 1;
        }
    }

    return 0;
}

void iniciaMonstros(CASA **mapa, int nivel, int yMAX, int xMAX){
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
            yRand = rand() % yMAX;
            xRand = rand() % xMAX;
        }while(mapa[yRand][xRand].obs != VAZIO || mapa[yRand][xRand].acessivel == 0 || monstrosPerto(mapa, yRand, xRand, yMAX, xMAX));

        mapa[yRand][xRand].acessivel = 0;
        mapa[yRand][xRand].temMonstro = 1;
        mapa[yRand][xRand].monstro = malloc(sizeof(struct monst));
        //inicializar stats do monstros
    }
}

void moveMonstros(CASA **mapa, JOGADOR jogador, int yMAX , int xMAX){
    
    for(int i = 0; i < yMAX ; i++){
        for (int j = 0; j < xMAX ; j++){    
            int dist = sqrt(((jogador->posX - j)*(jogador->posX - j)) + ((jogador->posY - i)*(jogador->posY - i)));
            
            if(mapa[i][j].temMonstro && dist<=5 && jogador->posX>j && mapa[i][j+1].acessivel){
                CASA aux=mapa[i][j];
                mapa[i][j]=mapa[i][j+1];
                mapa[i][j+1]=aux;
                //sleep (1);
            }
            else if(mapa[i][j].temMonstro && dist<=5 && jogador->posX<j && mapa[i][j-1].acessivel){
                CASA aux=mapa[i][j];
                mapa[i][j]=mapa[i][j-1];
                mapa[i][j-1]=aux;
                //sleep (1);            
            }
            else if(mapa[i][j].temMonstro && dist<=5 && jogador->posY>i && mapa[i+1][j].acessivel){
                CASA aux=mapa[i][j];
                mapa[i][j]=mapa[i+1][j];
                mapa[i+1][j]=aux;
                //sleep (1);
            }
            else if(mapa[i][j].temMonstro && dist<=5 && jogador->posY<i && mapa[i-1][j].acessivel){
                CASA aux=mapa[i][j];
                mapa[i][j]=mapa[i-1][j];
                mapa[i-1][j]=aux;
                //sleep(1);
            }
        }
    }
}