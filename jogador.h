typedef struct jogador {
	int posX;
	int posY;
    int 
} JOGADOR;

void mover_jogador(JOGADOR *jg, int dx, int dy) {
	jg->playerX += dx;
	jg->playerY += dy;
}