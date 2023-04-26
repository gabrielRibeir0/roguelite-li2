#include <stdlib.h>
#include <time.h>
#include "estado.h"
#include "jogador.h"
#include "mapa.h"

//função para incializar o jogador numa posição válida
int fazJogador(CASA **mapa, JOGADOR jogador, int yMAX, int xMAX){
	int posInvalida = 1;

	while(posInvalida){
		int y = rand() % yMAX;
		int x = rand() % xMAX;
		printf("[%d / %d / %d]", x,y,   mapa[y][x].acessivel);
		if(mapa[y][x].obs == VAZIO && mapa[y][x].acessivel == 1){
			jogador->posY = y;
			jogador->posX = x;

			posInvalida = 0;
		}
	}

	jogador->score = 0;

	return 0;
}

void moverJogador(JOGADOR jogador, int dx, int dy, CASA destino){
	if(destino.obs == VAZIO){
		jogador->posX += dx;
		jogador->posY += dy;
	}
}

void escreveJogador(JOGADOR jogador){
	mvaddch(jogador->posY, jogador->posX, '@');
}