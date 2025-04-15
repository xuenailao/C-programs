#include "MyPortfolio.hpp"
#include "MyMatrix.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <map>

int main() {
    // Loading Data
    std::string portfolioFile = "portfolio.txt";
    std::string infoFile = "extra_information.txt";
    std::vector<StockInfo> stocks = loadStockData(infoFile);
    MyMatrix covarianceMatrix = loadCovarianceMatrix(portfolioFile, stocks);

    // Problem 1: No transactino fees, limit stock numbers in each industry.
    double minExpectedReturn = 0.08;  //given portfolio rate

    std::vector<int> maxStocksPerField(26, 0);  
    maxStocksPerField[0] = 3;  //limit numbers
    maxStocksPerField[1] = 5;  
    maxStocksPerField[2] = 10; 

    // optimization
    std::vector<double> optimalAllocations = Portfolio_Optim_without_fees(stocks, covarianceMatrix, minExpectedReturn, maxStocksPerField);

    // print portfolio
    // Organize allocations by field
    std::map<char, std::vector<std::pair<std::string, double>>> fieldAllocations;

    for (size_t i = 0; i < optimalAllocations.size(); ++i) {
        if (optimalAllocations[i] != 0) {
            char field = stocks[i].field; // Get field category (e.g., 'A', 'B', 'C')
            fieldAllocations[field].emplace_back(stocks[i].name, optimalAllocations[i]);
        }
    }

    // Output the optimal allocations, grouped by field
    std::cout << "Under min expected return"<< minExpectedReturn << std::endl;
    std::cout << "Optimal allocations by field:" << std::endl;
    for (const auto& fieldEntry : fieldAllocations) {
        char field = fieldEntry.first;
        const auto& allocations = fieldEntry.second;

        std::cout << "Field " << field << ":" << std::endl;
        for (const auto& stockEntry : allocations) {
            const std::string& name = stockEntry.first;
            double allocation = stockEntry.second;
            std::cout << "  " << name << ": " << allocation << std::endl;
        }
        std::cout << std::endl;
    }


    // Problem 2: With transactino fees decided by weights.
    // optimization
    std::vector<double> optimalAllocations2 = Portfolio_Optim_with_fees(stocks, covarianceMatrix, minExpectedReturn);

    // print portfolio
    std::cout << "Under min expected return" << minExpectedReturn << std::endl;
    std::cout << "Optimal allocations:" << std::endl;
    for (size_t i = 0; i < optimalAllocations2.size(); ++i) {
        if (optimalAllocations2[i] != 0) {
            std::cout << stocks[i].name << ": " << optimalAllocations2[i] << std::endl;
        }

    }

    return 0;
}
