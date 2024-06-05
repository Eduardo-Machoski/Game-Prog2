#ifndef __CONTROLE__
#define __CONTROLE__ 

#include<stdlib.h>

typedef struct {
	char up;
	char down;
	char left;
	char right;
} controle;

//cria o controle de movimento do personagem e o inicializa
controle *cria_controle();

//flip no controle do up
void flip_up(controle *control);

//flip no controle do down
void flip_down(controle *control);

//flip no controle do left
void flip_left(controle *control);

//flip no controle do right
void flip_right(controle *control);

#endif
