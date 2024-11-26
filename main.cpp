#include <z3++.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;
using namespace z3;

// using int& to avoid copying the values into memory
vector<vector<int>> readMatrix(int& n, int& m) {
    cin >> n >> m;
    vector<vector<int>> matrix(n, vector<int>(m));

    // for each row
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> matrix[i][j];
        }
    }

    return matrix;
}

// using const int& to avoid copying the values into memory
void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}

int main() {
    int n, m;
    vector<vector<int>> matrix = readMatrix(n, m);

    cout << "Dimensões: " << n << " x " << m << endl;
    cout << "Matriz:" << endl;
    printMatrix(matrix);

    context c ;
    solver s(c) ;

    vector<vector<expr>> cell(n, vector<expr>(m, c.bool_val(false)));
    for(int i = 0; i<n; i++)
        for(int j = 0; j<m; j++)
            cell[i][j] = c.bool_const(("p_" + to_string(i) + "_" + to_string(j)).c_str()) ;

    for(int i = 0; i<n; i++){
        for(int j = 0; j<m; j++){
            expr neighbour_sum = c.int_val(0) ;

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

            if (matrix[i][j] == 1) {
                s.add((cell[i][j] && (neighbour_sum == 2 || neighbour_sum == 3)) ||
                        (!cell[i][j] && neighbour_sum == 3)) ;
            } else {
                s.add((!cell[i][j] && neighbour_sum != 3) ||
                        (cell[i][j] && (neighbour_sum < 2 || neighbour_sum > 3))) ;
            }
        }
    }

    optimize opt(c) ;

    expr total_live_cells = c.int_val(0) ;
    for(int i = 0; i<n; i++) {
        for(int j = 0 ; j<m; j++){
            total_live_cells = total_live_cells + ite(cell[i][j], c.int_val(1), c.int_val(0)) ;
        }
    }

    opt.minimize(total_live_cells) ;

    if(opt.check() == sat) {
        model mdl = opt.get_model() ;

        for(int i = 0; i<n; i++) {
            for(int j = 0; j<m; j++) {
                cout << (mdl.eval(cell[i][j]).is_true() ? "1 " : "0 ") ;
            }
            cout << "\n" ;
        }
    }

    return 0;
}
