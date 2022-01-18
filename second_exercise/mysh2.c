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

int cd_term(char **args) { //synarthsh pou ylopoiei to cd
	
		
	if(args[1] == NULL) { 	//an dwthei sketo cd xwris kapoio path
		if (chdir(getenv("HOME")) !=0 ) { //tote metaferomai sto /root directory
			perror("error"); //ektws an symvei kapoio lathos opote emfanizw minima lathous 
		}
		
	}
	else { //diaforetika 
		if(args[1][0] == '/' || args[1][0] == '.' || (args[1][0] == '.' && args[1][1] == '.') ) {
		//an dwthei absolute path ths morfhs cd /Documents/a/b/ 
		//h an dwthike entolh me relative paths ths morfhs cd ../Pictures afou h chdir anagnwrizei 
		//paths pou periexoun ta . kai ..
			if (chdir(args[1]) != 0) {
				perror("error");
			}
			
				
		}
	    else { //an dwthike relative path ths morfhs cd Documents 
			
			char path[4096]; 
			strcpy(path,args[1]); //apothikevw sthn metavlhth path to path pou dwthike apo to xrhsth
			char cwd[4096];
			
			getcwd(cwd,sizeof(cwd)); //vriskw to current working directory diladh to trexwn directory
			//mesw ths getcwd kai to apothikevw sthn cwd
			
			strcat(cwd,"/"); //prosthetw thn katheto "/" meta to current working directory sthn cwd
			strcat(cwd,path); //prosthetw to path meta thn katheto sto cwd
			
			if (chdir(cwd) != 0) {
				perror("error");
			}
			
			
		}
	}
	return 1;
}

/*---------------------------------------------------------------------*/

char* term_read_line(void) //synarthsh pou diavazei eisodo xrhsth
{
  char *trm_line = NULL;
  size_t buffersize = 0;  //H getline apofasizei gia to megethos tou buffer 
  
  getline(&trm_line, &buffersize, stdin); //xrisimopoiw thn getline gia na diavasw eisodo ap to stdin
  //kai to apothikeyw sthn trm_line
  return trm_line;
}

/*---------------------------------------------------------------------*/

char **term_parse_line(char *trm_line) //synarthsh pou epeksergazetai eisodo xrhsth
{
  int buffer_size = 64, pos = 0;
  char **tokens = malloc(buffer_size * sizeof(char*)); //gia to megethos tou pinaka diladh to 
  //posa arguments xwraei dinw ena default megethos me thn malloc dhladh 64
  char *token;
  
  if (!tokens) {
    fprintf(stderr, "malloc allocation error\n");
    exit(EXIT_FAILURE);
  }
  
  token = strtok(trm_line, " \t\r\n\a"); //me th xrhsh ths strtok krataw sto token thn prwth entolh 
  
  while (token != NULL) {
    tokens[pos] = token; //apothikevw thn entolh kai epeita ta arguments se enan 2d pinaka me strings
	//me onoma tokens
    pos++;

    if (pos >= buffer_size) { //an kseperasw to trexwn megethos tou pinaka 
      buffer_size += 64; 
      tokens = realloc(tokens, buffer_size * sizeof(char*)); //to diplasiazw
      if (!tokens) { //an symvei lathos emfanizw minima lathous 
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
	
	token = strtok(NULL, " \t\r\n\a"); //sinexizw na "spaw" thn eisodo tou xrhsth se 
	//arguments analoga me to an sinantisw kapoio keno
  }
  tokens[pos] = NULL; //telos shmatodotw to telos tou pinaka me thn entolh kai ta arguments
  return tokens;
}

/*---------------------------------------------------------------------*/

int term_exec(char **args) ////synarthsh gia thn ektelesh ths entolhs tou xrhsth
{
  pid_t childPid, wait_pid;
  int status;
  
  if (args[0] == NULL) {
	//den dwthike entolh h dwthike kenh entolh 
    return 1;
  }

  if (strcmp(args[0], "exit") == 0) { //an o xrhsths pliktrologise exit tote kaleitai h synarthsh exit_term
  //kai termatizei to shell
     return exit_term();
  }
  
  if (strcmp(args[0], "cd") == 0) { //an o xrhsths pliktrologise cd tote kaleitai h synarthsh cd_term
  
     return cd_term(args);
  }
  
  childPid = fork(); 
  // otan h fork() epistrefei , exoume 2 diergasies na trexun parallila 
  
  if (childPid == 0) { 
    // Child process
	//Sthn diergasia child theloume na ektelesoume thn entolh pou dwthike ap to xrhsth 
    if (execvp(args[0], args) == -1) { //ektelesh entolhs
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
	char **args;
	int stat;
	
	do {
		printf("$ "); //emfanish prompt
		term_line = term_read_line(); //kalw synarthsh gia diavasma eisodou xrhsth
		args = term_parse_line(term_line); //kalw synarthsh gia epeksergasia eisodou
		stat = term_exec(args); //kalw synarthsh gia ektelesh entolhs
		
		free(term_line);
		free(args);
	} while (stat);
	

	
}

