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
  struct sembuf up2 = {1, 1, 0}; //opou to prwto 1 o arithmos tou semaphore,o assos gia entolh up
  struct sembuf down2 = {1, -1, 0}; //opou to prwto 1 o arithmos tou semaphore,to -1 gia entolh down
  int my_sem; //semaphore array identifier
  my_sem = semget(IPC_PRIVATE, 2, 0600); //dimioutgia pinaka me semaphores
  //me key=IPC_PRIVATE ,2 semaphores kai dikaiomata eggrafhs kai anagnwshs apo to xrhsth  (user read write)
  semop(my_sem, &up2, 1); 
  
  //theloume panta na typonetai prwta to ab kai meta to cd\na
  
  
  //arxika ksekinaw thetontas prwto semaphore iso me 0 kai to deytero iso me 1
  //  me thn semop(my_sem, &up2, 1)

  int i;
  

  if (fork())
  {
    for (i=0;i<10;i++) {
	  semop(my_sem, &down2, 1); //mpainontas sto critical region thetw DOWN(sem) ston 2o semaphore
	  //ara an einai h prwth epanalypsh h an exei emfanistei to cd\n tote emfanizetai to ab alliws perimenw
      display("ab");
	  
	  
	  semop(my_sem, &up, 1); //vgainontas thetw UP(sem) sto 1o semampore
	  
      
	}
    wait(NULL);
  }
  else
  {
    for (i=0;i<10;i++) {
	  
	  
	  semop(my_sem, &down, 1); //sthn prwth epanalypsh kai otan den exei emfanistei to ab einai 0 opote perimenw
	  //alliws  emfanizw to cd\n
	  
      display("cd\n");
	  
	  semop(my_sem, &up2, 1); //vgainontas thetw UP(sem) ston 2o semaphore
	  
	  
	}
	
	
  }
  semctl(my_sem, 0, IPC_RMID); //mesw ths semctl me thn entolh IPC_RMID katastrefw to  prwto semaphore
  semctl(my_sem, 1, IPC_RMID); //mesw ths semctl me thn entolh IPC_RMID katastrefw to deytero semaphore
  return 0;
}
