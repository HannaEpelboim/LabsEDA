#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

typedef struct _item Item;

struct _item {
  int dist;
  int idno;
};

struct heap {
  int max; /* tamanho maximo do heap */
  int pos; /* proxima posicao disponivel no vetor */ 
  Item *itens; /* vetor de itens */
  int* posnos;
}; 

static void troca (Heap *h, int a, int b) {
  int ida = h->itens[a].idno;
  int idb = h->itens[b].idno;
  int da = h->itens[a].dist;

  h->itens[a].idno = idb;
  h->itens[a].dist = h->itens[b].dist;

  h->itens[b].idno = ida;
  h->itens[b].dist = da;

  /* corrigir posnos! */
  h->posnos[ida] = b;
  h->posnos[idb] = a;
}

static void corrige_abaixo (Heap *h, int atual) {
  int pai = atual;
  int filho_esq, filho_dir, filho;
  while (2*pai+1 < h->pos){
    filho_esq=2*pai+1;
    filho_dir=2*pai+2;
    if (filho_dir >= h->pos) filho_dir=filho_esq;
    if (h->itens[filho_esq].dist < h->itens[filho_dir].dist)
        filho=filho_esq;
    else
      filho=filho_dir;
    if (h->itens[pai].dist > h->itens[filho].dist)
      troca(h, pai, filho);
    else
      break;
    pai=filho;
  }
}

Heap* heap_cria (int max) {
  int i;
  Heap* heap=(Heap*)malloc(sizeof(struct heap));
  heap->max=max;
  heap->pos=0;
  heap->itens = (Item *)malloc(max*sizeof(struct _item));
  heap->posnos = (int *)malloc(max*sizeof(int));
  for (i = 0; i < max; i++) heap->posnos[i] = -1;
  return heap;
}

void heap_libera (Heap *h) {
  free(h->itens);
  free(h->posnos);
  free(h);
}

void heap_insere (Heap *h, int distancia, int idno) {
  if (h->pos >= h->max) {
    printf("Heap CHEIO!\n");
    exit(1);
  }
  h->itens[h->pos].dist = distancia;
  h->itens[h->pos].idno = idno;
  h->posnos[idno] = h->pos;
  h->pos++;
} 

int heap_remove (Heap *h) {
  int idno;
  if (h->pos == 0) return -1;
  idno = h->itens[0].idno;
  h->posnos[idno] = -1;

  h->itens[0].idno = h->itens[h->pos-1].idno;
  h->itens[0].dist = h->itens[h->pos-1].dist;
  h->posnos[h->itens[0].idno] = 0;

  h->pos--;
  corrige_abaixo(h, 0);

  return idno;
}
  
void heap_debug_show (Heap *h, char* title) {
  int i;
  printf("%s={",title);
  for (i=0; i<(h->pos); i++)
    printf("[%d , %d] ",h->itens[i].idno, h->itens[i].dist);
  printf("}\n");
  printf("posnos = ");
  for (i = 0; i < h->max; i++)
    printf("[%d -> %d] ",i, h->posnos[i]);
  printf("}\n");
}

void heap_corrige (Heap* h, int novadist, int idno) {
  int pai;
  int pos = h->posnos[idno];
  h->itens[pos].dist = novadist;
  while (pos > 0) {
    pai = (pos-1) / 2;
    if (h->itens[pai].dist > h->itens[pos].dist)
      troca(h, pos, pai);
    pos = pai;
  }
}