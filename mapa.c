#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "estado.h"
#include "mapa.h"

//Mudar o tipo das funções para int para aproveitar códigos de erro ou deixar void (?)

//função para criar o mapa random com muros à volta
void iniciarMapa(CASA **mapa, int yMAX, int xMAX){
    //prencher o 'interior' com VAZIO ou MURO, 43% chance de ser MURO
    for(int i = 1; i < yMAX - 1; i++){
        for(int j = 1; j < xMAX -1; j++){
            if(rand() % 100 < 43)
                mapa[i][j].obs = MURO;
            else
                mapa[i][j].obs = VAZIO;

            mapa[i][j].acessivel = 0; 
        }
    }

    //Colocar MURO nas bordas do mapa
    for(int i = 0; i < yMAX; i++){
        mapa[i][0].obs = MURO;
        mapa[i][0].acessivel = 0;
        mapa[i][xMAX - 1].obs = MURO;
        mapa[i][xMAX - 1].acessivel = 0;
    }
    for(int i = 0; i < xMAX; i++){
        mapa[0][i].obs = MURO;
        mapa[0][i].acessivel = 0;
        mapa[yMAX - 1][i].obs = MURO;
        mapa[yMAX - 1][i].acessivel = 0;
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

//função para processar o mapa, apenas trabalha no 'interior' já que as bordas têm de ser muros 
int compactaMapa(CASA **mapa, int yMAX, int xMAX, int fase){ // chamar na main ~4 vezes
    CASA mapaAux[yMAX][xMAX];
    int nMuros, nEspacos,nVazias;

    if(fase == 1){
       for (int i = 1; i < yMAX - 1; i++){
            for (int j = 1; j < xMAX - 1; j++){
                nMuros = contarObstaculo(mapa, i, j, MURO);
                nEspacos = contarObstaculo(mapa, i, j, VAZIO);
                
                if(nMuros >= 5 || nEspacos <= 2)
                    mapaAux[i][j].obs = MURO;
                
                else
                    mapaAux[i][j].obs = VAZIO;
                
            }
        }
    }
    else{
        nVazias=0;
        for (int i = 1; i < yMAX - 1; i++){
            for (int j = 1; j < xMAX - 1; j++){
                nMuros = contarObstaculo(mapa, i, j, MURO);
                
                if(nMuros >= 5)
                    mapaAux[i][j].obs = MURO;
                
                else{
                    mapaAux[i][j].obs = VAZIO;
                    nVazias++;
                }
                
            }
        }
    }

    for(int i = 1; i < yMAX - 1; i++){
        for(int j = 1; j < xMAX - 1; j++){
            mapa[i][j] = mapaAux[i][j];
        }
    }

    return nVazias;
}

/*Chamar com um y, x random em que o obstáculo nessa casa é VAZIO
Ver como fazer isto sem recursiva (?)
Ao spawnar o jogador e outras coisas tem de ser em casas com .obs = VAZIO e .acessivel = 1
Verificar se todas as casas vazias estão acessiveis se for para preencher essas casas com muros, ou só evitar gerar coisas nessas casas
*/
void verificaAcesso(CASA **mapa, int y, int x,int *nAcessiveis){
        if(mapa[y][x].obs == MURO || mapa[y][x].acessivel == 1)
			return;
		
		mapa[y][x].acessivel = 1;
        (*nAcessiveis)++;
		
		verificaAcesso(mapa, y + 1, x,nAcessiveis);
		verificaAcesso(mapa, y - 1, x,nAcessiveis);
		verificaAcesso(mapa, y, x + 1,nAcessiveis);
		verificaAcesso(mapa, y, x - 1,nAcessiveis);
}

//função para adicionar os mobs e outras cousas ao mapa

//função para escrever o mapa
void escreveMapa(CASA **mapa, int yMAX , int xMAX){    
    for(int i = 0; i < yMAX; i++){
        for(int j = 0; j < xMAX; j++){
            if(mapa[i][j].acessivel == 1){
                if(mapa[i][j].visivel == 1){
                    attron(COLOR_PAIR(COLOR_WHITE));
                    mvaddch(i, j, '.');
                    attroff(COLOR_PAIR(COLOR_WHITE));    
                }
                else{
                    attron(COLOR_PAIR(COLOR_BLUE));
                    mvaddch(i, j, '.');
                    attroff(COLOR_PAIR(COLOR_BLUE));
                }
            }
            else{
                attron(COLOR_PAIR(COLOR_YELLOW));
                if(mapa[i][j].obs == MURO)
                    mvaddch(i, j, '#');
                else
                    mvaddch(i, j, '.');
                attroff(COLOR_PAIR(COLOR_YELLOW));
            }
        }
    }
}

//Arranjar para a primeira linha do muro ser visivel (?)
int calcularVisivel(CASA **mapa, JOGADOR jogador, int yMAX, int xMAX){
    for (int i = 0; i < yMAX; i++){
        for (int j = 0;j < xMAX; j++){
            int distancia = sqrt(((jogador->posX - j)*(jogador->posX - j)) + ((jogador->posY - i)*(jogador->posY - i)));
            if(mapa[i][j].obs == VAZIO && distancia < 5) //5 foi posto atoa, valor a verificar
                mapa[i][j].visivel = 1;
            else
                mapa[i][j].visivel = 0;
        }
    }

    return 0;
}


