#include <iostream>
#include "MyMatrix.hpp"
#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm> 
#include <numeric>


MyMatrix::MyMatrix(int m, int n)
    : nRows(m), nCols(n)  // 使用初始化列表
{
    data = new double* [nRows];

    for (int i = 0; i < nRows; i++) {
        data[i] = new double[nCols];
        for (int j = 0; j < nCols; j++) {
            data[i][j] = 0;
        }
    }
}


MyMatrix::MyMatrix(const std::vector<std::vector<double>>& matrixData)
    : nRows(static_cast<int>(matrixData.size())), 
    nCols(static_cast<int>(matrixData[0].size()))
{
    data = new double* [nRows];
    for (size_t i = 0; i < nRows; ++i) {
        data[i] = new double[nCols];
        for (size_t j = 0; j < nCols; ++j) {
            data[i][j] = matrixData[i][j];
        }
    }
}


MyMatrix::MyMatrix(const std::vector<double>& vectorData)
    : nRows(static_cast<int>(vectorData.size())),
    nCols(1)
{
    data = new double* [nRows];
    for (size_t i = 0; i < nRows; ++i) {
        data[i] = new double[1];
        data[i][0] = vectorData[i];
        
    }
}


void MyMatrix::print() const {
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            std::cout << data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

MyMatrix MyMatrix::transpose() {
    MyMatrix newmatrix(nCols, nRows); 

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            newmatrix.data[j][i] = data[i][j];
        }
    }
    return newmatrix;

}

int MyMatrix::getCols() const {
    return nCols;
}

int MyMatrix::getRows() const {
    return nRows;
}

void MyMatrix::setvalue(int i, int j, double value) {
    if (i >= 0 && i < nRows && j >= 0 && j < nCols) {
        data[i][j] = value;  
    }
}


double MyMatrix::getvalue(int i, int j) const {
    if (i >= 0 && i < nRows && j >= 0 && j < nCols) {
        return data[i][j];
    }
    return 0.0;
}


std::vector<std::vector<double>> MyMatrix::getData() const {
    std::vector<std::vector<double>> matrixData(nRows, std::vector<double>(nCols));

    // Depulicate into MatrixData
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            matrixData[i][j] = data[i][j];
        }
    }

    return matrixData;
}


MyMatrix MyMatrix::transpose_external() {
    MyMatrix newmatrix(nCols, nRows);

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            newmatrix.data[j][i] = data[i][j];
        }
    }

    return newmatrix;
}

MyMatrix::~MyMatrix() {

    for (int i = 0; i < nRows; i++)
        delete[] data[i];

    delete[] data;

    nRows = 0;
    nCols = 0;
}

MyMatrix MyMatrix::operator=(MyMatrix &m2) {
    nRows = m2.nRows;
    nCols = m2.nCols;
    MyMatrix newmatrix(nRows, nCols);
    for (int i = 0; i < nRows; i++)
        for (int j = 0; j < nCols; j++)
            data[i][j] = m2.data[i][j];

    return newmatrix;
}

MyMatrix MyMatrix::operator+(MyMatrix &m2) {
    MyMatrix newmatrix(nRows, nCols);
    for (int i = 0; i < nRows; i++)
        for (int j = 0; j < nCols; j++)
            newmatrix.data[i][j] = data[i][j] + m2.data[i][j];
    return newmatrix;
}

MyMatrix MyMatrix::operator*(MyMatrix& m2) {
    MyMatrix newmatrix(nRows, m2.nCols);
    for (size_t k = 0; k < nRows; k++)
        for (size_t l = 0; l < m2.nCols; l++) {
            double sumproduct = 0;
            for (size_t j = 0; j < nCols; j++)
                sumproduct += data[k][j] * m2.data[j][l];
            newmatrix.data[k][l] = sumproduct;
        }
    return newmatrix;
}


std::vector<double> MyMatrix::operator*(const std::vector<double>& v1) {
    // check
    if (nCols != v1.size()) {
        throw std::invalid_argument("Vector size must match the number of columns in the matrix.");
    }

    std::vector<double> result(nRows, 0.0);

    // 进行矩阵乘法
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            result[i] += data[i][j] * v1[j];
        }
    }

    return result;
}



MyMatrix MyMatrix::operator*(double scalar) {
    MyMatrix newmatrix(nRows, nCols);
    for (size_t i = 0; i < nRows; i++) {
        for (size_t j = 0; j < nCols; j++) {
            newmatrix.data[i][j] = data[i][j] * scalar;
        }
    }
    return newmatrix;
}


