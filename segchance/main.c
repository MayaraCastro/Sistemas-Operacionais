#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>


struct paginaVirtual {
	int id;
    int real; // id do Real que o paginaVirtual esta
    struct paginaVirtual *prox;
};
struct paginaReal {
	int id; //id da pag virtual
    int r; //
    struct paginaReal *prox;
};



struct paginaReal *cabecaReal = NULL;
struct paginaVirtual *cabecapaginaVirtual = NULL;

//struct paginaReal *atual = NULL;

struct paginaReal *atualReal = NULL;
struct paginaVirtual *atualpaginaVirtual = NULL;



// Funções de lista
void printReals() {
   struct paginaReal *paginaReal = cabecaReal;
   printf("\n\n");

   while(paginaReal != NULL) {
        if(paginaReal->id == -1){
            printf("|  ,  |");
        }else{
            printf("| %i, R=%i |",paginaReal->id,paginaReal->r);
        }

      paginaReal = paginaReal->prox;
   }
}

//insereReal o paginaReal na primeira posiçao
void insereReal(int id, int r) {
   //cria um paginaReal
   struct paginaReal *paginaReal = (struct paginaReal*) malloc(sizeof(struct paginaReal));

   paginaReal->id = id;
   paginaReal->r = r;

   paginaReal->prox = NULL;
    if(atualReal != NULL){
           atualReal->prox = paginaReal;
    }
    if(cabecaReal == NULL){
        cabecaReal = paginaReal;
    }
   atualReal = paginaReal;
}

//insereReal o paginaReal na primeira posiçao
void inserepaginaVirtual(int id, int real) {
   //cria um paginaReal
   struct paginaVirtual *proceso = (struct paginaVirtual*) malloc(sizeof(struct paginaVirtual));

   proceso->id = id;
   proceso->real = real;

   proceso->prox = NULL;

   if(atualpaginaVirtual != NULL){
    atualpaginaVirtual->prox = proceso;

   }
   if(cabecapaginaVirtual == NULL){
        cabecapaginaVirtual = proceso;
   }
   atualpaginaVirtual = proceso;
}

//pop o primeiro item
struct paginaVirtual* poppaginaVirtual(){
    struct paginaVirtual* atual = cabecapaginaVirtual;
    cabecapaginaVirtual = cabecapaginaVirtual->prox;

    return atual;
}

//pop o primeiro item
struct paginaReal* poppaginaReal(){
    struct paginaReal* atual = cabecaReal;
    cabecaReal = cabecaReal->prox;

    return atual;
}



//se a lista ta vazia
bool isEmpty() {
   return (cabecaReal == NULL);
}

//retorna o tamanho da lista
int tamanho() {
   int tamanho = 0;
   struct paginaReal *atual;

   for(atual = cabecaReal; atual != NULL; atual = atual->prox) {
      tamanho++;
   }

   return tamanho;
}

//encontra o paginaReal na dada id na lista
struct paginaReal* findID(int id) {
   struct paginaReal* atual = cabecaReal;

   //ve se a lista esta vazia
   if(cabecaReal == NULL) {
      return NULL;
   }

   while(atual != NULL) {
        //se a posicao atual é a posicao passada no parametro
    if(atual->id==id){
        return atual;
    }
        //vai pro prox paginaReal
    atual = atual->prox;

   }

    return NULL;

}

//encontra o paginaReal na dada id na lista
struct paginaVirtual* findIDVirtual(int id) {
   struct paginaVirtual* atual = cabecapaginaVirtual;

   //ve se a lista esta vazia
   if(cabecapaginaVirtual == NULL) {
      return NULL;
   }

   while(atual != NULL) {
        //se a posicao atual é a posicao passada no parametro
    if(atual->id==id){
        return atual;
    }
        //vai pro prox paginaReal
    atual = atual->prox;

   }

    return NULL;

}

//deleta o paginaReal do id dado
struct paginaReal* deletepaginaReal(int id) {
   struct paginaReal* atual = cabecaReal;
   struct paginaReal* anterior = NULL;

   if(cabecaReal == NULL) {
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

   //found a match, update the paginaReal
   if(atual == cabecaReal) {
      //change first to point to prox paginaReal
      cabecaReal = cabecaReal->prox;
   } else {
      //bypass the atual paginaReal
      anterior->prox = atual->prox;
   }

   return atual;
}


void criaReals(void){

	insereReal(-1,0);
	insereReal(-1,0);
	insereReal(-1,0);
	insereReal(-1,0);
	insereReal(-1,0);
	insereReal(-1,0);
	insereReal(-1,0);
	insereReal(-1,0);


}
void criapaginaVirtuals(){

    inserepaginaVirtual(1, -1);
    inserepaginaVirtual(2, -1);
    inserepaginaVirtual(3, -1);
    inserepaginaVirtual(4, -1);

    inserepaginaVirtual(5, -1);
    inserepaginaVirtual(6, -1);
    inserepaginaVirtual(7, -1);
    inserepaginaVirtual(8, -1);

    inserepaginaVirtual(9, -1);
    inserepaginaVirtual(10, -1);
    inserepaginaVirtual(11, -1);
    inserepaginaVirtual(12, -1);

    inserepaginaVirtual(13, -1);
    inserepaginaVirtual(14, -1);
    inserepaginaVirtual(15, -1);
    inserepaginaVirtual(16, -1);


}


/***

***/
void segChance(){

    int menorReal = 10000;
    int difTam;
    struct paginaVirtual* paginaVirtual = cabecapaginaVirtual;

    while(paginaVirtual!= NULL){
    printf("\nEncaixando pag %i...", paginaVirtual->id);
        if(paginaVirtual->real == -1){//se tem um MISS, a pag n esta na memoria real
            struct paginaReal* Real = cabecaReal;
            while(Real!= NULL) {
                if(findID(-1)==NULL){
                    if(Real->r == 1){ //se tiver sido referenciada
                        if(Real->prox != NULL){
                            if((Real->prox)->id != -1){
                                poppaginaReal();
                                insereReal(Real->id, 0);
                            }

                        }

                    }else{

                        findIDVirtual(Real->id)->real = -1;
                        poppaginaReal();
                        insereReal(paginaVirtual->id, 1);
                        paginaVirtual->real = 1; //findPosicao(paginaVirtual->id);
                        break;
                    }
                }
                else{
                    findID(-1)->id = paginaVirtual->id;
                    findID(paginaVirtual->id)->r = 1;
                    paginaVirtual->real = 1; //findPosicao(paginaVirtual->id);
                    break;

                }

                Real = Real->prox;
                printReals();
                Sleep(1000);
              //  printf("\n");
            }
        }else{//se Nao tiver MISS
            findID(paginaVirtual->id)->r = 1;

        }
        paginaVirtual = paginaVirtual->prox;
        printReals();
        Sleep(1000);
    }

}
/* ************************************************** Main */
int main (void) {
	criaReals();
	criapaginaVirtuals();
	printReals();
    segChance();
	return 0;

}
