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

static struct conjunto * raiz_conjunto(struct conjunto *u)
{
	struct conjunto *k = u;
	while (k != k->pai) {
		k = k->pai;
	}
	return k;
}

static void unir_conjuntos(struct conjunto *u, struct conjunto *v)
{
	struct conjunto *Ru = raiz_conjunto(u);
	struct conjunto *Rv = raiz_conjunto(v);
	
	if (Ru == Rv)
		return;
	if (Ru->pos > Rv->pos) {
		Rv->pai = Ru;
	} else {
		Ru->pai = Rv;
		if (Ru->pos == Rv->pos)
			Rv->pos += 1;
	}
}

int main(int argc, char *argv[])
{
	struct grafo g;
	int N, i, j, agm_size;
	
	struct conjunto *conjuntos;
	struct aresta **ordenada;
	struct aresta **AGM;
	
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
	
	/*
	 * MAKESET
	 */
	for (i = 0; i < N; i++) {
		criar_conjunto(&conjuntos[i], &g.vertices[i]);
	}
	
	/*
	 * ORDENAR ARESTAS
	 */
	ordenada =  (struct aresta**) malloc(N * sizeof(struct aresta*));
	ordenar_arestas(&g, ordenada);
	
	
	/*
	 * MONTAR SUBÁRVORES
	 */
	AGM = (struct aresta**) malloc(N * sizeof(struct aresta*));
	agm_size = 0;
	
	for (i = 0; i < N; i++) {
		if (raiz_conjunto(&conjuntos[ordenada[i]->origem->nome]) !=
			raiz_conjunto(&conjuntos[ordenada[i]->destino->nome])
		) {
			
			AGM[agm_size++] = ordenada[i];
			
			unir_conjuntos(&conjuntos[ordenada[i]->origem->nome], 
				       &conjuntos[ordenada[i]->destino->nome]);
		}
	}
	
	/*
	 * Imprimindo AGM
	 */
	for (i = 0; i < agm_size; i++) {
		printf("%d -> %d w=%d\n", AGM[i]->origem->nome, 
		       AGM[i]->destino->nome, AGM[i]->peso);
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
	free(AGM);
	
	return 0;
}
