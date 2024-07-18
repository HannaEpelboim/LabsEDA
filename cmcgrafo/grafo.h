typedef struct _grafo Grafo;

Grafo* grafoLe (const char* filename);
Grafo* grafoLibera (Grafo* grafo);
void grafoMostra (const char* title, Grafo* grafo);
int grafoNumeroDeVertices (Grafo* grafo);

int* cmcBellmanFord (Grafo *grafo, int no_inicial, int* cmccusto);
int* cmcDijkstraBuscaLinear (Grafo *grafo, int no_inicial, int* cmccusto);
int* cmcDijkstraBuscaHeap (Grafo *grafo, int no_inicial, int* cmccusto);

/* formato do arquivo de teste.  grafo  
0/1     ! 0 - nao orientado 1 - orientado 
6       ! numero de nos 
5       ! numero de arestas 
0 1 20  ! aresta:  noi noj peso 
1 2 30  
1 3 25
3 4 15
4 5 10
*/
