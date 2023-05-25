#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include "estado.h"
#include "jogador.h"
#include "mapa.h"
#include "monstros.h"
#include "combate.h"

int gameLoop(CASA **mapa, MONSTRO *listaMonstros, JOGADOR jogador, int yMAX, int xMAX){
	int nMonstros = 0, nVazias = 0, nAcessiveis = 0;

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

	nMonstros = iniciaMonstros(mapa, &listaMonstros, jogador->score, yMAX, xMAX);
	gerarObjetos(mapa,yMAX,xMAX);

	posicaoJogador(mapa, listaMonstros, jogador, yMAX, xMAX, nMonstros);

	//escrever o inicio e loop
	calcularVisivel(mapa, jogador, yMAX, xMAX, nMonstros);
	escreveMapa(mapa, listaMonstros, jogador, yMAX, xMAX, nMonstros);
	escreveJogador(jogador);	

	int input;
	double ultimoTempoMov = -1.0;
	double delayFugir = -1.0;

	while(jogador->vida > 0){
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
			/*case 'p':
				int opc = desenharMenu();
				if(opc == sair)
					return 1; //o jogador saiu do jogo
				else{
					clear();
					escrevemapa(); 
					escreveJOgador();
				}
				break;*/
			default:
				break;		
		}
		moveMonstros(mapa, listaMonstros, jogador, nMonstros, &ultimoTempoMov);
		calcularVisivel(mapa, jogador, yMAX, xMAX, nMonstros);
		escreveMapa(mapa, listaMonstros, jogador, yMAX, xMAX, nMonstros);
		escreveJogador(jogador);
		verificaCombate(jogador, listaMonstros, &nMonstros, yMAX, &delayFugir);
		danoTrap(mapa, jogador, yMAX);
		//escadaAcessivel(mapa, nMonstros); <- ainda para fazer
		
		if(mapa[jogador->posY][jogador->posX].obs == ESCADA){
			jogador->lvl++;
			return gameLoop(mapa, listaMonstros, jogador, yMAX, xMAX); //dar return do que acontece no prox nível
		}
	}

	return 0; //o jogador morreu
}

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

	init_color(COLOR_MAGENTA, 545, 270, 74); //castanho
	init_color(COLOR_BLACK, 0,0,0);
	init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
	init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK); 
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);

	yMAX -= 3;
	//inicializar mapa, jogador e listaMonstros
	CASA *mapa[yMAX];
	for(int i = 0; i < yMAX; i++){
		mapa[i] = malloc(sizeof(CASA) * xMAX);
	}
	
	JOGADOR jogador = malloc(sizeof(struct jogador));
	iniciaJogador(jogador);

	MONSTRO *listaMonstros = NULL;

	/*opcMenu = escreveMenu() inicial
	//opções jogar e sair
	//if opc == jogar{
		int res;
		while(1){
			res = gameLoop
			if(res == 0){
				opcMEnu = escreveMenu() morreu
				if(opcMEnu = jogar)
					fazJogador(mapa, listaMonstros, jogador, yMAX, xMAX, nMonstros);
				else 
					break;
			}
			else
				break;
	}
	*/

	free(jogador);
	free(listaMonstros);
	for(int i = 0; i < yMAX; i++){
		free(mapa[i]);
	}
	return 0;
}
