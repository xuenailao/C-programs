#ifndef HANOI_HPP
#define HANOI_HPP

#include <iostream>
#include <vector>
#include <cmath>

class Hanoi {
public:
    Hanoi(int numDiscs, int numTowers); 

    void print() const; 
    void move(int n, int from, int to, int aux); 
    void move4(int n, int from, int to, int aux1, int aux2); 
    int getMoves() const; 

private:
    std::vector<std::vector<int>> towers; 
    int moves; 

    void printMove(int disc, int from, int to);
};

#endif // HANOI_HPP
