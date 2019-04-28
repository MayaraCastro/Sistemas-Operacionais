#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <pthread.h>


struct blocoMem {
    struct unidadeMem *inicio;
    struct unidadeMem *fim;
	int id;
    int tam;
    struct processo *processo; //
    struct blocoMem *prox;
};

struct unidadeMem {
	int id; //se -1 entao unidade ta livre
    struct unidadeMem *prox;
};

struct processo {
	int id;
    int tam;
    int tempo; //tempo do sleep
    int bloco; // id do bloco que o processo esta
    struct processo *prox;
    struct unidadeMem *blocoInicio;
};

struct blocoMem *cabecaBloco = NULL;
struct processo *cabecaProcesso = NULL;

struct unidadeMem *cabecaMemoria = NULL;
struct unidadeMem *atualMemoria = NULL;

struct blocoMem *atual = NULL;

struct blocoMem *atualBloco = NULL;
struct processo *atualProcesso = NULL;

int turn = 1;
int memTam= 512;
int memUso=0;
bool podeDes = true;
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
void insereUnidadeMemoria(int id) {
   //cria um blocoMem
   struct unidadeMem *unidadeMem = (struct unidadeMem*) malloc(sizeof(struct unidadeMem));

   unidadeMem->id = id;

   unidadeMem->prox = NULL;
    if(atualMemoria != NULL){
           atualMemoria->prox = unidadeMem;

    }
    if(cabecaMemoria == NULL){
        cabecaMemoria = unidadeMem;
    }
   atualMemoria = unidadeMem;
}

void printMemoria() {
   struct unidadeMem *unidadeMem = cabecaMemoria;
   printf("\n\n");

   int tam = 1;
   int idProcess;

   if(unidadeMem != NULL){
    idProcess = unidadeMem->id;
   }
   while(unidadeMem != NULL) {
        if(unidadeMem->prox != NULL){
            if(unidadeMem->prox->id == idProcess){
                tam++;
            }else{
                if(idProcess == -1){
                    printf("| %iMB Livre |\n", tam);
                }else{
                    printf("| %iMB P = %i |\n", tam, idProcess);
                }
                tam = 1;
                idProcess = unidadeMem->prox->id;
            }
        }
        else{
                if(idProcess == -1){
                    printf("| %iMB Livre |\n", tam);
                }else{
                    printf("| %iMB P = %i |\n", tam, idProcess);
                }

        }


      unidadeMem = unidadeMem->prox;
}
}


//insereBloco o blocoMem na primeira posiçao
struct processo* insereProcesso(int id, int tam, int tempo) {
   //cria um blocoMem
   struct processo *proceso = (struct processo*) malloc(sizeof(struct processo));

   proceso->id = id;
   proceso->tam = tam;
   proceso->bloco = -1;
   proceso->tempo = tempo;

   proceso->prox = NULL;

   if(atualProcesso != NULL){
    atualProcesso->prox = proceso;

   }
   if(cabecaProcesso == NULL){
        cabecaProcesso = proceso;
   }
   atualProcesso = proceso;

   return(proceso);
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

void alocarUnidades(struct unidadeMem* inicio,int tamBloco, int idProcesso){
    struct unidadeMem* unidadeMem = inicio;
    int tam = 0;
    while(tamBloco != tam){

        unidadeMem->id = idProcesso;
        tam++;
        unidadeMem = unidadeMem->prox;
        if(unidadeMem == NULL){
            break;
        }
    }
}
struct unidadeMem* tamBloco(struct unidadeMem* inicio, int* tam,int idProcesso){

