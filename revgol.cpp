#include <z3++.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iostream>
#include <thread>
using namespace std;
using namespace z3;

/*
 * Lê a matriz da entrada
 */
vector<vector<int>> readMatrix(int& n, int& m) {
    cin >> n >> m;
    vector<vector<int>> matrix(n, vector<int>(m));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> matrix[i][j];
        }
    }

    return matrix;
}
//##############################################################################



/*
 * Printa matriz para debug
 */
void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
    cout << "\n\n" ;
}
//##############################################################################



int main() {
    int n, m;
    vector<vector<int>> matrix = readMatrix(n, m) ;

    config cfg ;        // Configurações do solver
    context c(cfg) ;    // Contexto do solver
    optimize s(c) ;     // Solver
    vector<vector<expr>> cell(n, vector<expr>(m, c.bool_val(false))); // Resultado

    //Cria uma variável SAT para cada célula
    for(int i = 0; i<n; i++) for(int j = 0; j<m; j++)
        cell[i][j] = c.bool_const(("p_" + to_string(i) + "_" + to_string(j)).c_str()) ;

    for(int i = 0; i<n; i++){
        for(int j = 0; j<m; j++){
            expr neighbour_sum = c.int_val(0) ;

            // Faz a soma dos vizinhos
            for(int di = -1; di<=1; di++) {
                for(int dj = -1; dj<=1; dj++){
                    if(di == 0 && dj == 0) continue ;

                    int ni = i+di ;
                    int nj = j+dj ;

                    if(ni >= 0 && ni < n && nj >= 0 && nj < m) {
                        neighbour_sum = neighbour_sum + ite(cell[ni][nj], c.int_val(1), c.int_val(0)) ;
                    }
                }
            }

            int nbsum_t1 = 0 ;
            for(int di = -2; di<=2; di++) {
                for(int dj = -2; dj<=2; dj++){
                    if(di == 0 && dj == 0) continue ;

                    int ni = i+di ;
                    int nj = j+dj ;

                    if(ni >= 0 && ni < n && nj >= 0 && nj < m)
                        nbsum_t1 += matrix[ni][nj] ;
                }
            }

            // Aplica as regras do Game Of Life
            if (matrix[i][j] == 1) {
                s.add((cell[i][j] && (neighbour_sum == 2 || neighbour_sum == 3)) ||
                    (!cell[i][j] && neighbour_sum == 3)) ;
            } else {
                if(nbsum_t1 == 0) {
                    s.add(!cell[i][j]) ;
                } else {
                    s.add((!cell[i][j] && neighbour_sum != 3) ||
                        (cell[i][j] && (neighbour_sum < 2 || neighbour_sum > 3))) ;
                }
            }
        }
    }

    // Cria a expressão de minimização de células vivas
    expr total_live_cells = c.int_val(0) ;
    for(int i = 0; i<n; i++) {
        for(int j = 0 ; j<m; j++){
            total_live_cells = total_live_cells + ite(cell[i][j], c.int_val(1), c.int_val(0)) ;
        }
    }
    s.minimize(total_live_cells) ;

    if(s.check() == sat) {
        model mdl = s.get_model() ;
#ifdef DIMS
        cout << n << " " << m << "\n" ;
#endif
        for(int i = 0; i<n; i++) {
            for(int j = 0; j<m; j++) {
                expr e = mdl.eval(cell[i][j], true);
                cout << (e.is_true() ? "1" : "0");
                if(j<m-1) cout << " " ;
            }
            cout << "\n" ;
        }
    } else cout << "Não SAT\n" ;

    return 0;
}
