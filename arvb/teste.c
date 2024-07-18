#include <stdio.h>
#include "arvb23.h"

int main (void) {
  ArvB23 *b = cria();
  //int chave;
  
  //while (1) {
    // printf ("chave a inserir (-111 para terminar): ");
    // scanf ("%d", &chave);
    // if (chave==-111) break;

  b = insere(b, 500);
  b = insere(b, 300);
  b = insere(b, 700);
  b = insere(b, 200);
  b = insere(b, 100);
  b = insere(b, 800);
  b = insere(b, 900);
  b = insere(b, 250);
  b = insere(b, 150);
  b = insere(b, 120);
  printf("arvore: \n");
  mostra(b);
  printf("%d no nível %d\n",500,existe(b,500));
  printf("%d no nível %d\n",200,existe(b,200));
  printf("%d no nível %d\n",900,existe(b,900));
  printf("%d no nível %d\n",8,existe(b,8));
  printf("%d no nível %d\n",250,existe(b,250));
  printf("%d no nível %d\n",300,existe(b,300));
  printf("%d no nível %d\n",800,existe(b,800));
  printf("%d no nível %d\n",306,existe(b,306));
  printf("%d no nível %d\n",150,existe(b,150));
  //}
  destroi(b); 
  return 0;
}    
