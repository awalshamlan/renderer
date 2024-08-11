build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lSDL2main -o renderer

run:
	./renderer

clean:
	rm renderer
