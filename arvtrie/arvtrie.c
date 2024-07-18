#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "arvtrie.h"

struct arvtrie {
  int n;           // informa a quantidade de ponteiros != NULL em 'nos'
  ArvTrie* nos[M]; // vetor contendo os ponteiros correspondentes a cada digito
  bool palavra;    // informa se o no corresponde ao final de uma palavra
};

static int transformaparaindice (char c) {
  if (c < 'a' || c > 'z') {
    printf(">>> digito desconhecido\n");
    exit(1);
  }
  int d = c - 'a';
  return d;
}

static char transformaparadigito (int d) {
  char c = d + 'a';
  if (c < 'a' || c > 'z') {
    printf(">>> digito desconhecido\n");
    exit(1);
  }
  return c;
}

static ArvTrie* criano (void) {
  ArvTrie* t = (ArvTrie*)malloc(sizeof(ArvTrie));
  t->n = 0;
  t->palavra = false;
  for (int i = 0; i < M; i++)
    t->nos[i] = NULL;
  return t;  
}

ArvTrie* cria (void) {
  return criano();
}

void destroi (ArvTrie* t) {
  for (int i = 0; i < M; i++) {
    if (t->nos[i]) {
      destroi(t->nos[i]);
    }
  }
  free(t);
}

bool insere (ArvTrie* t, const char* chave) {

  // COMPLETAR
  int tam = strlen(chave);
  for(int i = 0; i<tam; i++){
    int ind = transformaparaindice(chave[i]);
    if(t->nos[ind]==NULL){ //NAO existe
      t->nos[ind]=cria();
      t->n += 1;
    }
    t = t->nos[ind];
  }
  //
  if (t == NULL || t->palavra) {
    printf("I: Chave \"%s\" nao inserida\n", chave);
    return false;
  }
  t->palavra = true;
  printf("I: Chave \"%s\" inserida\n", chave);
  return true;
}


static void imprimechaves (ArvTrie* t, char* chave) {
  if (t == NULL) return;

  // se for uma palavra, imprime o que foi acumulado
  if (t->palavra)
    printf("%s\n", chave);
  
  // se nao tiver nenhum proximo no, deve ser uma palavra, ou temos um erro 
  if (t->n == 0) {
    if (!t->palavra) {
      printf(">>> No folha e nao e' palavra, arvore contem algum erro\n");
      exit(1);
    }
  }
  else {
    // adiciona novo \0, em s+1, deixando a posicao s livre para o novo digito
    int s = strlen(chave);
    chave[s+1] = FIMDECHAVE;
    for (int d = 0; d < M; d++) {
      if (t->nos[d]) {
        // define digito na posicao s
        chave[s] = transformaparadigito(d);
        imprimechaves(t->nos[d], chave);
      }
    }
    // "reseta" o final da palavra
    chave[s] = FIMDECHAVE;
  }
}

void imprime (ArvTrie* t) {
  printf("Imprime chaves da arvore:\n");
  char chave[KMAX];
  chave[1] = FIMDECHAVE; 
  for (int d = 0; d < M; d++) {
    if (t->nos[d]) {
      chave[0] = transformaparadigito(d);
      imprimechaves(t->nos[d], chave);
    }
  }
}

bool retira (ArvTrie* t, const char* chave) {
  //printf("R[");
  // imprimir digitos removidos entre os []
  // COMPLETAR
  int tam =strlen(chave);
  int ind;
  ArvTrie* taux;
  int iaux;
  for(int i = 0; i<tam; i++){
    ind = transformaparaindice(chave[i]);
    //printf("%c\n",chave[i]);
    if(t->palavra||t->n>1){
      taux = t;
      iaux = i;
    }
    if(!(t->nos[ind])){
      printf("R[");
      printf("%c",chave[i]);
      printf("]: Chave \"%s\" nao encontrada\n", chave);
      return false;
    }
    t = t->nos[ind];
  }
  if(t->palavra){
    //printf("eeeee\n");
    if(t->n==0){
      //printf("hihi\n");
      ind = transformaparaindice(chave[iaux]);
      
      //printf("ooo\n");
      if(taux->nos[ind]){
        //printf("hello\n");
        //ArvTrie* timprime = taux->nos[ind];
        printf("R[");
        for(int i =iaux; i<tam;i++){
          printf("%c",chave[i]);
        }
        printf("]: Chave \"%s\" removida\n", chave); 
        destroi(taux->nos[ind]);
        taux->n--;
        taux->nos[ind]=NULL;
      }
    }
  t->palavra = false;
  return true;
  }
  printf("R[");
  printf("%c",chave[iaux]);
  printf("]: Chave \"%s\" nao removida\n", chave);
  return false;
  // Respostas esperadas:
  //printf("]: Chave \"%s\" nao encontrada\n", chave);
  //printf("]: Chave \"%s\" removida\n", chave);
  //printf("]: Chave \"%s\" nao removida\n", chave);
  //return false;
}


static void sugerirchaves (ArvTrie* t, const char* prefixo) {

  // COMPLETAR
  int tam =strlen(prefixo);
  ArvTrie* taux;
  int ind;
  char* pchave[KMAX];
  for(int i = 0; i<tam; i++){
    ind = transformaparaindice(prefixo[i]);
    if(!(t->nos[ind])){
      printf("preixo: %s inesitente:(\n",prefixo);
      return;
    }
    t = t->nos[ind];
  }
  strcpy(pchave,prefixo);
  taux = t;
  putchar('\n');
  imprimechaves(t,pchave);
  
}

void sugerir (ArvTrie* t, const char* prefixo) {
  printf("Sugerir completacoes para \"%s\": ", prefixo);
  sugerirchaves(t, prefixo);
  printf("\n");
}