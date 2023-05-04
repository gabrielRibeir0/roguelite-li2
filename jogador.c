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

	jogador->score = jogador->vida = jogador->vidaMax = jogador->expAtual = 0;
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