#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <cassert>
#include <iostream>
#include <cmath>

// Matrix class to handle basic matrix operations
class Matrix {
private:
    std::vector<std::vector<double>> data; // Stores the matrix elements
    int rows, cols;                        // Number of rows and columns in the matrix

public:
    // Constructor: Initialize matrix with given rows and columns, all elements set to 0
    Matrix(int rows, int cols);

    // Constructor: Initialize matrix with a 2D vector
    Matrix(const std::vector<std::vector<double>>& values);

    // Access and modify matrix elements
    double& operator()(int row, int col);       // Access a modifiable element
    double operator()(int row, int col) const; // Access a read-only element

    // Get the number of rows and columns
    int getRows() const;
    int getCols() const;

    // Matrix addition
    Matrix operator+(const Matrix& other) const;

    // Matrix subtraction
    Matrix operator-(const Matrix& other) const;

    // Matrix multiplication
    Matrix operator*(const Matrix& other) const;

    // Scalar multiplication
    Matrix operator*(double scalar) const;

    // Vector multiplication
    std::vector<double> operator*(const std::vector<double>& vec) const;

    // Transpose the matrix
    Matrix transpose() const;

    // Solve a system of linear equations using Gaussian elimination
    // Takes a vector 'b' as the right-hand side of the equations
    std::vector<double> gaussianElimination(std::vector<double> b) const;

    std::pair<Matrix, Matrix> qrDecomposition() const;

    std::vector<double> solveWithQR(const std::vector<double>& b) const;

    // Print the matrix to the console
    void print() const;
};

#endif // MATRIX_HPP