    struct unidadeMem* unidadeMem = inicio;
    while(unidadeMem->id == idProcesso){

        *tam = *tam +1;

    //printf("\n%i", *tam);

        if( unidadeMem->prox == NULL){
            //printf("\nBREAK  %i", *tam);
            break;
        }

        if( unidadeMem->prox->id != idProcesso){
            //printf("\nBREAK  %i", *tam);
            break;
        }
    unidadeMem = unidadeMem->prox;

    }
    return(unidadeMem);

}

void best_fit2(){

    struct blocoMem* menorBloco = (struct blocoMem*) malloc(sizeof(struct blocoMem));
    menorBloco->tam = 10000;
    menorBloco->id = -1;//n alocado bloco

    int difTam;
    struct processo* processo = cabecaProcesso;
    int* tam = malloc(sizeof(int));
    while(processo!= NULL){

        turn = processo->id;
        struct unidadeMem* unidadeMem = cabecaMemoria;
        *tam = 0;
        while(unidadeMem!= NULL) {

            if(unidadeMem->id == -1){ //se não tiver processo naquele bloco
               struct unidadeMem* fimBloco =  tamBloco(unidadeMem,tam,unidadeMem->id);

                difTam = *tam - processo->tam;
                if(difTam >= 0){//verifica se o tamanho do processo cabe no bloco
                    if(menorBloco->tam > difTam)
					{
						menorBloco->inicio = unidadeMem;
						menorBloco->fim = fimBloco;
						menorBloco->tam = difTam;
						menorBloco->id = processo->id; //bloco alocado
					}
                }
                unidadeMem = fimBloco;
            }else{
                unidadeMem = unidadeMem->prox;
            }


        }

        if(menorBloco->id != -1){ // se puder alocar
            alocarUnidades(menorBloco->inicio,processo->tam, processo->id);
            processo->bloco = 1; //alocado
            processo->blocoInicio = menorBloco->inicio;
        }else{
            printf("\n Memoria insuficiente para o processo %i\n", processo->id);
        }

        processo = processo->prox;
        printMemoria();
        Sleep(1000);
        menorBloco->tam = 10000;
        menorBloco->id = -1;//n alocado bloco
    }

}


void compactar(){
    int* tam = malloc(sizeof(int));
    struct unidadeMem* unidadeMem = cabecaMemoria;
    *tam = 0;
        while(unidadeMem!= NULL) {

            if(unidadeMem->prox != NULL && unidadeMem->prox->id == -1){ //se não tiver processo naquele bloco
               struct unidadeMem* fimBloco =  tamBloco(unidadeMem->prox,tam,unidadeMem->prox->id);


                if(fimBloco != NULL && fimBloco->prox != NULL){
                    atualMemoria->prox = unidadeMem->prox;

                    unidadeMem->prox = fimBloco->prox;
                    fimBloco->prox = NULL;
                    atualMemoria = fimBloco;
                }else{
                    break;
                }

            }

            if(unidadeMem->id == -1){ //se não tiver processo naquele bloco
               struct unidadeMem* fimBloco =  tamBloco(unidadeMem,tam,unidadeMem->id);



                if(fimBloco != NULL && fimBloco->prox != NULL){
                    atualMemoria->prox = unidadeMem;


                    if(cabecaMemoria == unidadeMem) {
                       cabecaMemoria =fimBloco->prox;
                    }
                    fimBloco->prox = NULL;
                    atualMemoria = fimBloco;
                }else{

                    break;
                }

            }

            unidadeMem = unidadeMem->prox;
        }
}
void best_fit3(struct processo* processo){
    struct blocoMem* menorBloco = (struct blocoMem*) malloc(sizeof(struct blocoMem));
    menorBloco->tam = 10000;
    menorBloco->id = -1;//n alocado bloco

    int difTam;
    int* tam = malloc(sizeof(int));

    while(true){ //quando aloca tem um break que sai do loop

        struct unidadeMem* unidadeMem = cabecaMemoria;
        *tam = 0;
        while(unidadeMem!= NULL) {

            if(unidadeMem->id == -1){ //se não tiver processo naquele bloco
               struct unidadeMem* fimBloco =  tamBloco(unidadeMem,tam,unidadeMem->id);

                difTam = *tam - processo->tam;
                if(difTam >= 0){//verifica se o tamanho do processo cabe no bloco
                    if(menorBloco->tam > difTam)
					{
						menorBloco->inicio = unidadeMem;
						menorBloco->fim = fimBloco;
						menorBloco->tam = *tam;
						menorBloco->id = processo->id; //bloco alocado
					}
                }
                unidadeMem = fimBloco->prox;
                *tam = 0;
            }else{
                unidadeMem = unidadeMem->prox;
            }


        }

        if(menorBloco->id != -1){ // se puder alocar
            alocarUnidades(menorBloco->inicio,processo->tam, processo->id);
            processo->bloco = 1; //alocado
            processo->blocoInicio = menorBloco->inicio;

            printf("\n processo tam %i em %i", processo->tam, menorBloco->tam);
            memUso = memUso + processo->tam;

            break;
        }else{
           // printf("\n Memoria insuficiente para o processo %i\n", processo->id);
           // printf("\nCompactando...\n");
            if(!podeDes){
                if((memTam - memUso) >= processo->tam){
                    printf("\n !!!!Memoria insuficiente para o processo %i\n", processo->id);
                    printf("\nCompactando...\n");
                    compactar();
                    printMemoria();
                }

                podeDes = !podeDes;
            }


        }
    }
}
void pth(struct processo* processo){

    while(turn != processo->id){
		//não faz nada
		//printf("  Thread %i: ... Ocupado, espere ... \n",processoID);
		//esperando a vez

		//printf("%\n P = %i", processo->id);

    }

    if(processo->bloco == -1){
        //Alocando
        best_fit3(processo);
        printf("\n\t-----Processo %i alocado: ", processo->id);
        printMemoria();

        Sleep(1000);
        turn = turn + 1 ;
    }

    //depois q alocou
    Sleep(processo->tempo); //espera 5 segs pra desalocar o processo da memoria
    while(!podeDes && !(turn>10 || memUso == processo->tam)){

    }
    if(podeDes || (turn>10 && memUso == processo->tam)){
        alocarUnidades(processo->blocoInicio, processo->tam, -1); //desaloca
        processo->bloco = -1;
        memUso = memUso - processo->tam;
        printf("\n\t-----Desaloca o processo %i:", processo->id);
        printMemoria();
        Sleep(1000);
        podeDes = !podeDes;
    }

}
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

