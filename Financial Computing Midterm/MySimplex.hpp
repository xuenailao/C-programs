#ifndef MySimplex_hpp
#define MySimplex_hpp

#include <stdio.h>
#include "MyMatrix.hpp"
#include <vector>

class Simplex {
private:
    MyMatrix* A; // constraint
    MyMatrix* B; // submatrix
    std::vector<int> basic_columns; // cols in basis
    std::vector<int> basic_rows;
    std::vector<int> artificial_columns; // artificial
    std::vector<double> cost; // cost vector
    std::vector<double> RHS; // right side
    std::vector<double> sol; // solution
    int status; // 0: No solution, 1: Optim, 2: Boundless, 3: NotAvaliable

public:
    Simplex(MyMatrix &constraint_matrix,
        std::vector<int> initial_basic_columns,
        std::vector<int> initial_basic_rows,
        std::vector<int> artificial_columns,
        std::vector<double> c, std::vector<double> b);

    std::vector<double> getBasicCosts();

    std::vector<double> getY(int j);

    double getZ(int j);

    int FindEntering();

    int FindExiting(int entering);

    void solve_bigM();

    void findInitialBasis();
};

#endif /* MySimplex_hpp */
