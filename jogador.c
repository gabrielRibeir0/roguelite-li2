#include <stdlib.h>
#include "estado.h"
#include "jogador.h"
#include "mapa.h"
#include "monstros.h"

//função para incializar o jogador numa posição válida
int fazJogador(CASA **mapa, JOGADOR jogador, int yMAX, int xMAX){
	int posInvalida = 1;

	while(posInvalida){
		int y = rand() % yMAX;
		int x = rand() % xMAX;

		if(mapa[y][x].obs == VAZIO && mapa[y][x].acessivel == 1 && !(monstrosPerto(mapa, y , x, yMAX, xMAX)) ){
			jogador->posY = y;
			jogador->posX = x;

			posInvalida = 0;
		}
	}

	jogador->score = jogador->expAtual = 0;
	jogador->vida=50;
	jogador->vidaMax=50;
	jogador->lvl = 1;

	return 0;
}

void moverJogador(JOGADOR jogador, int dx, int dy, CASA destino){
	if(destino.acessivel == 1){
		jogador->posX += dx;
		jogador->posY += dy;
	}
}

void escreveJogador(JOGADOR jogador){
	mvaddch(jogador->posY, jogador->posX, 'G');
}

void danoTrap (CASA **mapa, JOGADOR jogador, int yMAX){
	if (mapa[jogador->posY][jogador->posX].obs == TRAP || mapa[jogador->posY][jogador->posX].obs == LAVA){
		if (jogador->vida<10);
		else{ 
			jogador->vida-=10;
			mvprintw(yMAX, 1, "HP: %d/%d", jogador->vida, jogador->vidaMax);
			mvprintw(yMAX, 35, "Patada na boca     	");
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