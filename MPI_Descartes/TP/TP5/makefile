CFLAGS=-Wall -pedantic -std=c99 -fsanitize=address,undefined

build: TP5_enonce.c dicos.c
	gcc $(CFLAGS) -o TP5 dicos.c TP5_enonce.c -lm

