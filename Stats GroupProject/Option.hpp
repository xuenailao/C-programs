#include <vector>


class Option {
public:
	enum OptionType { CALL, PUT };

	double S0;
	double K;
	double T;
	double r;
	double q;
	double sigma;
	OptionType type;

public:
	Option(double S0, double K, double T, double r, double q, double sigma, OptionType type);
	double payoff(double ST) const;
	void print() const;

};


class AmericanOptionPricing_LSM {
private:
	Option option;
public:
	AmericanOptionPricing_LSM(Option option) :option(option) {}

	std::vector<std::vector<double>> simulatePaths(int numPaths, int numSteps) const;
	std::vector<double> basisFunctions(double S) const;
	void weightedLaguerre(const std::vector<double>& x, std::vector<std::vector<double>>& X, int k) const;
	double regression(const std::vector<std::vector<double>>& paths, const std::vector<double>& cashFlows, int step) const;
	double priceAmericanLSM(int numPaths, int numSteps) const;
};

static double BSM(double S0, double K, double T, double r,double q, double sigma);
double priceAmericanBinomial(Option option, int numSteps);
double priceAmericanPutBBSR(Option option, int numSteps);

double priceEuropeanMC(Option option, int numSamples);
double priceAsianMC(Option option, int numSteps, int numSamples);