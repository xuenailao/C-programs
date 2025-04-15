#ifndef MYPORTFOLIO_HPP
#define MYPORTFOLIO_HPP

#include "MyMatrix.hpp"
#include <vector>
#include <string>

// 股票信息结构体
struct StockInfo {
    std::string name;
    double expected_return;
    char field;
    double high_fee,mid_fee, low_fee;
    double lower_limit, upper_limit;
};

// 函数声明
std::vector<StockInfo> loadStockData(const std::string& portfolioFile);
MyMatrix loadCovarianceMatrix(const std::string& filename, std::vector<StockInfo>& stocks);

std::vector<double> Portfolio_Optim_without_fees(const std::vector<StockInfo>& stocks, MyMatrix& covarianceMatrix,
    double minExpectedReturn, const std::vector<int>& maxStocksPerField);
std::vector<double> Portfolio_Optim_with_fees(const std::vector<StockInfo>& stocks, MyMatrix& covarianceMatrix,
    double minExpectedReturn);

#endif /* MYPORTFOLIO_HPP */
