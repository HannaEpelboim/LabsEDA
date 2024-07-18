#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "heap.h"

typedef struct _viz Viz;
struct _viz {
  int noj;
  float peso;
  Viz*  prox;
};

struct _grafo {
  int orientado; /* se o grafo eh orientado ou nao */
  int nv;        /* numero de nos ou vertices */
  int na;        /* numero de arestas */
  Viz** viz;     /* viz[i] aponta para a lista de arestas vizinhas do no i */
};

/* insere vizinho no inicio da lista */
static Viz* criaViz (Viz* head, int noj, float peso) {
  Viz* no = (Viz*) malloc(sizeof(Viz));
  assert(no);
  no->noj = noj;
  no->peso = peso;
  no->prox = head;
  return no;
}

static Grafo* grafoCria (int nv) {
  int i;
  Grafo* g = (Grafo *) malloc(sizeof(Grafo));
  g->nv = nv;
  g->na = 0;
  g->viz = (Viz **) malloc(sizeof(Viz *) * nv);
  for (i = 0; i < nv; i++)
    g->viz[i] = NULL;
  return g;
}

/* cria grafo supondo que arquivo esta correto! */
Grafo* grafoLe (const char* filename) {

  FILE *arq = fopen(filename,"rt");
  int orientado;
  int nv, na, no1, no2 = 0;
  float peso;
  Grafo* novo;
	
  fscanf(arq, "%d %d %d", &orientado, &nv, &na);
  novo = grafoCria(nv);
  novo->orientado = orientado;
  assert(novo);
  while (fscanf(arq, "%d %d %f", &no1, &no2, &peso) == 3) {
    novo->viz[no1] = criaViz(novo->viz[no1], no2, peso);
    if (!orientado)
      novo->viz[no2] = criaViz(novo->viz[no2], no1, peso);
    novo->na++;
  }
  return novo;
}

Grafo* grafoLibera (Grafo* grafo) {
  if (grafo) {
    int i = 0;
    Viz* no,*aux;
    for (i = 0; i < grafo->nv; i++) {
      no = grafo->viz[i];
      while (no) {
        aux = no->prox;
        free(no);
        no = aux;
      }
    }
    free(grafo->viz);
    free(grafo);
  }
  return NULL;
}

void grafoMostra (const char* title, Grafo * grafo) {
  int i;
  if (title)
    printf("%s", title);
  if (grafo) {
    printf(" [NV: %d, NA: %d, %s]\n", grafo->nv, grafo->na, grafo->orientado ? "Orientado" : "Nao Orientado");
    for (i = 0; i < grafo->nv; i++){
    Viz* viz = grafo->viz[i];
    printf("[%d]->", i);
    while (viz) {
      printf("{%d, %g}->", viz->noj, viz->peso);
      viz = viz->prox;
    }
    printf("NULL\n");
    }
  }
}

int grafoNumeroDeVertices (Grafo* grafo) {
  return grafo->nv;
}

static int relaxa (int* cmccusto, int* cmcvant, int i, int j, int custo_i_j) {
  if (cmccusto[i] == INT_MAX) return 0;
  /* Verifica se e possivel diminuir o custo para chegar ate j, indo ate i + aresta i,j
   * Retorna 1 caso seja possivel.
  **/
  // Q1: COMPLETAR

  if (cmccusto[j] > cmccusto[i]+custo_i_j){
    cmccusto[j] = cmccusto[i] + custo_i_j;
    cmcvant[j] = i;
    return 1;
  } 

  return 0;
}

int* cmcBellmanFord (Grafo *grafo, int no_inicial, int* cmccusto) {
  // Q2: REMOVER AO IMPLEMENTAR QUESTAO 2
  //return NULL;

  if (no_inicial >= grafo->nv) return NULL;

  /* cria arrays auxiliares para calcular os caminhos mais curtos */
  int* cmcvant = (int*)malloc(grafo->nv*sizeof(int));

  /* Define os valores iniciais para cada um dos arrays auxiliares. */
  for (int i = 0; i < grafo->nv; i++) {
    cmccusto[i] = INT_MAX;
    cmcvant[i]  = i;
  }

  /* Inicializa os arrays auxiliares para o no inicial. */
  cmccusto[no_inicial] = 0;
  cmcvant[no_inicial]  = no_inicial;

  /* repetir gv->nv-1 vezes (pode ir de k = 1 ate k < g->nv) */
  for (int k = 1; k < grafo->nv; k++) {
    /* aplica relaxamento nas arestas (funcao "relaxa") */
    // Q2: COMPLETAR
    for (int i = 0; i < grafo->nv; i++) {
      Viz* vizinho = grafo->viz[i];
      while (vizinho) {
        relaxa(cmccusto, cmcvant, i, vizinho->noj, vizinho->peso);
        vizinho = vizinho->prox;
      }
    }
  }
  return cmcvant;
}

