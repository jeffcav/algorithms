#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"

static void ordenar_arestas(struct grafo *g, struct aresta **ord)
{
	int i, j, k, N;
	struct aresta *s;
	
	N = g->N;
	
	/*
	 * Montar vetor desordenado de arestas
	 */
	k = 0;
	for (i = 0; i < N; i++) {
		for (j = i; j < N; j++) {
			if (i == j) {
				continue;
			} else { //:)
				ord[k++] = &g->vertices[i].arestas[j];
			}
		}
	}
	
	/*
	 * Ordenar vetor de arestas
	 */
	for (i = 0; i < (N - 1); i++) {
		for (j = i+1; j < N; j++) {
			if (ord[j]->peso < ord[i]->peso) {
				s = ord[i];
				ord[i] = ord[j];
				ord[j] = s;
			}
		}
	}
}

static void criar_conjunto(struct conjunto* c, struct vertice *v)
{
	c->vert = v;
	c->pai = c;
	c->pos = 0;
}

static struct conjunto * raiz(struct conjunto *c)
{
	struct conjunto *k = c;
	while (k != k->pai) {
		k = k->pai;
	}
	return k;
}

int main(int argc, char *argv[])
{
	struct grafo g;
	int N, i, j;
	struct conjunto *conjuntos;
	struct aresta **ordenada;
	
	scanf("%d", &N);
	
	conjuntos = (struct conjunto*) malloc(N * sizeof(struct conjunto));
	
	/*
	 * Inicializando o grafo como listas de adjacências
	 */
	g.N = N;
	g.vertices = (struct vertice*) malloc(N * sizeof(struct vertice));
	for (i = 0; i < N; i++) {
		g.vertices[i].nome = i;
		g.vertices[i].arestas = (struct aresta*) malloc(N * sizeof(struct aresta));
	}
	
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			g.vertices[i].arestas[j].origem = &g.vertices[i];
			g.vertices[i].arestas[j].destino = &g.vertices[j];
			scanf("%d", &g.vertices[i].arestas[j].peso);
		}
	}
	
	/*
	 * Algoritmo de kruskal para encontrar uma AGM
	 */
	for (i = 0; i < N; i++) {
		criar_conjunto(&conjuntos[i], &g.vertices[i]);
	}
	
	ordenada =  (struct aresta**) malloc(N * sizeof(struct aresta*));
	ordenar_arestas(&g, ordenada);
	
	
	for (i = 0; i < N; i++) {
		if (raiz(&conjuntos[ordenada[i]->origem->nome]) != raiz(&conjuntos[ordenada[i]->destino->nome])) {
			printf("diff\n");
		}
	}
	
	/*
	 * Desalocando memória
	 */
	for (i = 0; i < N; i++) {
		free(g.vertices[i].arestas);
	}
	free(g.vertices);
	free(conjuntos);
	free(ordenada);
	
	return 0;
}
