#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "display.h"
#include <pthread.h>
#include <string.h>

pthread_mutex_t mutex; //dimiourgia neou mutex
pthread_cond_t cond_var;  //dimiourgia neou condition variable
//arxikopoihsh twn metavlhtwn predicate kai predicate2
//h predicate arxikopoihtai me 1 otan emfanistei to ab ginetai 0 (kai h predicate2 ginetai 1) 
//perimenontas etsi thn emfanish tou cd\n
//h predicate2 arxikopoihtai me 0 otan emfanistei to cd ginetai 0 (kai h predicate ginetai 1)
//perimenontas etsi thn emfanish tou ab
int predicate=1; 
int predicate2=0;
void *func1(void *param) { //synarthsh pou ekteleitai apo to thread sth thesh 0
	char * str = (char * ) param; //h parametros metatrepetai apo void se char *
	
	

		
		pthread_mutex_lock(&mutex);// kanw lock to mutex.an exei ginei hdh lock apo allo thread tote 
		//perimenw 
		//na ginei ksana diatheshmo dhladh na ginei unlock apo to allo thread
	for (int i=0;i<10;i++) {
		
		//sthn prwth epanalypsh to predicate exei thn timh 1 ara mesw ths display emfanizetai to ab
		//stis alles epanalipseis an to predicate einai iso me 0 pou simainei oti den exei emfanistei to 
		//cd\n
		//tote perimenw mexri na emfanistei gia na synexisw mesw ths pthread_cond_wait(&cond_var,&mutex); 
		while (predicate==0) 
			pthread_cond_wait(&cond_var,&mutex); 
		display(str);
		predicate=0; //afou teleiwsw me thn emfanish to ab thetw to predicate=0 wste sthn epomenh epanalypsh 
		//na perimenei 
		//thn emfanish tou cd\n prwta 
		predicate2=1; //kai thetw to predicate2 me thn timh 1 wste na na vgei apo to vrogxo while 
		//(predicate2==0) 
			//kai na ektelesei thn display pou emfanizei sthn othonh to cd\n sto deytero thread
		
		pthread_cond_signal(&cond_var); //ksemplokarw ta nhmata pou exoun mplokaristei apo mia condition 
		//variable
		
		
		
	}
	pthread_mutex_unlock(&mutex);//kanw unlock to mutex afou teleiwsw thn ektelesh ths krisimhs perioxhs
	
	
	return NULL;
}

void *func2(void *param) { //synarthsh pou ekteleitai apo to thread sth thesh 0
	char * str = (char * ) param; //h parametros metatrepetai apo void se char *
	
	

		
		pthread_mutex_lock(&mutex);// kanw lock to mutex.an exei ginei hdh lock apo allo thread tote 
		//perimenw 
		//na ginei ksana diatheshmo dhladh na ginei unlock apo to allo thread
	for (int i=0;i<10;i++) {
		//sthn prwth epanalypsh to predicate exei thn timh 0 ara perimenw na ektelestei prwta to nhma pou 
		//emfanizei mesw ths display to ab
		//stis alles epanalipseis an to predicate einai iso me 0 pou simainei oti den exei emfanistei to ab
		//tote perimenw mexri na emfanistei gia na synexisw ,mesw ths pthread_cond_wait(&cond_var,&mutex); 
		
		while (predicate2==0) 
			pthread_cond_wait(&cond_var,&mutex);

		display(str);
		predicate = 1;//afou teleiwsw me thn emfanish to cd\n thetw to predicate=1 wste na na vgei apo to 
		//vrogxo while (predicate==0) 
			//kai na ektelesei thn display pou emfanizei sthn othonh to ab sto prwto thread
		predicate2=0; //kai thetw to predicate2=0 wste sthn epomenh epanalypsh na perimenei 
		//thn emfanish tou ab prwta 
		
		pthread_cond_signal(&cond_var); //ksemplokarw ta nhmata pou exoun mplokaristei apo mia condition 
		//variable
		
		
	}
	pthread_mutex_unlock(&mutex);//kanw unlock to mutex afou teleiwsw thn ektelesh ths krisimhs perioxhs
	
	return NULL;
}






int main()
{
  pthread_t id1[2]; //dimiourgia enos array twn 2 thread
  //pthread_attr_t attr1,attr2;
  pthread_mutex_init(&mutex, NULL); //arxikopoihsh mutex
  pthread_cond_init(&cond_var, NULL); //arxikopoihsh condition variable
  
  char *str1 = "ab";
  char *str2 = "cd\n";
  
  
  pthread_create(&id1[0], NULL,(void *) func1, (void *) str1 ); //dimiourgw ena neo thread sthn thesh 0
  //tou pinaka id1 xwris attributes pou tha ektelei thn synarthsh func1 me orisma to str1 pou periexei to 
  //ab
  //h parametros ginetai cast se void*
  
  pthread_create(&id1[1], NULL,(void *) func2, (void *) str2 ); //dimiourgw ena neo thread sthn thesh 1
  //tou pinaka id1 xwris attributes pou tha ektelei thn synarthsh func2 me orisma to str2 pou periexei to 
  //cd\n
  //h parametros ginetai cast se void*
  
  
  pthread_join(id1[0], NULL); //perimenw na teleiwsei to thread sthn thesh 0 tou pinaka pou mporei na 
  //exei stamatisei
  pthread_join(id1[1], NULL); //perimenw na teleiwsei to thread sthn thesh 1 tou pinaka pou mporei na 
  //exei stamatisei
  pthread_mutex_destroy(&mutex);  //katastrofh tou mutex
  pthread_cond_destroy(&cond_var); //katastrofh condition variable
  return 0;
}
