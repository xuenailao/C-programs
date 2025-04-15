#include "Option.hpp"
#include "Matrix.hpp"
#include <iostream>


int main() {
    // Parameters for the American put option
    double S0 = 100.0;    // Initial stock price
    double K = 100.0;     // Strike price
    double T = 1.0/12;       // Time to maturity (in years)
    double r = 0.04;      // Risk-free interest rate
    double sigma = 0.2;   // Volatility
    double q = 0.02; // Dividend

    int numPaths = 10000; // Number of simulated paths
    int numSteps = 100;    // Number of time steps per path
    int numSamples = 20000; //Number of simulated samples

    // Create,an Option object
    Option Put(S0, K, T, r, q ,sigma, Option::PUT);
    Put.print();

    AmericanOptionPricing_LSM P1(Put);

    // Price the American put option using LSM
    double European_price = priceEuropeanMC(Put, numSamples);
    double Asian_price = priceAsianMC(Put, numSamples, numSteps);
    double American_price = P1.priceAmericanLSM(numPaths, numSteps);   
    double American_price2 = priceAmericanPutBBSR(Put, numSteps);
    
    return 0;
}
