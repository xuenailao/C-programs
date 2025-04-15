#include "TrinomialTree.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;


Option::Option(double S0, double K, double r, double sigma, double T, bool isCall)
    : S0(S0), K(K), r(r), sigma(sigma), T(T), isCall(isCall) {}


double Option::intrinsicValue(double ST) const {
    if (isCall) {
        return std::max(0.0, ST - K); // Call option
    }
    else {
        return std::max(0.0, K - ST); // Put option
    }
}

TrinomialTree::TrinomialTree(const Option& option, int nsteps, double lambda) :
    option(option), nsteps(nsteps) {
    dt = option.T / nsteps;
    u = exp(lambda * option.sigma * sqrt(dt));
    d = 1 / u;
    disc = exp(-option.r * dt);
    //pu = 1.0 / (2 * lambda * lambda) + option.r / (2 * lambda * option.sigma) * sqrt(dt);
    //pl = 1.0 / (2 * lambda * lambda) - option.r / (2 * lambda * option.sigma) * sqrt(dt);
    //pm = 1.0 - pu - pl;
    //cout << "pu: " << pu << ", pm: " << pm << ", pl: " << pl << endl;
    pu = 1.0 / (2 * lambda * lambda) + 1 / sqrt(2.0) * option.sigma * sqrt(dt) / lambda;
    pl = 1.0 / (2 * lambda * lambda) - 1 / sqrt(2.0) * option.sigma * sqrt(dt) / lambda;
    pm = 1.0 - pu - pl;

    // 验证概率是否合理
    if (pu < 0 || pm < 0 || pl < 0 || (pu + pm + pl) != 1.0) {
        throw std::runtime_error("Invalid probabilities. Check lambda and input parameters.");
    }



}


double TrinomialTree::priceOption() {
    memo.clear(); // 清空之前的计算结果
    return calculatePrice(0, 0, 0);
}


double TrinomialTree::calculatePrice(int step, int up, int down) {
    auto key = std::make_tuple(step, up, down); // 使用 tuple 作为键
    if (memo.count(key)) {
        return memo[key];
    }

    // 到期时计算资产价格
    if (step == nsteps) {
        double ST = option.S0 * pow(u, up) * pow(d, down);
        double intrinsic = option.intrinsicValue(ST);
        memo[key] = intrinsic;
        return intrinsic;
    }

    // 递归计算
    double upPrice = calculatePrice(step + 1, up + 1, down);
    double midPrice = calculatePrice(step + 1, up, down);
    double downPrice = calculatePrice(step + 1, up, down + 1);

    // 加权平均
    double expectedPrice = (pu * upPrice + pm * midPrice + pl * downPrice) * disc;

    memo[key] = expectedPrice;
    return expectedPrice;
}

