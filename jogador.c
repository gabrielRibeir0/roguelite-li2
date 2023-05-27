#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include "estado.h"
#include "jogador.h"
#include "mapa.h"
#include "monstros.h"

/*
a104171 - Gabriel Pereira Ribeiro
Função que inicializa os atributos do jogador
*/
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

/*
a104171 - Gabriel Pereira Ribeiro
Função para inicializar a posição do jogador
*/
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
/*
a104532 - Tomás Sousa Barbosa
Função que move o jogador se for possível
*/
void moverJogador(JOGADOR jogador, int dx, int dy, CASA destino){
	if((destino.acessivel == 1 && destino.obs != MURO) || destino.obs == VAZIO){
		jogador->posX += dx;
		jogador->posY += dy;
	}
}
/*
a104274 - João Miguel
Função que escreve o jogador
*/
void escreveJogador(JOGADOR jogador){
	mvaddch(jogador->posY, jogador->posX, 'G');
}

/*
a103993 - Júlia Costa
Função para as traps causarem dano
*/
void danoTrap (CASA **mapa, JOGADOR jogador, int yMAX){
	if (mapa[jogador->posY][jogador->posX].obs == TRAP){
			jogador->vida-=10;
			mvprintw(yMAX, 1, "HP: %d/%d", jogador->vida, jogador->vidaMax);
			mvprintw(yMAX+1, 35, "			     	");
			mvprintw(yMAX, 35, "Patada na boca     	");
			mvprintw(yMAX-1, 35, "			     	");
			mapa[jogador->posY][jogador->posX].obs=VAZIO;
			mapa[jogador->posY][jogador->posX].acessivel=1;
	}
}

/*
a104532 - Tomás Sousa Barbosa
Função para a lava dar dano
*/
void danoLava(CASA **mapa, JOGADOR jogador, int yMAX, double *ultimoTempo){
	double tempoAtual = clock() / CLOCKS_PER_SEC;
	if(*ultimoTempo < 0 || tempoAtual - *ultimoTempo >= 1.3){
		if(mapa[jogador->posY][jogador->posX].obs == LAVA && jogador->vida > 0){

			
				jogador->vida -= 5;
				mvprintw(yMAX+1, 35, "                                ");
				mvprintw(yMAX, 35, "Estás na lava, sai o mais rápido possível!         ");
				mvprintw(yMAX-1, 35, "                                ");
			
		}
		else
			mvprintw(yMAX, 35, "                                                 ");


		*ultimoTempo = tempoAtual;
	}
}

/*
a104274 - João Miguel
a103993 - Júlia Costa
Função para abrir os baús e dar o bónus de vida
*/
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