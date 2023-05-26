#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "estado.h"
#include "combate.h"

/*
a103993 - Júlia Costa
a104171 - Gabriel Pereira Ribeiro
a104272 - João Miguel
Função para fazer o combate por turnos 
*/
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
                mvprintw(yMAX, 35, "Causou %d dano!         ", dano);
            }
            else
                mvprintw(yMAX, 35, "Falhou o ataque         ");
        }
        else if(input == 'r'){
            fugir = 1;
            mvprintw(yMAX, 35, "Conseguiu fugir da luta!");
        }
        
        mvprintw(yMAX+1, 35, "Turno do Monstro!             ");

        y = rand () % 100;
        if(y <= monstro->precisao){
            int dmg = monstro->ataque - (0.20 * jogador->defesa);
            jogador->vida -= dmg;
            mvprintw(yMAX+1, 35, "Sofreu %d dano!           ", dmg);
        }
        else
            mvprintw(yMAX+1, 35, "O monstro falhou o ataque!");
    }

    nodelay(stdscr, true);
    mvprintw(yMAX+1, 35, "                          ");
    if(jogador->vida <= 0)
        return 0; //codigo se o jogador morreu
    if(monstro->vida <= 0)
        return 1; //codigo se o monstro foi derrotado
    if(fugir)
        return 2; //codigo se o jogador fugiu
    
    return 3;
}

/*
a104171 - Gabriel Pereira Ribeiro
a104274 - João Miguel
Função que verifica se algum dos monstros está perto do jogador e começa o combate em caso afirmativo e processa o resultado da luta
*/
void verificaCombate(JOGADOR jogador, MONSTRO *listaMonstros, int *nMonstros, int yMAX, double *delayFugir){
    double tempoAtual = clock() / CLOCKS_PER_SEC;
    if(*delayFugir >= 0 && tempoAtual - *delayFugir < 1.2)
        return;
    
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

                int maxExp = jogador->lvl * jogador->lvl * 15 - jogador->lvl * 5;
                
                switch(jogador->score){
                    case 0:
                        jogador->expAtual += rand() % 3 + 5;
                        if(jogador->expAtual >= maxExp){
                            jogador->expAtual = jogador->expAtual - maxExp;
                            jogador->lvl++;
                            jogador->ataque += 3;
                            jogador->defesa += 3;
                            jogador->precisao++;
                            jogador->vidaMax += 5;
                            jogador->vida = jogador->vida > 0.75 * jogador->vidaMax ? jogador->vida : jogador->vidaMax * 0.75;
                        }
                        break;
                    case 1:
                        jogador->expAtual += rand() % 3 + 8;
                        if(jogador->expAtual >= maxExp){
                            jogador->expAtual = jogador->expAtual - maxExp;
                            jogador->lvl++;
                            jogador->ataque += 5;
                            jogador->defesa += 5;
                            jogador->precisao++;
                            jogador->vidaMax += 7;
                            jogador->vida = jogador->vida > 0.75 * jogador->vidaMax ? jogador->vida : jogador->vidaMax * 0.75;
                        }
                        break;
                    case 2:
                        jogador->expAtual += rand() % 3 + 10;
                        if(jogador->expAtual >= maxExp){
                            jogador->expAtual = jogador->expAtual - maxExp;
                            jogador->lvl++;
                            jogador->ataque += 7;
                            jogador->defesa += 7;
                            jogador->precisao++;
                            jogador->vidaMax += 10;
                            jogador->vida = jogador->vida > 0.75 * jogador->vidaMax ? jogador->vida : jogador->vidaMax * 0.75;
                        }
                        break;
                    default:
                        jogador->expAtual += rand() % 3 + 12;
                        if(jogador->expAtual >= maxExp){
                            jogador->expAtual = jogador->expAtual - maxExp;
                            jogador->lvl++;
                            jogador->ataque += 10;
                            jogador->defesa += 10;
                            jogador->precisao++;
                            jogador->vidaMax += 13;
                            jogador->vida = jogador->vida > 0.75 * jogador->vidaMax ? jogador->vida : jogador->vidaMax * 0.75;
                        }
                        break;
                }
            }
            else if(resultado == 2)
                *delayFugir = clock() / CLOCKS_PER_SEC;

        }
    }
}