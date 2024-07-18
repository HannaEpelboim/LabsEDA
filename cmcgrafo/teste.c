#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "grafo.h"
 
void mostrarec (int i, int *caminhos) {
  if (caminhos[i] != i)  {
    mostrarec (caminhos[i], caminhos);
    printf("%d ", caminhos[i]);
  }
}

void mostracaminhos (int* caminhos, int i, int* cmccusto) {
  if (cmccusto[i] == INT_MAX) {
    printf("caminho para %d: nao alcancavel", i);
  }
  else {
    printf("caminho para %d (%d): ", i, cmccusto[i]);
    mostrarec(i, caminhos);
  }
  printf ("\n");
}

typedef int* (*CMCFunc)(Grafo *grafo, int no_inicial, int* cmccusto); 
void testecmcalgoritmo (Grafo *g, int noinicial, CMCFunc cmcf) {
  int* cmccusto  = (int*)malloc(grafoNumeroDeVertices(g)*sizeof(int));
  int* cmcvant = cmcf(g, noinicial, cmccusto);
  if (cmcvant == NULL) {
    printf("no invalido!\n");
  }
  else {
    for (int c = 0; c < grafoNumeroDeVertices(g); c++) {
      mostracaminhos(cmcvant, c, cmccusto);
    }
    free(cmcvant);
  }
  free(cmccusto);
}

static int nosiniciais[3] = { 0, 4, 6 };
void testecmc (const char* filename) {
  Grafo* g = grafoLe(filename);
  grafoMostra(filename, g);
  for (int i = 0; i < 3; i++) {
    printf(">>> Caminhos mais curtos a partir do no %d\n", nosiniciais[i]);
    printf("- Bellman Ford:\n");
    testecmcalgoritmo(g, nosiniciais[i], cmcBellmanFord);
    printf("- Dijkstra com Busca Linear:\n");
    testecmcalgoritmo(g, nosiniciais[i], cmcDijkstraBuscaLinear);
    printf("- Dijkstra com Busca Heap:\n");
    testecmcalgoritmo(g, nosiniciais[i], cmcDijkstraBuscaHeap);
  }
  free(g);
}

int main () {
  testecmc("grafo5.dat");
  testecmc("grafo6.dat");
  testecmc("grafo7.dat");
  testecmc("grafo8.dat");
  testecmc("grafo9.dat");
  return 0 ;
}
