#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "arvore.h"

struct arvno {
    int info;
    ArvNo* esq;
    ArvNo* dir;
};

int info(ArvNo* p)
{
    if (p == NULL) return -1;
    return p->info;
}

ArvNo* criano(int v, ArvNo* esq, ArvNo* dir)
{
    ArvNo* p = (ArvNo*)malloc(sizeof(ArvNo));
    if (p != NULL) {
        p->info = v;
        p->esq = esq;
        p->dir = dir;
    }
    return p;
}

void imprime(ArvNo* r)
{
    printf("(");
    if (r != NULL) {
        printf("%d ", r->info);
        imprime(r->esq);
        printf(", ");
        imprime(r->dir);
    }
    printf(")");
}

void libera(ArvNo* r)
{
    if (r != NULL) {
        libera(r->esq);
        libera(r->dir);
        free(r);
    }
}

int num_nos(ArvNo* r)
{
    if (r == NULL) return 0;
    return num_nos(r->esq) + num_nos(r->dir) + 1;
}

ArvNo* busca(ArvNo* r, int v)
{
#if 1 // implementacao recursiva
    if (r == NULL) return NULL;
    else if (v < r->info) return busca(r->esq, v);
    else if (v > r->info) return busca(r->dir, v);
    else return r;
#else // implementacao iterativa  
    while (r != NULL) {
        if (v < r->info)
            r = r->esq;
        else if (v > r->info)
            r = r->dir;
        else
            return r;
    }
    return NULL;
#endif
}

ArvNo* insere(ArvNo* r, int v)
{
    if (r == NULL)
        return criano(v, NULL, NULL);
    else if (v < r->info)
        r->esq = insere(r->esq, v);
    else if (v > r->info)
        r->dir = insere(r->dir, v);
    // se for igual, nao sera adicionado
    return r;
}

ArvNo* sucessor(ArvNo* r) {
    // COMPLETAR: codigo da aula passada (necessario para a funcao 'retira')
    if (r != NULL && r->dir != NULL) {
        r = r->dir;
        while (r->esq != NULL) {
            r = r->esq;
        }
        return r;
    }
}

ArvNo* retira(ArvNo* r, int v)
{
    // COMPLETAR: codigo da aula passada
    if (r == NULL) return NULL;
    else if (v < r->info)
        r->esq = retira(r->esq, v);
    else if (v > r->info)
        r->dir = retira(r->dir, v);
    else { /* achou o nó a remover */
        /* nó sem filhos */
        if (r->esq == NULL && r->dir == NULL) {
            free(r);
            r = NULL;
        }
        /* nó só tem filho à direita */
        else if (r->esq == NULL) {
            ArvNo* t = r;
            r = r->dir;
            free(t);
        }
        /* só tem filho à esquerda */
        else if (r->dir == NULL) {
            ArvNo* t = r;
            r = r->esq;
            free(t);
        }
        /* nó tem os dois filhos: busca o sucessor */
        else {
            ArvNo* sucessorNovo;
            sucessorNovo = sucessor(r);
            r->info = sucessorNovo->info;
            sucessorNovo->info = v;
            r->dir = retira(r->dir, v);
        }
    }
    return r;
}


ArvNo* menor_no(ArvNo* r)
{
    // COMPLETAR caso opte por utilizar essa funcao auxiliar
    return r;
}

ArvNo* maior_no(ArvNo* r)
{
    // COMPLETAR caso opte por utilizar essa funcao auxiliar
    return r;
}

ArvNo* predecessor(ArvNo* r)
{
    // COMPLETAR caso opte por utilizar essa funcao auxiliar
    if (r != NULL && r->esq != NULL) {
        r = r->esq;
        while (r->dir != NULL) {
            r = r->dir;
        }
        return r;
    }
}



int altura(ArvNo* r)
{
    // COMPLETAR
    if (!r) {
        return -1;
    }

    if (altura(r->esq) > altura(r->dir)) {
        return 1 + altura(r->esq);
    }
    else {
        return 1 + altura(r->dir);
    }
}

int e_balanceada(ArvNo* r)
{
    // COMPLETAR
    if (!r) {
        return 1;
    }
    return (abs(altura(r->esq) - altura(r->dir)) <= 1) && e_balanceada(r->dir) && e_balanceada(r->esq);
}

ArvNo* balancear(ArvNo* r)
{

    // COMPLETAR
    if (r == NULL) {
        return r;
    }

    int numE = num_nos(r->esq);
    int numD = num_nos(r->dir);
    int infor = 0;
    int inforRaiz = 0;
    ArvNo* pre = NULL;
    ArvNo* suc = NULL;

    while ((abs(numE - numD)) > 1) {
        if (numE > (numD + 1)) {
            pre = predecessor(r);
            infor = pre->info;
            inforRaiz = r->info;
            r = retira(r, infor);
            r->info = infor;
            r = insere(r, inforRaiz);
            numE = numE - 1;
            numD = numD + 1;
        }
        else {
            suc = sucessor(r);
            infor = suc->info;
            inforRaiz = r->info;
            r = retira(r, infor);
            r->info = infor;
            r = insere(r, inforRaiz);
            numE = numE + 1;
            numD = numD - 1;
        }

    }
    r->esq = balancear(r->esq);
    r->dir = balancear(r->dir);

    return r;


}

