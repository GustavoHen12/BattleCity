#GRR20190485 Gustavo Henrique da Silva Barbosa
 
CFLAGS = -Wall
LDFLAGS = -lm

ALLEGRO_FLAGS = $$(pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags)

all: main

main: main.o states.o display.o game.o
	gcc -o battleCity main.o states.o display.o game.o $(ALLEGRO_FLAGS) $(CFLAGS) $(LDFLAGS)

main.o: main.c states.h
	gcc -c main.c -o main.o $(CFLAGS)

states.o: states.c states.h display.h game.h
	gcc -c states.c $(CFLAGS)

display.o: display.c display.h
	gcc -c display.c $(CFLAGS) $(ALLEGRO_FLAGS)

game.o: game.c game.h
	gcc -c game.c $(CFLAGS)

clean:
	-rm -f *~ *.o
	
purge: clean
	-rm -f battleCity