all:
	gcc main.c -Wall -o main
run:
	./main < grafo.in
clean:
	rm -rf *.o main || true