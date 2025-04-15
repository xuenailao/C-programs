#include <iostream>
#include <vector>
#include <fstream>
#include <gurobi_c++.h>

class Assignment {
private:
    int n; // people
    int m; // tasks
    std::vector<std::vector<double>> cost; // cost matrix

public:

    Assignment() : n(0), m(0) {}

    bool read(const std::string& filename) {
        std::ifstream infile(filename);
        if (!infile.is_open()) {
            std::cerr << "can't open: " << filename << std::endl;
            return false;
        }

        infile >> n >> m;
        cost.resize(n, std::vector<double>(m, 0.0));

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (!(infile >> cost[i][j])) {
                    std::cerr << "read error£¬position (" << i + 1 << ", " << j + 1 << ")" << std::endl;
                    return false;
                }
            }
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                std::cout << "Cost of assigning agent " << i + 1 << " to task " << j + 1 << ": " << cost[i][j] << std::endl;
            }
        }


        infile.close();
        return true;
    }

    void solve() {
        try {
            GRBEnv env = GRBEnv(true);
            env.set("LogFile", "assignment.log");
            env.start();
            GRBModel model = GRBModel(env);

            std::vector<std::vector<GRBVar>> x(n, std::vector<GRBVar>(m));
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    x[i][j] = model.addVar(0.0, 1.0, cost[i][j], GRB_BINARY, "x_" + std::to_string(i) + "_" + std::to_string(j));
                }
            }

            // Target function: minimize
            GRBLinExpr obj = 0.0;
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < m; ++j)
                {
                    obj += cost[i][j] * x[i][j];
                }
            model.setObjective(obj, GRB_MINIMIZE);


            for (int i = 0; i < n; ++i) {
                GRBLinExpr expr = 0.0;
                for (int j = 0; j < m; ++j)
                    expr += x[i][j];
                model.addConstr(expr == 1.0, "Agent_" + std::to_string(i));
            }

          
            for (int j = 0; j < m; ++j) {
                GRBLinExpr expr = 0.0;
                for (int i = 0; i < n; ++i)
                    expr += x[i][j];
                model.addConstr(expr == 1.0, "Task_" + std::to_string(j));
            }

            
            model.optimize();

            if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
                std::cout << "find solution, total cost: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;
                
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < m; ++j) {
                        if (x[i][j].get(GRB_DoubleAttr_X) > 0.5) { 
                            std::cout << "Agent " << i + 1 << " task " << j + 1 << std::endl;
                        }
                    }
                }
            }
            else {
                std::cout << "No optimal solution" << std::endl;
            }

        }
        catch (GRBException e) {
            std::cerr << "Gurobi mistake: " << e.getMessage() << std::endl;
        }
        catch (...) {
            std::cerr << "Unknown mistake" << std::endl;
        }

    }

};


int main() {
    Assignment assignment;
    if (assignment.read("linear_assignment.dat")) {
        assignment.solve();
    }
    return 0;


}
