allegro = `pkg-config --cflags --libs allegro-5 allegro_image-5 allegro_audio-5 allegro_font-5 allegro_primitives-5 allegro_ttf-5`
flags = -Wall 
nome = game

all: $(nome)

$(nome): main.o  player.o
	gcc -o $(nome) main.o  player.o $(flags) $(allegro)

main.o: main.c
	gcc -c main.c $(flags) $(allegro)

player.o: player.h player.c
	gcc -c player.c $(flags)

clean:
	rm -f *.o

purge:
	rm -f *.o *.gch $(nome)
