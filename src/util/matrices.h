#include <iostream>
#include <vector>

namespace util{
    using namespace std;
    vector<vector<double>> transpose_matrix(const vector<vector<double>>& matrix);
    vector<vector<double>> multiply_matrices(const vector<vector<double>>& matrix1, const vector<vector<double>>& matrix2);
    vector<vector<double>> inverse_matrix(const vector<vector<double>>& matrix);
    double determinant(const vector<vector<double>>& matrix);

}