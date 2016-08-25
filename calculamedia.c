#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1
#define N 20
#define NP 10

typedef struct {
             int indice;
	     float media;
         } elem_pipe;


typedef struct {
            elem_pipe vet_elem[N];
	    int pid;
         } processo;


int main( void ){
	float nn;
	float n1[N], n2[N], n3[N];
  	FILE *fd;
	int n, np, fdp[NP][2], id, l;
	processo proc[NP];
	elem_pipe buffer;

	for( np = 0 ; np < NP ; np++ ){

     		pipe( fdp[ np ] );

		switch( proc[np].pid = fork() ) {

      			case -1 : printf( "Erro na criacao do processo filho\n" );
                		  exit( -1 );
                		  break;

			case 0  : fd = fopen( "notas.lst", "r" );
				  if( fd == NULL ) {
                 		  	printf( "Erro na abertura do arquivo\n" );
                  		  	return -1;
				  }
				  
					  for( l=0 ; l<np*20 ; l++){ //loop de leitura de posicionamento
					  	fscanf( fd, "%d %f %f %f\n", &id, &nn, &nn, &nn );
					  }
				  
				  for( n=0 ; n<N ; n++){//loop de leitura das notas
				  	fscanf( fd, "%d %f %f %f\n", &proc[np].vet_elem[n].indice, &n1[n], &n2[n], &n3[n] );
				  }
				  fclose( fd );
				  for( n=0 ; n<N ; n++){//loop de gravação no pipe
				  	if( n1[n] + n2[n] <= 14.0 )
                  				proc[np].vet_elem[n].media = ((n1[n]+n2[n]+n3[n])/3.0);
					else
	           				proc[np].vet_elem[n].media =  ((n1[n]+n2[n])/2.0);
					write( fdp[ np ][ WRITE ], &proc[np].vet_elem[n], sizeof(elem_pipe) );

				  }
				  break;	  
		}
	}
	for( np = 0 ; np < NP ; np++ ){
		for( n = 0 ; n < N ; n++){
			read( fdp[ np ][ READ ], &buffer, sizeof(elem_pipe) );
			printf("\n%d : Media %4.1f", buffer.indice, buffer.media);
		}
		
	}
}
