#include"gui.h"

//cria um menu e inicializa suas informacoes
menus *cria_menu(int tam){
	
	//cria o menu e verifica o alocamento de memoria
	menus *aux;
	if(!(aux = malloc(sizeof(menus))))
		return NULL;

	//cria o vetor de codigos e verifica o alocamento de memoria
	int *codes;
	if(!(codes = calloc(0, sizeof(int) * tam))){
		free(aux);
		return NULL;
	}

	//cria o vetor de strings e verifica o alocamento de memoria
	char **strings;
	if(!(strings = (char**)malloc(sizeof(char*) * tam))){
		free(aux);
		free(codes);
		return NULL;
	}

	//inicializa o menu e o retorna
	aux->opcoes = tam;
	aux->codes = codes;
	aux->atual = 0;
	aux->strings = strings;
	return aux;

}

//pausa o jogo, removendo todos os inputs ainda nao processados
bool pause_gui(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_FONT *font, display_info *disp){

	//limpa a queue de eventos
	al_flush_event_queue(queue);

	//cria o menu que sera usado no display
	menus *m;
	if(!(m = cria_menu(2)))
		return;

	//cria as strings do menu
	char fala1[] = "Return Game";
	char fala2[] = "Exit Game";

	//inicializa as strings do menu
	m->strings[0] = fala1;
	m->strings[1] = fala2;

	//inicializa os codigos
	m->codes[0] = RETURN_GAME;
       	m->codes[1] = MAIN_MENU;
	
	//mostra o menu e realiza a operacao desejada pelo usuario
	bool aux = display_menu(m, disp, font, queue);

	//destroi o menu apos o seu uso
	destroy_menu(m);

	return aux;
}

//imprime ambos os players na tela
void imprime_players(player *p1, player *p2){
	al_draw_filled_rectangle(p1->x - p1->side/2, p1->y + p1->height/2, p1->x + p1->side/2, p1->y - p1->height/2, al_map_rgb(0, 0, 255));
	al_draw_filled_rectangle(p2->x - p2->side/2, p2->y + p2->height/2, p2->x + p2->side/2, p2->y - p2->height/2, al_map_rgb(255, 0, 0));
}

//destroi um menu e seus componentes
void destroy_menu(menus *m){
	free(m->strings);
	free(m->codes);
	free(m);
}
