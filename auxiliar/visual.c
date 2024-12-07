#include <stdio.h>
int main(){
    int n, m, k ;
    scanf("%d %d", &n, &m) ;
    for(int i = 0; i<n; i++){
        for(int j = 0; j<m; j++){
            scanf("%d", &k) ;
            if(k){
                printf("\e[1;32m██\e[0m") ;
            } else printf("\e[1;31m██\e[0m") ;
        }
        printf("\n") ;
    }
}