double MyMatrix::determinant() {

    if (nRows == 1)
        return data[0][0];
    else if (nRows == 2)
        return data[0][0] * data[1][1] - data[0][1] * data[1][0];
    else {
        double det = 0;
        // first row -- row=0
        for (int j = 0; j < nCols; j++) {
            det += cofactor(0, j) * data[0][j] * minor(0, j); // auxiliary functionality
        }

        return det;
    }
}

int MyMatrix::cofactor(int i, int j) {
    return (i + j) % 2 == 0 ? 1 : -1;
}


double MyMatrix::minor(int i, int j) {
    // first produce the new square submatrix
    // remove the i-th row, remove the j-th column
    MyMatrix m(nRows - 1, nCols - 1);
    for (int i2 = 0; i2 < i; i2++) {
        // copy all rows up to i-th row
        for (int j2 = 0; j2 < j; j2++) {
            // copy all columns up to the j-th column
            m.data[i2][j2] = data[i2][j2];
        }
        for (int j2 = j + 1; j2 < nCols; j2++) {
            // copy all columns from the j+1-th column onwards
            m.data[i2][j2 - 1] = data[i2][j2];
        }
    }

    for (int i2 = i + 1; i2 < nRows; i2++) {
        // copy all rows from i+1-st row and onwards
        for (int j2 = 0; j2 < j; j2++) {
            // copy all columns up to the j-th column
            m.data[i2 - 1][j2] = data[i2][j2];
        }
        for (int j2 = j + 1; j2 < nCols; j2++) {
            // copy all columns from the j+1-th column onwards
            m.data[i2 - 1][j2 - 1] = data[i2][j2];
        }
    }
    return m.determinant();
}

void MyMatrix::read_data(std::string s) {
    std::ifstream f(s);
    if (!f.is_open()) {
        std::cerr << "Error: Could not open file " << s << std::endl;
        return;
    }
    f >> nRows >> nCols;
    for (int i = 0; i < nRows; i++)
        for (int j = 0; j < nCols; j++)
            f >> data[i][j];
}


int MyMatrix::findRank() const {
    int m = getRows();
    int n = getCols();

    // copy
    std::vector<std::vector<double>> tempMatrix(m, std::vector<double>(n));

    // repulicate
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            tempMatrix[i][j] = data[i][j];
        }
    }

    gaussian_elimination(tempMatrix, true);

    // rank
    int rank = 0;
    for (int i = 0; i < m; ++i) {
        bool all_zero = true;
        for (int j = 0; j < n; ++j) {
            if (tempMatrix[i][j] != 0) {
                all_zero = false;
                break;
            }
        }
        if (!all_zero) {
            rank++;
        }
    }

    return rank;
}


std::vector<std::vector<int>> combos(int n, int r) {
    std::vector<std::vector<int>> result;
    std::vector<bool> v(n);
    std::fill(v.end() - r, v.end(), true);
    do {
        std::vector<int> current_result;
        for (int i = 0; i < n; i++)
            if (v[i])
                current_result.push_back(i);
        result.push_back(current_result);
    } while (std::next_permutation(v.begin(), v.end()));

    return result;
}


void gaussian_elimination(std::vector<std::vector<double>>& matrix, bool for_inverse) {
    int m = static_cast<int>(matrix.size());
    int n = static_cast<int>(matrix[0].size());
    const double epsilon = 1e-9; // 用于浮点误差的容差

    for (int i = 0; i < m; ++i) {
        // 检查主元是否为零（考虑浮点误差）
        if (std::abs(matrix[i][i]) < epsilon) {
            bool found = false;
            for (int j = i + 1; j < m; ++j) {
                if (std::abs(matrix[j][i]) > epsilon) { // 检查非零行
                    std::swap(matrix[i], matrix[j]);
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (for_inverse) {
                    //std::cout << "can't reverse" << std::endl;
                    break;// 无法求逆
                }
                else {
                    continue; // 非求逆情况，继续进行
                }
            }
        }

        // 归一化主元，仅在求逆时执行
        if (for_inverse) {
            double pivot = matrix[i][i];
            for (int j = 0; j < n; ++j) {
                matrix[i][j] /= pivot;
            }
        }

        // 消去其他行中该列的元素
        for (int j = 0; j < m; ++j) {
            if (i != j) {
                double factor = matrix[j][i];
                for (int k = 0; k < n; ++k) {
                    matrix[j][k] -= factor * matrix[i][k];
                }
            }
        }
    }
}


MyMatrix MyMatrix::reverse_matrix() const {
    int n = nRows;
    std::vector<std::vector<double>> augmented(n, std::vector<double>(2 * n));

    // Copy matrix A to the left side of the augmented matrix, and fill the right side with the identity matrix.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmented[i][j] = data[i][j];   // A 
            augmented[i][j + n] = (i == j) ? 1.0 : 0.0;  // I 
        }
    }

    gaussian_elimination(augmented, true);

    // Right Reverse Matrix
    MyMatrix inverse(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {

            inverse.data[i][j] = augmented[i][j + n];
        }
    }

    return inverse;
}

