#include "estado.h"

#ifndef MAPA_H
#define MAPA_H

void iniciarMapa(CASA **mapa, int yMAX, int xMAX);

int contarObstaculo(CASA **mapa, int y, int x, OBSTACULO obst);

int compactaMapa(CASA **mapa, int yMAX, int xMAX, int fase);

void verificaAcesso(CASA **mapa, int y, int x,int *nAcessiveis);

char obstac(int y, int x, CASA **mapa); //tmp

int gerarObjetos(CASA **mapa, int yMAX, int xMAX);

void escreveMapa(CASA **mapa, MONSTRO *listaMonstros, int yMAX , int xMAX, int nMonstros);

void linhaVisao(CASA **mapa, int xAtual, int yAtual, int xDestino, int yDestino);

int calcularVisivel(CASA **mapa, JOGADOR jogador, int yMAX, int xMAX);

#endif