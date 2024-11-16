#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

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

    return 0;
}