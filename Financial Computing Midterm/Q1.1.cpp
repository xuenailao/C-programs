#include <iostream>
#include <vector>
#include <limits>
#include "MySimplex.hpp" 

int main() {
    // Example1
    std::cout << "Example1" << std::endl;
    std::vector<double> cost = { 4, 1, 0, 0 };  // min z = 4*x1 + x2 + 0*x3 + 0*x4
    std::vector<std::vector<double>> constraint_data = {
        {3, 1, 0, 0},      // 3*x1 + x2 = 3
        {4, 3, -1, 0},     // 4*x1 + 3*x2 - x3 = 6
        {1, 2, 0, 1}       // x1 + 2*x2 + x4 = 4
    };
    MyMatrix A(constraint_data);
    std::vector<double> RHS = { 3, 6, 4 };
    std::vector<int> basic_columns = {3,4,5};
    std::vector<int> basic_rows = { 0,1,2 };
    std::vector<int> artificial_columns = { 4,5 };

    Simplex simplex(A, basic_columns, basic_rows, artificial_columns, cost, RHS);
    simplex.solve_bigM();


    //Example2
    std::cout <<"\n\n\n" << std::endl;
    std::cout << "Example2" << std::endl;
    std::vector<double> cost2 = { 7, 3, 0, 0 }; // min 7*x1 + 3*x2
    const std::vector<std::vector<double>> cons_data2 = {
        {3, 1, 1, 0},  // x1 + x2 + x3 + 0*x4 <= 3
        {-2, 1, 0, 1},  // -2*x1 + x2 + 0*x3 - x4 <= 1
    };
    MyMatrix A2(cons_data2);

    std::vector<int> basic_columns2 = { 4,5 };//initialize
    std::vector<int> basic_rows2 = { 0, 1 };
    std::vector<int> artificial_columns2 = { 4,5 };
    std::vector<double> RHS2 = { 3, 1 };

    Simplex simplex2(A2, basic_columns2, basic_rows2, artificial_columns2, cost2, RHS2);
    simplex2.solve_bigM();

    return 0;
}
