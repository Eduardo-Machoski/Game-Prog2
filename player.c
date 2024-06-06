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
void move_players(player *p1, player *p2, int disp_x, int disp_y, bool *keys){

	//atualiza a posicao do player1
	p1->x += VELOCIDADE * (keys[4] - keys[1]);
	p1->y += VELOCIDADE * (keys[19] - keys[23]);
	
	//verifica colisao do player1 com a borda da tela
	if(p1->x < p1->side/2 || p1->x > disp_x - p1->side/2)
		p1->x = p1->side/2 * keys[1] + (disp_x - p1->side/2) * keys[4];
	if(p1->y < p1->side/2 || p1->y > disp_y - p1->side/2)
		p1->y = p1->side/2 * keys[23] + (disp_y - p1->side/2) * keys[19];
	
	
	//atualiza a posicao do player2
	p2->x += VELOCIDADE * (keys[83] - keys[82]);
	p2->y += VELOCIDADE * (keys[85] - keys[84]);
	
	//verifica colisao do player2 com a borda da tela
	if(p2->x < p2->side/2 || p2->x > disp_x - p2->side/2)
		p2->x = p2->side/2 * keys[82] + (disp_x - p2->side/2) * keys[83];
	if(p2->y < p2->side/2 || p2->y > disp_y - p2->side/2)
		p2->y = p2->side/2 * keys[84] + (disp_y - p2->side/2) * keys[85];
	return;
}

//destroi um player e seus componentes
void destroy_player(player *elem){
	free(elem);
}
