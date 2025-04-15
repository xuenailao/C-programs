#ifndef MYPORTFOLIO_HPP
#define MYPORTFOLIO_HPP

#include "MyMatrix.hpp"
#include <vector>
#include <string>

// ��Ʊ��Ϣ�ṹ��
struct StockInfo {
    std::string name;
    double expected_return;
    char field;
    double high_fee,mid_fee, low_fee;
    double lower_limit, upper_limit;
};

// ��������
std::vector<StockInfo> loadStockData(const std::string& portfolioFile);
MyMatrix loadCovarianceMatrix(const std::string& filename, std::vector<StockInfo>& stocks);

std::vector<double> Portfolio_Optim_without_fees(const std::vector<StockInfo>& stocks, MyMatrix& covarianceMatrix,
    double minExpectedReturn, const std::vector<int>& maxStocksPerField);
std::vector<double> Portfolio_Optim_with_fees(const std::vector<StockInfo>& stocks, MyMatrix& covarianceMatrix,
    double minExpectedReturn);

#endif /* MYPORTFOLIO_HPP */
