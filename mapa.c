#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "estado.h"
#include "mapa.h"

//Mudar o tipo das funções para int para aproveitar códigos de erro ou deixar void (?)

//TAMANHO DO MAPA A DEFINIR

void iniciarMapa(CASA **mapa, int yMAX, int xMAX){
    srand(time(NULL));
    //prencher o 'interior' com VAZIO ou MURO, 40% chance de ser MURO
    for(int i = 1; i < yMAX - 1; i++){
        for(int j = 1; j < xMAX -1; j++){
            if(rand() % 100 < 43){
                mapa[i][j].obs = MURO;
                mapa[i][j].atravessavel = 0;
            }
            else{
                mapa[i][j].obs = VAZIO;
                mapa[i][j].atravessavel = 1;
            }
        }
    }

    //Colocar MURO nas bordas do mapa
    for(int i = 0; i < yMAX; i++){
        mapa[i][0].obs = MURO;
        mapa[i][0].atravessavel = 0;
        mapa[i][xMAX - 1].obs = MURO;
        mapa[i][xMAX - 1].atravessavel = 0;
    }
    for(int i = 0; i < xMAX; i++){
        mapa[0][i].obs = MURO;
        mapa[0][i].atravessavel = 0;
        mapa[yMAX - 1][i].obs = MURO;
        mapa[yMAX - 1][i].atravessavel = 0;
    }
}

//função para contar os muros no quadrado 3x3
int contarObstaculo(CASA **mapa, int y, int x, OBSTACULO obst){
    int nObstaculo = 0;

    for(int i = y - 1; i <= y + 1; i++){
        for(int j = x - 1; j <= x + 1; j++){
            if(mapa[i][j].obs == obst)
                nObstaculo++;
        }
    }

    return nObstaculo;
}

//função para processar o mapa, apenas trabalha no 'interior' já que as bordas têm de ser muros (versão só com a verificação >= 5)
int compactaMapa(CASA **mapa, int yMAX, int xMAX, int fase){ // chamar na main ~4 vezes
    CASA mapaAux[yMAX][xMAX];
    int nMuros, nEspacos;

    if(fase == 1){
       for (int i = 1; i < yMAX - 1; i++){
            for (int j = 1; j < xMAX - 1; j++){
                nMuros = contarObstaculo(mapa, i, j, MURO);
                nEspacos = contarObstaculo(mapa, i, j, VAZIO);
                
                if(nMuros >= 5 || nEspacos <= 2){
                    mapaAux[i][j].obs = MURO;
                    mapaAux[i][j].atravessavel = 0;
                }
                else{
                    mapaAux[i][j].obs = VAZIO;
                    mapaAux[i][j].atravessavel = 1;
                }
            }
        }
    }
    else{
        for (int i = 1; i < yMAX - 1; i++){
            for (int j = 1; j < xMAX - 1; j++){
                nMuros = contarObstaculo(mapa, i, j, MURO);
                
                if(nMuros >= 5){
                    mapaAux[i][j].obs = MURO;
                    mapaAux[i][j].atravessavel = 0;
                }
                else{
                    mapaAux[i][j].obs = VAZIO;
                    mapaAux[i][j].atravessavel = 1;
                }
            }
        }
    }

    for(int i = 1; i < yMAX - 1; i++){
        for(int j = 1; j < xMAX - 1; j++){
            mapa[i][j] = mapaAux[i][j];
        }
    }

    return 0;
}

/*void verificaAcesso(Casa **mapa, int yMAX, int xMAX){
    if()
}*/

//função para adicionar os mobs e outras cousas ao mapa

//função para escrever o mapa
void escreveMapa(CASA **mapa, int yMAX , int xMAX){
    attron(COLOR_PAIR(COLOR_YELLOW));
    for(int i = 0; i < yMAX; i++){
        for(int j = 0; j < xMAX; j++){
            if(mapa[i][j].obs == MURO) 
                mvaddch(i, j, '#');
            else
                mvaddch(i, j, ' ');
        }
    }
}

//Arranjar para a primeira linha do muro ser visivel (?)
int calcularVisivel(CASA **mapa, JOGADOR jogador, int yMAX, int xMAX){
    for (int i = 0; i < yMAX; i++){
        for (int j = 0;j < xMAX; j++){
            int distancia = sqrt(((jogador->posX - j)^2) + ((jogador->posY - i)^2));

            if(mapa[i][j].obs == VAZIO && distancia < 5){ //5 foi posto atoa, valor a verificar
                mapa[i][j].visivel = 1;
            }
            else
                mapa[i][j].visivel = 0;
        }
    }

    return 0;
}

void escreveVisivel (CASA **mapa, int yMAX, int xMAX){
    attron(COLOR_PAIR(COLOR_BLACK));
    for(int i = 0; i < yMAX; i++){
        for(int j = 0; j < xMAX; j++){
            if(mapa[i][j].visivel == 0) 
                mvaddch(i, j, '#');
        }
    }
}