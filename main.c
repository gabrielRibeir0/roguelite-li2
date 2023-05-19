#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include "estado.h"
#include "jogador.h"
#include "mapa.h"
#include "monstros.h"

int main() {
	//inicializar coisas
	srand(time(NULL));
	WINDOW *wnd = initscr();
	int yMAX, xMAX;
	getmaxyx(wnd, yMAX, xMAX);
	start_color();
	cbreak();
	noecho();
	nonl();
	nodelay(stdscr, true);
	curs_set(0);
	intrflush(stdscr, false);
	keypad(stdscr, true);

    use_default_colors();
	init_color(COLOR_MAGENTA, 545, 270, 74);
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(1, COLOR_RED, -1);
	init_pair(2, COLOR_BLACK, -1); 
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(COLOR_CYAN, COLOR_GREEN, COLOR_BLACK);

	yMAX -= 3;
	//inicializar mapa
	CASA *mapa[yMAX];
	for(int i = 0; i < yMAX; i++){
		mapa[i] = malloc(sizeof(CASA) * xMAX);
	}
	
	int nMonstros = 0, nVazias = 0, nAcessiveis = 0;
	MONSTRO *listaMonstros = NULL;

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
	} while(nAcessiveis <= (nVazias * 0.7));

	nMonstros = iniciaMonstros(mapa, &listaMonstros, 1, yMAX, xMAX);
	gerarObjetos(mapa,yMAX,xMAX);
	
	//cria o jogador
	JOGADOR jogador = malloc(sizeof(struct jogador));
	fazJogador(mapa, listaMonstros, jogador, yMAX, xMAX, nMonstros);

	//escrever o inicio e loop
	
	calcularVisivel(mapa, jogador, yMAX, xMAX);
	escreveMapa(mapa, jogador, yMAX, xMAX);
	escreveJogador(jogador);	

	int input;
	double ultimoTempoMov = -1.0;
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
		moveMonstros(mapa, listaMonstros, jogador, nMonstros, &ultimoTempoMov);
		verificaCombate(jogador, listaMonstros, nMonstros, yMAX);
		calcularVisivel(mapa, jogador, yMAX, xMAX);
		escreveMapa(mapa, jogador, yMAX, xMAX);
		escreveJogador(jogador);
		danoTrap(mapa, jogador, yMAX);

		if(jogador->vida<=0){
			clear();
			mvaddstr(yMAX/2,xMAX/2,"NAO");
		}
	}


	return 0;
}
