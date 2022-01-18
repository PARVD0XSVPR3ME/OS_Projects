#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int poss=0;

/*---------------------------------------------------------------------*/

int exit_term()
{
  return 0;
}

/*---------------------------------------------------------------------*/

int cd_term(char **args) { //synarthsh pou ylopoiei to cd
	
		
	if(args[1] == NULL) { 	 //an dwthei sketo cd xwris kapoio path
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

char **term_split_line(char *trm_line) // synarthsh pou xwrizei thn eisodo tou xrhsth 
//se 2 entoles analoga me to pou vrisketai o pipe operator
{
  int  pos_1=-1;
  
  char *token_1;
  char **tokens_1 = malloc(2 * sizeof(char*)); //dhmiourgia enos 2d pinaka string gia thn apothikeysh twn
  //2 entolwn
  
	do 
    {
        token_1 = strsep(&trm_line, "|"); // me th xrhsh ths strsep() vriskw to prwto token sthn trm_line
		//opou ta token xwrizontai apo ta delimiter ths morfhs "|" diladi kathetous
		// kai to svinw grafontas panw sth katheto "|" to xarakthra \0 
		//kai h metavlhth trm_line enhmerwnetai wste na deixnei amesws meta to prwto token
		//h strsep epistrefei enan pointer pou deixnei sto token  
		//ara to token_1 deixnei sto prwto token pou vriskoume kai h strsep enhmerwnetai na deixnei amesws meta to token
		
 		

        if(token_1 != NULL)  //an den exei ftasei to telos tou string
        {
            if(strlen(token_1) > 0) //an to mikos tou einai megalitero tou 0 
            {
                tokens_1[++pos_1] = token_1; //tote apothikevw to token_1 pou einai h entolh h ta arguments ths entolhs pou dinei o xrisths
				//ston pinaka tokens_1
            }
        }
    } while(token_1); //mexri na skanarw olo to string eisodou pou dinei o xrhsths
  
  poss=pos_1;
  return tokens_1;
}

/*---------------------------------------------------------------------*/

int term_parse_exec(char **args)
{
  pid_t childPid2;
  int status;
  char *com1[4096];
  int countc;
  int newpipefd[2],oldpipefd[2];
  int buffer_size = 64;
  char *temp = NULL;
  

com1[++poss] = NULL; //simatodwtw to telos tou com1 pou einai oso to mikos tou tokens_1

for (int j=0;j<2;j++)	{  //exw 1 pipe ara epanalipsh gia 2 entoles
  countc = -1;

   do {
    temp = strsep(&args[j], " \t\r\n\a"); //me th xrhsh ths strsep epistrefw sthn temp to prwto token
	//mexri na sinantisw ena keno dhladh sthn temp apothikevetai h entolh h ena argument kai h args
	//enhmerwnetai na dixnei amesws meta to token 
        if(temp != NULL) //an den exei ftasei to telos tou string 
        {
            if(strlen(temp) > 0) //an to mikos tou einai megalitero tou 0 
            {
                
                com1[++countc] = temp; //apothikevw sto com1 to token pou einai apothikevmeno sto temp
            }
        }
    } while(temp); //mexri na skanarw olo to string eisodou pou dinei o xrhsths

    com1[++countc] = NULL;  //simatodwtw to telos tou com1
  //******************************************************************
  if (com1[0] == NULL) {
	//den dwthike entolh h dwthike kenh entolh 
    return 1;
  }

  if (strcmp(com1[0], "exit") == 0) { //an o xrhsths pliktrologise exit tote kaleitai h synarthsh exit_term
  //kai termatizei to shell
     return exit_term();
  }
  
  if (strcmp(com1[0], "cd") == 0) { //an o xrhsths pliktrologise cd tote kaleitai h synarthsh cd_term
     return cd_term(com1);
  }
  if(j < poss-1) pipe(newpipefd); //an yparxoun akoma entoles xrhsth pou prepei na ektelestoun dimiourgw neo pipe
  //To newpipefd einai enas pinakas 2 file descriptors pou epistrefontai mesw ths pipe
  //to newpipefd[0] anaferetai sthn anagnwsh apo to pipe 
  //kai to newpipefd[1] sthn egrafh sto pipe
  

	childPid2 = fork(); 
	// otan h fork() epistrefei , exoume 2 diergasies na trexun parallila 
	if (childPid2 == 0) { 
	// Child process
	
	
    if(j > 0) //an yparxei prohgoumenh entolh 
    {
        close(oldpipefd[1]); //klinw th meria ths eggrafhs sto pipe afou den th xreiazomai
        dup2(oldpipefd[0], 0); //anakateythinsh tou stdin sto input tou pipe
        close(oldpipefd[0]); //klinw th meria ths anagnwshs sto pipe afou den th xreiazomai
    }
	if(j < poss-1) //an yparxoun akoma entoles xrhsth pou prepei na ektelestoun
    {
        close(newpipefd[0]); //klinw th meria ths anagnwshs sto pipe afou den th xreiazomai
        dup2(newpipefd[1], 1); //anakateythinsh tou stdout sto output tou pipe
        close(newpipefd[1]); //klinw th meria ths eggrafhs sto pipe afou den th xreiazomai
    }

	if (execvp(com1[0], com1) == -1) { //ektelesh entolhs
      perror("execvp error"); //ektws an symvei lathos opote emfanizw minima lathous 
    }
    exit(EXIT_FAILURE); // termatizw me lathos 
    } 
	else if (childPid2 < 0) { //an egine lathos sto fork  
    // Error forking
    perror("fork error"); //emfanizw minima lathous
    }
	
	else {  // an h fork() ektelesthke kanonika
    // Parent process

		if(j > 0) //an yparxei prohgoumenh entolh 
        {
            close(oldpipefd[0]); //klinw th meria ths anagnwshs sto pipe afou den th xreiazomai
            close(oldpipefd[1]); //klinw th meria ths eggrafhs sto pipe afou den th xreiazomai
        }
		if(j < poss-1) //an yparxei epomenh entolh 
        {
            oldpipefd[0] = newpipefd[0];
            oldpipefd[1] = newpipefd[1];
        }

		if(j == poss-1) // an eimai sthn teleytaia entolh
        {
            waitpid(childPid2, &status, 0);
        }

		
	
	}
	
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
		args = term_split_line(term_line); //kalw synarthsh gia epeksergasia eisodou
		stat = term_parse_exec(args); //kalw synarthsh gia ektelesh entolhs
		
		free(term_line);
		free(args);
	} while (stat);
	

	
}

