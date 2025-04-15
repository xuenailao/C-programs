//
//  main_sudoku.cpp
//  Sudoku.h
//  Sudoku.hpp


#include <iostream>
#include "Sudoku.h"

int main() {
    
    // Test sample1
    Sudoku mySudoku("sudoku.dat");
    
    mySudoku.print();
    
    if(mySudoku.solve()){ 
        mySudoku.print(); 
        std::cout << "Solved!\n"; 
    }
    else
        std::cout << "The Sudoku cannot be solved. \n"; 

    Sudoku mySudoku2("sudoku2.dat");

    // Test sample2
    mySudoku2.print();

    if (mySudoku2.solve()) {
        mySudoku2.print();
        std::cout << "Solved!\n";
    }
    else
        std::cout << "The Sudoku cannot be solved. \n";
    
    return 0;
}

