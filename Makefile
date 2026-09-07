WARNINGS = -Wall

all: main.o pintor.o bmp.o
	gcc main.o pintor.o bmp.o -o main -lSDL2

main.o: main.c pintor.h bmp.h
	gcc $(WARNINGS) -c main.c

pintor.o: pintor.c pintor.h
	gcc $(WARNINGS) -c pintor.c

bmp.o: bmp.h bmp.c
	gcc $(WARNINGS) -c bmp.c

clean:
	rm -f *.o main