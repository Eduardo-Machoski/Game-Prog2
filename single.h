#ifndef __SINGLE__
#define __SINGLE__


#include<allegro5/allegro.h>
#include<stdlib.h>

#include"player.h"
#include"display.h"

#define SPRITES_BOSS 4
#define BOLA 1
#define OLHO 2
#define COLUNA 3

typedef struct{
	int *i_sprites; //indica o indice inicial de cada grupo de sprits em uma animacao
        int *sprite_atual; //vetor dos indices das sprites que o boss imprimiu por ultimo
        int *tempo_ciclo; //vetor do tempo de cada sprite
        int *num_sprites; //vetor do numero de sprites que o personagem possui
        ALLEGRO_BITMAP **bitmap; //vetor de bitmaps com todas as sprites do boss
        ALLEGRO_BITMAP **sprite; //vetor de bitmap com as ultimas sprites impressa do boss
        int vida; //vida atual
        int x; //pos x do boss
        int y; //pos y do boss
        int side; //tamanho do lado do hitbox
        int side_sprite; //tamanho do lado do hitbox da sprite
        int height; //altura do hitbox
        int height_sprite; //altura da hitbox na sprite
        int sprite_w; //width da sprite
        int sprite_h; //height da sprite
        bool olha_esquerda; //indica se a sprite esta olhando para a esquerda
        bool attack_done; //indica se um ataque ja foi concluido
        char attack; //indica se um ataque esta sendo realizado e qual (0 se nao)


} boss;

//main do modo single player
bool single_player(display_info *disp, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, ALLEGRO_BITMAP **background, bool keys[], player **p, bool *single, int dev_mode);

//cria a struct do boss e inicializa ela
boss *cria_boss(display_info *disp);

//destroi a struct do boss e todos os seus componentes
void destroy_boss(boss *elem);

#endif
