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
		for (j = 0; j < i; j++) {
			ord[k++] = &g->vertices[i].arestas[j];
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
	int N, i, j, k, agm_size;
	int origem, destino;
	
	struct conjunto *conjuntos;
	struct aresta **ordenada;
	struct aresta **AGM;
	
	struct aresta modificada;
	struct aresta *swap;
	
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
	 * Imprimindo AGM. 
	 * Nota: agm_size deve ser igual a N-1
	 */
	for (i = 0; i < agm_size; i++) {
		printf("%d -> %d w=%d\n", AGM[i]->origem->nome, 
		       AGM[i]->destino->nome, AGM[i]->peso);
	}
	
	for (i = 0; i < N; i++) {
		printf("aresta: %d, raiz: %d pai: %d\n", 
		       conjuntos[i].vert->nome,
			((struct conjunto*)raiz_conjunto(&conjuntos[i]))->vert->nome,
			conjuntos[i].pai->vert->nome
			);
	}
	
	/*
	 * Ler mudança de peso em aresta (5.22)
	 */
	scanf("%d", &origem);
	scanf("%d", &destino);
	scanf("%d", &modificada.peso);
	modificada.origem = &g.vertices[origem];
	modificada.destino = &g.vertices[destino];
	
	/*
	 * Identificar aresta que mudou no vetor de arestas ordenadas
	 */
	
	for (i = 0; i < N; i++) {
		if (ordenada[i]->peso > modificada.peso)
			break;
	}
	
	for (j = 0; j < N; j++) {
		if ((ordenada[j]->origem->nome == modificada.origem->nome) &&
			(ordenada[j]->destino->nome == modificada.destino->nome)
		)
			break;
	}
	
	/*
	 * Reposicionar aresta modificada no vetor de arestas ordenadas
	 */
	
	if (i == j)
		goto end;
	
	if (i < j) { //PESO DIMINUIU
		
		for (k = 0; k < agm_size; k++) { //SE ESTA NA AGM, NADA A FAZER
			if ((AGM[k]->origem->nome == modificada.origem->nome) &&
				(AGM[k]->destino->nome == modificada.destino->nome)) {
				
				break;
			}
		}
		if (k < agm_size)
			goto end;
		
		
		ordenada[j]->peso = modificada.peso;
		while (ordenada[i]->peso > ordenada[j]->peso) {
			swap = ordenada[j-1];
			ordenada[j-1] = ordenada[j];
			ordenada[j] = swap;
			j--;
		}
	} else { //PESO AUMENTOU
		
		for (k = 0; k < agm_size; k++) { //SE NAO ESTA NA AGM, NADA A FAZER
			if ((AGM[k]->origem->nome == modificada.origem->nome) &&
				(AGM[k]->destino->nome == modificada.destino->nome)) {
				
				break;
			}
		}
		if (k == agm_size)
			goto end;
		
		
		ordenada[j]->peso = modificada.peso;
		while (ordenada[j]->peso < ordenada[j+1]->peso) {
			swap = ordenada[j+1];
			ordenada[j+1] = ordenada[j];
			ordenada[j] = swap;
			j++;
		}
	}
	
	/*
	 * TODO: Cortar subárvore não ótima
	 */
	
	for (i = 0; i < agm_size; i++) {
		if( AGM[i] == ordenada[i]) {
			continue;
		}
		else { //Separar subárvore
			
			int pos_origem = conjuntos[AGM[i]->origem->nome].pos;
			int pos_destino = conjuntos[AGM[i]->destino->nome].pos;
			
			if (pos_origem < pos_destino) { //cortar origem
				conjuntos[AGM[i]->origem->nome].pai = &conjuntos[AGM[i]->origem->nome];
			} else if (pos_origem > pos_destino) {
				conjuntos[AGM[i]->destino->nome].pai = &conjuntos[AGM[i]->destino->nome];
			} else {
				printf("CONCLUIR\n");
			}
			
			break;
		}
	}
	
	/*
	 * Nada mudou
	 */
	if (i == agm_size) 
		goto end;
	
	/*
	 * TODO: Remontar parte da AGM
	 */
	agm_size = i;
	for (j = i; j < N; j++) {
		printf("%d diff %d\n", ordenada[j]->origem->nome, ordenada[j]->destino->nome);
		
		if (raiz_conjunto(&conjuntos[ordenada[j]->origem->nome]) !=
			raiz_conjunto(&conjuntos[ordenada[j]->destino->nome])
		) {
			printf("%d diff %d\n", ordenada[j]->origem->nome, ordenada[j]->destino->nome);
			AGM[agm_size++] = ordenada[j];
			
			unir_conjuntos(&conjuntos[ordenada[j]->origem->nome], 
				       &conjuntos[ordenada[j]->destino->nome]);
		}
	}
	
	/*
	 * Imprimindo nova AGM. 
	 * Nota: agm_size deve ser igual a N-1
	 */
	printf("agm size=%d\n", agm_size);
	for (i = 0; i < agm_size; i++) {
		printf("%d -> %d w=%d\n", AGM[i]->origem->nome, 
		       AGM[i]->destino->nome, AGM[i]->peso);
	}

end:
	/*
	 * Desalocando memória
	 */
	//free(AGM);
	
	/*
	for (i = 0; i < N; i++) {
		free(g.vertices[i].arestas);
	}
	*/
	
	free(g.vertices);
	
	free(conjuntos);
	
	free(ordenada);
	
	return 0;
}
