#include "TrinomialTree.hpp"
#include<iostream>

using namespace std;


int main() {
    // Define option parameters
    double S = 1;       // Initial stock price
    double K = 2;       // Strike price
    double r = 0.08;        // Risk-free rate
    double sigma = 0.2; // Volatility
    double T = 10;         // Time to maturity (in years)
    int nsteps = 100;       // Number of steps

    double lambda = 1;

    //Call option
    Option CALL(S, K, r, sigma, T, true);
    Option PUT(S, K, r, sigma, T, false);
    TrinomialTree TREE1(CALL, nsteps, lambda);
    TrinomialTree TREE2(PUT, nsteps, lambda);

    double C= TREE1.priceOption();
    std::cout << "Option Price (Call): " << C << std::endl;

    double P = TREE2.priceOption();
    std::cout << "Option Price (Put): " << P << std::endl;
    
    double left = S + P - C;
    double right = K * exp(-r * T);

    std::cout << "S+P-C " << left << std::endl;
    std::cout << "K*e^(-rt)" << right << std::endl;
    return 0;
}