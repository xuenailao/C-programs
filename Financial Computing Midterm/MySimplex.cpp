#include "MySimplex.hpp"
#include <algorithm>
#include <limits>
#include <iostream>


Simplex::Simplex(MyMatrix& constraint_matrix,
    std::vector<int> initial_basic_columns,
    std::vector<int> initial_basic_rows,
    std::vector<int> artificial_columns,
    std::vector<double> c, std::vector<double> b)
    : A(&constraint_matrix),
    basic_columns(initial_basic_columns),
    basic_rows(initial_basic_rows),
    artificial_columns(artificial_columns),
    cost(c), RHS(b) {

    B = new MyMatrix(A->getSubmatrix(basic_rows, basic_columns));  // subset orignial basis from A
    sol = std::vector<double>(A->getCols(), 0); // intialize
    status = 0;
}


std::vector<double> Simplex::getBasicCosts() {
    std::vector<double> basic_costs;
    for (int col : basic_columns) {
        basic_costs.push_back(cost[col]);
    }
    return basic_costs;
}

// delete artificial variable Y
std::vector<double> Simplex::getY(int j) {
    MyMatrix y = A->getColumn(j);

    // Bx=y
    MyMatrix x = solve(*B, y);
    std::vector<double> result(x.getRows());
    auto matrixData = x.getData();
    for (int i = 0; i < x.getRows(); ++i) {
        result[i] = matrixData[i][0]; 
    }

    return result;
}


double Simplex::getZ(int j) {
    double z = 0;
    auto basic_costs = getBasicCosts();
    for (int i = 0; i < basic_costs.size(); ++i) {
        z += basic_costs[i] * B->getvalue(i, j);
    }
    return z;
}


int Simplex::FindEntering() {
    int entering = -1;

    for (int j = 0; j < cost.size(); ++j) {
        if (std::find(basic_columns.begin(), basic_columns.end(), j) != basic_columns.end()) {
            continue;
        }

        double reduced_cost = cost[j] - getZ(j);
        if (reduced_cost < 0) {
            entering = j;
            break;
        }
    }
    return entering;
}


// 查找退出变量
int Simplex::FindExiting(int entering) {
    std::vector<double> y = getY(entering);
    double min_ratio = std::numeric_limits<double>::max();
    //std::cout << "Initial min_ratio: " << min_ratio << std::endl;
    int exiting = -1;
    bool foundone = false;

    for (int i = 0; i < y.size(); ++i) {
        if (y[i] > 0) {
            double ratio = sol[basic_columns[i]] / y[i];
            foundone = true;
            if (ratio < min_ratio) {
                min_ratio = ratio;
                exiting = basic_columns[i];
            }
        }
    }
    if (!foundone) {
        status = 2; // unbounded
    }
    return exiting;
}

// Big-M 
void Simplex::findInitialBasis() {
    double M = 10000; 
    int num_original_vars = A->getCols(); 
    
    // ensure all RHS is positive
    for (int i = 0; i < RHS.size(); ++i) {
        if (RHS[i] < 0) {
            for (int j = 0; j < A->getCols(); ++j) {
                A->setvalue(i, j, -A->getvalue(i, j));
            }
            RHS[i] = -RHS[i];
        }
    }

    // add artificial vars
    for (int i = 0; i < artificial_columns.size(); ++i) {
        cost.push_back(M);  //add to costs
        //basic_columns.push_back(num_original_vars + i);//add to column index

        // add to cons matrix A
        std::vector<double> artificial_col(A->getRows(), 0.0);
        artificial_col[i] = 1.0;  
        A->addColumn(artificial_col); 
    }

    // renew basic matrix B
    B = new MyMatrix(A->getSubmatrix(basic_rows, basic_columns));
    sol = std::vector<double>(A->getCols(), 0); // intialize
    MyMatrix B_inv = B->reverse_matrix();
    std::vector<double> b_solution = B_inv * RHS;

    for (int i = 0; i < basic_columns.size(); ++i) {
        sol[basic_columns[i]] = b_solution[i];
    }
}



void Simplex::solve_bigM() {
    findInitialBasis(); 

    while (status == 0) {
        int entering = FindEntering();
        B = new MyMatrix(A->getSubmatrix(basic_rows, basic_columns));
        B->print();
        MyMatrix B_inv = B->reverse_matrix();
        B_inv.print();
        std::vector<double> b_solution = B_inv * RHS;

        // 将基变量的解填充到 sol 的对应位置
        sol = std::vector<double>(A->getCols(), 0); // intialize
        for (int i = 0; i < basic_columns.size(); ++i) {
            sol[basic_columns[i]] = b_solution[i];
        }

        if (entering == -1) {
            status = 1; // 找到最优解
            break;
        }
        std::cout << "Entering variable: X" << (entering + 1) << std::endl;

        int exiting = FindExiting(entering);
        
        if (exiting == -1) {
            status = 2; // 无界解
            break;
        }       
        std::cout << "Exiting variable: X" << (exiting + 1) << std::endl;
        // 更新基
        std::replace(basic_columns.begin(), basic_columns.end(), exiting, entering);
        std::sort(basic_columns.begin(), basic_columns.end());

    }

    if (status == 1) {
        std::cout << "Optimal solution found." << std::endl;
        for (double val : sol) {
            std::cout << val << " ";
        }
        std::cout << std::endl; // 换行
        
        double optimal_value = 0.0;
        for (size_t i = 0; i < sol.size(); ++i) {
            optimal_value += sol[i] * cost[i];
        }
        std::cout << "Optimal objective value: " << optimal_value << std::endl;

    }
    else if (status == 2) {
        std::cout << "The solution is unbounded." << std::endl;
    }
    else {
        std::cout << "The problem is infeasible." << std::endl;
    }
}

