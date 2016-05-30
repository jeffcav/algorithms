struct aresta{
	int peso;
	struct vertice *origem;
	struct vertice *destino;
};

struct vertice{
	int nome;
	struct aresta *arestas;
};

struct grafo{
	int N;
	struct vertice *vertices;
};

struct agm{
	struct aresta *aresta;
	struct aresta* prox;
};

/*
 * Estruturas de dados descritas no livro do Dasgupta
 */
struct conjunto{
	struct conjunto *pai;
	struct vertice *vert;
	int pos;
};
