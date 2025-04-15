// TrinomialTree.hpp
#ifndef TRINOMIALTREE_HPP
#define TRINOMIALTREE_HPP

#include<map>
#include <functional>
#include <tuple> 


class Option {
public:
	double S0;
	double K;
	double r;
	double sigma;
	double T;
	bool isCall;

	// Constructor
	Option(double S0, double K, double r, double sigma, double T, bool isCall);

	double intrinsicValue(double ST) const;

};


class TrinomialTree {
public:
	Option option;
	int nsteps;
	double dt;
	double u, d;
	double pu, pm, pl; //Probs
	double disc; //discount factor

	std::map<std::tuple<int, int, int>, double> memo;


	TrinomialTree(const Option& option, int nsteps, double lambda);
	double priceOption();

private:
	//Recursive
	double calculatePrice(int step, int up, int down);
};

#endif // TRINOMIALTREE_HPP