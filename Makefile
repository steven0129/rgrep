CC = gcc

rgrep: main.c
	$(CC) main.c -o rgrep
clean:
	rm ./rgrep