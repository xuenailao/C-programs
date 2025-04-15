#include "MyPortfolio.hpp"
#include "MyMatrix.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "gurobi_c++.h"
#include <unordered_map>


// Loading Data about stocks
std::vector<StockInfo> loadStockData(const std::string& portfolioFile) {
    std::vector<StockInfo> stocks;
    std::ifstream file(portfolioFile);

    if (!file.is_open()) {
        throw std::runtime_error("cant open");
    }

    int numStocks;
    file >> numStocks;

    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        StockInfo stock;
        stock.lower_limit = 0.025; 
        stock.upper_limit = 0.15; 
        if (!(iss >> stock.name >> stock.field >> stock.high_fee >> stock.mid_fee >> stock.low_fee)) {
            continue;
        }
        stocks.push_back(stock);
    }

    file.close();
    return stocks;
}


MyMatrix loadCovarianceMatrix(const std::string & filename, std::vector<StockInfo>&stocks) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件");
    }

    int numStocks;
    file >> numStocks;

    MyMatrix CovarianceMatrix(numStocks, numStocks);

    // Skip the remainder of the first line
    std::string line;
    std::getline(file, line);

    // Read stock names in the header line and skip it
    std::getline(file, line);
    std::istringstream nameStream(line);
    std::string stockName;
    nameStream >> stockName;  // Skip the first placeholder name

    // Read each row of the file
    for (int i = 0; i < numStocks; ++i) {
        file >> stockName;  // Read the stock name

        // Set stock name and expected return in StockInfo
        if (i < stocks.size()) {
            stocks[i].name = stockName;
            file >> stocks[i].expected_return;  // Read and set the expected return
        }
        else {
            throw std::runtime_error("Mismatch between number of stocks and provided stock info.");
        }

        // Read and set the covariance values for the row
        for (int j = 0; j < numStocks; ++j) {
            double value;
            file >> value;
            CovarianceMatrix.setvalue(i, j, value);
        }
    }

    file.close();
    return CovarianceMatrix;
}


std::vector<double> Portfolio_Optim_without_fees(const std::vector<StockInfo>& stocks, MyMatrix& covarianceMatrix,
    double minExpectedReturn, const std::vector<int>& maxStocksPerField) {
    try {

        GRBEnv env = GRBEnv(true);
        env.start();
        GRBModel model = GRBModel(env);

        std::vector<GRBVar> allocations;
        for (size_t i = 0; i < stocks.size(); ++i) {
            allocations.push_back(model.addVar(0, 1, 0.0, GRB_CONTINUOUS));
 
        }

        // Target Function: minimize Variance
        GRBQuadExpr riskExpr = 0;
        for (size_t i = 0; i < stocks.size(); ++i) {
            for (size_t j = 0; j < stocks.size(); ++j) {
                riskExpr += allocations[i] * covarianceMatrix.getvalue(i, j) * allocations[j];
            }
        }

        model.setObjective(riskExpr, GRB_MINIMIZE);

        // Constraints
        // 1. min expected return
        GRBLinExpr expectedReturnExpr = 0;
        for (size_t i = 0; i < stocks.size(); ++i) {
            expectedReturnExpr += stocks[i].expected_return * allocations[i];
        }
        model.addConstr(expectedReturnExpr >= minExpectedReturn, "min_return");

        // 2. limited stock number
        // shows whether to invest
        std::vector<GRBVar> investInStock;
        for (size_t i = 0; i < stocks.size(); ++i) {
            investInStock.push_back(model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "invest_" + std::to_string(i)));
        }

        // allocations[i] >0 ， investInStock[i] = 1
        for (size_t i = 0; i < stocks.size(); ++i) {
            model.addConstr(allocations[i] <= investInStock[i], "binary_activation" + std::to_string(i));
        }

        std::vector<GRBLinExpr> fieldConstraints(26, 0); 
        for (size_t i = 0; i < stocks.size(); ++i) {
            int fieldIdx = stocks[i].field - 'A';

            if (maxStocksPerField[fieldIdx] == 0) {
                // Force investInStock[i] to be 0 if no investment is allowed in this field
                model.addConstr(investInStock[i] == 0, "no_investment_field_" + std::to_string(i));
            }
            else {
                // Accumulate investments for fields with a max limit
                fieldConstraints[fieldIdx] += investInStock[i];
            }
        }

        // Apply field constraints for fields with max limits greater than zero
        for (size_t i = 0; i < fieldConstraints.size(); ++i) {
            if (maxStocksPerField[i] > 0) {
                model.addConstr(fieldConstraints[i] <= maxStocksPerField[i], "field_limit_" + std::to_string(i));
            }
        }

        // 3. weights=1
        GRBLinExpr totalAllocationExpr = 0;
        for (auto& alloc : allocations) {
            totalAllocationExpr += alloc;
        }
        model.addConstr(totalAllocationExpr == 1, "total_allocation");


        // 4. optimize
        model.optimize();

        if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
            std::vector<double> optimalAllocations(stocks.size(), 0.0);
            for (size_t i = 0; i < stocks.size(); ++i) {
                optimalAllocations[i] = allocations[i].get(GRB_DoubleAttr_X);
            }
            return optimalAllocations;
        }
        else {
            std::cerr << "Optimization was unsuccessful." << std::endl;
            return {};
        }

    }
    catch (GRBException& e) {
        std::cerr << "Gurobi error: " << e.getMessage() << std::endl;
        return {};
    }
    catch (...) {
        std::cerr << "Unknown error during optimization." << std::endl;
        return {};
    }
}


