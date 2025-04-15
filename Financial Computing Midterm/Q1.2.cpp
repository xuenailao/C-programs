#include <iostream>
#include <vector>
#include "MyMatrix.hpp"


// Function to set up matrix A with x and y variables
MyMatrix setupMatrix(double x, double y) {
    MyMatrix A(4, 5);
    A.setvalue(0, 0, 1); A.setvalue(0, 1, 2); A.setvalue(0, 2, 3); A.setvalue(0, 3, 4); A.setvalue(0, 4, 0);
    A.setvalue(1, 0, 5); A.setvalue(1, 1, 6); A.setvalue(1, 2, 7); A.setvalue(1, 3, 8); A.setvalue(1, 4, 0);
    A.setvalue(2, 0, x); A.setvalue(2, 1, x); A.setvalue(2, 2, x); A.setvalue(2, 3, x); A.setvalue(2, 4, 0);
    A.setvalue(3, 0, 0); A.setvalue(3, 1, 0); A.setvalue(3, 2, 0); A.setvalue(3, 3, 0); A.setvalue(3, 4, y);
    return A;
}


void findMinMaxRank() {
    double minRank = std::numeric_limits<double>::max();
    double maxRank = std::numeric_limits<double>::min();
    double bestXMin, bestYMin, bestXMax, bestYMax;

    for (double x = 0; x <= 1; x += 1.0) { // range
        for (double y = 0; y <= 1; y += 1.0) {
            MyMatrix A = setupMatrix(x, y);
            int rank = A.findRank();

            if (rank < minRank) {
                minRank = rank;
                bestXMin = x;
                bestYMin = y;
            }

            if (rank > maxRank) {
                maxRank = rank;
                bestXMax = x;
                bestYMax = y;
            }
        }
    }

    std::cout << "Minimum rank is " << minRank << " with x = " << bestXMin << " and y = " << bestYMin << std::endl;
    std::cout << "Maximum rank is " << maxRank << " with x = " << bestXMax << " and y = " << bestYMax << std::endl;
}


void checkSolvability(std::vector<double> b) {
    for (double x = -1; x <= 1; x += 1.0) { // range
        for (double y = -1; y <= 1; y += 1.0) {
            MyMatrix A = setupMatrix(x, y);
            std::vector<std::vector<double>> augmented_matrix = A.getData();
            // Print the reduced augmented matrix
            std::cout << "Reduced Augmented Matrix:" << std::endl;

            for (size_t i = 0; i < augmented_matrix.size(); ++i) {
                augmented_matrix[i].push_back(b[i]);
            }

            MyMatrix augmented_matrix1 = MyMatrix(augmented_matrix);
            augmented_matrix1.print();

            int rank_augment = augmented_matrix1.findRank();
            int rank_A = A.findRank();
            if (rank_A == rank_augment) {
                std::cout <<"X="<<x<<",Y="<<y<< "Solvable" << std::endl;
            }
            else {
                std::cout << "Non-Solvable" << std::endl;
            }
        }
    }
}
    

int main() {

    // (1)Find rank range
    findMinMaxRank();

    // (2)Find solutions
    // Different vectors b to test
    std::vector<double> b1 = { 1, 1, 0, 0 };
    std::vector<double> b2 = { 1, 1, 0, 1 };
    std::vector<double> b3 = { 1, 1, 1, 1 };

    // Checking solvability for each b
    std::cout << "b1" << std::endl;
    checkSolvability(b1);
    std::cout << "b2" << std::endl;
    checkSolvability(b2);
    std::cout << "b3" << std::endl;
    checkSolvability(b3);

    std::cout << "b1, for any x and y, is solvable" << std::endl;
    std::cout << "b2, for any y inequals 0, is solvable" << std::endl;
    std::cout << "b3, for any x and y both inequal 0, is solvable" << std::endl;
    return 0;
}
