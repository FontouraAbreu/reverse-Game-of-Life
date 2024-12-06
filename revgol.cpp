#include <z3++.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;
using namespace z3;

vector<vector<int>> matrix ;
int n, m;

std::unique_ptr<z3::context> createContext() {
    return std::make_unique<z3::context>();
}

std::unique_ptr<z3::optimize> createSolver(z3::context& c) {
    return std::make_unique<z3::optimize>(c);
}

int gol(vector<vector<int>> &mt0, vector<vector<int>> &falses){
    int count = 0;
    for(int i = 0; i<n; i++){
        for(int j = 0; j<m; j++){
            bool reset_neighbors = false ;
            int sum = 0 ;
            for(int di = -1; di <=1; di++){
                for(int dj = -1; dj<=1; dj++){
                    if(di == 0 && dj == 0) continue ;
                    if(i+di>=0 && i+di<n && j+dj>=0 && j+dj<m)
                        sum += mt0[i+di][j+dj] ;
                }
            }
            if(mt0[i][j]){
                if(sum>=2 && sum<=3){
                    if(!matrix[i][j]){
                        reset_neighbors = true ;
                        count ++ ;
                    }
                } else if(matrix[i][j]){
                    reset_neighbors = true ;
                    count++ ;
                }
            } else {
                if(sum == 3){
                    if(!matrix[i][j]){
                        reset_neighbors = true ;
                        count++ ;
                    }
                } else if(matrix[i][j]){
                    reset_neighbors = true ;
                    count++ ;
                }
            }
            if(reset_neighbors){
                for(int di = -1; di<=1; di++) for(int dj=-1; dj<=1; dj++) {
                    if(di == 0 && dj == 0) continue ;
                    int ni = i + di ;
                    int nj = j + dj ;
                    if(ni<0 || ni>= n || nj<0 || nj>=m) continue ;
                    falses[ni][nj] = 0 ;
                }
            }
        }
    }
    return count ;
}

/*
 * Lê a matriz da entrada
 */
vector<vector<int>> readMatrix(int n, int m) {
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
    cout << n << " " << m << "\n" ;
    for (const auto& row : matrix) {
        for (int cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}
//##############################################################################

int find_falses(vector<vector<int>> &mf) {
    int c = 0;
    for(int i = 0; i<n; i++) for(int j = 0; j<m; j++) {
        if(matrix[i][j]) continue ;
        int nb = 0 ;
        for(int di = -1; di<=1; di++) for(int dj=-1; dj<=1; dj++) {
            if(di == 0 && dj == 0) continue ;
            int ni = i + di ;
            int nj = j + dj ;
            if(ni<0 || ni>=n || nj<0 || nj>=m) continue ;

            nb += matrix[ni][nj] ;
        }
        if(nb == 0){
            mf[i][j] = 1 ;
            c++ ;
        }
    }
    return c ;
}

void init_solver(vector<vector<expr>> &cell, vector<vector<int>> &falses,
        unique_ptr<optimize> &s, unique_ptr<context> &c){
    // Cria as variáveis da forma p_i_j para cada célula
    for(int i = 0; i<n; i++) for(int j = 0; j<m; j++)
        cell[i][j] = c->bool_const(("p_" + to_string(i) + "_" + to_string(j)).c_str()) ;

    for(int i = 0; i<n; i++){
        for(int j = 0; j<m; j++){
            expr neighbour_sum = c->int_val(0) ;

            for(int di = -1; di<=1; di++) {
                for(int dj = -1; dj<=1; dj++){
                    // Ignora a célula atual
                    if(di == 0 && dj == 0) continue ;

                    // Ignora os vizinhos fora do tabuleiro
                    int ni = i+di ;
                    int nj = j+dj ;
                    if(ni < 0 || ni >= n || nj < 0 || nj >= m) continue ;
                    // Soma apenas as células vivas
                    neighbour_sum = neighbour_sum + ite(cell[ni][nj], c->int_val(1), c->int_val(0)) ;
                }
            }

            // Aplica as regras do Game Of Life
            if (matrix[i][j] == 1) {
                // se (celula viva e 2 ou 3 vizinhos vivos) OU (celula morta e 3 vizinhos vivos)
                s->add((cell[i][j] && (neighbour_sum == 2 || neighbour_sum == 3)) ||
                    (!cell[i][j] && neighbour_sum == 3)) ;
            } else {
                // se (celula morta e 3 vizinhos vivos) OU (celula viva e menos de 2 ou mais de 3 vizinhos vivos)
                if(falses[i][j]) {
                    s->add(!cell[i][j]) ;
                } else {
                    s->add((!cell[i][j] && neighbour_sum != 3) ||
                        (cell[i][j] && (neighbour_sum < 2 || neighbour_sum > 3))) ;
                }
            }
        }
    }
}

int main() {
    cin >> n >> m;

    matrix = readMatrix(n, m) ; // Matriz que representa t1
    vector<vector<int>> mt0(n, vector<int>(m)) ;
    vector<vector<int>> try_false(n, vector<int>(m, 0)) ;

    find_falses(try_false) ;

    do {
        auto c = createContext() ;
        auto s = createSolver(*c) ;
        vector<vector<expr>> cell(n, vector<expr>(m, c->bool_val(false))); // Resultado

        // Cria as restrições do solver
        init_solver(cell, try_false, s, c) ;


        // Cria a expressão de minimização de células vivas
        expr total_live_cells = c->int_val(0) ;
        for(int i = 0; i<n; i++) {
            for(int j = 0 ; j<m; j++){
                total_live_cells = total_live_cells + ite(cell[i][j], c->int_val(1), c->int_val(0)) ;
            }
        }
        s->minimize(total_live_cells) ; // Minimiza o número de células vivas

        // Resolve o problema
        if(s->check() == sat) {
            model mdl = s->get_model() ;

            for(int i = 0; i<n; i++) {
                for(int j = 0; j<m; j++) {
                    expr e = mdl.eval(cell[i][j], true);
                    mt0[i][j] = e.is_true() ? 1 : 0 ;
                }
            }
        } else cout << "Não SAT\n" ;
    } while (gol(mt0, try_false) != 0) ;

    printMatrix(mt0) ;
    return 0;
}
