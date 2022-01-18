#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "display.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int main()
{
  struct sembuf up = {0, 1, 0}; //opou to prwto 0 o arithmos tou semampore , o assos gia entolh up ,kai 0 ta flags ths entolhs
  struct sembuf down = {0, -1, 0}; //to -1 gia entolh down
  int my_sem; //semaphore array identifier
  my_sem = semget(IPC_PRIVATE, 1, 0600); //dimioutgia pinaka me semaphores
  //me key=IPC_PRIVATE ,enan semaphore kai dikaiomata eggrafhs kai anagnwshs apo to xrhsth  (user read write)
  semop(my_sem, &up, 1); // mesw ths semop ,ston semaphore array identifier mysep ,kanw up() me th vohtheia tou struct up
  //pou periexei entoles pros ektelesh dhladh thn up ,kai 1 sthn semop to megethos tou struct up 
  //gia na apofigoume to anakatema twn minimatwn poy typonontai xreizetai na ilopoihthei amoivaios apokleismos
  //wste na min ektelesoun to krisimo kommati kwdika perisoteres apo mia diergasies taytoxrona
  //ara ksekinaw thetontas sem=1 mesw ths semop(my_sem, &up, 1);
  int i;

  if (fork())
	  //parent
  { 
    for (i=0;i<10;i++) {
		semop(my_sem, &down, 1); //mpainontas sto critical region thetw DOWN(sem)
        display("Hello world\n");
		semop(my_sem, &up, 1); //vgainontas thetw UP(sem)
	}
	
    wait(NULL);
	
	
  }
  else
	  //child
  {
	 
    for (i=0;i<10;i++) {
	  semop(my_sem, &down, 1); //mpainontas sto critical region thetw DOWN(sem)
      display("Kalimera kosme\n");
	  semop(my_sem, &up, 1); //vgainontas thetw UP(sem)
	}
	
	 
  }

  semctl(my_sem, 0, IPC_RMID); //mesw ths semctl me thn entolh IPC_RMID katastrefw to semaphore
  return 0;
}