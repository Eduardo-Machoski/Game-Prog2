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
	aux->control = cria_controle();
	return aux;
}

//atualiza a posicao x e y dos players a partir de seus controles
void move_players(player *p1, player *p2, int disp_x, int disp_y){

	//atualiza a posicao x do player1
	p1->x += VELOCIDADE * (p1->control->right - p1->control->left);
	p1->y += VELOCIDADE * (p1->control->down - p1->control->up);
	
	//verifica colisao do player1 com a borda da tela
	if(p1->x < p1->side/2 || p1->x > disp_x - p1->side/2)
		p1->x = p1->side/2 * p1->control->left + (disp_x - p1->side/2) * p1->control->right;
	if(p1->y < p1->side/2 || p1->y > disp_y - p1->side/2)
		p1->y = p1->side/2 * p1->control->up + (disp_y - p1->side/2) * p1->control->down;
	return;
}

//destroi um player e seus componentes
void destroy_player(player *elem){
	free(elem->control);
	free(elem);
}
