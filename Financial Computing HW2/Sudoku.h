#include "Sudoku.hpp"
#include <fstream>
#include <sstream>

Sudoku::Sudoku(const std::string& filename) : board(9, std::vector<int>(9, 0)) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return;
    }

    std::string line;
    int row = 0;
    while (std::getline(file, line) && row < 9) {
        std::istringstream iss(line);
        for (int col = 0; col < 9; ++col) {
            char ch;
            if (!(iss >> ch)) {
                board[row][col] = 0;  // 如果读取失败，设置为0
            }
            else if (ch == 'X') {
                board[row][col] = 0;  // 'X' 用0表示
            }
            else {
                board[row][col] = ch - '0';  // 将字符转换为整数
            }
        }
        ++row;
    }

    file.close();
}


void Sudoku::print() const {
    for (int i = 0; i < 9; ++i) {
        if (i % 3 == 0 && i != 0) {
            std::cout << "------+-------+------" << std::endl;
        }
        for (int j = 0; j < 9; ++j) {
            if (j % 3 == 0 && j != 0) {
                std::cout << "| ";
            }
            if (board[i][j] == 0) {
                std::cout << "X ";  // 用 'X' 表示空格
            }
            else {
                std::cout << board[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}



bool Sudoku::findEmptyPosition(int& row, int& col) const {
    for (row = 0; row < 9; ++row) {
        for (col = 0; col < 9; ++col) {
            if (board[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}

// check
bool Sudoku::isValid(int row, int col, int num) const {
    // check row
    for (int j = 0; j < 9; ++j) {
        if (board[row][j] == num) {
            return false;
        }
    }

    // check col
    for (int i = 0; i < 9; ++i) {
        if (board[i][col] == num) {
            return false;
        }
    }

    // check each 3x3
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = startRow; i < startRow + 3; ++i) {
        for (int j = startCol; j < startCol + 3; ++j) {
            if (board[i][j] == num) {
                return false;
            }
        }
    }

    return true;
}


bool Sudoku::solve() {
    int row, col;

    if (!findEmptyPosition(row, col)) {
        return true;
    }


    for (int num = 1; num <= 9; ++num) {
        if (isValid(row, col, num)) {
            board[row][col] = num; 

            if (solve()) { 
                return true;
            }

            board[row][col] = 0; 
        }
    }

    return false; // No solution
}
