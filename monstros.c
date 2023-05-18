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

void moveMonstros(CASA **mapa, MONSTRO *listaMonstros, int **listaProximidade, JOGADOR jogador, int yMAX , int xMAX, int nMonstros){
    int casasLado[4];
    for(int i = 0; i < nMonstros; i++){
        int dist = sqrt(((listaMonstros[i].posX - jogador->posX)*(listaMonstros[i].posX - jogador->posX)) + ((listaMonstros[i].posY - jogador->posY)*(listaMonstros[i].posY - jogador->posY)));

        if(dist < 5){
            casasLado[0] = listaProximidade[listaMonstros[i].posY + 1][listaMonstros[i].posX];
            casasLado[1] = listaProximidade[listaMonstros[i].posY - 1][listaMonstros[i].posX];
            casasLado[2] = listaProximidade[listaMonstros[i].posY][listaMonstros[i].posX + 1];
            casasLado[3] = listaProximidade[listaMonstros[i].posY][listaMonstros[i].posX - 1];
        }
    }

}

void modoCombate(CASA **mapa, JOGADOR jogador,MONSTRO *listaMonstros){
    for(int i = 0; i < nMonstros; i++){                                   
        int dist = sqrt(((listaMonstros[i].posX - jogador->posX)*(listaMonstros[i].posX - jogador->posX)) + ((listaMonstros[i].posY - jogador->posY)*(listaMonstros[i].posY - jogador->posY)));

        while(dist < 2){
            nodelay(stdscr, false);
            jogador->combate = 1;
            if(jogador->vida <= 0) break;
            if(monstro[i]->vida <= 0) break;
             
            switch(input){
                case "z":
                    int x = rand () % 100;
                    if (x <= 90) monstro[i]->vida -= 10;
                default:
                    break;
            
            }

            int y = rand () % 100;
            if (y <= 85) jogador->vida -= 5;

            dist = sqrt(((listaMonstros[i].posX - jogador->posX)*(listaMonstros[i].posX - jogador->posX)) + ((listaMonstros[i].posY - jogador->posY)*(listaMonstros[i].posY - jogador->posY)));
        }

        if(jogador->vida <= 0) break;
        if(monstro->vida <= 0) break;

    }
    jogador->combate = 0;
    
    nodelay(stdscr, true);
}
    