MyMatrix solve(MyMatrix& A, MyMatrix& b){
    MyMatrix A_inv = A.reverse_matrix();
    //std::cout << "The Reverse Matrix of A:" << std::endl;
    MyMatrix x = A_inv * b;
    
    return x;
}


MyMatrix MyMatrix::getSubmatrix(const std::vector<int>& rows, const std::vector<int>& columns) const {
    MyMatrix submatrix(static_cast<int>(rows.size()), static_cast<int>(columns.size()));
    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < columns.size(); ++j) {
            submatrix.data[i][j] = data[rows[i]][columns[j]];
        }
    }
    return submatrix;
}


void MyMatrix::addRow(const MyMatrix& row) {
    if (row.nCols != nCols || row.nRows != 1) {
        throw std::invalid_argument("Row must equal col, and only add by one");
    }

    // add new rows
    double** newData = new double* [nRows + 1];
    for (int i = 0; i < nRows; ++i) {
        newData[i] = data[i];
    }

    // allocate new rows data
    newData[nRows] = new double[nCols];
    for (int j = 0; j < nCols; ++j) {
        newData[nRows][j] = row.data[0][j];
    }

    // renew pointers
    delete[] data;
    data = newData;
    nRows++;
}


void MyMatrix::addColumn(const std::vector<double>& col) {
    if (col.size() != nRows) {
        throw std::invalid_argument("Column size must match the number of rows in the matrix.");
    }

    // 创建新的数据结构，增加一列
    double** newData = new double* [nRows];
    for (int i = 0; i < nRows; ++i) {
        newData[i] = new double[nCols + 1]; // 增加一列
        // 将旧数据复制到新行
        for (int j = 0; j < nCols; ++j) {
            newData[i][j] = data[i][j];
        }
        // 将新列数据添加到末尾
        newData[i][nCols] = col[i];
    }

    // 删除旧数据
    for (int i = 0; i < nRows; ++i) {
        delete[] data[i];
    }
    delete[] data;

    // 更新指针和列数
    data = newData;
    nCols++;
}


MyMatrix MyMatrix::enumerateBasicSolutions(const MyMatrix& b) const {
    // make sure the rows of A and b are equal
    if (nRows != b.getRows()) {
        throw std::invalid_argument("Matrix A and vector b must have the same number of rows.");
    }

    int rank = findRank();

    // create a set for the solution
    MyMatrix feasibleSolutions(0, nCols);

    std::vector<std::vector<int>> column_combinations_A = combos(nCols, rank);

    // Generate a submatrix
    for (const std::vector<int>& columns : column_combinations_A) {

        std::vector<int> row_indices(nRows);
        std::iota(row_indices.begin(), row_indices.end(), 0);

        MyMatrix submatrix = getSubmatrix(row_indices, columns);  
        // Extract the submatrix corresponding to the column combination of A
        // submatrix.print();

        // Check determinant is not zero
        if (submatrix.determinant() != 0) {
            MyMatrix basicSolution(1, nCols);

            // Extract the subvector of b corresponding to the submatrix
            std::vector<int> row_indices(nRows);
            std::iota(row_indices.begin(), row_indices.end(), 0);  // Generate a sequence 0, 1, 2,... nRows

            MyMatrix sub_b = b.getSubmatrix(row_indices, { 0 });
            // sub_b.print();


            MyMatrix subSolution = solve(submatrix, sub_b);

            for (int i = 0; i < rank; ++i) {
                basicSolution.data[0][columns[i]] = subSolution.data[i][0]; // Place the subsolution into the complete solution
            }

            // Check if all solutions are non-negative
            bool feasible = true;
            for (int i = 0; i < nCols; ++i) {
                if (basicSolution.data[0][i] < 0) {
                    feasible = false;
                    std::cout << "Discarding negative solution." << std::endl;
                    break;
                }
            }

            // if non-negative, add it
            if (feasible) {
                feasibleSolutions.addRow(basicSolution);
            }
        }
    }

    return feasibleSolutions;
}

// Find specific col in the matrix
MyMatrix MyMatrix::getColumn(int col) const {
    if (col < 0 || col > nCols) throw std::out_of_range("Invalid column index.");
    MyMatrix column(nRows, 1);
    for (int i = 0; i < nRows; ++i) {
        column.data[i][0] = data[i][col];
    }
    return column;
}

