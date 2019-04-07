
#include <stdio.h>
//#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>
#include <unistd.h> 
#define N 5 //numero de processos

// Declaration of thread condition variable 
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER; 
  
// declaring mutex 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
  
int done = 1; 
int count=11;
// Thread function 
void* th(int processoID) 
{ // acquire a lock 
  pthread_mutex_lock(&lock); 

  while(count>=0){


    if (done == 1) { 
  
        // let's wait on conition varible cond1 
        done = 2; 
        //printf("Waiting on condition variable cond1\n"); 
        printf("  Thread %i: ... Esta suspenso ... \n\n",processoID);

        pthread_cond_wait(&cond1, &lock); 
    } 
    else { 
  
        // Let's signal condition variable cond1 
        //printf("Signaling condition variable cond1 thread %i \n", processoID); 
        pthread_cond_signal(&cond1); 
        printf("  Thread %i: ... Esta executando ... \n",processoID);
        done=1;
        Sleep(1000);
    } 
    
    count--;
    
  }
    
    // release lock 
  pthread_mutex_unlock(&lock); 
  
    printf("Returning thread\n"); 
  
    return NULL; 
} 

int main() 
{ 
    pthread_t th1, th2, th3, th4,th5; 

    if( pthread_create( &th1, NULL, (void *) th,(void *) 0 ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 0.\n");
    exit(1);
  }


  if( pthread_create( &th2, NULL, (void *) th,(void *) 1 ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 1.\n");
    exit(1);
  }

  if( pthread_create( &th3, NULL, (void *) th,(void *) 2 ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 0.\n");
    exit(1);
  }


  if( pthread_create( &th4, NULL, (void *) th,(void *) 3 ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 1.\n");
    exit(1);
  }

  if( pthread_create( &th5, NULL, (void *) th,(void *) 4 ) != 0 ) {
    printf("Error \"pthread_create\" p/ Thread 1.\n");
    exit(1);
  }

    pthread_join(th2, NULL); 
    pthread_join(th3, NULL); 
    pthread_join(th4, NULL);
    pthread_join(th5, NULL); 
    return 0; 
} 

