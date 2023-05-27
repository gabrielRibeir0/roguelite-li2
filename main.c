#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "estado.h"
#include "jogador.h"
#include "mapa.h"
#include "monstros.h"
#include "combate.h"

/*
a103993 - Júlia Costa
a104532 - Tomás Sousa Barbosa
a104274 - João Miguel
Função que desenha o menu
*/
int desenharMenu(int yMAX, int xMAX, char tipo){

	WINDOW * janela = newwin(yMAX,xMAX,0,0);
	box(janela,0,0);
	wrefresh(janela);
	keypad(janela,true);
	nodelay(janela, false);
	cbreak();
	start_color();

	init_color(COLOR_BLACK, 0,0,0);
	init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);

	char escolha[2][6]={"JOGAR","SAIR"}; //string?
	int pick;                                                            
	int atual=0;

	while(1){

		for(int i=0;i<2;i++){
			if(i==atual) wattron(janela,A_REVERSE);
			mvwprintw(janela, i+(yMAX/2),(xMAX/2)-2,"%s", escolha[i]);
			wattroff(janela,A_REVERSE);
			wattron(janela,A_BOLD);
			wattron(janela,COLOR_PAIR(COLOR_RED)); 
			if(tipo == 'm') mvwprintw(janela, (yMAX/2)-11, (xMAX/2)-4,"Morreste!");
			wattroff(janela,COLOR_PAIR(COLOR_RED));
			mvwprintw(janela, (yMAX/2)+3 ,(xMAX/2)-18,"Pressione ESPACO para escolher a opcão");
			mvwprintw(janela, (yMAX/2)-9 ,(xMAX/2)-8,"Comandos do jogo:");
			mvwprintw(janela, (yMAX/2)-7 ,(xMAX/2)-8,"Mover : Setas");
			mvwprintw(janela, (yMAX/2)-6 ,(xMAX/2)-8,"Abrir Bau : ESPACO");
			mvwprintw(janela, (yMAX/2)-5 ,(xMAX/2)-8,"Atacar : A");
			mvwprintw(janela, (yMAX/2)-4 ,(xMAX/2)-8,"Fugir da luta : R");
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
				clear();
				endwin();
				return atual;
			default:
				break;
		}

		if(pick==10) break;

	}
	clear();
	endwin();
	return 1;
}

/*
a104171 - Gabriel Pereira Ribeiro
O jogo em si - inicialização das coisas e processamento dos acontecimentos
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
				break;
			case 27:
				return 0;
				break;
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
			jogador->score++;
			return gameLoop(mapa, listaMonstros, jogador, yMAX, xMAX); //dar return do que acontece no prox nível
		}
	}

	return 0; //o jogador morreu
}

/*
a104171 - Gabriel Pereira Ribeiro
a104532 - Tomás Sousa Barbosa
Função main - declaração das coisas e loop dos menus
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

	MONSTRO *listaMonstros = NULL;

	int opcMenu = desenharMenu(yMAX+3, xMAX, 'i');
	while(opcMenu == 0){
		iniciaJogador(jogador);
		gameLoop(mapa, listaMonstros, jogador, yMAX, xMAX);
		opcMenu = desenharMenu(yMAX+3, xMAX, 'm');
	}

	free(jogador);
	free(listaMonstros);
	for(int i = 0; i < yMAX; i++){
		free(mapa[i]);
	}
	
	endwin();
	return 0;
}	
