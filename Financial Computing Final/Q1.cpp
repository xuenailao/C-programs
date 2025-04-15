#include "TrinomialTree.hpp"
#include <iostream>
#include <chrono> // 包含计时功能的头文件

using namespace std;

int main() {
    // 定义期权参数
    double S = 1;       // 初始股票价格
    double K = 2;       // 行权价
    double r = 0.08;    // 无风险利率
    double sigma = 0.2; // 波动率
    double T = 10;      // 到期时间（年）
    int nsteps = 100;   // 时间步数

    double lambda = 1;  // Lambda 参数

    // 创建期权和三叉树模型
    Option CALL(S, K, r, sigma, T, true);
    Option PUT(S, K, r, sigma, T, false);
    TrinomialTree TREE1(CALL, nsteps, lambda);
    TrinomialTree TREE2(PUT, nsteps, lambda);

    // 开始计时
    auto start = chrono::high_resolution_clock::now();

    // 计算看涨期权价格
    double C = TREE1.priceOption();
    auto endCall = chrono::high_resolution_clock::now(); // 记录看涨期权结束时间
    cout << "Option Price (Call): " << C << endl;

    // 计算看跌期权价格
    double P = TREE2.priceOption();
    auto endPut = chrono::high_resolution_clock::now(); // 记录看跌期权结束时间
    cout << "Option Price (Put): " << P << endl;

    // 验证Put-Call平价
    double left = S + P - C;
    double right = K * exp(-r * T);
    cout << "S + P - C: " << left << endl;
    cout << "K * e^(-rT): " << right << endl;

    // 计算运行时间
    auto durationCall = chrono::duration_cast<chrono::milliseconds>(endCall - start);
    auto durationPut = chrono::duration_cast<chrono::milliseconds>(endPut - endCall);
    auto totalDuration = chrono::duration_cast<chrono::milliseconds>(endPut - start);

    // 输出运行时间
    cout << "Time taken for Call Option: " << durationCall.count() << " ms" << endl;
    cout << "Time taken for Put Option: " << durationPut.count() << " ms" << endl;
    cout << "Total Time taken: " << totalDuration.count() << " ms" << endl;

    return 0;
}
