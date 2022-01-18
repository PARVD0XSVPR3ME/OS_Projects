#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "display.h"
#include <pthread.h>

pthread_mutex_t mutex; //dimiourgia neou mutex

void *func(void *param) { //synarthsh pou ekteleitai apo ta threads
	char * str = (char * ) param; //h parametros metatrepetai apo void se char *
	
	for (int i=0;i<10;i++) {
		//sthn krisimh perioxh ginetai amoivaios apoklismos gia thn apofigh tou anakatematos twn minimatwn
		pthread_mutex_lock(&mutex); // kanw lock to mutex.an exei ginei hdh lock apo allo thread tote perimenw 
		//na ginei ksana diatheshmo dhladh na ginei unlock apo to allo thread
		display(str);
		pthread_mutex_unlock(&mutex); //kanw unlock to mutex afou teleiwsw thn ektelesh ths krisimhs perioxhs
	}
	
	return NULL;
}




int main()
{	
  pthread_t id1[2]; //dimiourgia enos array twn 2 thread
  
  pthread_mutex_init(&mutex, NULL); //arxikopoihsh mutex
  
  char *str1 = "Hello world\n";
  char *str2 = "Kalimera kosme\n";
  
  
  pthread_create(&id1[0], NULL,(void *) func, (void *) str1 ); //dimiourgw ena neo thread sthn thesh 0
  //tou pinaka id1 xwris attributes pou tha ektelei thn synarthsh func me orisma to str1 pou periexei to 
  //hello world 
  //h parametros ginetai cast se void*
  
  pthread_create(&id1[1], NULL,(void *) func, (void *) str2 ); //dimiourgw ena neo thread sthn thesh 1
  //tou pinaka id1 xwris attributes pou tha ektelei thn synarthsh func me orisma to str2 pou periexei to 
  //kalimera kosme 
  //h parametros ginetai cast se void*
  
  pthread_join(id1[0], NULL); //perimenw na teleiwsei to thread sthn thesh 0 tou pinaka pou mporei na 
  //exei stamatisei
  pthread_join(id1[1], NULL); //perimenw na teleiwsei to thread sthn thesh 1 tou pinaka pou mporei na 
  //exei stamatisei
  pthread_mutex_destroy(&mutex); //katastrofh tou mutex
  return 0;
}