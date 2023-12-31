CC = gcc
CFLAGS = -Wall
ARTISTAS = Andres_Calamaro Canciones_para_ninos Diego_Torres Fabiana_Cantilo Fito_Paez Luis_Spinetta
SALIDAS = Salidas Entradas *.out

all: program

program: main.c
	$(CC) $(CFLAGS) -o main.out main.c

test: program
	@for artista in $(ARTISTAS); do \
		./main.out $$artista; \
	done

clean:
	rm -rf $(SALIDAS)
