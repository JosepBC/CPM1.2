#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <omp.h>

#define CERT 1
#define FALS 0
#define MAX_THREADS 128

int taules[MAX_THREADS][9][9];
int max_num_treads;

// Ha de ser inicialment correcta !!
int taula[9][9] = \
        {1,2,3, 4,5,6,  7,8,9,  \
         9,8,7, 3,2,1,  6,5,4,  \
         6,5,4, 7,8,9,  1,2,3,  \
\
         7,9,8, 0,0,0,  0,0,0,  \
         0,0,0, 0,0,0,  0,0,0,  \
         0,0,0, 0,0,0,  0,0,0,  \
\
         0,0,0, 0,0,0,  0,0,0,  \
         0,0,0, 0,0,0,  0,0,0,  \
         0,0,0, 0,0,0,  0,0,0};

void init_taules() {
    #pragma omp parallel for
    for (int table = 0; table < MAX_THREADS; table++) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                taules[table][i][j] = taula[i][j];
            }
        }
    }

}

void print_table (int table[9][9]) {
    for( int i = 0; i < 9; i++){
        for( int j = 0; j < 9; j++){
            printf( "%i ", table[i][j]);
        }
        printf("\n");
    }
}

/*Comprova que en la posició (x,y) del sudoku es pugi ficar l'element z*/
int puc_posar(int x, int y, int z, int thread) {
    int i, j, pi, pj;

    for (i=0; i<9; i++ ){
        if (taules[thread][x][i] == z) return FALS;
        if (taules[thread][i][y] == z) return FALS;
    }

    // Quadrat
    pi = (x / 3) * 3; //truncament
    pj = y - y % 3; //truncament

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if ( taules[thread][ pi+i ][ pj+j ] == z ) return FALS;
        }
    }
    
    return CERT;
}

////////////////////////////////////////////////////////////////////
int recorrer(int i, int j, int thread) {
    int k;
    int s = 0;
    //int thread = omp_get_thread_num();

    if ( taules[thread][i][j] ) {//Valor fixe no s'ha d'iterar
        if (j < 8) return(recorrer(i, j + 1, thread));
        else if (i < 8) return(recorrer(i + 1, 0, thread));
        else return 1; // Final de la taula
    } else {// hi ha un 0 hem de provar
        for (k = 1; k < 10; k++) {
            if (puc_posar(i, j, k, thread)) {
                taules[thread][i][j] = k; 
                if (j < 8) s += recorrer(i, j + 1, thread);
                else if (i < 8) s += recorrer(i + 1, 0, thread);
                else s++;
                taules[thread][i][j] = 0;
            }
        }
    }
    return s;
}

////////////////////////////////////////////////////////////////////
int firstRecorrer(int i, int j) {
    int k;
    int s = 0;
    int thread = 0;

    if (taules[thread][i][j]) {//Valor fixe no s'ha d'iterar
        if (j < 8) return(firstRecorrer(i, j + 1));
        else if (i < 8) return(firstRecorrer(i + 1, 0));
        else return 1; // Final de la taula
    } else {// hi ha un 0 hem de provar
        #pragma omp parallel for private( thread ) reduction( +:s )
        for (k = 1; k < 10; k++) {
            thread = omp_get_thread_num();
            if (puc_posar(i, j, k, thread)) {
                taules[thread][i][j] = k; 
                if (j<8) s += recorrer( i, j+1, thread );
                else if (i<8) s += recorrer( i+1, 0, thread );
                else s++;
                taules[thread][i][j] = 0;
                //printf("%d Results using thread %d: %li\n", k, thread, s);
            }
        }
    }

    return s;
}

////////////////////////////////////////////////////////////////////
int main() {
    int nsol = 0;

    init_taules();
    nsol = firstRecorrer(0,0);
    if (nsol == 54474240) {
        printf("Nsol ok\n");
        exit(0);
    } else {
        printf("Bad Nsol: %i\n", nsol);
        exit(1);
    }
}