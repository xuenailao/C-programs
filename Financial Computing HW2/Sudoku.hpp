#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <vector>
#include <string>
#include <iostream>

class Sudoku {
public:
    Sudoku(const std::string& filename);
    void print() const;
    bool solve();
    bool isValid(int row, int col, int num) const;

private:
    std::vector<std::vector<int>> board; // 9x9 

    // find next
    bool findEmptyPosition(int& row, int& col) const;
};

#endif 
