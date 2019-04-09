#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

struct node {
   int b; // burst time - o tempo necessário para executar na cpu
   int p; // prioridade
   int id; // id do processo
   struct node *prox;
};
int quantum=5;
struct node *cabeca = NULL;
struct node *atual = NULL;

// Funções de lista
void printLista() {
   struct node *node = cabeca;
   printf("\n |  Processo  |  Prioridade  |  Tempo de Brust  |\n\n");

   while(node != NULL) {
      printf(" |\t %d \t|\t %d \t|\t %d \t|\n",node->id,node->p, node->b);
      node = node->prox;
   }

}

//insere o node na primeira posiçao
void insere(int id, int b, int p) {
   //cria um node
   struct node *node = (struct node*) malloc(sizeof(struct node));

   node->id = id;
   node->b = b;
   node->p =p;

   node->prox = cabeca;

   cabeca = node;
}

//se a lista ta vazia
bool isEmpty() {
   return (cabeca == NULL);
}

//retorna o tamanho da lista
int tamanho() {
   int tamanho = 0;
   struct node *atual;

   for(atual = cabeca; atual != NULL; atual = atual->prox) {
      tamanho++;
   }

   return tamanho;
}

//encontra o node na dada posição da lista
struct node* findPosicao(int posi) {
   struct node* atual = cabeca;

   //ve se a lista esta vazia
   if(cabeca == NULL) {
      return NULL;
   }

   int pos=0;

   while(atual->prox != NULL) {
        //se a posicao atual é a posicao passada no parametro
    if(pos==posi){
        return atual;
    }
        //vai pro prox node
    atual = atual->prox;
    pos++;

   }
   if(pos == posi){
        return atual;
   }else{
        return NULL;
   }
}

//deleta o node do id dado
struct node* deleteNode(int id) {
   struct node* atual = cabeca;
   struct node* anterior = NULL;

   if(cabeca == NULL) {
      return NULL;
   }

   while(atual->id != id) {

      if(atual->prox == NULL) {
         return NULL;
      } else {

         anterior = atual;
         atual = atual->prox;
      }
   }

   //found a match, update the node
   if(atual == cabeca) {
      //change first to point to prox node
      cabeca = cabeca->prox;
   } else {
      //bypass the atual node
      anterior->prox = atual->prox;
   }

   return atual;
}

//ordena a lista de acordo com as prioridades
void ordenaPrioridades() {

   int i, j, k, tempid, tempP, tempB;
   struct node *atual;
   struct node *prox;

   int size = tamanho();
   k = size ;

   for ( i = 0 ; i < size - 1 ; i++, k-- ) {
      atual = cabeca;
      prox = cabeca->prox;

      for ( j = 1 ; j < k ; j++ ) {

         if ( atual->p > prox->p ) {
            tempP = atual->p;
            atual->p = prox->p;
            prox->p = tempP;

            tempid = atual->id;
            atual->id = prox->id;
            prox->id = tempid;

	    tempB = atual->b;
            atual->b = prox->b;
            prox->b = tempB;

         }

         atual = atual->prox;
         prox = prox->prox;

      }
   }
}

/* Thread */

void criaProcessos(void){
	//id, burst, prioridade
	insere(1,rand()%100,2);
	insere(2,rand()%100,3);
	insere(3,rand()%100,2);
	insere(4,rand()%100,1);
	insere(5,rand()%100,8);
	insere(6,rand()%100,8);
	insere(7,rand()%100,8);
	insere(8,rand()%100,1);
	insere(9,rand()%100,1);
	insere(10,rand()%100,5);

}
//Robin sem prioridades/ básico
void robin(){

    struct node *node = (struct node*) malloc(sizeof(struct node));
    while(!isEmpty()) {

      int processN= tamanho()-1;
    struct node* process = findPosicao(processN); //pega o processo que entrou primeiro na fila

		printf("\n	<<<<   Processando:   %i>>>>	\n", process->id);

		if ((process->b) > 0)
		{
		   	process->b -= quantum;

			if((process->b) <= 0)
			{
		        	process->b=0;

		        	deleteNode(process->id);
			}else{


			    node->id = process->id;
                node->b = process->b;
                node->p = process->p;

                deleteNode(process->id);
                insere(node->id,node->b,node->p);
			}

		}else{
            deleteNode(process->id);
		}
     printLista();
   }

}

//Robin com prioridades
void robinPrioridades(){

    struct node *node = (struct node*) malloc(sizeof(struct node));

    while(!isEmpty()) {
    ordenaPrioridades(); //ordena a fila de acordo com as prioridades
    int processN= tamanho()-1;
    struct node* process = findPosicao(processN); //pega o processo com maior prioridade da fila

		printf("\n	<<<<   Processando:   %i>>>>	\n", process->id);

		if ((process->b) > 0)
		{
		   	process->b -= quantum;

			if((process->b) <= 0)
			{
		        	process->b=0;
		        	deleteNode(process->id);
			}else{

			    node->id = process->id;
                node->b = process->b;
                node->p = process->p;

                deleteNode(process->id);
                insere(node->id,node->b,node->p);
			}

		}else{
            deleteNode(process->id);
		}
     printLista();
   }
}

/* ************************************************** Main */
int main (void) {
	criaProcessos();
	printLista();
    //robin();
    robinPrioridades();

	return 0;

}
