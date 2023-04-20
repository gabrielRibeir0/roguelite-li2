#include <estado.h>
#include <jogador.h>

void mover_jogador(JOGADOR *jg, int dx, int dy) {
	jg->posX += dx;
	jg->posY += dy;
}