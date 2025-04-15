#include <vector>
#include <string>
#ifndef MYMATRIX_HPP
#define MYMATRIX_HPP


class MyMatrix {
private: 
    int nRows;
    int nCols;
    double** data; 
public: 
    MyMatrix(int m, int n); 
    MyMatrix(const std::vector<std::vector<double>>& matrixData);
    MyMatrix(const std::vector<double> & vectorData);

    virtual ~MyMatrix(); 

    int getCols() const;
    int getRows() const;
    std::vector<std::vector<double>> getData() const;
    void setvalue(int i, int j, double value);
    double getvalue(int i, int j) const;

    MyMatrix transpose();
    MyMatrix transpose_external(); 
    void print() const;
    double determinant();

    int cofactor(int i, int j);
    double minor(int i, int j);

    MyMatrix operator +(MyMatrix &m2);
    MyMatrix operator *(MyMatrix &m2);
    MyMatrix operator *(double scalar);
    std::vector<double> operator*(const std::vector<double>& v1);
    MyMatrix operator=(MyMatrix& m2);

    void read_data(std::string s);
    

    int findRank() const;
    MyMatrix reverse_matrix() const;
    void addRow(const MyMatrix& row);
    void addColumn(const std::vector<double>& col);

    // Find Solution for AX=B
    MyMatrix enumerateBasicSolutions(const MyMatrix& b) const;
    MyMatrix getSubmatrix(const std::vector<int>& rows, const std::vector<int>& columns) const;
   
    // Find Solution for BigM
    // std::vector<double> solve_y(const std::vector<double>& b) const;
    MyMatrix getColumn(int col) const; // Get specific column as vector
};

std::vector<std::vector<int>> combos(int n, int r);
void gaussian_elimination(std::vector<std::vector<double>>& matrix, bool for_inverse = false);
MyMatrix solve(MyMatrix& A, MyMatrix& b);


#endif /* MYMATRIX_HPP */