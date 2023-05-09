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

            mapa[i][j].acessivel = mapa[i][j].temMonstro = 0; 
        }
    }

    //Colocar MURO nas bordas do mapa
    for(int i = 0; i < yMAX; i++){
        mapa[i][0].obs = MURO;
        mapa[i][0].acessivel = mapa[i][0].temMonstro = 0;
        mapa[i][xMAX - 1].obs = MURO;
        mapa[i][xMAX - 1].acessivel = mapa[i][xMAX - 1].temMonstro = 0;
    }
    for(int i = 0; i < xMAX; i++){
        mapa[0][i].obs = MURO;
        mapa[0][i].acessivel = mapa[0][i].temMonstro = 0;
        mapa[yMAX - 1][i].obs = MURO;
        mapa[yMAX - 1][i].acessivel = mapa[yMAX - 1][i].temMonstro = 0;
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
int compactaMapa(CASA **mapa, int yMAX, int xMAX, int fase){
    CASA mapaAux[yMAX][xMAX];
    int nMuros, nEspacos, nVazias;

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
    int yt, xt;
    
    while(nTrapsGeradas < 10){
        yt = rand() % (yMAX - 2) + 1; // evitar gerar em bordas
        xt = rand() % (xMAX - 2) + 1;

        if(mapa[yt][xt].obs == VAZIO && mapa[yt][xt].acessivel == 1){
            mapa[yt][xt].obs = TRAP;
            nTrapsGeradas++;
        }
    }

    //baus, a maneira como está gerada é fraca
    int nBausGerados = 0;
    int yb,xb;

    while(nBausGerados < 6){
        yb = rand() % (yMAX - 2) + 1; // evitar gerar em bordas
        xb = rand() % (xMAX - 2) + 1;

        if(mapa[yb][xb].obs == VAZIO && mapa[yb][xb].obs != TRAP && mapa[yb][xb].acessivel == 1){
            mapa[yb][xb].obs = BAU;
            mapa[yb][xb].acessivel = 0;
            nBausGerados++;
        }
    }
    // lava
    int nLavaGerada1 = 0;
    int yl1, xl1;

    while (nLavaGerada1 < 8) {
        yl1 = rand() % (yMAX - 2) + 1; // evitar gerar em bordas
        xl1 = rand() % (xMAX - 2) + 1;

        if (mapa[yl1][xl1].obs == VAZIO && mapa[yl1][xl1].acessivel == 1) {
             if (mapa[yl1 - 1][xl1].obs != BAU && mapa[yl1 - 1][xl1].obs != TRAP && mapa[yl1-1][xl1].acessivel ==1 &&
                mapa[yl1 + 1][xl1].obs != BAU && mapa[yl1 + 1][xl1].obs != TRAP && mapa[yl1+1][xl1].acessivel ==1 &&
                mapa[yl1][xl1 - 1].obs != BAU && mapa[yl1][xl1 - 1].obs != TRAP && mapa[yl1][xl1-1].acessivel ==1 &&
                mapa[yl1][xl1 + 1].obs != BAU && mapa[yl1][xl1 + 1].obs != TRAP && mapa[yl1][xl1+1].acessivel ==1 &&
                mapa[yl1 + 1][xl1 + 1].obs != BAU && mapa[yl1 + 1][xl1 + 1].obs != TRAP && mapa[yl1 + 1][xl1+1].acessivel ==1 &&
                mapa[yl1 - 1][xl1 + 1].obs != BAU && mapa[yl1 - 1][xl1 + 1].obs != TRAP && mapa[yl1 - 1][xl1+1].acessivel ==1 &&
                mapa[yl1 + 1][xl1 - 1].obs != BAU && mapa[yl1 + 1][xl1 - 1].obs != TRAP && mapa[yl1 + 1][xl1-1].acessivel ==1 &&
                mapa[yl1- 1][xl1 - 1].obs != BAU && mapa[yl1 - 1][xl1 - 1].obs != TRAP && mapa[yl1- 1][xl1-1].acessivel ==1 && 
                mapa[yl1][xl1 +2].obs != BAU && mapa[yl1][xl1 + 2].obs != TRAP && mapa[yl1][xl1+2].acessivel ==1 &&
                mapa[yl1][xl1 -2].obs != BAU && mapa[yl1][xl1 -2].obs != TRAP && mapa[yl1][xl1-2].acessivel ==1 && 
                mapa[yl1 + 2][xl1].obs != BAU && mapa[yl1 + 2][xl1].obs != TRAP && mapa[yl1 + 2][xl1].acessivel ==1 && 
                mapa[yl1 - 2][xl1].obs != BAU && mapa[yl1 - 2][xl1].obs != TRAP && mapa[yl1- 2][xl1].acessivel ==1){
            
            
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

        if (mapa[yl2][xl2].obs == VAZIO && mapa[yl2][xl2].acessivel == 1) {
             if (mapa[yl2 - 1][xl2].obs != BAU && mapa[yl2 - 1][xl2].obs != TRAP && mapa[yl2-1][xl2].acessivel ==1 &&
                mapa[yl2 + 1][xl2].obs != BAU && mapa[yl2 + 1][xl2].obs != TRAP && mapa[yl2+1][xl2].acessivel ==1 &&
                mapa[yl2][xl2 - 1].obs != BAU && mapa[yl2][xl2 - 1].obs != TRAP && mapa[yl2][xl2-1].acessivel ==1 &&
                mapa[yl2][xl2 + 1].obs != BAU && mapa[yl2][xl2 + 1].obs != TRAP && mapa[yl2][xl2+1].acessivel ==1 &&
                mapa[yl2 + 1][xl2 + 1].obs != BAU && mapa[yl2 + 1][xl2 + 1].obs != TRAP && mapa[yl2 + 1][xl2+1].acessivel ==1 &&
                mapa[yl2 - 1][xl2 + 1].obs != BAU && mapa[yl2 - 1][xl2 + 1].obs != TRAP && mapa[yl2 - 1][xl2+1].acessivel ==1 &&
                mapa[yl2 + 1][xl2 - 1].obs != BAU && mapa[yl2 + 1][xl2 - 1].obs != TRAP && mapa[yl2 + 1][xl2-1].acessivel ==1 &&
                mapa[yl2][xl2 +2].obs != BAU && mapa[yl2][xl2 + 2].obs != TRAP && mapa[yl2][xl2+2].acessivel ==1 &&
                mapa[yl2-1][xl2 +2].obs != BAU && mapa[yl2-1][xl2 +2].obs != TRAP && mapa[yl2-1][xl2 +2].acessivel ==1 && 
                mapa[yl2 -2][xl2+2].obs != BAU && mapa[yl2 -2][xl2+2].obs != TRAP && mapa[yl2 -2][xl2+2].acessivel ==1 && 
                mapa[yl2 - 2][xl2+1].obs != BAU && mapa[yl2 - 2][xl2+1].obs != TRAP && mapa[yl2 - 2][xl2+1].acessivel ==1 &&
                mapa[yl2 -3][xl2+3].obs != BAU && mapa[yl2 -3][xl2+3].obs != TRAP && mapa[yl2 -3][xl2+3].acessivel ==1 &&
                mapa[yl2 -2][xl2+3].obs != BAU && mapa[yl2 -2][xl2+3].obs != TRAP && mapa[yl2 -2][xl2+3].acessivel ==1 &&
                mapa[yl2 -1][xl2+3].obs != BAU && mapa[yl2 -1][xl2+3].obs != TRAP && mapa[yl2 -1][xl2+3].acessivel ==1){

            
            
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
    
    return 0;
}
char obstac(int y, int x, CASA **mapa){
    if(mapa[y][x].obs == MURO)
        return 'M';
    if(mapa[y][x].obs == VAZIO)
        return 'V';
    if(mapa[y][x].obs == TRAP)
        return 'T';
    if(mapa[y][x].obs == BAU)
        return 'B';
    /*if(mapa[x][x].obs == LAVA)
        return 'X';
    */
    else  
        return 'S';

}

//função para escrever o mapa
void escreveMapa(CASA **mapa, JOGADOR jogador, int yMAX , int xMAX){  
    for(int i = 0; i < yMAX; i++){
        for(int j = 0; j < xMAX; j++){
            if(mapa[i][j].acessivel == 1){
                 if(mapa[i][j].obs == TRAP){
                    attron(COLOR_PAIR(1));
                    mvaddch(i, j, 'T');
                    attroff(COLOR_PAIR(1));
                }
                else if (mapa[i][j].obs == LAVA){
                    attron(COLOR_PAIR(COLOR_YELLOW));
                    mvaddch(i, j, 'X');
                    attroff(COLOR_PAIR(COLOR_YELLOW));
                }
                
                else if(mapa[i][j].visivel == 1){
                    attron(COLOR_PAIR(COLOR_YELLOW));
                    mvaddch(i, j, '.');
                    attroff(COLOR_PAIR(COLOR_YELLOW));    
                }
                else{
                    attron(COLOR_PAIR(2));
                    mvaddch(i, j, '.');
                    attroff(COLOR_PAIR(2));
                }
            }
            else{
                if(mapa[i][j].obs == MURO){
                    attron(COLOR_PAIR(COLOR_WHITE));
                    mvaddch(i, j, '#');
                    attroff(COLOR_PAIR(COLOR_WHITE));                    
                }
                else if(mapa[i][j].obs == BAU){
                    attron(COLOR_PAIR(COLOR_BLUE));
                    mvaddch(i, j, 'B');
                    attroff(COLOR_PAIR(COLOR_BLUE));
                }
                else if(mapa[i][j].temMonstro == 1){
                    attron(COLOR_PAIR(COLOR_CYAN));
                    mvaddch(i, j, 'M');
                    attroff(COLOR_PAIR(COLOR_CYAN));
                }
                else{
                    attron(COLOR_PAIR(COLOR_WHITE));
                    mvaddch(i, j, '.');
                    attroff(COLOR_PAIR(COLOR_WHITE)); 
                }
            }    
        }
    }

    attron(COLOR_PAIR(COLOR_WHITE));
    mvprintw(yMAX, 1, "HP: %d/%d", jogador->vida, jogador->vidaMax);
    mvprintw(yMAX + 1, 1, "EXP: %d/%d Nível %d", jogador->expAtual, 20 + jogador->lvl * 5,jogador->lvl); //ver o scale do xp
    mvprintw(yMAX + 2, 1, "Score: %d", jogador->score);
    mvprintw(0, 0, "Cima: obs:%c/vis:%s/aces:%s/mons:%s", obstac(jogador->posY-1,jogador->posX, mapa), mapa[jogador->posY-1][jogador->posX].visivel == 1 ? "V" : "NV",mapa[jogador->posY-1][jogador->posX].acessivel == 1 ? "A" : "NA",mapa[jogador->posY-1][jogador->posX].temMonstro == 1 ? "M" : "NM");
    mvprintw(0, 50, "Baxi: obs:%c / vis:%s / aces:%s / monst:%s", obstac(jogador->posY+1,jogador->posX, mapa), mapa[jogador->posY+1][jogador->posX].visivel == 1 ? "V" : "NV",mapa[jogador->posY+1][jogador->posX].acessivel == 1 ? "A" : "NA",mapa[jogador->posY+1][jogador->posX].temMonstro == 1 ? "M" : "NM");
    mvprintw(yMAX-1, 0, "Dir: obs:%c / vis:%s / aces:%s / monst:%s", obstac(jogador->posY,jogador->posX+1, mapa), mapa[jogador->posY][jogador->posX+1].visivel == 1 ? "V" : "NV",mapa[jogador->posY][jogador->posX+1].acessivel == 1 ? "A" : "NA",mapa[jogador->posY][jogador->posX+1].temMonstro == 1 ? "M" : "NM");
    mvprintw(yMAX-1, 50, "Esq: obs:%c / vis:%s / aces:%s / monst:%s", obstac(jogador->posY,jogador->posX-1, mapa), mapa[jogador->posY][jogador->posX-1].visivel == 1 ? "V" : "NV",mapa[jogador->posY][jogador->posX-1].acessivel == 1 ? "A" : "NA",mapa[jogador->posY][jogador->posX-1].temMonstro == 1 ? "M" : "NM");
    attroff(COLOR_PAIR(COLOR_WHITE));
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



