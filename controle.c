#include"controle.h"


//cria o controle de movimento do personagem e o iniciliza
controle *cria_controle(){

	//cria o controle e verifica a alocacao de memoria
	controle *aux;
	if(!(aux = malloc(sizeof(controle))))
		exit(1);

	//inicializa o controle
	aux->up = 0;
	aux->down = 0;
	aux->left = 0;
	aux->right = 0;

	//retorna o controle criado
	return aux;
}

//flip no controle do up
void flip_up(controle *control){
	control->up = control->up ^ 1;
}

//flip no controle do down
void flip_down(controle *control){
	control->down = control->down ^ 1;
}

//flip no controle do left
void flip_left(controle *control){
	control->left = control->left^ 1;

}

//flip no controle do right
void flip_right(controle *control){
	control->right = control->right ^ 1;
}
