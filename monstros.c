#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include "estado.h"
#include "monstros.h"

int monstrosPerto(MONSTRO *listaMonstros, int y, int x, int nMonstros){
    //ajustar raio de procura de monstro
    for(int i = 0; i < nMonstros; i++){
        int dist = sqrt(((listaMonstros[i].posX - x)*(listaMonstros[i].posX - x)) + ((listaMonstros[i].posY - y)*(listaMonstros[i].posY - y)));

        if(dist < 5)
            return 1;
    }

    return 0;
}

int iniciaMonstros(CASA **mapa, MONSTRO **listaMonstros, int nivel, int yMAX, int xMAX){
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

    *listaMonstros = malloc(sizeof(struct monstro) * nMonstros);

    for(int i = 0; i < nMonstros; i++){
        do{
            yRand = rand() % yMAX;
            xRand = rand() % xMAX;
        }while(mapa[yRand][xRand].obs != VAZIO || mapa[yRand][xRand].acessivel == 0 || monstrosPerto(*listaMonstros, yRand, xRand, i));
        mapa[yRand][xRand].obs = MONST;
        (*listaMonstros)[i].posX = xRand;
        (*listaMonstros)[i].posY = yRand;
        //inicializar stats do monstros
    }

    return nMonstros;
}

/*void moveMonstros(CASA **mapa, JOGADOR jogador, int yMAX , int xMAX){
    
    for(int i = 0; i < yMAX ; i++){
        for (int j = 0; j < xMAX ; j++){    
            int dist = sqrt(((jogador->posX - j)*(jogador->posX - j)) + ((jogador->posY - i)*(jogador->posY - i)));
            
            if(mapa[i][j].temMonstro && dist<=5 && jogador->posX>j && mapa[i][j+1].obs == VAZIO){ //mexe pra direita
                //CASA aux=mapa[i][j];
                mapa[i][j].visivel = 1;
                mapa[i][j].temMonstro = 0;
                mapa[i][j+1].temMonstro = 1;
                mapa[i][j+1].acessivel = 0;
                //sleep (1);
            }
            else if(mapa[i][j].temMonstro && dist<=5 && jogador->posX<j && mapa[i][j-1].obs == VAZIO){ // mexe pra esquerda
                //CASA aux=mapa[i][j];
                mapa[i][j].visivel = 1;
                mapa[i][j].temMonstro = 0;
                mapa[i][j-1].temMonstro = 1;
                mapa[i][j-1].acessivel = 0;
                //sleep (1);            
            }
            else if(mapa[i][j].temMonstro && dist<=5 && jogador->posY>i && mapa[i+1][j].obs == VAZIO){ //
               // CASA aux=mapa[i][j];    
                mapa[i][j].visivel = 1;
                mapa[i][j].temMonstro = 0;
                mapa[i+1][j].temMonstro = 1;
                mapa[i+1][j].acessivel = 0;
                //sleep (1);
            }
            else if(mapa[i][j].temMonstro && dist<=5 && jogador->posY<i && mapa[i-1][j].obs == VAZIO){
               // CASA aux=mapa[i][j];
                mapa[i][j].visivel = 1;
                mapa[i][j].temMonstro = 0;
                mapa[i-1][j].temMonstro = 1;
                mapa[i-1][j].acessivel = 0;
                //sleep(1);
            }
        }
    }   
}*/

// void ataquedeMonstro(CASA **mapa, JOGADOR jogador, )