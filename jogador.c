#include <stdlib.h>
#include <stdio.h>
#include "estado.h"
#include "jogador.h"
#include "mapa.h"
#include "monstros.h"

//função para incializar o jogador numa posição válida
void fazJogador(CASA **mapa, MONSTRO *listaMonstros, JOGADOR jogador, int yMAX, int xMAX, int nMonstros){
	int posInvalida = 1;

	while(posInvalida){
		int y = rand() % yMAX;
		int x = rand() % xMAX;
		if(mapa[y][x].obs == VAZIO && mapa[y][x].acessivel == 1 && monstrosPerto(listaMonstros, y, x, nMonstros) == 0){

			jogador->posY = y;
			jogador->posX = x;

			posInvalida = 0;
		}

	}

	jogador->score = jogador->expAtual = 0;
	jogador->vida=50;
	jogador->vidaMax=50;
	jogador->lvl = 1;
}

void moverJogador(JOGADOR jogador, int dx, int dy, CASA destino){
	if(destino.acessivel == 1 && destino.obs != MURO){
		jogador->posX += dx;
		jogador->posY += dy;
	}
}

void escreveJogador(JOGADOR jogador){
	mvaddch(jogador->posY, jogador->posX, 'G');
}

void danoTrap (CASA **mapa, JOGADOR jogador, int yMAX){
	if (mapa[jogador->posY][jogador->posX].obs == TRAP){
		if (jogador->vida<=10)
			return;
		else{ 
			jogador->vida-=10;
			mvprintw(yMAX, 1, "HP: %d/%d", jogador->vida, jogador->vidaMax);
			mvprintw(yMAX, 35, "Patada na boca     	");
			mapa[jogador->posY][jogador->posX].obs=VAZIO;
			mapa[jogador->posY][jogador->posX].acessivel=1;
		}
	}
}

void abreBau (CASA **mapa, JOGADOR jogador, int yMAX){

	int temp;
    if(mapa[jogador->posY - 1 ][jogador->posX - 1].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX,35,"vida máxima");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX, 35, "bonus de %d pontos", temp);
		}

		mapa[jogador->posY - 1 ][jogador->posX - 1].obs = VAZIO;
		mapa[jogador->posY - 1 ][jogador->posX - 1].acessivel = 1;
	}   

    else if(mapa[jogador->posY - 1 ][jogador->posX + 0].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX,35,"vida máxima");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX, 35, "bonus de %d pontos", temp);
		}

		mapa[jogador->posY - 1 ][jogador->posX + 0].obs = VAZIO;
		mapa[jogador->posY - 1 ][jogador->posX + 0].acessivel = 1;
	}   

	else if(mapa[jogador->posY - 1 ][jogador->posX + 1].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX,35,"vida máxima");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX, 35, "bonus de %d pontos", temp);
		}

		mapa[jogador->posY - 1 ][jogador->posX + 1].obs = VAZIO;
		mapa[jogador->posY - 1 ][jogador->posX + 1].acessivel = 1;
	}   

    else if (mapa[jogador->posY + 0 ][jogador->posX - 1].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX,35,"vida máxima");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX, 35, "bonus de %d pontos", temp);
		}

		mapa[jogador->posY + 0 ][jogador->posX - 1].obs = VAZIO;
		mapa[jogador->posY + 0 ][jogador->posX - 1].acessivel = 1;
	}   

    else if (mapa[jogador->posY + 0 ][jogador->posX + 0].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX,35,"vida máxima");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX, 35, "bonus de %d pontos", temp);
		}

		mapa[jogador->posY + 0 ][jogador->posX + 0].obs = VAZIO;
		mapa[jogador->posY + 0 ][jogador->posX + 0].acessivel = 1;
	}   

    else if (mapa[jogador->posY + 0 ][jogador->posX + 1].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX,35,"vida máxima");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX, 35, "bonus de %d pontos", temp);
		}

		mapa[jogador->posY + 0 ][jogador->posX + 1].obs = VAZIO;
		mapa[jogador->posY + 0 ][jogador->posX + 1].acessivel = 1;
	}   

    else if (mapa[jogador->posY + 1 ][jogador->posX - 1].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX,35,"vida máxima");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX, 35, "bonus de %d pontos", temp);
		}

		mapa[jogador->posY + 1 ][jogador->posX - 1].obs = VAZIO;
		mapa[jogador->posY + 1 ][jogador->posX - 1].acessivel = 1;
	}   

    else if (mapa[jogador->posY + 1 ][jogador->posX + 0].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX,35,"vida máxima");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX, 35, "bonus de %d pontos", temp);
		}

		mapa[jogador->posY + 1 ][jogador->posX + 0].obs = VAZIO;
		mapa[jogador->posY + 1 ][jogador->posX + 0].acessivel = 1;
	}   

    else if (mapa[jogador->posY + 1 ][jogador->posX + 1].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX,35,"vida máxima");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX, 35, "bonus de %d pontos", temp);
		}

		mapa[jogador->posY + 1 ][jogador->posX + 1].obs = VAZIO;
		mapa[jogador->posY + 1 ][jogador->posX + 1].acessivel = 1;
	}   
}

void initMapaProximidade(CASA **mapa, int **listaProximidade, int passos, int i, int j, int yMax , int xMax){
	if(i >= yMax || i < 0 ||  j >= xMax || j < 0)
		return;
	
	if(mapa[i][j].obs == MURO || mapa[i][j].obs == BAU)
		listaProximidade[i][j] = -1;
	else
		listaProximidade[i][j] = passos;

	passos++;

	initMapaProximidade(mapa, listaProximidade, passos, i+1, j, yMax, xMax);
	initMapaProximidade(mapa, listaProximidade, passos, i-1, j, yMax, xMax);
	initMapaProximidade(mapa, listaProximidade, passos, i, j+1, yMax, xMax);
	initMapaProximidade(mapa, listaProximidade, passos, i, j-1, yMax, xMax);
}

//chamar casa vez que o jogador se mexe
void atualizarProximidade(int **listaProximidade, char move, int newY, int newX, int yMAX, int xMAX){
	switch (move){
		case 'U':
			for(int i = 0; i < yMAX; i++){
				for(int j = 0; j < xMAX; j++){
					if(i <= newY){
						if(listaProximidade[i][j] >= 0)
							listaProximidade[i][j]--;
					}
					else{
						if(listaProximidade[i][j] >= 0)
							listaProximidade[i][j]++;
					}
				}
			}
			break;
		case 'D':
			for(int i = 0; i < yMAX; i++){
				for(int j = 0; j < xMAX; j++){
					if(i >= newY){
						if(listaProximidade[i][j] >= 0)
							listaProximidade[i][j]--;
					}
					else{
						if(listaProximidade[i][j] >= 0)
							listaProximidade[i][j]++;
					}
				}
			}
			break;
		case 'R':
			for(int i = 0; i < yMAX; i++){
				for(int j = 0; j < xMAX; j++){
					if(j >= newX){
						if(listaProximidade[i][j] >= 0)
							listaProximidade[i][j]--;
					}
					else{
						if(listaProximidade[i][j] >= 0)
							listaProximidade[i][j]++;
					}
				}
			}	
			break;
		case 'L':
			for(int i = 0; i < yMAX; i++){
				for(int j = 0; j < xMAX; j++){
					if(j <= newX){
						if(listaProximidade[i][j] >= 0)
							listaProximidade[i][j]--;
					}
					else{
						if(listaProximidade[i][j] >= 0)
							listaProximidade[i][j]++;
					}
				}
			}
			break;
		default:
			break;
	}
}