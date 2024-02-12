#include "matrices.h"
#include <cmath>
using namespace std;

namespace util {
    vector<vector<double>> transpose_matrix(const vector<vector<double>>& matrix){
        int rows = matrix.size();
        int cols = matrix[0].size();
        
        vector<vector<double>> transposed(cols, vector<double>(rows));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                transposed[j][i] = matrix[i][j];
            }
        }

        return transposed;
    }

    vector<vector<double>> multiply_matrices(const vector<vector<double>>& matrix1, const vector<vector<double>>& matrix2){
        int rows1 = matrix1.size();
        int cols1 = matrix1[0].size();
        int cols2 = matrix2[0].size();
        
        vector<vector<double>> intermediate(rows1, vector<double>(cols2, 0.0));
        double tmp=0;
        for (int i = 0; i < rows1; ++i) {
            for (int j = 0; j < cols2; ++j) {
                for (int k = 0; k < cols1; ++k) {
                    tmp += matrix1[i][k] * matrix2[k][j];
                }
                intermediate[i][j] = tmp;
                tmp = 0;
            }
        }

        return intermediate;
    }

    vector<vector<double>> inverse_matrix(const vector<vector<double>>& matrix){
        int n = matrix.size();
        
        vector<vector<double>> inverse(n, vector<double>(n, 0.0));
        double det = determinant(matrix);
        if (det == 0){
            std::cerr<<("Matrix is not invertible");
            exit(1);
        }
        if (n == 1){
            inverse[0][0] = 1/matrix[0][0];
            return inverse;
        }
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                vector<vector<double>> submatrix(n-1, vector<double>(n-1, 0.0));
                int subi = 0;
                for (int ii = 0; ii < n; ii++){
                    if (ii == i){
                        continue;
                    }
                    int subj = 0;
                    for (int jj = 0; jj < n; jj++){
                        if (jj == j){
                            continue;
                        }
                        submatrix[subi][subj] = matrix[ii][jj];
                        subj++;
                    }
                    subi++;
                }
                inverse[i][j] = pow(-1, i+j) * determinant(submatrix);
            }
        }
        inverse = transpose_matrix(inverse);
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                inverse[i][j] /= det;
            }
        }
        return inverse;
    }

    double determinant(const vector<vector<double>>& matrix){
        int n = matrix.size();
        if (n == 1){
            return matrix[0][0];
        }
        double det = 0;
        for (int i = 0; i < n; i++){
            vector<vector<double>> submatrix(n-1, vector<double>(n-1, 0.0));
            int subi = 0;
            for (int ii = 1; ii < n; ii++){
                int subj = 0;
                for (int jj = 0; jj < n; jj++){
                    if (jj == i){
                        continue;
                    }
                    submatrix[subi][subj] = matrix[ii][jj];
                    subj++;
                }
                subi++;
            }
            det += pow(-1, i) * matrix[0][i] * determinant(submatrix);
        }
        return det;
    }


}