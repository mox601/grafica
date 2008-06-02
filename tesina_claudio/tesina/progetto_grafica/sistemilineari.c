#include   <math.h>
#include   <stdio.h>
#include   <stdlib.h>

#define MAX_DIM 3

static int pivot(int j, float matr[MAX_DIM][MAX_DIM], float tn[MAX_DIM]);
void stampa(float matr[MAX_DIM][MAX_DIM], float term_noti[MAX_DIM]);

static int dbg = 0;

int gauss( float coef_mat[MAX_DIM][MAX_DIM], float term_noti[MAX_DIM], float sol[MAX_DIM]) {
   
	int i, j, k, // indici di scansione della matrice
	check = 0;
   	float piv = 0.0, alfa = 0.0, temp = 0.0;


   	for( j=0; j<MAX_DIM-1; j++) {
      		// scegli pivot;
      		check = pivot(j, coef_mat, term_noti);
      		if( check == 0 ) {
            		piv=coef_mat[j][j];
			if(dbg) printf("piv=%f\n",piv);
		}
      		else
            		return(-1);
      		// combinazione lineare
      		for( i=j+1;i<MAX_DIM; i++) {
         		// calcolo del coefficente della combinazione lineare
         		alfa = coef_mat[i][j]/piv;
         		// sottrazione
         		for( k=j; k<MAX_DIM; k++)
         		   	coef_mat[i][k]=coef_mat[i][k]-alfa*coef_mat[j][k];
         			// termini noti
         			term_noti[i]=term_noti[i]-alfa*term_noti[j];
       		} // for su righe i da j+1 a n
    	} //for sui primi n-1 elemnti della diagonale a[j][j]
    	// Check su determinante
    	if( coef_mat[MAX_DIM-1][MAX_DIM-1] == 0) {
       		if(dbg) printf("Righe linearmente dipendenti. Soluzioni infinite\n\n");
       		return(1);
    	}
    	//Trovo ora la soluzione
    	sol[MAX_DIM-1]=term_noti[MAX_DIM-1]/coef_mat[MAX_DIM-1][MAX_DIM-1];
	if (dbg) 
		printf("sol[%d] = %f/%f = %f\n",
			MAX_DIM-1,term_noti[MAX_DIM-1],coef_mat[MAX_DIM-1][MAX_DIM-1],term_noti[MAX_DIM-1]/coef_mat[MAX_DIM-1][MAX_DIM-1]);
    	for(i=MAX_DIM-2; i>=0; i--) {
      		temp = term_noti[i];
      		for( k=i+1; k<MAX_DIM; k++)
         		temp=temp-coef_mat[i][k]*sol[k];
      		sol[i] = temp/coef_mat[i][i];
    	} // for sulle righe n-1,n-2, ..., 1

    
	return(0);

}// gauss()



// Funzione di pivoting parziale;
// cerca il pivot per la colonna j-esima
// e se diverso da a[j][j] esegue lo scambio
static int pivot(int j, float matr[MAX_DIM][MAX_DIM], float tn[MAX_DIM]) {
   
	int k,r;
   	float temp = 0.0;
	int tempOrd = 0;
   	//for( k=j; matr[k,j]==0 & k<=dim; k++);
   	//system("cls");
   	if (dbg) printf(" Pivoting: passo (%d)\n",j);
   	//stampa(matr,tn);
   	//system("PAUSE");
   	// cerco il primo elemento non nullo
   	k=j;
    	while( k<MAX_DIM && matr[k][j]==0 ) ++k;
    	if( k<MAX_DIM ) {// esiste k tc matr[k,j]==0
        	if (dbg) printf("matr[j][j] non nullo per j=%d\n",k+1);
	}
   	else {
        	if (dbg) printf("Pivot NULLO !!\n\n");
        	return(-1);
   	}
   	if( k!=j ) {
       		if (dbg) printf("Scambio Riga %d con %d\n",j,k);
       		// scambio righe
      		for( r=j; r<MAX_DIM; r++ ) {
        		temp = matr[j][r];
        		matr[j][r] = matr[k][r];
        		matr[k][r] = temp;
      		}

      		temp = tn[j];
      		tn[j] = tn[k];
      		tn[k] = temp;
		
		
    	}
    
	return(0);

} // pivot()


void stampa(float matr[MAX_DIM][MAX_DIM], float term_noti[MAX_DIM]) {
  
	int i,j;
  	printf("Matrice\n");
  	for( i=0; i<MAX_DIM; i++) {
    		for( j=0; j<MAX_DIM; j++)
			printf("\ta(%d,%d)=%f",i+1,j+1,matr[i][j]);
    		printf("\ttn=%f\n",term_noti[i]);
  	}

}


void stampa_sol(float sol[MAX_DIM]) {
 
 	int i;
   	printf("Vettore soluzioni\n");
  	for( i=0; i<MAX_DIM; i++) {
    		//cout << "\tx(" << i+1 << ")=" << setprecision(3) << sol[i];
		printf("\tx(%d)=%f",i+1,sol[i]);
    		printf("\n");
  	}
} // stampa_sol()

/*
main() {
   
	float A[MAX_DIM][MAX_DIM] = {{1.0,1.0,2.0},{3.0,-2.0,1.0},{1.0,-1.0,4.0}}; // matrice coefficenti
 	float B[MAX_DIM] = {0.0,3.0,2.0};          // matrice termini noti
       	float X[MAX_DIM];          // vettore soluzioni
   	int dim = 3; // dimensione richiesta da sistema corrente
   	int success; // variabile di ritorno dala procedura Gauss()
                // success==0 => trovate soluzioni
                // success==1 => non trovate soluzioni

	

      	//system("cls"); 
	printf("\n\n\n");
   	// leggi matrice ingresso
      	//leggi(A, dim);
   	// Leggi vettore termini noti
      	//leggi_tn( dim, B);
   	//stampa il sistema
      	stampa(A,B);
   	// Gauss() per la soluzione
      	success=gauss(A,B,X);
   	// stampa()
      	if ( success==0 ) {
         	printf("\n\nLa Matrice Triangolare superiore e:\n");
         	stampa(A,B);
         	printf("\n\nIl vettore delle soluzioni e:\n");
         	stampa_sol(X);
      	} 
	else 
		printf("Il sistema e indeterminato.\n\n");

}  // main()
*/
