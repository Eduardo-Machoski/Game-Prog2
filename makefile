allegro = `pkg-config --cflags --libs allegro-5 allegro_image-5 allegro_audio-5 allegro_font-5 allegro_primitives-5 allegro_ttf-5`
flags = -Wall 
nome = game

all: $(nome)

$(nome): main.o single.o display.o gui.o player.o
	gcc -o $(nome) main.o display.o gui.o player.o single.o $(flags) $(allegro)

main.o: main.c
	gcc -c main.c $(flags) $(allegro)

single.o: single.h single.c
	gcc -c single.c $(flags) $(allegro)

display.o: display.h display.c
	gcc -c display.c $(flags) $(allegro)

gui.o: gui.h gui.c
	gcc -c gui.c $(flags) $(allegro)

player.o: player.h player.c
	gcc -c player.c $(flags) $(allegro)


clean:
	rm -f *.o

purge:
	rm -f *.o *.gch $(nome)
