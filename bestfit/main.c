#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>


struct processo {
	int id;
    int tam;
    int bloco; // id do bloco que o processo esta
    struct processo *prox;
};
struct blocoMem {
	int id;
    int tam;
    struct processo *processo; //
    struct blocoMem *prox;
};



struct blocoMem *cabecaBloco = NULL;
struct processo *cabecaProcesso = NULL;

struct blocoMem *atual = NULL;

struct blocoMem *atualBloco = NULL;
struct processo *atualProcesso = NULL;



// Funções de lista
void printBlocos() {
   struct blocoMem *blocoMem = cabecaBloco;
   printf("\n\n");

   while(blocoMem != NULL) {
      printf(" | H%i=%d | ",blocoMem->id,blocoMem->tam);
      if(blocoMem->processo == NULL){
        printf("\t | ----- |\n");
      }else{

      printf("\t | P%i=%d |\n",(blocoMem->processo)->id, (blocoMem->processo)->tam);
      }

      blocoMem = blocoMem->prox;
   }

}

//insereBloco o blocoMem na primeira posiçao
void insereBloco(int id, int tam) {
   //cria um blocoMem
   struct blocoMem *blocoMem = (struct blocoMem*) malloc(sizeof(struct blocoMem));

   blocoMem->id = id;
   blocoMem->tam = tam;
   blocoMem->processo = NULL;

   blocoMem->prox = NULL;
    if(atualBloco != NULL){
           atualBloco->prox = blocoMem;

    }
    if(cabecaBloco == NULL){
        cabecaBloco = blocoMem;
    }
   atualBloco = blocoMem;
}

//insereBloco o blocoMem na primeira posiçao
void insereProcesso(int id, int tam) {
   //cria um blocoMem
   struct processo *proceso = (struct processo*) malloc(sizeof(struct processo));

   proceso->id = id;
   proceso->tam = tam;
   proceso->bloco = -1;

   proceso->prox = NULL;

   if(atualProcesso != NULL){
    atualProcesso->prox = proceso;

   }
   if(cabecaProcesso == NULL){
        cabecaProcesso = proceso;
   }
   atualProcesso = proceso;
}

//se a lista ta vazia
bool isEmpty() {
   return (cabecaBloco == NULL);
}

//retorna o tamanho da lista
int tamanho() {
   int tamanho = 0;
   struct blocoMem *atual;

   for(atual = cabecaBloco; atual != NULL; atual = atual->prox) {
      tamanho++;
   }

   return tamanho;
}

//encontra o blocoMem na dada id na lista
struct blocoMem* findPosicao(int id) {
   struct blocoMem* atual = cabecaBloco;

   //ve se a lista esta vazia
   if(cabecaBloco == NULL) {
      return NULL;
   }

   while(atual != NULL) {
        //se a posicao atual é a posicao passada no parametro
    if(atual->id==id){
        return atual;
    }
        //vai pro prox blocoMem
    atual = atual->prox;

   }

    return NULL;

}

//deleta o blocoMem do id dado
struct blocoMem* deleteblocoMem(int id) {
   struct blocoMem* atual = cabecaBloco;
   struct blocoMem* anterior = NULL;

   if(cabecaBloco == NULL) {
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

   //found a match, update the blocoMem
   if(atual == cabecaBloco) {
      //change first to point to prox blocoMem
      cabecaBloco = cabecaBloco->prox;
   } else {
      //bypass the atual blocoMem
      anterior->prox = atual->prox;
   }

   return atual;
}

/* Thread */

void criaBlocos(void){
	//id, burst, prioridade
	insereBloco(1,200);
	insereBloco(2,200);
	insereBloco(3,300);
	insereBloco(4,400);
	insereBloco(5,200);
	insereBloco(6,500);
	insereBloco(7,600);
	insereBloco(8,100);
	insereBloco(9,300);
	insereBloco(10,400);

}
void criaProcessos(){

    insereProcesso(1, 100);
    insereProcesso(2, 350);
    insereProcesso(3, 400);
    insereProcesso(4, 150);
    insereProcesso(5, 100);
    insereProcesso(6, 200);
    insereProcesso(7, 300);
    insereProcesso(8, 500);
    insereProcesso(9, 200);
    insereProcesso(10, 100);


}


/***

***/
void best_fit(){

    int menorBloco = 10000;
    int difTam;
    struct processo* processo = cabecaProcesso;

    while(processo!= NULL){

        struct blocoMem* bloco = cabecaBloco;
        while(bloco!= NULL) {

            if(bloco->processo == NULL){ //se não tiver processo naquele bloco
                difTam = bloco->tam - processo->tam;
                if(difTam >= 0){//verifica se o tamanho do processo cabe no bloco
                    if(menorBloco>difTam)
					{
						processo->bloco = bloco->id;
						menorBloco = difTam;
					}
                }
            }

            bloco = bloco->prox;
        }

        if(processo->bloco != -1){
                findPosicao(processo->bloco)->processo = processo; //pega o processo que entrou primeiro na fila
        }

        processo = processo->prox;
        printBlocos();
        Sleep(1000);
        menorBloco = 10000;
    }

}
/* ************************************************** Main */
int main (void) {
	criaBlocos();
	criaProcessos();
	best_fit();
//exibir mensagem de eroo e fazer compactacao
	return 0;

}
