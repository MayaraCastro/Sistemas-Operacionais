
#include <stdio.h>
//#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>

#define N 5 //numero de processos

int turn; //Indica de quem é a vez de entrar na RC

//-----------------------
void regiao_critica(int processoID){
    /* Processo dentro da Regiao Critica */
	printf("  Thread %i: ... Entrou na Regiao Critica ... \n",processoID);

	/* Simula um Processamento qualquer */
	Sleep(5000);
}

void regiao_naoCritica(int processoID){
    printf("  Thread %i: ... Saiu da Regiao Critica ... \n",processoID);
    if(processoID+1 >=N){
		turn = 0;
	}
	else{
		turn = processoID+1;
	}
}


void pth( int processoID ) //cria uma thread generica. Só precisa entrar com o numero da thread. 0 ou 1, por enquando tepois vai ate 5
{
	
		while(turn!=processoID){
		//não faz nada
		//printf("  Thread %i: ... Ocupado, espere ... \n",processoID);
		}

		regiao_critica(processoID);

		regiao_naoCritica(processoID);
	   
}


int main( int argc, char* argv[] )
{
  pthread_t th0, th1, th2, th3,th4;
  void * r_th0;
  void * r_th1;
  void * r_th2;
  void * r_th3;
  void * r_th4;

  turn = 0;

  printf("Algoritmo de alternancia estrita \n");

  if( pthread_create( &th0, NULL, (void *) pth,(void *) 0 ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 0.\n");
    exit(1);
  }


  if( pthread_create( &th1, NULL, (void *) pth,(void *) 1 ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 1.\n");
    exit(1);
  }
  if( pthread_create( &th2, NULL, (void *) pth,(void *) 2 ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 2.\n");
    exit(1);
  }

  if( pthread_create( &th3, NULL, (void *) pth,(void *) 3 ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 3.\n");
    exit(1);
  }

  if( pthread_create( &th4, NULL, (void *) pth,(void *) 4 ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 4.\n");
    exit(1);
  }

 	pthread_join(th1, NULL); 
    pthread_join(th2, NULL); 
    pthread_join(th3, NULL);
    pthread_join(th4, NULL); 

  exit(0);

}

