#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include "Matrix.hpp"

// Matrix class to handle matrix operations

// Constructor: Initialize matrix with given rows and columns, all elements set to 0
Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols), data(rows, std::vector<double>(cols, 0.0)) {}

// Constructor: Initialize matrix with a 2D vector
Matrix::Matrix(const std::vector<std::vector<double>>& values) : data(values), rows(values.size()), cols(values[0].size()) {}

// Access elements
double & Matrix::operator()(int row, int col) {
    assert(row >= 0 && row < rows && col >= 0 && col < cols);
    return data[row][col];
}

double Matrix::operator()(int row, int col) const {
    assert(row >= 0 && row < rows && col >= 0 && col < cols);
    return data[row][col];
}

// Get number of rows
int Matrix::getRows() const { return rows; }

// Get number of columns
int Matrix::getCols() const { return cols; }

// Matrix addition
Matrix Matrix::operator+(const Matrix& other) const {
    assert(rows == other.rows && cols == other.cols);
    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result(i, j) = data[i][j] + other(i, j);
        }
    }
    return result;
}

// Matrix subtraction
Matrix Matrix::operator-(const Matrix& other) const {
    assert(rows == other.rows && cols == other.cols);
    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result(i, j) = data[i][j] - other(i, j);
        }
    }
    return result;
}

// Matrix multiplication
Matrix Matrix::operator*(const Matrix& other) const {
    assert(cols == other.rows);
    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            for (int k = 0; k < cols; ++k) {
                result(i, j) += data[i][k] * other(k, j);
            }
        }
    }
    return result;
}

// Scalar multiplication
Matrix Matrix::operator*(double scalar) const {
    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result(i, j) = data[i][j] * scalar;
        }
    }
    return result;
}

// Matrix-vector multiplication
std::vector<double> Matrix::operator*(const std::vector<double>& vec) const {
    assert(cols == vec.size());
    std::vector<double> result(rows, 0.0);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i] += data[i][j] * vec[j];
        }
    }
    return result;
}

// Transpose of the matrix
Matrix Matrix::transpose() const {
    Matrix result(cols, rows);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result(j, i) = data[i][j];
        }
    }
    return result;
}

// Gaussian elimination to solve linear equations
std::vector<double> Matrix::gaussianElimination(std::vector<double> b) const {
    assert(rows == cols && rows == b.size());
    int n = rows;
    Matrix A = *this;

    // Forward elimination
    for (int i = 0; i < n; ++i) {
        // Pivoting
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (std::abs(A(k, i)) > std::abs(A(maxRow, i))) {
                maxRow = k;
            }
        }
        for (int k = i; k < n; ++k) {
            std::swap(A(i, k), A(maxRow, k));
        }
        std::swap(b[i], b[maxRow]);

        // Make all rows below this one 0 in current column
        for (int k = i + 1; k < n; ++k) {
            double factor = A(k, i) / A(i, i);
            for (int j = i; j < n; ++j) {
                A(k, j) -= factor * A(i, j);
            }
            b[k] -= factor * b[i];
        }
    }

    // Back substitution
    std::vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; --i) {
        x[i] = b[i] / A(i, i);
        for (int k = i - 1; k >= 0; --k) {
            b[k] -= A(k, i) * x[i];
        }
    }

    return x;
}

// QR decomposition using Gram-Schmidt process
std::pair<Matrix, Matrix> Matrix::qrDecomposition() const {
    assert(rows >= cols && "Matrix must have more rows than columns for QR decomposition.");

    Matrix Q(rows, cols); // Orthogonal matrix
    Matrix R(cols, cols); // Upper triangular matrix

    for (int k = 0; k < cols; ++k) {
        // Compute the k-th column of Q
        for (int i = 0; i < rows; ++i) {
            Q(i, k) = data[i][k];
        }

        for (int j = 0; j < k; ++j) {
            double dotProduct = 0.0;
            for (int i = 0; i < rows; ++i) {
                dotProduct += Q(i, j) * data[i][k];
            }
            R(j, k) = dotProduct;

            for (int i = 0; i < rows; ++i) {
                Q(i, k) -= R(j, k) * Q(i, j);
            }
        }

        double norm = 0.0;
        for (int i = 0; i < rows; ++i) {
            norm += Q(i, k) * Q(i, k);
        }
        norm = std::sqrt(norm);

        R(k, k) = norm;
        for (int i = 0; i < rows; ++i) {
            Q(i, k) /= norm;
        }
    }

    return { Q, R };
}

// Solve a system of linear equations using QR decomposition
std::vector<double> Matrix::solveWithQR(const std::vector<double>& b) const {
    assert(rows == b.size() && "Matrix row size must match the size of the vector b.");

    // Perform QR decomposition
    std::pair<Matrix, Matrix> pair = qrDecomposition();
    Matrix Q = pair.first;
    Matrix R = pair.second;

    // Compute Q^T * b
    std::vector<double> Qtb(cols, 0.0);
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            Qtb[i] += Q(j, i) * b[j];
        }
    }

    // Solve R * x = Q^T * b using back substitution
    std::vector<double> x(cols, 0.0);
    for (int i = cols - 1; i >= 0; --i) {
        x[i] = Qtb[i];
        for (int j = i + 1; j < cols; ++j) {
            x[i] -= R(i, j) * x[j];
        }
        x[i] /= R(i, i);
    }

    return x;
}

// Print matrix
void Matrix::print() const {
    for (const auto& row : data) {
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}