/* busca linear do vertice com menor custo total (desconsiderando nos ja visitados) */
static int buscamenordistancialinear (int* cmccusto, int* visitados, int nv) {
  int minimo = INT_MAX;
  int nomin = -1;
  for (int i = 0; i < nv; i++) {
    if (!visitados[i] && cmccusto[i] < minimo) {
      nomin = i;
      minimo = cmccusto[i];
    }
  }
  return nomin;
}

int* cmcDijkstraBuscaLinear (Grafo *grafo, int no_inicial, int* cmccusto) {
  // Q3: REMOVER AO IMPLEMENTAR QUESTAO 3
 // return NULL;

  if (no_inicial >= grafo->nv) return NULL;

  /* Cria arrays auxiliares para calcular os caminhos mais curtos. */
  int* visitados = (int*)malloc(grafo->nv*sizeof(int));
  int* cmcvant   = (int*)malloc(grafo->nv*sizeof(int));

  /* Define os valores iniciais para cada um dos arrays auxiliares. */
  // Q3: COMPLETAR
  
  for (int i =0; i<grafo->nv;i++){
    visitados[i] = 0;
    cmcvant[i] = i;
    cmccusto[i] = INT_MAX;
  }  


  /* Inicializa os arrays auxiliares para o no inicial. */
  // Q3: COMPLETAR
  cmcvant[no_inicial] = no_inicial;
  cmccusto[no_inicial] = 0;

  /* Enquanto o no corrente for diferente de -1, visita 
   *  os vizinhos e realiza a relaxacao de arestas.
  **/
  int corrente = no_inicial;
  while(corrente != -1 && cmccusto[corrente] != INT_MAX) {
    /* Verifica os vizinhos nao visitados, realizando a relaxacao das arestas (se for o caso). 
    **/
    // Q3: COMPLETAR
    if(visitados[corrente]!=-1){
      Viz* vizinho = grafo->viz[corrente];
      while (vizinho) {
        relaxa(cmccusto, cmcvant, corrente, vizinho->noj, vizinho->peso);
        // if(cmccusto[vizinho->noj] > cmccusto[corrente]+vizinho->peso){
        //   cmccusto[vizinho->noj] = cmccusto[corrente]+vizinho->peso;
        // }
        vizinho = vizinho->prox;
      } 
    }
    /* O no corrente foi visitado (apos todos os vizinhos serem examinados). */
    // Q3: COMPLETAR
    visitados[corrente] = 1;

    /* O proximo no a visitar eh o que tem a menor distancia calculada. */
    // Q3: COMPLETAR
    corrente = buscamenordistancialinear(cmccusto,visitados,grafo->nv);

  }
  
  free(visitados);
  return cmcvant;
}

int* cmcDijkstraBuscaHeap (Grafo *grafo, int no_inicial, int* cmccusto) {
  // Q4: REMOVER AO IMPLEMENTAR QUESTAO 4
  //return NULL;

  if (no_inicial >= grafo->nv) return NULL;

  /* cria arrays de estado para calcular os caminhos mais curtos */
  int *visitados = (int *)malloc(grafo->nv*sizeof(int));
  int *cmcvant = (int *)malloc((grafo->nv)*sizeof(int));

  /* Inicializa Heap auxiliar e define os valores iniciais para os arrays auxiliares 
   * Insere no heap todos os vertices, com excecao do no inicial, com custo INT_MAX
  **/
  Heap *heap = heap_cria(grafo->nv);
  
  // Q4: COMPLETAR
  for (int i = 0; i<grafo->nv;i++){
    if(i != no_inicial){
      heap_insere(heap,INT_MAX,i);
    }
    visitados[i] = 0;
    cmcvant[i] = i;
    cmccusto[i] = INT_MAX;
  }

  /* Inicializa os arrays auxiliares para o no inicial */
  // Q4: COMPLETAR

  cmcvant[no_inicial] = no_inicial;
  cmccusto[no_inicial] = 0;

  /* Enquanto o no corrente for diferente de -1, visita 
   *  os vizinhos e realiza a relaxacao de arestas.
  **/
  int corrente = no_inicial;
  while(corrente != -1 && cmccusto[corrente] != INT_MAX) {
    /* Verifica os vizinhos nao visitados, realizando a relaxacao das arestas (se for o caso).
     * Nesse caso, corrige o heap apos cada relaxacao (caso ocorra).
    **/
    // Q4: COMPLETAR
    if(visitados[corrente]!=-1){
      Viz* vizinho = grafo->viz[corrente];
      while (vizinho) {
        if(relaxa(cmccusto, cmcvant, corrente, vizinho->noj, vizinho->peso)){
          heap_corrige(heap,cmccusto[corrente]+vizinho->peso,vizinho->noj);
        }
        vizinho = vizinho->prox;
      } 
    }      

    /* O no corrente foi visitado (apos todos os vizinhos serem examinados). */
    // Q4: COMPLETAR
    visitados[corrente] = 1;
    /* O proximo no a visitar eh o que tem a menor distancia calculada. */
    // Q4: COMPLETAR
    corrente = heap_remove(heap);
  }
  
  free(visitados);
  heap_libera(heap);
  return cmcvant;
}

// versao final