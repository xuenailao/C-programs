#include "Hanoi.h"


Hanoi::Hanoi(int numDiscs, int numTowers) : moves(0) {
    towers.resize(numTowers); 
    for (int i = numDiscs; i >= 1; --i) {
        towers[0].push_back(i);
    }
}


void Hanoi::print() const {
    for (int i = 0; i < towers.size(); ++i) {
        std::cout << "Tower " << i+1 << ": ";
        for (int disc : towers[i]) {
            std::cout << disc << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void Hanoi::move(int n, int from, int to, int aux) {
    if (n == 0) return;

    move(n - 1, from, aux, to); 

    int disc = towers[from].back();

    towers[to].push_back(disc);  
    towers[from].pop_back();     
    printMove(disc, from, to);   

    move(n - 1, aux, to, from); 
}



void Hanoi::move4(int n, int from, int to, int aux1, int aux2) {
    if (n == 0) return;

    if (n == 1) {
        towers[to].push_back(towers[from].back());
        towers[from].pop_back();
        printMove(n, from, to);
    }
    else {
      
        int k = static_cast<int>(sqrt(2 * n));

        
        move4(n - k, from, aux1, aux2, to);

        move(k, from, to, aux2);
        
        move4(n-k, aux1, to, from, aux2);
    }
}


void Hanoi::printMove(int disc, int from, int to) {
    ++moves;
    std::cout << "Move disc " << disc << " from Tower " << from+1 << " to Tower " << to+1 << std::endl;
}


int Hanoi::getMoves() const {
    return moves;
}
