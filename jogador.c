#include <stdlib.h>
#include <time.h>
#include "estado.h"
#include "jogador.h"
#include "mapa.h"

/*int fazJogador(CASA **mapa,int yMAX,int xMAX){
	for(int i=0;i<1;){
		if(mapa[int a=rand()%yMAX][int b=rand()%xMAX].obs==VAZIO){
			JOGADOR->posY=a;
			JOGADOR->postX=b
			i++;
		}
	}
	JOGADOR->score=0;
}*/

void mover_jogador(JOGADOR jg, int dx, int dy) {
	jg->posX += dx;
	jg->posY += dy;
}