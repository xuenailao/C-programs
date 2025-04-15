#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include "Matrix.hpp"
#include "Option.hpp"


Option::Option(double S0, double K, double T, double r, double q, double sigma, OptionType type) : S0(S0), K(K), T(T), r(r),q(q), sigma(sigma), type(type) {};

double Option::payoff(double ST) const {
	if (type == CALL) {
		return std::max(ST - K, 0.0);
	}
	else if (type == PUT) {
		return std::max(K - ST, 0.0);
	}
	return 0.0;
}


static double BSM(double S0, double K, double T, double r, double q, double sigma) {
    double d1 = (std::log(S0 / K) + (r - q + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    double N_d1 = 0.5 * (1.0 + std::erf(-d1 / std::sqrt(2.0)));
    double N_d2 = 0.5 * (1.0 + std::erf(-d2 / std::sqrt(2.0)));
    return K * std::exp(-r * T) * N_d2 - S0 * exp(-q * T) * N_d1;
}


void Option::print() const {
	std::cout << "Option Details:" << std::endl;
	std::cout << "Type: " << (type == CALL ? "Call" : "Put") << std::endl;
	std::cout << "Initial Stock Price (S0): " << S0 << std::endl;
	std::cout << "Strike Price (K): " << K << std::endl;
	std::cout << "Time to Maturity (T): " << T << " years" << std::endl;
	std::cout << "Risk-Free Interest Rate (r): " << r << std::endl;
    std::cout << "Dividend Rate (q) :" << q << std::endl;
	std::cout << "Volatility (sigma): " << sigma << std::endl;
}
       

// Function to simulate stock price paths using Geometric Brownian Motion
std::vector<std::vector<double>> AmericanOptionPricing_LSM::simulatePaths(int numPaths, int numSteps) const {
    std::vector<std::vector<double>> paths(numPaths, std::vector<double>(numSteps + 1, option.S0));
    double dt = option.T / numSteps;
    std::mt19937 rng(std::random_device{}());
    std::normal_distribution<> norm(0.0, 1.0);

    for (int i = 0; i < numPaths; ++i) {
        for (int j = 1; j <= numSteps; ++j) {
            double dW = norm(rng) * std::sqrt(dt);
            paths[i][j] = paths[i][j - 1] * std::exp((option.r - option.q - 0.5 * option.sigma * option.sigma) * dt + option.sigma * dW);
        }
    }
    return paths;
}

// Basis functions: Here we use Laguerre polynomials
std::vector<double> AmericanOptionPricing_LSM::basisFunctions(double S) const {
    return { 1.0, std::exp(-S / 2.0), std::exp(-S / 2.0) * (1 - S), std::exp(-S / 2.0) * (1 - 2 * S + S * S / 2) };
}


// Weighted Laguerre polynomials for regression
void AmericanOptionPricing_LSM::weightedLaguerre(const std::vector<double>& x, std::vector<std::vector<double>>& X, int k) const {
    int n = x.size();
    X.assign(n, std::vector<double>(k, 0.0));

    for (int i = 0; i < n; ++i) {
        X[i][0] = 1.0;
        if (k > 1) {
            X[i][1] = 1.0 - x[i];
        }
        for (int j = 2; j < k; ++j) {
            X[i][j] = ((2 * j - 1 - x[i]) * X[i][j - 1] - (j - 1) * X[i][j - 2]) / j;
        }
        double weight = std::exp(-x[i] / 2);
        for (int j = 0; j < k; ++j) {
            X[i][j] *= weight;
        }
    }
}


// Least squares regression to estimate continuation value
double AmericanOptionPricing_LSM::regression(const std::vector<std::vector<double>>& paths, const std::vector<double>& cashFlows, int step) const {
    int numPaths = paths.size();
    int numBasis = 4; // Number of basis functions
    std::vector<std::vector<double>> X;
    std::vector<double> scaledPrices(numPaths);

    // Scale prices relative to strike
    for (int i = 0; i < numPaths; ++i) {
        scaledPrices[i] = paths[i][step] / option.K;
    }

    // Generate basis functions using weighted Laguerre polynomials
    weightedLaguerre(scaledPrices, X, numBasis);

    // Perform regression using Gaussian elimination or other solver
    Matrix X_mat(X.size(), numBasis);
    std::vector<double> Y = cashFlows;
    for (int i = 0; i < X.size(); ++i) {
        for (int j = 0; j < numBasis; ++j) {
            X_mat(i, j) = X[i][j];
        }
    }
    Matrix XT = X_mat.transpose();
    Matrix XTX = XT * X_mat;
    std::vector<double> XTY = XT * Y;
    std::vector<double> beta = XTX.gaussianElimination(XTY);

    // Estimate continuation value based on regression coefficients
    double continuationValue = 0.0;
    for (int j = 0; j < numBasis; ++j) {
        continuationValue += beta[j] * X[0][j]; // Example using the first path
    }

    return continuationValue;
}


double AmericanOptionPricing_LSM::priceAmericanLSM(int numPaths, int numSteps) const {
    auto start = std::chrono::high_resolution_clock::now();

    // Step1: Simulate paths
    std::vector<std::vector<double>> paths = simulatePaths(numPaths, numSteps);
    double dt = option.T / numSteps;

    // Step2: Initialize cash flows
    std::vector<double> cashFlows(numPaths, 0.0);
    for (int i = 0; i < numPaths; ++i) {
        cashFlows[i] = option.payoff(paths[i][numSteps]);
    }

    // Step3: Backward induction
    for (int step = numSteps - 1; step > 0; --step) {
        for (int i = 0; i < numPaths; ++i) {
            // Discount cash flows
            cashFlows[i] *= std::exp(-option.r * dt);
        }
        // Estimate continuation value
        double continuationValue = regression(paths, cashFlows, step);

        // Decide whether to exercise
        for (int i = 0; i < numPaths; ++i) {
            double exerciseValue = option.payoff(paths[i][step]);
            if (exerciseValue > continuationValue) {
                cashFlows[i] = exerciseValue;
            }
        }
    }

    // Step4: Calculate the price as the average cash flows
    double optionPrice = 0.0;
    for (double cashFlow : cashFlows) {
        optionPrice += cashFlow;
    }
    optionPrice = optionPrice / numPaths * std::exp(-option.r * dt);

    // Step5: Calculate variance, standard error, and confidence interval
    double mean = optionPrice;
    double variance = 0.0;
    for (double cashFlow : cashFlows) {
        variance += (cashFlow - mean) * (cashFlow - mean);
    }
    variance /= numPaths;
    double standardError = std::sqrt(variance / numPaths);
    double confidenceLow = mean - 1.96 * standardError;
    double confidenceHigh = mean + 1.96 * standardError;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "---------------------------------------" << std::endl;
    std::cout << "LSM American Put Option Pricing" << std::endl;
    std::cout << "Option Price: " << optionPrice << std::endl;
    std::cout << "Standard Error: " << standardError << std::endl;
    std::cout << "95% Confidence Interval: [" << confidenceLow << ", " << confidenceHigh << "]" << std::endl;
    std::cout << "Computation Time: " << elapsed.count() << " seconds" << std::endl;

    return optionPrice;
}



// Binomial Black-Scholes with Richardson Extrapolation for American Put Option
double priceAmericanBinomial(Option option, int numSteps) {
    double dt = option.T / numSteps;
    double u = std::exp(option.sigma * std::sqrt(dt));
    double d = 1.0 / u;
    double p = (std::exp((option.r - option.q) * dt) - d) / (u - d);
    double discountFactor = std::exp(-option.r * dt);

    // Set up the price tree
    std::vector<double> prices(numSteps + 1);
    for (int i = 0; i <= numSteps; ++i) {
        prices[i] = option.S0 * std::pow(u, numSteps - i) * std::pow(d, i);
    }

    // Set up the option values at maturity
    std::vector<double> values(numSteps + 1);
    for (int i = 0; i <= numSteps; ++i) {
        values[i] = std::max(option.K - prices[i], 0.0);
    }

    // Work backwards through the tree
    for (int step = numSteps - 1; step >= 0; --step) {
        for (int i = 0; i <= step; ++i) {
            double remainingTime = option.T - step * dt;
            double continuationValue = 0.0;
            if (step == numSteps-1) {//Use BS
               continuationValue = BSM(prices[i], option.K, remainingTime, option.r, option.q, option.sigma);
            }
            else {
               continuationValue = discountFactor * (p * values[i] + (1.0 - p) * values[i + 1]);
            }
            prices[i] = prices[i] / u; //Move prices back to previous level
            values[i] = std::max(continuationValue, option.K - prices[i]); // Check for early exercise
        }
    }


    return values[0];

}


double priceAmericanPutBBSR(Option option, int numSteps) {
    auto start = std::chrono::high_resolution_clock::now();
    double priceN = priceAmericanBinomial(option, numSteps);
    double price2N = priceAmericanBinomial(option, 2 * numSteps);
    double bbsrPrice = 2 * price2N - priceN;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "---------------------------------------" << std::endl;
    std::cout << "BBSR American Put Option Pricing" << std::endl;
    std::cout << "Option Price: " << bbsrPrice << std::endl;
    std::cout << "Computation Time: " << elapsed.count() << " seconds" << std::endl;
    return bbsrPrice;
}


double priceEuropeanMC(Option option, int numSamples) {
    std::mt19937 rng(std::random_device{}());
    std::normal_distribution<> norm(0.0, 1.0);

    double sumPayoff = 0.0;
    double sumPayoffSquared = 0.0;

    auto start = std::chrono::high_resolution_clock::now();

    // Monte Carlo simulation
    for (int i = 0; i < numSamples; ++i) {
        double ST = option.S0 * std::exp((option.r - option.q - 0.5 * option.sigma * option.sigma) * option.T + option.sigma * norm(rng) * std::sqrt(option.T));
        double payoff = option.payoff(ST);
        sumPayoff += payoff;
        sumPayoffSquared += payoff * payoff;
    }

    double discountFactor = std::exp(-option.r * option.T);
    double optionPrice = discountFactor * (sumPayoff / numSamples);
    double variance = (sumPayoffSquared / numSamples) - (sumPayoff / numSamples) * (sumPayoff / numSamples);
    double standardError = discountFactor * std::sqrt(variance / numSamples);

    double lowConfidence = optionPrice - 1.96 * standardError;
    double uppConfidence = optionPrice - 1.96 * standardError;

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Monte Carlo European Put Option Pricing" << std::endl;
    std::cout << "Number of Samples: " << numSamples << std::endl;
    std::cout << "Option Price: " << optionPrice << std::endl;
    std::cout << "Estimated Standard Error: " << standardError << std::endl;
    std::cout << "95% Confidence Interval: [" << lowConfidence << ", " << uppConfidence << "]" << std::endl;
    std::cout << "Computation Time: " << elapsed.count() << " seconds" << std::endl;

    return optionPrice;
}



double priceAsianMC(Option option, int numSteps, int numSamples) {
    // Set up random number generation
    std::mt19937 rng(std::random_device{}());
    std::normal_distribution<> norm(0.0, 1.0);

    double sumPayoff = 0.0;
    double sumPayoffSquared = 0.0;
    double dt = option.T / numSteps;

    // Start timing the simulation
    auto start = std::chrono::high_resolution_clock::now();

    // Monte Carlo simulation
    for (int i = 0; i < numSamples; ++i) {
        double ST = option.S0;
        double sumST = 0.0;
        for (int j = 0; j < numSteps; ++j) {
            double dW = norm(rng) * std::sqrt(dt);
            ST *= std::exp((option.r - option.q - 0.5 * option.sigma * option.sigma) * dt + option.sigma * dW);
            sumST += ST;
        }
        double averageST = sumST / numSteps;
        double payoff = option.payoff(averageST);
        sumPayoff += payoff;
        sumPayoffSquared += payoff * payoff;
    }

    // Calculate option price, standard error, and confidence interval
    double discountFactor = std::exp(-option.r * option.T);
    double optionPrice = discountFactor * (sumPayoff / numSamples);
    double variance = (sumPayoffSquared / numSamples) - (sumPayoff / numSamples) * (sumPayoff / numSamples);
    double standardError = discountFactor * std::sqrt(variance / numSamples);

    double confidenceLower = optionPrice - 1.96* standardError;
    double confidenceUpper = optionPrice + 1.96 * standardError;

    // End timing the simulation
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Output the results
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Monte Carlo Asian Call Option Pricing" << std::endl;
    std::cout << "Number of Samples: " << numSamples << std::endl;
    std::cout << "Option Price: " << optionPrice << std::endl;
    std::cout << "Estimated Standard Error: " << standardError << std::endl;
    std::cout << "95% Confidence Interval: [" << confidenceLower << ", " << confidenceUpper << "]" << std::endl;
    std::cout << "Computation Time: " << elapsed.count() << " seconds" << std::endl;

    return optionPrice;
}
