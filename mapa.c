#include <stdlib.h>
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

//função para adicionar os objetos do mapa
int gerarObjetos(CASA **mapa, int yMAX, int xMAX){
    //traps, por enquanto tenta-se gerar só 10 traps, depois coloca-se uma percentagem
    int nTrapsGeradas = 0;
    int y, x;
    
    while(nTrapsGeradas < 10){
        y = rand() % (yMAX - 2) + 1; // evitar gerar em bordas
        x = rand() % (xMAX - 2) + 1;

        if(mapa[y][x].obs == VAZIO && mapa[y][x].acessivel == 1){
            mapa[y][x].obs = TRAP;
            nTrapsGeradas++;
        }
    }
    return 0;
}

//função para escrever o mapa
void escreveMapa(CASA **mapa, int yMAX , int xMAX){    
    for(int i = 0; i < yMAX; i++){
        for(int j = 0; j < xMAX; j++){
            if(mapa[i][j].acessivel == 1){
                 if(mapa[i][j].obs == TRAP){
                    attron(COLOR_PAIR(COLOR_BLACK));
                    mvaddch(i, j, 'T');
                    attroff(COLOR_PAIR(COLOR_BLACK));
                }
                else if(mapa[i][j].visivel == 1){
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
                if(mapa[i][j].obs == MURO){
                    attron(COLOR_PAIR(COLOR_YELLOW));
                    mvaddch(i, j, '#');
                    attroff(COLOR_PAIR(COLOR_YELLOW));                    
                }
                
                else{
                   attron(COLOR_PAIR(COLOR_YELLOW));
                    mvaddch(i, j, '.');
                    attroff(COLOR_PAIR(COLOR_YELLOW)); 
                }
            }
            
        }
    }
}

void linhaVisao(CASA **mapa, int xAtual, int yAtual, int xDestino, int yDestino){
    int dx = abs(xDestino - xAtual);
    int dy = -abs(yDestino  - yAtual);
    int sinalX = xAtual < xDestino ? 1 : -1;
    int sinalY = yAtual < yDestino ? 1 : -1;
    int erro = dx + dy; //erro em xy
    int erroAux;

    while(1){
        //se encontrar um muro para
        if(mapa[yAtual][xAtual].obs == MURO)
            break;
        
        /*se chegarmos ao destino, marca a casa como visível e para, 
        não é preciso ir marcando a linha em si, pois a função já é chamada para os pontos da linha, logo já estão marcados*/
        if(xAtual == xDestino && yAtual == yDestino){
            mapa[yAtual][xAtual].visivel = 1;
            break;
        }
            
        erroAux = 2 * erro; 

        if (erroAux >= dy){ //se err_xy + err_x > 0 -> mover na dir X e atualizar o erro
            erro += dy;
            xAtual += sinalX;
        }
        if (erroAux <= dx){ //se err_xy + err_y < 0 -> mover na dir Y e atualizar o erro
            erro += dx; 
            yAtual += sinalY;
        }
    }
}

//Arranjar para a primeira linha do muro ser visivel (?)
int calcularVisivel(CASA **mapa, JOGADOR jogador, int yMAX, int xMAX){
    for (int i = 0; i < yMAX; i++){
        for (int j = 0;j < xMAX; j++){
            int distancia = sqrt(((jogador->posX - j)*(jogador->posX - j)) + ((jogador->posY - i)*(jogador->posY - i)));
            if(distancia < 9){ 
                mapa[i][j].visivel = 0;
                linhaVisao(mapa, jogador->posX, jogador->posY, j, i);
            }
            else
                mapa[i][j].visivel = 0;
        }
    }

    return 0;
}


