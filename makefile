all:
	gcc main.c -Wall -o main
clean:
	rm -rf *.o main || true