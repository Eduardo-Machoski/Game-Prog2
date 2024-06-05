#include<stdlib.h>
#include<stdio.h>

#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_primitives.h>

#include"player.h"
#include"controle.h"

int main(){
	//inicializa os componentos do allegro
	al_init();
	al_init_primitives_addon();
	al_install_keyboard();

	//cria os componentes do allegro
	ALLEGRO_TIMER* timer = al_create_timer(1.0/30.0);
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_FONT* font = al_create_builtin_font();
	ALLEGRO_DISPLAY* disp = al_create_display(640,640);

	//regista os eventos que serao colocados na queue
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	//começa o relogio do programa
	al_start_timer(timer);


	//cria amobos os players
	player *player_1 = cria_player(25, 20, 20);
	player *player_2 = cria_player(25, 100, 100);

	//evento atual sendo lidado no loop
	ALLEGRO_EVENT event;

	//codigo do evento atual
	int code;

	//roda até que o programa seja encerrado
	while(1){
		//aguarda e obtem o proximo evento, assim como seu codigo
		al_wait_for_event(queue, &event);
		code = event.type;

		if(code == 30){
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_filled_rectangle(player_1->x-player_1->side/2, player_1->y-player_1->side/2, player_1->x+player_1->side/2, player_1->y+player_1->side/2, al_map_rgb(255,0,0));
			al_draw_filled_rectangle(player_2->x-player_2->side/2, player_2->y-player_2->side/2, player_2->x+player_2->side/2, player_2->y+player_2->side/2, al_map_rgb(0,0,255));
			al_flip_display();
		
		} else if (code == 10 || code == 12){
			int key = event.keyboard.keycode;
			switch(key){
				case(1):
					flip_left(player_1->control);
				break;
				case(19):
					flip_down(player_1->control);
				break;
				case(4):
					flip_right(player_1->control);
				break;
				case(23):
					flip_up(player_1->control);
				break;
				case(82):
					flip_left(player_2->control);
				break;
				case(85):
					flip_down(player_2->control);
				break;
				case(83):
					flip_right(player_2->control);
				break;
				case(84):
					flip_up(player_2->control);
				break;
				default:
				break;
			}
		} else if(code == 42)
			break;
		move_players(player_1, player_2, 640, 640);
	}

	//destroi os jogadores e seus componentes
	destroy_player(player_1);
	destroy_player(player_2);

	//destroi os componentes do allegro
	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_uninstall_keyboard();
	al_uninstall_system();

	return 0;
}
