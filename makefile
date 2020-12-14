#GRR20190485 Gustavo Henrique da Silva Barbosa
 
CFLAGS = -Wall
LDFLAGS = -lm
DEBUG_FLAGS = -g
COMPILER = -std=gnu99
ALLEGRO_FLAGS = $$(pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags)

all: main

main: main.o states.o display.o game.o gameBase.o file.o
	gcc -o battleCity main.o states.o display.o game.o gameBase.o file.o $(ALLEGRO_FLAGS) $(CFLAGS) $(LDFLAGS) $(COMPILER) $(DEBUG_FLAGS)

main.o: main.c states.h
	gcc -c main.c -o main.o $(CFLAGS)

states.o: states.c states.h display.h game.h
	gcc -c states.c $(CFLAGS)

display.o: display.c display.h gameObject.h
	gcc -c display.c $(CFLAGS) $(ALLEGRO_FLAGS)

game.o: game.c game.h gameObject.h gameBase.h file.h
	gcc -c game.c $(CFLAGS)

gameBase.o: gameBase.c gameBase.h
	gcc -c gameBase.c $(CFLAGS)

file.o: file.c file.h
	gcc -c file.c $(CFLAGS)

clean:
	-rm -f *~ *.o
	
purge: clean
	-rm -f battleCity