   /* insereProcesso(1, 100);
    insereProcesso(2, 350);
    insereProcesso(3, 400);
    insereProcesso(4, 150);
    insereProcesso(5, 100);
    insereProcesso(6, 200);
    insereProcesso(7, 300);
    insereProcesso(8, 500);
    insereProcesso(9, 200);
    insereProcesso(10, 100);*/

    pthread_t th0, th1, th2, th3,th4;
    pthread_t th5, th6, th7, th8,th9;

    if( pthread_create( &th0, NULL, (void *) pth,(void *) insereProcesso(1, 100,5000) ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 0.\n");
    exit(1);
  }


  if( pthread_create( &th1, NULL, (void *) pth,(void *) insereProcesso(2, 100,50000) ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 1.\n");
    exit(1);
  }
  if( pthread_create( &th2, NULL, (void *) pth,(void *) insereProcesso(3, 100,50000) ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 2.\n");
    exit(1);
  }

  if( pthread_create( &th3, NULL, (void *) pth,(void *) insereProcesso(4, 200,5000) ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 3.\n");
    exit(1);
  }

  if( pthread_create( &th4, NULL, (void *) pth,(void *) insereProcesso(5, 300,7000) ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 4.\n");
    exit(1);
  }

  if( pthread_create( &th5, NULL, (void *) pth,(void *) insereProcesso(6, 100,10000) ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 4.\n");
    exit(1);
  }
  if( pthread_create( &th6, NULL, (void *) pth,(void *) insereProcesso(7, 100,15000) ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 4.\n");
    exit(1);
  }
  if( pthread_create( &th7, NULL, (void *) pth,(void *) insereProcesso(8, 200,8000) ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 4.\n");
    exit(1);
  }
  if( pthread_create( &th8, NULL, (void *) pth,(void *) insereProcesso(9, 200,20000) ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 4.\n");
    exit(1);
  }
  if( pthread_create( &th9, NULL, (void *) pth,(void *) insereProcesso(10, 300,5000) ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 4.\n");
    exit(1);
  }

 	pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    pthread_join(th4, NULL);

    pthread_join(th5, NULL);
    pthread_join(th6, NULL);
    pthread_join(th7, NULL);
    pthread_join(th8, NULL);
    pthread_join(th9, NULL);
}

void criaMemoria(){
    //cria unidades livres de memoria
    for(int i = 0; i<512 ; i++){
        insereUnidadeMemoria(-1);
    }

}
/* ************************************************** Main */
int main (void) {
	/*criaBlocos();
	criaProcessos();
	best_fit();*/
	criaMemoria();
	printMemoria();
	criaProcessos();
	//best_fit2();
//exibir mensagem de eroo e fazer compactacao



	return 0;

}
