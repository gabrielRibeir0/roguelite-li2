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
    int precisao;
    int ataque;
    int defesa;
} *JOGADOR;

typedef struct monstro{
    int posX;
    int posY;
    int spawnX;
    int spawnY;
    int vida;
    int vidaMax;
    int precisao;
    int ataque;
    int defesa;
} MONSTRO;
//DADOS DO MAPA
//tipo de dados para representar obstáculos que uma casa pode ter
typedef enum obs{
    MURO, //a casa pode ter um muro
    VAZIO, //não ter obstáculos (pode ter um mob na mesma, não quer dizer que está totalmente vazio)
    TRAP,
    BAU,
    LAVA,
    TOCHA,
    ESCADA,
} OBSTACULO;

//uma casa do mapa tem um obstaculo (ou falta dele), um tipo de terreno, um tipo de ambiente e flags que dizem se é visivel e se é acessivel
//->POR os mobs e loot nos obstaculos e assim desaparece o parametro do atravessavel (?)
typedef struct casa{
    OBSTACULO obs;
    int visivel;
    int acessivel;
} CASA;
// um mapa é um array de 2 dimensões de casas -> CASA mapa[linhas][colunas]

#endif