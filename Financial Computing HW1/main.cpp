#include <iostream>
#include <vector>
#include "mymatrix.h"


int main(int argc, const char * argv[]) {
    
    // Code to test how the combinations work!
    //std::vector<std::vector<int>> c=combos(5,3);
    
    /*std::vector<std::vector<int>>::iterator it=c.begin();
    for(it=c.begin();it!=c.end();it++){
        std::vector<int>::iterator it2;
        for(it2=(*it).begin();it2!=(*it).end();it2++)
            std::cout << *it2 << " ";
        std::cout << std::endl;
    }*/

 
    MyMatrix A(2, 4); // random initialization of a 3x3 matrix
    A.read_data("matrix_A.dat"); // read matrix contents from file
    

    MyMatrix b(2, 1); // random initialization of a 3x3 matrix
    b.read_data("matrix_b.dat"); // read matrix contents from file
    

    // Question1: rank
    int rank = A.findRank();
    std::cout << "Matrix Rank of A: " << rank << std::endl;


    // Question2: Find the solution of x = A^{-1}b by reverse matrix
    MyMatrix A2(4, 4); 
    A2.read_data("matrix_A2.dat"); 
    A2.print(); 

    MyMatrix b2(4, 1);
    b2.read_data("matrix_b2.dat");
    b2.print();

    MyMatrix x1 = solve(A2, b2);
    std::cout << "Solution of Ax=b(Question2) " << std::endl;
    x1.print();


    // Question3: Find the solution of x = A^{-1}b by sub matrix
    std::cout << "Solution of Ax=b(Question3) " << std::endl;
    A.print(); // print the read matrix
    b.print(); // print the read matrix
    MyMatrix x2 = A.enumerateBasicSolutions(b);
    x2.print();

    return 0;
}
