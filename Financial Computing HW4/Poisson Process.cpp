#include <iostream>
#include <vector>
#include <random>
#include <cmath>

using namespace std;

class PoissonProcess {
private:
    double lambda;
    std::mt19937 gen;

    static int factorial(int n) {
        if (n <= 1) return 1;
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    }

public:
    // Constructor
    PoissonProcess(double l) : lambda(l), gen(std::random_device{}()) {}

    // Expectation of N(t)
    double expectation(double t) const {
        return lambda * t;
    }

    // P(N(t)=n)
    double probability(int n, double t) const {
        double mean = expectation(t);
        return (std::pow(mean, n) * std::exp(-mean)) / factorial(n);
    }

    // P(N(t)-N(s)=n)
    double probability(int n, double t, double s) const {
        double deltaT = t - s;
        double mean = lambda * deltaT;
        return (std::pow(mean, n) * std::exp(-mean)) / factorial(n);
    }

    // Simulate event times up to time t
    std::vector<double> simulate(double t) {
        std::exponential_distribution<double> exp_dis(lambda);
        std::vector<double> event_times;

        double current_t = 0;
        while (current_t < t) {
            double inter_t = exp_dis(gen);
            current_t += inter_t;
            if (current_t < t) {
                event_times.push_back(current_t);
            }
        }
        return event_times;
    }

    // Simulate events by types
    std::vector<std::vector<double>> simulate_types(const std::vector<double>& probs, double t) {
        std::vector<std::vector<double>> type_events(probs.size());
        auto events = simulate(t);
        std::discrete_distribution<int> type_dis(probs.begin(), probs.end());

        for (double event_time : events) {
            int type = type_dis(gen);
            type_events[type].push_back(event_time);
        }

        return type_events;
    }
};

int main() {
    double lambda = 3;
    std::vector<double> probs = { 0.03, 0.97 };
    std::vector<int> ntrials = { 10, 100, 1000 };
    std::vector<double> mean_counts(3, 0);
    PoissonProcess pp(lambda);

    // Compute expectations and probabilities
    double prob1 = pp.probability(3, 1);
    double prob2 = pp.probability(3, 2, 1);

    std::cout << "P(N(3)=20) = " << prob1 << std::endl;
    std::cout << "P(N(3)-N(1)=25) = " << prob2 << std::endl;


    PoissonProcess pp2(1e6);
    // Simulate events and calculate mean counts
    for (int j = 0; j < 2; ++j) {
        int sumup = 0;
        for (int i = 0; i < ntrials[j]; ++i) {
            auto event_types = pp2.simulate_types(probs, 1);
            sumup += event_types[0].size();
        }
        mean_counts[j] = static_cast<double>(sumup) / ntrials[j];
        std::cout << ntrials[j] << " Trials, mean counts: " << mean_counts[j] << std::endl;
    }

    return 0;
}
