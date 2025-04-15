//
//  main.cpp
//  tower_of_hanoi
//
// By comparison 3 and 4 towers with the same plates,

#include <iostream>
#include "Hanoi.h"

int main(int argc, const char * argv[]) {
    
    int ndiscs=7;
    int ntowers=4; 
    
    Hanoi h2(ndiscs,ntowers); 
    h2.print(); 
    h2.move(ndiscs,0,ntowers-1,ntowers-2); 
    h2.print();
    
    std::cout << "Total moves: " << h2.getMoves() << std::endl;
    
    
    Hanoi h(ndiscs,ntowers); 
    h.print(); 
    h.move4(ndiscs,0,ntowers-1,ntowers-2,ntowers-3); 
    h.print(); 

    std::cout << "Total moves: " << h.getMoves() << std::endl;
    
    // What do you observe about the difference in moves?
    /*The 4-tower method is more efficient due to the additional tower.
    The 4-tower solution generally requires fewer moves than the 3-tower solution, especially as the number of discs increases.
    */
    return 0;
    
    
}
