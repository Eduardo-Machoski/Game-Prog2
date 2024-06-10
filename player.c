#include"player.h"

//cria a estrutura guardando as informações do player e inicializa ela
player *cria_player(display_info *disp, int ini_x){

	//cria a estrutura e verifica a alocação de memoria
	player *aux;
        if(!(aux = malloc(sizeof(player))))
	       exit(1);

	//inicializa o player
	aux->side = disp->tam_x / 19;
	aux->height = disp->tam_x / 8;
	aux->vida = 100;
	aux->jump = false;
	aux->jump_height = VELOCIDADE_MAX_Y;

	//posicao horizontal inicial (ini_x% da tela)
	aux->x = disp->tam_x * (ini_x/100.0);

	//inicializa os players no chao)
	aux->y = disp->tam_y - disp->chao - aux->height/2;
	return aux;
}



//atualiza a posicao x e y dos players a partir de seus controles
void move_players(player *p1, player *p2, display_info  *disp, bool *keys){

	int half_side_p1 = p1->side/2;
	int half_side_p2 = p2->side/2;

	//atualiza a posicao x do player1
	p1->x += VELOCIDADE_X * (keys[4] - keys[1]);
	//verifica colisao do player1 com a borda da tela nas laterais
	if(p1->x < half_side_p1 || p1->x > disp->tam_x - half_side_p1)
		p1->x = half_side_p1 * keys[1] + (disp->tam_x - half_side_p1) * keys[4];

	
	//atualiza a posicao x do player2
	p2->x += VELOCIDADE_X * (keys[83] - keys[82]);
	//verifica colisao do player2 com a borda da tela nas lateraiss
	if(p2->x < half_side_p2 || p2->x > disp->tam_x - half_side_p2)
		p2->x = half_side_p2 * keys[82] + (disp->tam_x - half_side_p2) * keys[83];

	//verifica se o p1 esta saltando
	if(p1->jump){
		
		//atualiza posicao vertical de p1
		p1->y -= disp->tam_y * p1->jump_height;

		//verifica se o pulo foi encerrado
		if(p1->jump_height <= -(VELOCIDADE_MAX_Y - 0.005)){
			p1->jump_height = VELOCIDADE_MAX_Y;
			p1->y = disp->tam_y - (p1->height/2 + disp->chao);
			p1->jump = false;
		}

		//atualiza o valor do salto para efeito da gravidade
		p1->jump_height -= 0.01;
	} else if(keys[23] && p1->y == disp->tam_y - (p1->height/2 + disp->chao)) //p1 em posicao valida para saltar
		p1->jump = true;

	//verifica se o p2 esta saltando
	if(p2->jump){

		//atualiza posicao vertical de p2
		p2->y -= disp->tam_y * p2->jump_height;
		if(p2->jump_height <= -(VELOCIDADE_MAX_Y - 0.005)){
			p2->jump_height = VELOCIDADE_MAX_Y;
			p2->y = disp->tam_y - (p2->height/2 + disp->chao);
			p2->jump = false;
		}

		//atualiza o valor do salto para efeito da gravidade
		p2->jump_height -= 0.01;
	} else if(keys[84] && p2->y == disp->tam_y - (p2->height/2 + disp->chao)) //p2 em posicao valida para saltar
		p2->jump = true;


	return;
}

//destroi um player e seus componentes
void destroy_player(player *elem){
	free(elem);
}
