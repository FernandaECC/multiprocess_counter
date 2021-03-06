/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define N_PROCESSOS 4		//numero maximo de processos


//funcao que calcula se o numero eh primo ou nao
unsigned int primo(unsigned int N) {
	int c;
	int div = 0;
	for (c = 1; c <= N; c++) {
    		if (N % c == 0) { 
     		div++;
    		}
  	}
    
  	if ( div == 2) {return 1;}
  	else {return 0; }
}


int main() {
  pid_t filho[N_PROCESSOS];
  
  int num_processos = 0;
  
  /*Tratamento e armazenamento da sequência de números (input)*/
  int i=0, j=0, v=0; //variaveis utilizadas nos for's
  
  int array[100]; //vetor da entrada
  int array_f0[100], array_f1[100], array_f2[100], array_f3[100]; 
  //um vetor para cada processo filho, distribuindo assim a entrada entre os processos filhos ---> cada processo filho sabera previamente quais numeros ira processar

  char temp;
  do {
      scanf("%d%c", &array[i], &temp);
      i++;
      } while(temp != '\n');
      
        
/*distribuicao dos numeros recebidos na entrada para cada vetor de cada processo filho*/        
  int w0=0, w1=0, w2=0, w3=0; //variaveis auxiliadoras na atribuicao  
  for(j=0; j<i; j++) {
 	v = j%4;
 	if(v == 0) {array_f0[w0] = array[j];
 	w0++; }
 	if(v == 1) {array_f1[w1] = array[j];
 	w1++; }
 	if(v == 2) {array_f2[w2] = array[j];
 	w2++; }
 	if(v == 3) {array_f3[w3] = array[j];
 	w3++; }
      }
 
  
  // definindo o número de processos
  if(i <= 4){
   num_processos = (i % 5);
  }
  
  else if(i > 4){
  num_processos = N_PROCESSOS;
  }
  
 
  
  
  unsigned int p; // variavel que indicara se o numero eh primo (p = 1) ou nao (p = 0)

  //definir flags de protecao e visibilidade de memoria 
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  /* Criar area de memoria compartilhada --> variavel que ira contar quantos numeros primos ha na entrada */
  int *count;
  count = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);

  
  
  /*inicializando...*/

  (*count) = 0;
  
  int k0, k1, k2, k3; //variaveis que armazenarao os IDs de cada processo filho
  int g; //variavel auxiliar
  
  for (int k=0; k<num_processos; k++) { //gerando ate quatro processos filhos
    filho[k] = fork();
    
    //obtendo o ID de cada processo filho
    if (filho[k] == 0) {
    	if(k == 0) {k0 = getpid(); //obtem o ID do filho 0
    	}
   
    	else if(k == 1) {k1 = getpid(); //obtem o ID do filho 1
    	}	
    
    	else if(k == 2) {k2 = getpid(); //obtem o ID do filho 2
    	}	
    
    	else if(k == 3) {k3 = getpid(); //obtem o ID do filho 3
    	}	
    	
    	sleep(1); //aguarda o processo pai criar todos os processos filhos e os ID's serem obtidos
    	
    	//cada processo filho ira verificar se o seu conjunto de numeros previamente armazenados em vetores sao primos ou nao
    	// se for primo, incrementa a variavel COUNT compartilhada entre processos por meio de mmap()
    	
    	if(getpid() == k0) { 
    		for(g=0; g< w0; g++){
      			p = primo(array_f0[g]);
      			if (p == 1) {(*count)++ ;}
    			}
    	}
    	
    	else if(getpid() == k1) { 
    		for(g=0; g< w1; g++){
      			p = primo(array_f1[g]);
      			if (p == 1) {(*count)++ ;}
    			}
    	}
    	
    	else if(getpid() == k2) {
    		for(g=0; g< w2; g++){
      			p = primo(array_f2[g]);
      			if (p == 1) {(*count)++ ;}
    			}
    	}
    	
    	else if(getpid() == k3) { 
    		for(g=0; g< w3; g++){
      			p = primo(array_f3[g]);
      			if (p == 1) {(*count)++ ;}
    			}
    	}    	    	
    	exit(0) ;
    	
    }
  }

  //espera cada processo filho terminar de executar
  for (int k=0; k<num_processos; k++) {
    waitpid(filho[k], NULL, 0);
  }

  printf("%d\n", *count); //imprime na tela o resultado final da operacao
  return 0;
}
