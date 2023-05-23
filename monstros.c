#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include <time.h>
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

//TODO balancear valores e iniciar resto dos stats
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
        (*listaMonstros)[i].posX = xRand;
        (*listaMonstros)[i].posY = yRand;
        (*listaMonstros)[i].spawnX = xRand;
        (*listaMonstros)[i].spawnY = yRand;
        (*listaMonstros)[i].vida = 30;
        (*listaMonstros)[i].vidaMax = 30;
        (*listaMonstros)[i].ataque = 6;
        (*listaMonstros)[i].defesa = 4;
        (*listaMonstros)[i].precisao = 80;
    }

    return nMonstros;
}

int visaoMonstro(CASA **mapa, int xMonstro, int yMonstro, int xJogador, int yJogador, int *newPosX, int *newPosY){
    int dx = abs(xJogador - xMonstro);
    int dy = -abs(yJogador  - yMonstro);
    int sinalX = xMonstro < xJogador ? 1 : -1;
    int sinalY = yMonstro < yJogador ? 1 : -1;
    int erro = dx + dy;
    int erroAux;
    int cont = 0;
    while(1){
        if(mapa[yMonstro][xMonstro].obs == MURO)
            return 0;
        
        if(xMonstro == xJogador && yMonstro == yJogador)
            return 1;
            
        erroAux = 2 * erro; 

        if (erroAux >= dy){
            erro += dy;
            xMonstro += sinalX;
            if(cont == 0){
                if(mapa[yMonstro][xMonstro].obs == VAZIO){
                    *newPosX = xMonstro;
                    *newPosY = yMonstro;
                    cont = 1;
                }
            }
        }
        if (erroAux <= dx){
            erro += dx; 
            yMonstro += sinalY;
            if(cont == 0){
                *newPosX = xMonstro;
                *newPosY = yMonstro;
                cont = 1;
            }
        }
    }
}

void moveMonstros(CASA **mapa, MONSTRO *listaMonstros, JOGADOR jogador, int nMonstros, double *ultimoTempo){
    double tempoAtual = clock() / CLOCKS_PER_SEC;
    if(*ultimoTempo >= 0){
        if(tempoAtual -  *ultimoTempo < 0.4)
            return;
    }
    
    int newPosY; int newPosX; int haVisao;
    for(int i = 0; i < nMonstros; i++){
        float dist = sqrt(((listaMonstros[i].posX - jogador->posX)*(listaMonstros[i].posX - jogador->posX)) + ((listaMonstros[i].posY - jogador->posY)*(listaMonstros[i].posY - jogador->posY)));
        if(roundf(dist) <= 1) return;

        int distSpawn = sqrt(((listaMonstros[i].spawnX - jogador->posX)*(listaMonstros[i].spawnX - jogador->posX)) + ((listaMonstros[i].spawnY - jogador->posY)*(listaMonstros[i].spawnY - jogador->posY)));
        //aproveitar que esta função processa os monstros todos para regenerar a vida (50% da vida em falta) caso eles estejam na posição inicial
        if(listaMonstros[i].posY == listaMonstros[i].spawnY && listaMonstros[i].posX == listaMonstros[i].spawnX && listaMonstros[i].vida !=  listaMonstros[i].vidaMax){
            int dif = listaMonstros[i].vidaMax -  listaMonstros[i].vida;
            if(dif <= 2)
                listaMonstros[i].vida = listaMonstros[i].vidaMax;
            else
                listaMonstros[i].vida += dif / 2;
        }

        haVisao = visaoMonstro(mapa, listaMonstros[i].posX, listaMonstros[i].posY, jogador->posX, jogador->posY, &newPosX, &newPosY);
        if(!haVisao || distSpawn > 15){ //se o jogador não conseguir ver o monstro
            if(listaMonstros[i].posY != listaMonstros[i].spawnY || listaMonstros[i].posX != listaMonstros[i].spawnX){
                listaMonstros[i].posY = listaMonstros[i].spawnY;
                listaMonstros[i].posX = listaMonstros[i].spawnX;
            }
        }
        else{
            if(distSpawn <= 5 && listaMonstros[i].vida == listaMonstros[i].vidaMax){ //se o jogador entrou dentro da area de deteção do monstro e este tem a vida cheia
                if(jogador->posY != newPosY || jogador->posX != newPosX){ //se a próxima posição não é a mesma que a do jogador (já estão lado a lado), move-se
                    listaMonstros[i].posY = newPosY;
                    listaMonstros[i].posX = newPosX;
                }
            }
            else{//se o jogador consegue ver o monstro, mas nao está dentro do raio ou o monstro está ferido, este volta para a sua posição inicial
                if(listaMonstros[i].posY != listaMonstros[i].spawnY || listaMonstros[i].posX != listaMonstros[i].spawnX){
                    visaoMonstro(mapa, listaMonstros[i].posX, listaMonstros[i].posY, listaMonstros[i].spawnX , listaMonstros[i].spawnY, &newPosX, &newPosY);
                    listaMonstros[i].posY = newPosY;
                    listaMonstros[i].posX = newPosX;
                }
            }
        }
    }

    *ultimoTempo = clock() / CLOCKS_PER_SEC;
}

