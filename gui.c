#include"gui.h"

//pausa o jogo, removendo todos os inputs ainda nao processados
void pause_gui(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font){

	//limpa a queue de eventos
	al_flush_event_queue(queue);

	
}

//imprime ambos os players na tela
void imprime_players(player *p1, player *p2){
	al_draw_filled_rectangle(p1->x - p1->side/2, p1->y + p1->height/2, p1->x + p1->side/2, p1->y - p1->height/2, al_map_rgb(0, 0, 255));
	al_draw_filled_rectangle(p2->x - p2->side/2, p2->y + p2->height/2, p2->x + p2->side/2, p2->y - p2->height/2, al_map_rgb(255, 0, 0));
}
