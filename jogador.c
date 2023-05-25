#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "estado.h"
#include "jogador.h"
#include "mapa.h"
#include "monstros.h"

void iniciaJogador(JOGADOR jogador){
	jogador->score = 1;
	jogador->expAtual = 0;
	jogador->vida=50;
	jogador->vidaMax=50;
	jogador->lvl = 1;
	jogador->ataque = 10;
	jogador->defesa = 6;
	jogador->precisao = 90;
}

void posicaoJogador(CASA **mapa, MONSTRO *listaMonstros, JOGADOR jogador, int yMAX, int xMAX, int nMonstros){
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

//a103993
void danoTrap (CASA **mapa, JOGADOR jogador, int yMAX){
	if (mapa[jogador->posY][jogador->posX].obs == TRAP){
		if (jogador->vida<=10)
			return;
		else{ 
			jogador->vida-=10;
			mvprintw(yMAX, 1, "HP: %d/%d", jogador->vida, jogador->vidaMax);
			mvprintw(yMAX+1, 35, "			     	");
			mvprintw(yMAX, 35, "Patada na boca     	");
			mvprintw(yMAX-1, 35, "			     	");
			mapa[jogador->posY][jogador->posX].obs=VAZIO;
			mapa[jogador->posY][jogador->posX].acessivel=1;
		}
	}
}

//a103993
void abreBau (CASA **mapa, JOGADOR jogador, int yMAX){

	int temp;
    if(mapa[jogador->posY - 1 ][jogador->posX - 1].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX,35,"vida máxima			");
			mvprintw(yMAX+1, 35, "                 	");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX, 35, "bonus de %d pontos		", temp);
			mvprintw(yMAX+1, 35, "                 	");
		}

		mapa[jogador->posY - 1 ][jogador->posX - 1].obs = VAZIO;
		mapa[jogador->posY - 1 ][jogador->posX - 1].acessivel = 1;
	}   

    else if(mapa[jogador->posY - 1 ][jogador->posX + 0].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX,35,"vida máxima			");
			mvprintw(yMAX+1, 35, "                 	");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX, 35, "bonus de %d pontos		", temp);
			mvprintw(yMAX+1, 35, "                 	");
		}

		mapa[jogador->posY - 1 ][jogador->posX + 0].obs = VAZIO;
		mapa[jogador->posY - 1 ][jogador->posX + 0].acessivel = 1;
	}   

	else if(mapa[jogador->posY - 1 ][jogador->posX + 1].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX,35,"vida máxima			");
			mvprintw(yMAX+1, 35, "                 	");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX, 35, "bonus de %d pontos		", temp);
			mvprintw(yMAX+1, 35, "                 	");
		}

		mapa[jogador->posY - 1 ][jogador->posX + 1].obs = VAZIO;
		mapa[jogador->posY - 1 ][jogador->posX + 1].acessivel = 1;
	}   

    else if (mapa[jogador->posY + 0 ][jogador->posX - 1].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX,35,"vida máxima			");
			mvprintw(yMAX+1, 35, "                 	");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX, 35, "bonus de %d pontos		", temp);
			mvprintw(yMAX+1, 35, "                 	");
		}

		mapa[jogador->posY + 0 ][jogador->posX - 1].obs = VAZIO;
		mapa[jogador->posY + 0 ][jogador->posX - 1].acessivel = 1;
	}   

    else if (mapa[jogador->posY + 0 ][jogador->posX + 0].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX,35,"vida máxima			");
			mvprintw(yMAX+1, 35, "                 	");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX, 35, "bonus de %d pontos		", temp);
			mvprintw(yMAX+1, 35, "                 	");
		}

		mapa[jogador->posY + 0 ][jogador->posX + 0].obs = VAZIO;
		mapa[jogador->posY + 0 ][jogador->posX + 0].acessivel = 1;
	}   

    else if (mapa[jogador->posY + 0 ][jogador->posX + 1].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX,35,"vida máxima			");
			mvprintw(yMAX+1, 35, "                 	");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX, 35, "bonus de %d pontos		", temp);
			mvprintw(yMAX+1, 35, "                 	");
		}

		mapa[jogador->posY + 0 ][jogador->posX + 1].obs = VAZIO;
		mapa[jogador->posY + 0 ][jogador->posX + 1].acessivel = 1;
	}   

    else if (mapa[jogador->posY + 1 ][jogador->posX - 1].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX,35,"vida máxima			");
			mvprintw(yMAX+1, 35, "                 	");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX, 35, "bonus de %d pontos		", temp);
			mvprintw(yMAX+1, 35, "                 	");
		}

		mapa[jogador->posY + 1 ][jogador->posX - 1].obs = VAZIO;
		mapa[jogador->posY + 1 ][jogador->posX - 1].acessivel = 1;
	}   

    else if (mapa[jogador->posY + 1 ][jogador->posX + 0].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX,35,"vida máxima			");
			mvprintw(yMAX+1, 35, "                 	");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX, 35, "bonus de %d pontos		", temp);
			mvprintw(yMAX+1, 35, "                 	");
		}

		mapa[jogador->posY + 1 ][jogador->posX + 0].obs = VAZIO;
		mapa[jogador->posY + 1 ][jogador->posX + 0].acessivel = 1;
	}   

    else if (mapa[jogador->posY + 1 ][jogador->posX + 1].obs == BAU){
        
		temp=((rand()%5)*10);
		if(jogador->vida+temp>jogador->vidaMax){
			jogador->vida=jogador->vidaMax;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX,35,"vida máxima			");
			mvprintw(yMAX+1, 35, "                 	");
		}
		else{ 
			jogador->vida+=temp;
			mvprintw(yMAX-1, 35, "                 	");
			mvprintw(yMAX, 35, "bonus de %d pontos		", temp);
			mvprintw(yMAX+1, 35, "                 	");
		}

		mapa[jogador->posY + 1 ][jogador->posX + 1].obs = VAZIO;
		mapa[jogador->posY + 1 ][jogador->posX + 1].acessivel = 1;
	}   
}