int modoCombate(JOGADOR jogador, MONSTRO *monstro, int yMAX){
    nodelay(stdscr, false);
    int x, y, input;
    int fugir = 0;
    while(jogador->vida > 0 && monstro->vida > 0 && !fugir){
        mvprintw(yMAX-1, 35, "Turno do jogador!");

        do{
            input = getch();
        }while(input != 'a' && input != 'r');   

        if(input == 'a'){
                x = rand() % 100;
                if(x <= jogador->precisao){
                    int dano = jogador->ataque - (0.15 *  monstro->defesa);
                    monstro->vida -= dano;
                    mvprintw(yMAX, 35, "Causou %d dano!", dano);
                }
                else
                    mvprintw(yMAX, 35, "Falhou o ataque");
        }
        else if(input == 'r'){
                fugir = 1;
                mvprintw(yMAX, 35, "Conseguiu fugir da luta!");
        }
        
        mvprintw(yMAX+1, 53, "Turno do Monstro!");
        y = rand () % 100;
        if(y <= monstro->precisao){
            int dmg = monstro->ataque - (0.20 * jogador->defesa);
            jogador->vida -= dmg;
            mvprintw(yMAX+1, 35, "Sofreu %d dano!", dmg);
        }
        else
            mvprintw(yMAX+1, 35, "O monstro falhou o ataque!");
    }
    nodelay(stdscr, true);
    if(jogador->vida <= 0)
        return 0; //codigo se o jogador morreu
    if(monstro->vida <= 0)
        return 1; //codigo se o monstro foi derrotado
    if(fugir)
        return 2; //codigo se o jogador fugiu
    return 3;
}

int verificaCombate(JOGADOR jogador, MONSTRO *listaMonstros, int *nMonstros, int yMAX){
    for(int i = 0; i < (*nMonstros); i++){
        int dist = sqrt(((listaMonstros[i].posX - jogador->posX)*(listaMonstros[i].posX - jogador->posX)) + ((listaMonstros[i].posY - jogador->posY)*(listaMonstros[i].posY - jogador->posY)));
        int resultado;
        if(dist <= 1){
            resultado = modoCombate(jogador, &(listaMonstros[i]), yMAX);
            if(resultado == 1){
                (*nMonstros)--;
                for(int j = i; j < (*nMonstros); j++) {
                    listaMonstros[j] = listaMonstros[j + 1];
                }

                jogador->expAtual += 6;
                if(jogador->expAtual >= 20 + jogador->lvl*5){
                    jogador->expAtual = jogador->expAtual - 20 + jogador->lvl*5;
                    jogador->lvl++;
                    jogador->ataque += 3;  //ver scale dos valores
                    jogador->defesa += 3;
                    jogador->precisao++;
                }
            }
            return resultado;
        }
    }
    return 3;
}

int fimdeNivel (MONSTRO *listaMonstros, int *nMonstros){
    for(int i = 0 ;i < (*nMonstros); i++){
        if (listaMonstros[i].vida > 0) return 1;
    }
    return 0;
}