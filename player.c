#include"player.h"

//cria a estrutura guardando as informações do player e inicializa ela
player *cria_player(int tam, int ini_x, int ini_y){

	//cria a estrutura e verifica a alocação de memoria
	player *aux;
        if(!(aux = malloc(sizeof(player))))
	       exit(1);

	//inicializa o player
	aux->side = tam/2;
	aux->x = ini_x;
	aux->y = ini_y;
	aux->vida = 100;
	return aux;
}



//atualiza a posicao x e y dos players a partir de seus controles
void move_players(player *p1, player *p2, display_info  *disp, bool *keys){

	//atualiza a posicao x do player1
	p1->x += VELOCIDADE * (keys[4] - keys[1]);
	p1->y += VELOCIDADE;
	//verifica colisao do player1 com a borda da tela nas laterais
	if(p1->x < p1->side/2 || p1->x > disp->tam_x - p1->side/2)
		p1->x = p1->side/2 * keys[1] + (disp->tam_x - p1->side/2) * keys[4];

	if(p1->y > disp->tam_y - (p1->side/2 + disp->chao))
		p1->y = disp->tam_y - (p1->side/2 + disp->chao);
	
	//atualiza a posicao x do player2
	p2->x += VELOCIDADE * (keys[83] - keys[82]);
	//verifica colisao do player2 com a borda da tela nas lateraiss
	if(p2->x < p2->side/2 || p2->x > disp_x - p2->side/2)
		p2->x = p2->side/2 * keys[82] + (disp_x - p2->side/2) * keys[83];




	return;
}

//destroi um player e seus componentes
void destroy_player(player *elem){
	free(elem);
}
