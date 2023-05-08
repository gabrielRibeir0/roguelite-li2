#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include "estado.h"
#include "jogador.h"
#include "mapa.h"
#include "monstros.h"

/**
 *
 * Falta muita coisa, incluindo por exemplo:
 * - o mapa
 * - os monstros
 * - a fila de prioridade para saber quem se move
 * - o que está em cada casa
 *
 */
/*typedef struct state {
	int playerX;
	int playerY;
	struct Mapa;
	int scoreMax;
} STATE;


*
 *
 * Um pequeno exemplo que mostra o que se pode fazer
 *
void do_movement_action(STATE *st, int dx, int dy) {
	st->playerX += dx;
	st->playerY += dy;
}

void update(STATE *st) {
	int key = getch();

	mvaddch(st->playerX, st->playerY, ' ');
	switch(key) {
		case KEY_A1:
		case '7': do_movement_action(st, -1, -1); break;
		case KEY_UP:
		case '8': do_movement_action(st, -1, +0); break;
		case KEY_A3:
		case '9': do_movement_action(st, -1, +1); break;
		case KEY_LEFT:
		case '4': do_movement_action(st, +0, -1); break;
		case KEY_B2:
		case '5': break;
		case KEY_RIGHT:
		case '6': do_movement_action(st, +0, +1); break;
		case KEY_C1:
		case '1': do_movement_action(st, +1, -1); break;
		case KEY_DOWN:
		case '2': do_movement_action(st, +1, +0); break;
		case KEY_C3:
		case '3': do_movement_action(st, +1, +1); break;
		case 'q': endwin(); exit(0); break;
	}
}*/

int main() {
	//inicializar coisas
	srand(time(NULL));
	WINDOW *wnd = initscr();
	int yMAX, xMAX, nVazias, nAcessiveis = 0;
	getmaxyx(wnd,yMAX,xMAX);

	yMAX -= 3;
	//inicializar mapa
	CASA *mapa[yMAX];
	for(int i = 0; i < yMAX; i++){
		mapa[i] = malloc(sizeof(CASA) * xMAX);
	}

	//enquanto pelo menos 70% das casas vazias não forem acessíveis, continua a gerar um mapa
	do{
		iniciarMapa(mapa, yMAX,xMAX);
		for(int i = 0; i < 4; i++){
			compactaMapa(mapa, yMAX, xMAX, 1);
		}
		for(int i = 0; i < 3; i++){
			nVazias = compactaMapa(mapa, yMAX, xMAX, 2);
		}

		int yRAND = rand() % yMAX;
		int xRAND = rand() % xMAX;
		while(mapa[yRAND][xRAND].obs != VAZIO){
			yRAND = rand() % yMAX;
			xRAND = rand() % xMAX;
		}
		verificaAcesso(mapa, yRAND, xRAND, &nAcessiveis);
	} while((nAcessiveis)<=(nVazias*0.7));
	gerarObjetos(mapa,yMAX,xMAX);
	iniciaMonstros(mapa, 1, yMAX, xMAX);
	
	//cria o jogador
	JOGADOR jogador = malloc(sizeof(struct jogador));
	fazJogador(mapa, jogador, yMAX, xMAX);

	start_color();
	cbreak();
	noecho();
	nonl();
	curs_set(0);
	intrflush(stdscr, false);
	keypad(stdscr, true);

    use_default_colors();
    init_pair(1, COLOR_RED, -1);
	init_pair(2, COLOR_BLACK, -1); 
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(COLOR_CYAN, COLOR_GREEN, COLOR_BLACK);

	//escrever o inicio e loop
	
	calcularVisivel(mapa,jogador,yMAX,xMAX);
	escreveMapa(mapa,jogador,yMAX,xMAX);
	escreveJogador(jogador);	

	int input;
	while(1){
		move(jogador->posY, jogador->posX);
		
		input = getch();
		if(input == 27)
			break;

		switch(input){
			case KEY_UP:
				moverJogador(jogador, 0, -1, mapa[jogador->posY - 1][jogador->posX]);
				break;
			case KEY_DOWN:
				moverJogador(jogador, 0, 1, mapa[jogador->posY + 1][jogador->posX]);
				break;
			case KEY_RIGHT:
				moverJogador(jogador, 1, 0, mapa[jogador->posY][jogador->posX + 1]);
				break;
			case KEY_LEFT:
				moverJogador(jogador, -1, 0, mapa[jogador->posY][jogador->posX - 1]);
				break;
			case ' ':
				abreBau(mapa,jogador,yMAX);
			default:
				break;		
		}
		moveMonstros(mapa,jogador,yMAX,xMAX);
		calcularVisivel(mapa,jogador,yMAX,xMAX);
		escreveMapa(mapa,jogador,yMAX,xMAX);
		escreveJogador(jogador);
		danoTrap(mapa,jogador,yMAX);
		//moveMonstros(mapa,jogador,yMAX,xMAX);

		if(jogador->vida<=0){
			clear();
			mvaddstr(yMAX/2,xMAX/2,"NAO");
		}
	}


	return 0;
}
