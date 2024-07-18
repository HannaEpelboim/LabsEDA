#include <stdio.h>
#include <stdlib.h>

#include "arvb23.h"

struct arvb23 {
  /**
   * Chaves: k1<k2, se k2 existir.
   * Se k2=-1, significa que ele não existe.
  **/
  int k1, k2;
  ArvB23* pai;
  ArvB23* esq;
  ArvB23* meio;
  ArvB23* dir;
};

ArvB23* cria (void) {
  return NULL;
}

static ArvB23* novoNo (int chave) {
  ArvB23* b = (ArvB23*)malloc(sizeof(struct arvb23));
  if (b == NULL) {
    printf("erro no malloc! \n");
    exit(1);
  }
  b->pai = NULL;
  b->k1 = chave; 
  b->k2 = -1; // -1 = não existe chave
  b->esq = b->meio = b->dir = NULL;
  return b;
}

static ArvB23* overflowQuebra (ArvB23* b, int *valorainserir, ArvB23* novofilho) {
  ArvB23* novo;

  novo = (ArvB23*)malloc(sizeof(struct arvb23));
  if (novo == NULL) {
    printf("erro em malloc! \n");
    exit(1);
  }

  if (*valorainserir < b->k1) {
    novo->esq = novofilho;
    if (novo->esq) {
      novo->esq->pai = novo;
    }
    novo->k1 = *valorainserir;
    novo->meio = b->esq;
    if (novo->meio) {
      novo->meio->pai = novo;
    }
    novo->k2 = -1;
    novo->dir = NULL;
    *valorainserir = b->k1;
    b->esq = b->meio;
    b->k1 = b->k2;
  }
  else if (*valorainserir < b->k2) {
    // COMPLETAR
    novo->esq = b->esq;
    if(novo->esq){
      novo->esq->pai = novo;
    }
    novo->k1 = b->k1;
    novo->meio = novofilho;
    if (novo->meio) {
      novo->meio->pai = novo;
    }
    novo->k2 = -1;
    novo->dir = NULL;
    b->esq = b->meio;
    b->k1 = b->k2;
  } 
  else {
    // COMPLETAR
    novo->meio = b->meio;
    if(novo->meio){
      novo->meio->pai = novo;
    }
    novo->k1 = b->k1;
    novo->esq = b->esq;
    if (novo->esq) {
      novo->esq->pai = novo;
    }
    novo->k2 = -1;
    novo->dir = NULL;
    b->k1 = *valorainserir;
    *valorainserir = b->k2;
    b->esq = novofilho;
    if(b->esq){
      b->esq->pai = b;
    }
  }
  b->meio = b->dir;
  b->k2 = -1;
  b->dir = NULL;
  return novo;
}

static int insere2 (ArvB23* b, int chave, int* valorainserir, ArvB23** novofilho) {
  // indica se deve inserir neste nó
  int inseriraqui = 0;

  if (b == NULL) {
    printf("erro! subárvore nula! \n");
    exit(1);
  }

  // não é folha, só insere neste nó se subir um valor
  if (b->esq != NULL) {
    if (chave < b->k1) {
      inseriraqui = insere2(b->esq, chave, valorainserir, novofilho); 
    }
    // está entre as duas chaves ou só tem uma chave no nó
    else if (((b->k2 != -1) && (chave < b->k2)) || (b->k2 == -1)) {
      inseriraqui = insere2(b->meio, chave, valorainserir, novofilho);
    }
    else { // chave > b->k2
      inseriraqui = insere2(b->dir, chave, valorainserir, novofilho);
    }
  }
  // este nó é folha, tem que inserir nele
  else {
    *valorainserir = chave;
    inseriraqui = 1;
    *novofilho = NULL;
  } 

  if (!inseriraqui) // se inserção já está completa
    return 0;

  // Agora devemos procurar um espaço no nó

  if (b->k2 == -1) { // tem espaço no nó
    // se nova chave for menor, empurra chave existente para a direita
    if (*valorainserir < b->k1) {
      // COMPLETAR
      b->k2 = b->k1;
      b->k1 = *valorainserir;
      b->dir = b->meio;
      b->meio = b->esq;
      b->esq = *novofilho;
      if(b->esq){
        b->esq->pai = b;
      } 
    }
    // é maior que a chave que já está lá
    else {
      b->k2 = *valorainserir;
      b->dir = b->meio;
      b->meio = *novofilho;
      if (b->meio) b->meio->pai = b;
    }
    // como havia espaço, não sobem valores a serem inseridos
    return 0;
  }

  *novofilho = overflowQuebra(b, valorainserir, *novofilho); 

  // quando há quebra, sobe a mediana para nova inserção 
  return 1;
}

ArvB23* insere (ArvB23* b, int chave) {
  int valorquesubiu;
  ArvB23* novofilho;
  ArvB23* novaraiz;

  if (b == NULL) {
    b = novoNo(chave);
    b->pai = novoNo(-1);
  }
  else {
    if (insere2(b, chave, &valorquesubiu, &novofilho)) {
      // cria nova raiz
      novaraiz = novoNo(valorquesubiu);
      novaraiz->pai = b->pai;
      novaraiz->esq = novofilho;
      novaraiz->esq->pai = novaraiz;
      novaraiz->meio = b;
      novaraiz->meio->pai = novaraiz;
      b = novaraiz;
    } 
  }
  return b;
}

int existe (ArvB23* b, int chave) {
  // COMPLETAR
  if(chave==b->k1 || chave==b->k2){
    return 0;
  }
  if(chave<b->k1){
    if(b->esq){
      return 1+existe(b->esq,chave);
    }
  }
  else if(b->k2==-1||chave<b->k2){
    if(b->meio){
      return 1+existe(b->meio,chave);
    }
  }
  else if(b->k2!=-1&&chave>b->k2){
    if(b->dir){
      return 1+existe(b->dir,chave);
    }
  }
  //return -1;
  //ArvB23* aux;
  //aux = b;
  int cont = 1;
  while(b->pai->k1!=-1){
    cont++;
    b = b->pai;
  }
  return ((-1)*cont);
}

void destroi (ArvB23* b) {
  if (b != NULL) {
    destroi(b->esq);
    destroi(b->meio);
    destroi(b->dir);
    free(b);
  }
}

static void mostra_int (ArvB23* b) {
  printf ("[");
  if (b != NULL) {
    printf("%d:%d->%d ", b->k1, b->k2, b->pai->k1);
    mostra_int(b->esq);
    mostra_int(b->meio);
    if (b->k2 != -1)
      mostra_int(b->dir);
  }
  printf ("]");
}

void mostra (ArvB23* b) {
  mostra_int(b);
  printf("\n");
}

int iguais (ArvB23* b1, ArvB23* b2) {
  if (b1==NULL) return (b2==NULL);
  if (b2==NULL) return 0;

  return (b1->k1 == b2->k1) &&
         (b1->k2 == b2->k2) &&
          iguais(b1->esq,  b2->esq)  &&
          iguais(b1->meio, b2->meio) &&
          iguais(b1->dir,  b2->dir)  &&
         (b1->pai->k1 == b2->pai->k1);
}