std::vector<double> Portfolio_Optim_with_fees(const std::vector<StockInfo>& stocks, MyMatrix& covarianceMatrix,
    double minExpectedReturn) {
    try {

        GRBEnv env = GRBEnv(true);
        env.start();
        GRBModel model = GRBModel(env);

        std::vector<GRBVar> allocations; //weights
        std::vector<GRBVar> investInStock; //whether to invest

        for (size_t i = 0; i < stocks.size(); ++i) {
            allocations.push_back(model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "allocation_" + std::to_string(i)));
            investInStock.push_back(model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "invest_" + std::to_string(i)));
        }

        GRBQuadExpr objectiveExpr = 0;

        // 1. Variance
        for (size_t i = 0; i < stocks.size(); ++i) {
            for (size_t j = 0; j < stocks.size(); ++j) {
                objectiveExpr += allocations[i] * covarianceMatrix.getvalue(i, j) * allocations[j];
            }
        }

        // 2. transaction fees
        for (size_t i = 0; i < stocks.size(); ++i) {
            double high_fee = stocks[i].high_fee;
            double low_fee = stocks[i].low_fee;
            double mid_fee = stocks[i].mid_fee;
            double lower_limit = stocks[i].lower_limit;
            double upper_limit = stocks[i].upper_limit;

            GRBVar low_fee_part = model.addVar(0.0, lower_limit, 0.0, GRB_CONTINUOUS, "low_fee_part_" + std::to_string(i));
            GRBVar mid_fee_part = model.addVar(0.0, upper_limit - lower_limit, 0.0, GRB_CONTINUOUS, "mid_fee_part_" + std::to_string(i));
            GRBVar high_fee_part = model.addVar(0.0, 1.0 - upper_limit, 0.0, GRB_CONTINUOUS, "high_fee_part_" + std::to_string(i));

            model.addConstr(allocations[i] == low_fee_part + mid_fee_part + high_fee_part);

            objectiveExpr += low_fee * low_fee_part + mid_fee * mid_fee_part + high_fee * high_fee_part;
        }

        model.setObjective(objectiveExpr, GRB_MINIMIZE);

        //Constraints

        //1. expected_returns
        GRBLinExpr expectedReturnExpr = 0;
        for (size_t i = 0; i < stocks.size(); ++i) {
            expectedReturnExpr += stocks[i].expected_return * allocations[i];
        }
        model.addConstr(expectedReturnExpr >= minExpectedReturn, "min_return");


        // 2. weights=1
        GRBLinExpr totalAllocationExpr = 0;
        for (auto& alloc : allocations) {
            totalAllocationExpr += alloc;
        }
        model.addConstr(totalAllocationExpr == 1, "total_allocation");

        // 3. limits of weights between upper and lower
        for (size_t i = 0; i < stocks.size(); ++i) {
            model.addConstr(allocations[i] >= stocks[i].lower_limit * investInStock[i], "lower_limit_" + std::to_string(i));
            model.addConstr(allocations[i] <= stocks[i].upper_limit * investInStock[i], "upper_limit_" + std::to_string(i));
        }

        //optimize
        model.optimize();

        if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
            std::vector<double> optimalAllocations(stocks.size(), 0.0);
            for (size_t i = 0; i < stocks.size(); ++i) {
                optimalAllocations[i] = allocations[i].get(GRB_DoubleAttr_X);
            }
            return optimalAllocations;
        }
        else {
            std::cerr << "Optimization was unsuccessful." << std::endl;
            return {};
        }

    }
    catch (GRBException& e) {
        std::cerr << "Gurobi error: " << e.getMessage() << std::endl;
        return {};
    }
    catch (...) {
        std::cerr << "Unknown error during optimization." << std::endl;
        return {};
    }
}
