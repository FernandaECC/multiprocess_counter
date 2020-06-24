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
  int i=0, j=0;
  int array[100];
  char temp;
  do {
      scanf("%d%c", &array[i], &temp);
      i++;
      } while(temp != '\n');
  printf("numero de elementos: %d\n", i);    
  for(j=0; j<i; j++) {
  //printf("%d\n", array[j]);
      }
  
  // definindo o número de processos
  if(i <= 4){
   num_processos = (i % 5);
  }
  
  else if(i > 4){
  num_processos = N_PROCESSOS;
  }
  
  printf("numero de processos = %d", num_processos);
  
  //###########
  unsigned int p;
  unsigned int aux2;

  /* Definir flags de protecao e visibilidade de memoria */
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  /* Criar area de memoria compartilhada */
  int *count;
  count = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
  int *pointer;
  pointer = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
  
  
  /*inicializando...*/
  (*pointer) = 0;
  (*count) = 0;

  for (int k=0; k<num_processos; k++) {
    filho[k] = fork();
    
    if (filho[k] == 0) {
    
      /* Esta parte do codigo executa no processo filho */
      aux2 = (*pointer);
      p = primo(array[aux2]);
      
      
      if (p == 1) {
      (*count)++ ;
      (*pointer)++;
      }
      
      else {
      (*pointer)++;
      }
      
      printf("pointer = %d\n", (*pointer));
      printf("Filho %d achou primo(%d)=%d\n", k, array[aux2] , p);
      exit(0);
    }
  }

  printf("Todos os filhos foram gerados. Esperando...\n");
  for (int k=0; k<N_PROCESSOS; k++) {
    waitpid(filho[k], NULL, 0);
  }

  printf("Todos os filhos terminaram! Final: *count=%d\n", *count);
  return 0;

}
