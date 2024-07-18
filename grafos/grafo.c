//#define _CRT_SECURE_NO_WARNINGS
#include "grafo.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* estrutura auxiliar para fila pilha(stack) / fila(queue) */
typedef struct _lista Lista;
struct _lista {
    int info;
    Lista* prox;
};

static Lista* criaLista() { return NULL; }

static Lista* enfileira(Lista* fila, int info) {
    Lista* novo = (Lista*)malloc(sizeof(Lista));
    Lista* aux = fila;
    assert(novo);
    novo->info = info;
    novo->prox = NULL;
    if (!fila) {
        return novo;
    }
    while (aux->prox) {
        aux = aux->prox;
    }
    aux->prox = novo;
    return fila;
}
static Lista* desenfileira(Lista* fila, int* info) {
    Lista* removido;
    if (!fila) {
        *info = -1;
        return NULL;
    }
    *info = fila->info;
    removido = fila;
    fila = fila->prox;
    free(removido);
    return fila;
}

static Lista* empilha(Lista* pilha, int info) {
    Lista* novo = (Lista*)malloc(sizeof(Lista));
    assert(novo);
    novo->info = info;
    novo->prox = pilha;
    return novo;
}

static Lista* desempilha(Lista* pilha, int* info) {
    Lista* removido;
    if (!pilha) {
        *info = -1;
        return NULL;
    }
    *info = pilha->info;
    removido = pilha;
    pilha = pilha->prox;
    free(removido);
    return pilha;
}

typedef struct _viz Viz;
struct _viz {
    int noj;
    float peso;
    Viz* prox;
};

struct _grafo {
    int nv;    /* numero de nos ou vertices */
    int na;    /* numero de arestas */
    Viz** viz; /* viz[i] aponta para a lista de arestas vizinhas do no i */
};

static Viz* criaViz(Viz* head, int noj, float peso) {
    /* insere vizinho no inicio da lista */
    Viz* no = (Viz*)malloc(sizeof(Viz));
    assert(no);
    no->noj = noj;
    no->peso = peso;
    no->prox = head;
    return no;
}

static Grafo* grafoCria(int nv, int na) {
    int i;
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    g->nv = nv;
    g->na = na;
    g->viz = (Viz**)malloc(sizeof(Viz*) * nv);
    for (i = 0; i < nv; i++)
        g->viz[i] = NULL;
    return g;
}

Grafo* grafoLe(char* filename) {
    /* cria grafo nao orientado - supoe que arquivo esta correto! */
    // COMPLETAR
    FILE* entrada = fopen(filename, "r");
    int numNos, numArestas, n1, n2;
    float peso;

    fscanf(entrada, "%d\n%d\n", &numNos, &numArestas);
    Grafo* novo = grafoCria(numNos, numArestas);
    assert(novo);
    while ((fscanf(entrada, "%d %d %f\n", &n1, &n2, &peso)) == 3) {
        novo->viz[n1] = criaViz(novo->viz[n1], n2, peso);
        novo->viz[n2] = criaViz(novo->viz[n2], n1, peso);
    }
    return novo;
}

Grafo* grafoLibera(Grafo* grafo) {
    if (grafo) {
        int i = 0;
        Viz* no, * aux;
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

void grafoMostra(char* title, Grafo* grafo) {
    int i;
    if (title)
        printf("%s", title);
    if (grafo) {
        printf("NV: %d, NA: %d\n", grafo->nv, grafo->na);
        for (i = 0; i < grafo->nv; i++) {
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

void auxRecProf(Grafo* grafo, int no_inicial, Lista* listinha) {
    Viz* vizinho = grafo->viz[no_inicial];
    printf("%d ", no_inicial);
    listinha[no_inicial].info = 1;
    while (vizinho) {
        if (listinha[vizinho->noj].info != 1) {
            auxRecProf(grafo, vizinho->noj, listinha);
        }
        vizinho = vizinho->prox;
    }
    // printf("%d ", no_inicial);
}

void grafoPercorreProfundidadeRec(Grafo* grafo, int no_inicial) {
    // COMPLETAR
    if (!grafo || no_inicial >= (grafo->nv) || grafo->nv == 0) {
        return;
    }
    Lista* jahvisitado = (Lista*)malloc(sizeof(Lista) * grafo->nv);
    assert(jahvisitado);
    for (int i = 0; i < grafo->nv; i++) {
        jahvisitado[i].info = 0;
    }
    auxRecProf(grafo, no_inicial, jahvisitado);
    free(jahvisitado);
    printf("\n");
}

void grafoPercorreLargura(Grafo* grafo, int no_inicial) {
    // COMPLETAR
    if (!grafo || no_inicial >= (grafo->nv) || grafo->nv == 0) {
        return;
    }
    Lista* jahvisitado = (Lista*)malloc(sizeof(Lista) * grafo->nv);
    assert(jahvisitado);
    for (int i = 0; i < grafo->nv; i++) {
        jahvisitado[i].info = 0;
    }
    Lista* fila = criaLista();
    // assert(fila);
    int info;
    fila = enfileira(fila, no_inicial);
    jahvisitado[no_inicial].info = 1;

    while (fila) {
        fila = desenfileira(fila, &info);
        printf("%d ", info);
        Viz* vizinho = grafo->viz[info];
        while (vizinho) {
            if (jahvisitado[vizinho->noj].info != 1) {
                jahvisitado[vizinho->noj].info = 1;
                fila = enfileira(fila, vizinho->noj);
                // printf("%d ", vizinho->noj);
            }
            vizinho = vizinho->prox;
        }
    }
    free(jahvisitado);
    printf("\n");
}

void grafoPercorreProfundidade(Grafo* grafo, int no_inicial) {
    // COMPLETAR
    if (!grafo || no_inicial >= (grafo->nv) || grafo->nv == 0) {
        return;
    }
    int* jahvisitado = (int*)malloc(sizeof(int) * grafo->nv);
    assert(jahvisitado);
    for (int i = 0; i < grafo->nv; i++) {
        jahvisitado[i] = 0;
    }
    Lista* pilha = criaLista();
    int info;
    pilha = empilha(pilha, no_inicial);
    jahvisitado[no_inicial] = 1;

    while (pilha) {
        pilha = desempilha(pilha, &info);
        printf("%d ", info);
        Viz* vizinho = grafo->viz[info];
        while (vizinho) {
            if (jahvisitado[vizinho->noj] != 1) {
                pilha = empilha(pilha, vizinho->noj);
                jahvisitado[vizinho->noj] = 1;
            }
            vizinho = vizinho->prox;
        }
    }

    printf("\n");
}
