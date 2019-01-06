CC = gcc

rgrep: main.c
	$(CC) main.c -o rgrep
test: main.c
	$(CC) -g main.c -o rgrep
clean:
	rm ./rgrep