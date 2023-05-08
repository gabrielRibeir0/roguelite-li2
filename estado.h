#include <ncurses.h>

#ifndef ESTADO_H
#define ESTADO_H

//DADOS DO JOGADOR
typedef struct jogador{
	int posX; //posição X
	int posY; //posição Y
    int score; //score
    int vida; //vida no momento
    int vidaMax; //vida max
    int expAtual; //exp no momento
    int lvl; //nivel do jogador
} *JOGADOR;

typedef struct monst{
    int posX;
    int posY;
    int vida;
} *MONSTRO;
//DADOS DO MAPA
//tipo de dados para representar obstáculos que uma casa pode ter
typedef enum{
    MURO, //a casa pode ter um muro
    VAZIO, //não ter obstáculos (pode ter um mob na mesma, não quer dizer que está totalmente vazio)
    TRAP,
    BAU,
    LAVA
} OBSTACULO;

//tipo para representar o tipo de terreno da casa
typedef enum{
    PEDRA
} TERRENO;

//tipo para representar o ambiente da casa (que é o mesmo para as outras casas, ou pelo menos algumas casas à volta)
typedef enum{
    ABERTO,
    CAVERNA
} AMBIENTE;

//uma casa do mapa tem um obstaculo (ou falta dele), um tipo de terreno, um tipo de ambiente e flags que dizem se é visivel e se é acessivel
//->POR os mobs e loot nos obstaculos e assim desaparece o parametro do atravessavel (?)
typedef struct casa{
    OBSTACULO obs;
    TERRENO terr;
    AMBIENTE ambiente;
    int visivel;
    int acessivel;
    int temMonstro;
    MONSTRO monstro;
} CASA;
// um mapa é um array de 2 dimensões de casas -> CASA mapa[linhas][colunas]

#endif