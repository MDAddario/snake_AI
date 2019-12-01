all: snake hamilton

snake: snake.c
	gcc snake.c -o snake

hamilton: hamilton.c
	gcc hamilton.c -o hamilton

clean:
	rm -f snake hamilton
