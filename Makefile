all: snake

snake: snake.c
	gcc snake.c -o snake

clean:
	rm -f snake