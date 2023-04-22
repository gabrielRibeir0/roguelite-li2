#include <ncurses.h>

#ifndef ESTADO_H
#define ESTADO_H

//TAMANHOS DO MAPA A DEFINIR
int yMAX = 45;
int xMAX = 189;

//DADOS DO JOGADOR
typedef struct jogador{
	int posX; //posição X
	int posY; //posição Y
    int score; //score
    //outros stats
} *JOGADOR;

//DADOS DO MAPA
//tipo de dados para representar obstáculos que uma casa pode ter
typedef enum{
    MURO, //a casa pode ter um muro
    VAZIO //não ter obstáculos (pode ter um mob na mesma, não quer dizer que está totalmente vazio)
} OBSTACULO;

//tipo para repretentar o tipo de terreno da casa
typedef enum{
    PEDRA,
    LAVA
} TERRENO;

//tipo para representar o ambiente da casa (que é o mesmo para as outras casas, ou pelo menos algumas casas à volta)
typedef enum{
    ABERTO,
    CAVERNA
} AMBIENTE;

//uma casa do mapa tem um obstaculo (ou falta dele), um tipo de terreno, um tipo de ambiente e uma flag que diz que dá para 
//atravessar (0 ou 1)
//->POR os mobs e loot nos obstaculos e assim desaparece o parametro do atravessavel (?)
typedef struct casa{
    OBSTACULO obs;
    TERRENO terr;
    AMBIENTE ambiente;
    int atravessavel;
    int visivel;
} CASA;
// um mapa é um array de 2 dimensões de casas -> CASA mapa[linhas][colunas]

#endif