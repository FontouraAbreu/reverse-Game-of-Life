#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv){
    srand(time(0)) ;
    int n, m;
    int density = random()%20  + 20 ;
    if(argc == 1) {
        scanf("%d %d", &n, &m) ;
    }
    if(argc >= 2){
        n = atoi(argv[1]) ;
        m = n ;
    }
    if(argc >= 3) {
        m = atoi(argv[2]) ;
    }
    if(argc == 4){
        density = atoi(argv[3]) ;
    }


    int **matrix = malloc(25 * sizeof(int *)) ;
    for(int k = 0; k<25; k++)
        matrix[k] = calloc(25, sizeof(int)) ;

    for(int i = 1; i<n-1; i++){
        for(int j = 1; j<m-1; j++){
            if(random()%100 < density)
                matrix[i][j] = 1 ;
        }
    }


    for(int k = 1; k+2<m-1; k++){
        if(matrix[1][k] && matrix[1][k+1] && matrix[1][k+2])
            matrix[1][k+1] = 0 ;

        if(matrix[n-2][k] && matrix[n-2][k+1] && matrix[n-2][k+2])
            matrix[n-2][k+1] = 0 ;
    }
    for(int k = 1; k+2<n-1; k++){
        if(matrix[k][1] && matrix[k+1][1] && matrix[k+2][1])
            matrix[k+1][1] = 0 ;
        if(matrix[k][m-2] && matrix[k+1][m-2] && matrix[k+2][m-2])
            matrix[k+1][m-2] = 0 ;
    }

    printf("%d %d\n", n, m) ;
    for(int i = 0; i<n; i++){
        for(int j = 0; j<m; j++){
            printf("%d", matrix[i][j]) ;
            if(j < m-1) printf(" ") ;
        }
        printf("\n") ;
    }

    free(matrix);
    return 0 ;
}
