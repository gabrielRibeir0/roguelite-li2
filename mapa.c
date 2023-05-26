#include <stdlib.h>
#include <math.h>
#include <ncurses.h>
#include "estado.h"
#include "mapa.h"


/*
a104171 - Gabriel Pereira Ribeiro
a104532 - Tomás Sousa Barbosa
Função para criar um mapa com obstáculos aleatórios (VAZIO ou MURO)
*/
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
        mapa[i][0].obs = mapa[i][xMAX - 1].obs = MURO;
        mapa[i][0].acessivel = mapa[i][xMAX - 1].acessivel = 0;
    }
    for(int i = 0; i < xMAX; i++){
        mapa[0][i].obs = mapa[yMAX - 1][i].obs = MURO;
        mapa[0][i].acessivel = mapa[yMAX - 1][i].acessivel = 0;
    }
}

/*
a104171 - Gabriel Pereira Ribeiro
Função para contar o número de um tipo de obstáculo num quadrado 3x3
*/
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

/*
a104171 - Gabriel Pereira Ribeiro
Função para processar o mapa, compactá-lo e torná-lo jogável
*/
//função para processar o mapa, apenas trabalha no 'interior' já que as bordas têm de ser muros 
int compactaMapa(CASA **mapa, int yMAX, int xMAX, int fase){
    CASA mapaAux[yMAX][xMAX];
    int nMuros, nEspacos, nVazias = 0;

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

/*
a104171 - Gabriel Pereira Ribeiro
Função para marcar as casas vazias acessíveis como acessíveis
*/
void verificaAcesso(CASA **mapa, int y, int x, int *nAcessiveis){
    if(mapa[y][x].obs != VAZIO || mapa[y][x].acessivel == 1)
        return;
    
    mapa[y][x].acessivel = 1;
    (*nAcessiveis)++;
    
    verificaAcesso(mapa, y + 1, x, nAcessiveis);
    verificaAcesso(mapa, y - 1, x, nAcessiveis);
    verificaAcesso(mapa, y, x + 1, nAcessiveis);
    verificaAcesso(mapa, y, x - 1, nAcessiveis);
}

/*
a103993 - Júlia Costa
a104532 - Tomás Sousa Barbosa
Função para adicionar os objetos do mapa
*/
int gerarObjetos(CASA **mapa, int yMAX, int xMAX){

    int nTrapsGeradas = 0;
    int yt, xt;
    
    while(nTrapsGeradas < 10){
        yt = rand() % (yMAX - 2) + 1; // evitar gerar em bordas
        xt = rand() % (xMAX - 2) + 1;

        if(mapa[yt][xt].obs == VAZIO && mapa[yt][xt].acessivel == 1){
            mapa[yt][xt].obs = TRAP;
            nTrapsGeradas++;
        }
    }

    int nTochasGeradas = 0;
    int yto, xto;

    while(nTochasGeradas<5){
        yto = rand() % (yMAX - 2) + 1;
        xto = rand() % (xMAX - 2) + 1;

        if(mapa[yto][xto].obs==VAZIO && mapa[yto][xto].acessivel == 1){
            mapa[yto][xto].obs = TOCHA;
            mapa[yto][xto].acessivel = 0;
            mapa[yto][xto].visivel = 1;
            nTochasGeradas++;
        }
    }

    int nBausGerados = 0;
    int yb,xb;

    while(nBausGerados < 6){
        yb = rand() % (yMAX - 2) + 1; // evitar gerar em bordas
        xb = rand() % (xMAX - 2) + 1;

        if(mapa[yb][xb].obs == VAZIO && mapa[yb][xb].acessivel == 1){
            mapa[yb][xb].obs = BAU;
            mapa[yb][xb].acessivel = 0;
            nBausGerados++;
        }
    }

    int nLavaGerada1 = 0;
    int yl1, xl1;                      

    while (nLavaGerada1 < 8) {
        yl1 = rand() % (yMAX - 2) + 1; // evitar gerar em bordas
        xl1 = rand() % (xMAX - 2) + 1;

        if (mapa[yl1][xl1].obs == VAZIO && mapa[yl1][xl1].acessivel == 1) {
             if (mapa[yl1 - 1][xl1].obs != BAU && mapa[yl1 - 1][xl1].obs != TRAP && mapa[yl1 - 1][xl1].obs != TOCHA && mapa[yl1-1][xl1].acessivel ==1 &&
                mapa[yl1 + 1][xl1].obs != BAU && mapa[yl1 + 1][xl1].obs != TRAP && mapa[yl1 + 1][xl1].obs != TOCHA && mapa[yl1+1][xl1].acessivel ==1 &&
                mapa[yl1][xl1 - 1].obs != BAU && mapa[yl1][xl1 - 1].obs != TRAP && mapa[yl1][xl1 - 1].obs != TOCHA && mapa[yl1][xl1-1].acessivel ==1 &&
                mapa[yl1][xl1 + 1].obs != BAU && mapa[yl1][xl1 + 1].obs != TRAP && mapa[yl1][xl1 + 1].obs != TOCHA && mapa[yl1][xl1+1].acessivel ==1 &&
                mapa[yl1 + 1][xl1 + 1].obs != BAU && mapa[yl1 + 1][xl1 + 1].obs != TRAP && mapa[yl1 + 1][xl1 + 1].obs != TOCHA && mapa[yl1+1][xl1+1].acessivel ==1 &&
                mapa[yl1 - 1][xl1 + 1].obs != BAU && mapa[yl1 - 1][xl1 + 1].obs != TRAP && mapa[yl1 -1][xl1 + 1].obs != TOCHA && mapa[yl1-1][xl1+1].acessivel ==1 &&
                mapa[yl1 + 1][xl1 - 1].obs != BAU && mapa[yl1 + 1][xl1 - 1].obs != TRAP && mapa[yl1 + 1][xl1 - 1].obs != TOCHA && mapa[yl1+1][xl1-1].acessivel ==1 &&
                mapa[yl1- 1][xl1 - 1].obs != BAU && mapa[yl1 - 1][xl1 - 1].obs != TRAP && mapa[yl1 - 1][xl1 - 1].obs != TOCHA && mapa[yl1-1][xl1-1].acessivel ==1 && 
                mapa[yl1][xl1 + 2].obs != BAU && mapa[yl1][xl1 + 2].obs != TRAP && mapa[yl1][xl1+2].obs != TOCHA && mapa[yl1][xl1+2].acessivel ==1 &&
                mapa[yl1][xl1 - 2].obs != BAU && mapa[yl1][xl1 - 2].obs != TRAP && mapa[yl1][xl1 - 2].obs != TOCHA && mapa[yl1][xl1-2].acessivel ==1 && 
                mapa[yl1 + 2][xl1].obs != BAU && mapa[yl1 + 2][xl1].obs != TRAP && mapa[yl1 + 2][xl1].obs != TOCHA && mapa[yl1 + 2][xl1].acessivel ==1 && 
                mapa[yl1 - 2][xl1].obs != BAU && mapa[yl1 - 2][xl1].obs != TRAP && mapa[yl1 - 2][xl1].obs != TOCHA && mapa[yl1- 2][xl1].acessivel ==1){
            
            
                mapa[yl1][xl1].obs = LAVA;
                mapa[yl1-1][xl1].obs = LAVA;
                mapa[yl1+1][xl1].obs = LAVA;
                mapa[yl1][xl1-1].obs = LAVA;
                mapa[yl1][xl1+1].obs = LAVA;
                mapa[yl1+1][xl1+1].obs = LAVA;
                mapa[yl1-1][xl1+1].obs = LAVA;
                mapa[yl1+1][xl1-1].obs = LAVA;
                mapa[yl1-1][xl1-1].obs = LAVA;
                mapa[yl1][xl1+2].obs = LAVA;
                mapa[yl1][xl1-2].obs = LAVA;
                mapa[yl1+2][xl1].obs = LAVA;
                mapa[yl1-2][xl1].obs = LAVA;

            
            nLavaGerada1 ++;
                }
        }
    }
    int nLavaGerada2 = 0;
    int yl2, xl2;

    while (nLavaGerada2 < 8) {
        yl2 = rand() % (yMAX - 2) + 1; // evitar gerar em bordas
        xl2 = rand() % (xMAX - 2) + 1;

        if (mapa[yl2][xl2].obs == VAZIO && mapa[yl2][xl2].acessivel == 1){
             if (mapa[yl2 - 1][xl2].obs != BAU && mapa[yl2 - 1][xl2].obs != TRAP && mapa[yl2 - 1][xl2].obs != TOCHA && mapa[yl2-1][xl2].acessivel ==1 &&
                mapa[yl2 + 1][xl2].obs != BAU && mapa[yl2 + 1][xl2].obs != TRAP && mapa[yl2 + 1][xl2].obs != TOCHA && mapa[yl2+1][xl2].acessivel ==1 &&
                mapa[yl2][xl2 - 1].obs != BAU && mapa[yl2][xl2 - 1].obs != TRAP && mapa[yl2][xl2 - 1].obs != TOCHA && mapa[yl2][xl2-1].acessivel ==1 &&
                mapa[yl2][xl2 + 1].obs != BAU && mapa[yl2][xl2 + 1].obs != TRAP && mapa[yl2][xl2 + 1].obs != TOCHA && mapa[yl2][xl2+1].acessivel ==1 &&
                mapa[yl2 + 1][xl2 + 1].obs != BAU && mapa[yl2 + 1][xl2 + 1].obs != TRAP && mapa[yl2 + 1][xl2 + 1].obs != TOCHA && mapa[yl2+1][xl2+1].acessivel ==1 &&
                mapa[yl2 - 1][xl2 + 1].obs != BAU && mapa[yl2 - 1][xl2 + 1].obs != TRAP && mapa[yl2  - 1][xl2 + 1].obs != TOCHA && mapa[yl2-1][xl2+1].acessivel ==1 &&
                mapa[yl2 + 1][xl2 - 1].obs != BAU && mapa[yl2 + 1][xl2 - 1].obs != TRAP && mapa[yl2 + 1][xl2 - 1].obs != TOCHA && mapa[yl2+1][xl2-1].acessivel ==1 &&
                mapa[yl2][xl2 + 2].obs != BAU && mapa[yl2][xl2 + 2].obs != TRAP && mapa[yl2][xl2 + 2].obs != TOCHA && mapa[yl2][xl2+2].acessivel ==1 &&
                mapa[yl2 - 1][xl2 + 2].obs != BAU && mapa[yl2 - 1][xl2 + 2].obs != TRAP && mapa[yl2 - 1][xl2 + 2].obs != TOCHA && mapa[yl2-1][xl2+2].acessivel ==1 && 
                mapa[yl2 - 2][xl2 + 2].obs != BAU && mapa[yl2 - 2][xl2 + 2].obs != TRAP && mapa[yl2 - 2][xl2 + 2].obs != TOCHA && mapa[yl2-2][xl2+2].acessivel ==1 && 
                mapa[yl2 - 2][xl2 + 1].obs != BAU && mapa[yl2 - 2][xl2 + 1].obs != TRAP && mapa[yl2 - 2][xl2 + 1].obs != TOCHA && mapa[yl2-2][xl2+1].acessivel ==1 &&
                mapa[yl2 - 3][xl2 + 3].obs != BAU && mapa[yl2 - 3][xl2 + 3].obs != TRAP && mapa[yl2 - 3][xl2 + 3].obs != TOCHA && mapa[yl2-3][xl2+3].acessivel ==1 &&
                mapa[yl2 - 2][xl2 + 3].obs != BAU && mapa[yl2 - 2][xl2 + 3].obs != TRAP && mapa[yl2 - 2][xl2 + 3].obs != TOCHA && mapa[yl2-2][xl2+3].acessivel ==1 &&
                mapa[yl2 - 1][xl2 + 3].obs != BAU && mapa[yl2 - 1][xl2 + 3].obs != TRAP && mapa[yl2 - 1][xl2 + 3].obs != TOCHA && mapa[yl2-1][xl2+3].acessivel ==1){


                mapa[yl2][xl2].obs = LAVA;
                mapa[yl2-1][xl2].obs = LAVA;
                mapa[yl2+1][xl2].obs = LAVA;
                mapa[yl2][xl2-1].obs = LAVA;
                mapa[yl2][xl2+1].obs = LAVA;
                mapa[yl2+1][xl2+1].obs = LAVA;
                mapa[yl2-1][xl2+1].obs = LAVA;
                mapa[yl2+1][xl2-1].obs = LAVA;
                mapa[yl2][xl2 +2].obs = LAVA;
                mapa[yl2-1][xl2+2].obs = LAVA;
                mapa[yl2-2][xl2+2].obs = LAVA;
                mapa[yl2-2][xl2+1].obs = LAVA;
                mapa[yl2-3][xl2+3].obs =LAVA;
                mapa[yl2-2][xl2+3].obs =LAVA;
                mapa[yl2-1][xl2+3].obs =LAVA;

                nLavaGerada2 ++;
            }
        }
    }
    
    int escadaGerada = 0;
    int ye, xe;
    
    while(escadaGerada < 1){
        ye = rand() % (yMAX -2) + 1;
        xe = rand() % (xMAX -2) + 1;
        
        if(mapa[ye][xe].obs==VAZIO && mapa[ye][xe].acessivel == 1){
            mapa[ye][xe].obs = ESCADA;
            mapa[ye][xe].acessivel = 0;
            escadaGerada++;
        }
        
    }
    
    return 0;
}

/*
a103993 - Júlia Costa
a104532 - Tomás Sousa Barbosa
Função para escrever o mapa
*/
void escreveMapa(CASA **mapa, MONSTRO *listaMonstros, JOGADOR jogador, int yMAX , int xMAX, int nMonstros){  
    for(int i = 0; i < yMAX; i++){
        for(int j = 0; j < xMAX; j++){
            if(mapa[i][j].obs == MURO){
                if(mapa[i][j].visivel == 1){
                    attron(COLOR_PAIR(COLOR_WHITE));
                    mvaddch(i, j, '#');
                    attroff(COLOR_PAIR(COLOR_WHITE));
                }
                else{
                    mvaddch(i,j,' ');
                }
            }
            else{
                if(mapa[i][j].visivel == 1){
                    switch (mapa[i][j].obs){
                        case VAZIO:
                            attron(COLOR_PAIR(COLOR_YELLOW));
                            mvaddch(i, j, '.');
                            attroff(COLOR_PAIR(COLOR_YELLOW));  
                            break;
                        case TRAP:
                            attron(COLOR_PAIR(COLOR_RED));
                            mvaddch(i, j, 'T');
                            attroff(COLOR_PAIR(COLOR_RED));
                            break;
                        case TOCHA:
                            attron(COLOR_PAIR(COLOR_YELLOW));
                            mvaddch(i,j,'I');
                            attroff(COLOR_PAIR(COLOR_YELLOW));
                            break;
                        case BAU:
                            attron(COLOR_PAIR(COLOR_BLUE));
                            mvaddch(i, j, 'B');
                            attroff(COLOR_PAIR(COLOR_BLUE));
                            break;
                        case LAVA:
                            attron(COLOR_PAIR(COLOR_YELLOW));
                            mvaddch(i, j, 'X');
                            attroff(COLOR_PAIR(COLOR_YELLOW));
                            break;
                        case ESCADA:
                            attron(COLOR_PAIR(COLOR_MAGENTA));
                            mvaddch(i, j, 'H');
                            attroff(COLOR_PAIR(COLOR_MAGENTA));
                            break;
                        default:
                            break;
                    }
                }
                else{
                    mvaddch(i,j,' ');
                }
            }
        }
    }

    //desenhar monstros
    attron(COLOR_PAIR(COLOR_GREEN));
    for(int i = 0; i < nMonstros; i++){
        if(mapa[listaMonstros[i].posY][listaMonstros[i].posX].visivel == 1)
            mvaddch(listaMonstros[i].posY, listaMonstros[i].posX, 'M');
    }
    attroff(COLOR_PAIR(COLOR_GREEN));

    attron(COLOR_PAIR(COLOR_WHITE));
    mvaddch(yMAX, 0, '|');
    mvaddch(yMAX+1, 0, '|');
    mvaddch(yMAX+2, 0, '|');
    mvprintw(yMAX, 1, "HP: ");
    if(jogador->vida > 0.2 * jogador->vidaMax){
        attron(COLOR_PAIR(COLOR_GREEN));
        mvprintw(yMAX, 5, "%d/%d  ", jogador->vida, jogador->vidaMax);
        attroff(COLOR_PAIR(COLOR_GREEN));
    }
    else{
        attron(COLOR_PAIR(COLOR_RED));
        mvprintw(yMAX, 5, "%d/%d  ", jogador->vida, jogador->vidaMax);
        attroff(COLOR_PAIR(COLOR_RED));
    }
    mvprintw(yMAX + 1, 1, "EXP: %d/%d Nivel: %d ",jogador->expAtual, jogador->lvl * jogador->lvl * 15 - jogador->lvl * 4, jogador->lvl);
    mvprintw(yMAX + 2, 1, "Score: %d  ", jogador->score);
    attroff(COLOR_PAIR(COLOR_WHITE));

}

/*
a104171 - Gabriel Pereira Ribeiro
Função que cria uma linha de visão entre 2 posições, se for possível chegar ao destino, marca a casa como visível
*/
void linhaVisao(CASA **mapa, int xAtual, int yAtual, int xDestino, int yDestino){
    int dx = abs(xDestino - xAtual);
    int dy = -abs(yDestino  - yAtual);
    int sinalX = xAtual < xDestino ? 1 : -1;
    int sinalY = yAtual < yDestino ? 1 : -1;
    int erro = dx + dy; //erro em xy
    int erroAux;

    while(1){
        //se encontrar um muro para
        if(mapa[yAtual][xAtual].obs == MURO){
            mapa[yAtual][xAtual].visivel = 1;
            break;
        }
        
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

/*
a104274 - João Miguel
a103993 - Júlia Costa
Função para calcular a visibilidade das casas - em torno do jogador, tochas e escada, se necessário
*/
void calcularVisivel(CASA **mapa, JOGADOR jogador, int yMAX, int xMAX, int nMonstros){
    for (int i = 0; i < yMAX; i++){
        for (int j = 0; j < xMAX; j++){
            mapa[i][j].visivel = 0;

            float distancia = sqrt(((jogador->posX-j)*(jogador->posX-j)) + ((jogador->posY-i)*(jogador->posY-i)));
            if(roundf(distancia) < 15)
                linhaVisao(mapa, jogador->posX, jogador->posY, j, i);
        }
    }

    for(int i = 1; i < yMAX-1; i++){
        for(int j = 1; j < xMAX-1; j++){
            if(mapa[i][j].obs == TOCHA){
                int menorY = i-3 > 0 ? i-3 : 0;
                int maiorY = i+3 < yMAX ? i+3 : yMAX-1; 
                int menorX = j-3 > 0 ? j-3 : 0;
                int maiorX = j+3 < xMAX ? j+3 : xMAX-1;
                float raio;

                for(int v = menorY; v <= maiorY; v++){
                    for(int w = menorX; w <= maiorX; w++){
                        raio = sqrt(((j-w)*(j-w)) + ((i-v)*(i-v)));
                        if(roundf(raio) <= 3)
                            linhaVisao(mapa, j, i, w, v);
                    }
                }
            }
            else if(nMonstros == 0 && mapa[i][j].obs == ESCADA){
                int menorY = i-5 > 0 ? i-5 : 0;
                int maiorY = i+5 < yMAX ? i+5 : yMAX-1; 
                int menorX = j-5 > 0 ? j-5 : 0;
                int maiorX = j+5 < xMAX ? j+5 : xMAX-1;
                float raio;

                for(int v = menorY; v <= maiorY; v++){
                    for(int w = menorX; w <= maiorX; w++){
                        raio = sqrt(((j-w)*(j-w)) + ((i-v)*(i-v)));
                        if(roundf(raio) <= 5)
                            linhaVisao(mapa, j, i, w, v);
                    }
                }      
            }
        }
    }
}

/*
a103993 - Júlia Costa
Função que torna a escada acessível se não houver mais monstros
*/
int escadaAcessivel(CASA **mapa, int yMAX, int xMAX, int nMonstros){
    if(nMonstros==0){
        for(int i=0;i<yMAX;i++){
            for(int j=0;j<xMAX;j++){
                if(mapa[i][j].obs==ESCADA){
                    mapa[i][j].acessivel=1;
                }
            }
        }
    }

    return 0;
}
