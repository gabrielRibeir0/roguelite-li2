#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "estado.h"
#include "jogador.h"
#include "mapa.h"
#include "monstros.h"
#include "combate.h"

//a103993
//a104532 - Tomás Sousa Barbosa
//a104274 - João Miguel
int desenharMenu(int yMAX, int xMAX){

	WINDOW * janela = newwin(yMAX,xMAX,0,0);
	box(janela,0,0);
	wrefresh(janela);
	keypad(janela,true);
	nodelay(janela, false);
	cbreak();

	char escolha[2][6]={"JOGAR","SAIR"}; //string?
	int pick;                                                            
	int atual=0;

	while(1){

		for(int i=0;i<2;i++){
			if(i==atual) wattron(janela,A_REVERSE);
			mvwprintw(janela, i+(yMAX/2),(xMAX/2)-2,"%s", escolha[i]);
			wattroff(janela,A_REVERSE);
			wattron(janela,A_BOLD);
			mvwprintw(janela, (yMAX/2)+3 ,(xMAX/2)-18,"Pressione ESPACO para escolher a opcão");
			wattroff(janela,A_BOLD);
			wattroff(janela,A_REVERSE);
		}

		pick=wgetch(janela);
		
		switch (pick){
			case KEY_UP:
				atual--;
				if(atual==-1) atual=0;
				break;

			case KEY_DOWN:
				atual++;
				if(atual==2) atual=1;
				break;
			case ' ': 
				endwin();
				return atual;
			default:
				break;
		}

		if(pick==10) break;

	}
	endwin();
	return 1;
}

/*
a104171 - Gabriel Pereira Ribeiro
*/
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
	double ultimoTempoLava = -1.0;

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
		danoLava(mapa, jogador, yMAX, &ultimoTempoLava);
		escadaAcessivel(mapa, yMAX, xMAX, nMonstros);
		
		if(mapa[jogador->posY][jogador->posX].obs == ESCADA){
			jogador->lvl++;
			return gameLoop(mapa, listaMonstros, jogador, yMAX, xMAX); //dar return do que acontece no prox nível
		}
	}

	return 0; //o jogador morreu
}

/*
a104171 - Gabriel Pereira Ribeiro
a104532 - Tomás Sousa Barbosa
*/
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

	int opcMenu = desenharMenu(yMAX+3, xMAX);
	if(opcMenu == 0){
		//int res;
		int opc = 0;
		while(opc == 0){
			 gameLoop(mapa, listaMonstros, jogador, yMAX, xMAX);
			//if(res == 0){
				opc = desenharMenu(yMAX+3, xMAX);
		}
			//else
			//	break;
		}
	

	free(jogador);
	free(listaMonstros);
	for(int i = 0; i < yMAX; i++){
		free(mapa[i]);
	}
	
	endwin();
	return 0;
}	
