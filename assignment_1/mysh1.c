#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*---------------------------------------------------------------------*/

int exit_term() 
{
  return 0;
}
/*---------------------------------------------------------------------*/

char* term_read_line(void) //synarthsh pou diavazei eisodo xrhsth
{
  char *trm_line = NULL;
  size_t buffersize = 0; //H getline apofasizei gia to megethos tou buffer 
  
  getline(&trm_line, &buffersize, stdin); //xrisimopoiw thn getline gia na diavasw eisodo ap to stdin 
  //kai to apothikeyw sthn trm_line
  return trm_line;
}

/*---------------------------------------------------------------------*/

char *term_parse_line(char *trm_line) //synarthsh pou epeksergazetai eisodo xrhsth
{
  char *token;
  token = strtok(trm_line, " \t\r\n\a"); //me th xrhsh ths strtok krataw sto token thn prwth entolh 
  //dld apo to trm line (eisodos xrhsth) krataw to meros pou prokiptei mexri na sinantisw to prwto keno
  //ayto tha einai kai h entolh tou xrhsth
  return token;
}

/*---------------------------------------------------------------------*/

int term_exec(char *args) //synarthsh gia thn ektelesh ths entolhs tou xrhsth
{
  pid_t childPid, wait_pid;
  int status;
  char *argv[2];
  argv[0] = args;
  argv[1] = NULL;
  
  

  if (args == NULL) {
	//den dwthike entolh h dwthike kenh entolh 
    return 1;
  }

  if (strcmp(args, "exit") == 0) { //an o xrhsths pliktrologise exit tote kaleitai h synarthsh exit_term
  //kai termatizei to shell
     return exit_term();
  }
  
  childPid = fork(); 
  // otan h fork() epistrefei , exoume 2 diergasies na trexun parallila 
  
  if (childPid == 0) { 
    // Child process (diergasia paidi)
	//Sthn diergasia child theloume na ektelesoume thn entolh pou dwthike ap to xrhsth 
    if (execvp(args, argv) == -1) { //ektelesh entolhs
      perror("execvp error"); //ektws an symvei lathos opote emfanizw minima lathous
    }
    exit(EXIT_FAILURE); //termatizw me lathos 
    } 
	else if (childPid < 0) { //an egine lathos sto fork  
    // Error forking
    perror("fork error"); //emfanizw minima lathous
    } 
	else { // an h fork() ektelesthke kanonika
    // Parent process
	//h diergasia parent prepei na perimenei thn entolh na teleiwsei 
		do {
			wait_pid = waitpid(childPid, &status, WUNTRACED); 
			//WUNTRACED : To status opoioudipote child process poy simatodothtai ap to childPid
			//kai stamatise kai pou to status  den exei akoma anaferthei tha prepei na anaferthei sthn 
			//diergasia pou to zitaei
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		//perimenoume mexri oi diergasies na termatisoun (kanonika h me sfalma) h na termatisoun apo ena signal 
	}
	return 1;
}

/*---------------------------------------------------------------------*/

int main (int argc, char **argv) {
	char *term_line;
	char *args;
	int stat;
	
	do {
		printf("$ "); //emfanish prompt
		term_line = term_read_line(); //kalw synarthsh gia diavasma eisodou xrhsth
		args = term_parse_line(term_line); //kalw synarthsh gia epeksergasia eisodou
		stat = term_exec(args); //kalw synarthsh gia ektelesh entolhs
		
	} while (stat);
	

	
}

