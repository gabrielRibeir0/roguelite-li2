#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include <time.h>
#include "estado.h"
#include "monstros.h"

/*
a104274 - João Miguel
Função que verifica se um monstro está perto de uma dada posição
*/
int monstrosPerto(MONSTRO *listaMonstros, int y, int x, int nMonstros){
    for(int i = 0; i < nMonstros; i++){
        int dist = sqrt(((listaMonstros[i].posX - x)*(listaMonstros[i].posX - x)) + ((listaMonstros[i].posY - y)*(listaMonstros[i].posY - y)));

        if(dist < 5)
            return 1;
    }

    return 0;
}

/*
a104171 - Gabriel Pereira Ribeiro
Função que inicia a lista de monstros e os seus dados em função do nível
*/
int iniciaMonstros(CASA **mapa, MONSTRO **listaMonstros, int nivel, int yMAX, int xMAX){
    int nMonstros = 0, yRand, xRand;
    switch(nivel){
        case 1:
            nMonstros = rand() % 3 + 4;
            break;
        case 2:
            nMonstros = rand() % 4 + 7;
            break;
        case 3:
            nMonstros = rand() % 3 + 9;
            break;
        default:
            nMonstros = rand() % 4 + 11;
            break;
    }

    if(*listaMonstros != NULL){
        free(*listaMonstros);
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
        int vida;
        switch(nivel){
            case 0:
                vida = rand() % 4 + 27;
                (*listaMonstros)[i].vida = vida;
                (*listaMonstros)[i].vidaMax = vida;
                (*listaMonstros)[i].ataque = rand() % 3 + 4;
                (*listaMonstros)[i].defesa = rand() % 3 + 3;
                (*listaMonstros)[i].precisao = rand() % 4 + 77;
                break;
            case 1:
                vida = rand() % 4 + 32;
                (*listaMonstros)[i].vida = vida;
                (*listaMonstros)[i].vidaMax = vida;
                (*listaMonstros)[i].ataque = rand() % 3 + 6;
                (*listaMonstros)[i].defesa = rand() % 3 + 5;
                (*listaMonstros)[i].precisao = rand() % 4 + 81;
                break;
            case 2:
                vida = rand() % 4 + 39;
                (*listaMonstros)[i].vida = vida;
                (*listaMonstros)[i].vidaMax = vida;
                (*listaMonstros)[i].ataque = rand() % 3 + 9;
                (*listaMonstros)[i].defesa = rand() % 3 + 7;
                (*listaMonstros)[i].precisao = rand() % 4 + 85;
                break;
            default:
                vida = rand() % 5 + 46;
                (*listaMonstros)[i].vida = vida;
                (*listaMonstros)[i].vidaMax = vida;
                (*listaMonstros)[i].ataque = rand() % 3 + 13;
                (*listaMonstros)[i].defesa = rand() % 3 + 10;
                (*listaMonstros)[i].precisao = rand() % 5 + 90;
                break;
        }
    }

    return nMonstros;
}

/*
a104171 - Gabriel Pereira Ribeiro
Função que verifica se existe uma linha de visão entre o monstro e o jogador e calcula a primeira posição a percorrer nessa linha
*/
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

/*
a104171 - Gabriel Pereira Ribeiro
Função que realiza o movimento dos monstros
*/
void moveMonstros(CASA **mapa, MONSTRO *listaMonstros, JOGADOR jogador, int nMonstros, double *ultimoTempo){
    double tempoAtual = clock() / CLOCKS_PER_SEC;
    if(*ultimoTempo >= 0 && tempoAtual -  *ultimoTempo < 0.4)
        return;
    
    int newPosY;
    int newPosX;
    int haVisao;
    for(int i = 0; i < nMonstros; i++){
        float dist = sqrt(((listaMonstros[i].posX - jogador->posX)*(listaMonstros[i].posX - jogador->posX)) + ((listaMonstros[i].posY - jogador->posY)*(listaMonstros[i].posY - jogador->posY)));
        if(roundf(dist) <= 1) return;

        int distSpawn = sqrt(((listaMonstros[i].spawnX - jogador->posX)*(listaMonstros[i].spawnX - jogador->posX)) + ((listaMonstros[i].spawnY - jogador->posY)*(listaMonstros[i].spawnY - jogador->posY)));
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