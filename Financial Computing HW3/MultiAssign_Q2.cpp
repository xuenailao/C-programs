#include <iostream>
#include <vector>
#include <fstream>
#include <gurobi_c++.h>

class MultiAssignment {
private:
    int n; // set 
    int d; // elements of each set
    std::vector<double> cost; // cost

public:
    MultiAssignment() : n(0), d(0) {}

    int getIndex(int i1, int i2, int i3, int i4, int n) {
        return i1 * n * n * n + i2 * n * n + i3 * n + i4;
    }


    bool read(const std::string& filename) {
        std::ifstream infile(filename);
        if (!infile.is_open()) {
            std::cerr << "cant open the file: " << filename << std::endl;
            return false;
        }

        infile >> d >> n;
        // intitalize cost matrix
        cost.resize(n*n*n*n); // Mind this might cause overflow when n^4>int.

        // input cost
        for (int i3 = 0; i3 < n; ++i3) {
            for (int i2 = 0; i2 < n; ++i2) {
                for (int i1 = 0; i1 < n; ++i1) {
                    for (int i4 = 0; i4 < n; ++i4) {
                        int index = getIndex(i1, i2, i3, i4, n); // find its index
                        infile >> cost [index];
                        std::cout << "cost[" << i1+1<<i2+1<<i3+1<<i4+1 << "] = " << cost[index] << std::endl;
                    }
                }
            }
        }

        infile.close();
        return true;
    }


    void solve() {
        try {
            GRBEnv env = GRBEnv(true);
            env.start();
            GRBModel model = GRBModel(env);

            std::vector<GRBVar> x(n * n * n * n); // size n^4

            // add variable X(i1, i2, i3, i4): to mark which combo is selected
            for (int i1 = 0; i1 < n; ++i1) {
                for (int i2 = 0; i2 < n; ++i2) {
                    for (int i3 = 0; i3 < n; ++i3) {
                        for (int i4 = 0; i4 < n; ++i4) {
                            int index = getIndex(i1, i2, i3, i4, n);
                            x[index] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY,
                                "x_" + std::to_string(i1+1) + "_" + std::to_string(i2+1) + "_" + std::to_string(i3+1) + "_" + std::to_string(i4+1));
                        }
                    }
                }
            }


            // set object: final cost
            GRBLinExpr objective = 0.0;
            for (int i1 = 0; i1 < n; ++i1) {
                for (int i2 = 0; i2 < n; ++i2) {
                    for (int i3 = 0; i3 < n; ++i3) {
                        for (int i4 = 0; i4 < n; ++i4) {
                            int index = getIndex(i1, i2, i3, i4, n);
                            objective += cost[index] * x[index];
                        }
                    }
                }
            }

            model.setObjective(objective, GRB_MINIMIZE);

            // set constraint for i1: can't be selected twice
            for (int i1 = 0; i1 < n; ++i1) {
                GRBLinExpr expr = 0.0;
                for (int i2 = 0; i2 < n; ++i2) {
                    for (int i3 = 0; i3 < n; ++i3) {
                        for (int i4 = 0; i4 < n; ++i4) {
                            
                            int index = getIndex(i1, i2, i3, i4, n);
                            expr += x[index];  
                        }
                    }
                }
                model.addConstr(expr == 1, "assign_i1_" + std::to_string(i1));
            }

            // set constraint for i2
            for (int i2 = 0; i2 < n; ++i2) {
                GRBLinExpr expr = 0.0;
                for (int i1 = 0; i1 < n; ++i1) {
                    for (int i3 = 0; i3 < n; ++i3) {
                        for (int i4 = 0; i4 < n; ++i4) {
                            int index = getIndex(i1, i2, i3, i4, n);
                            expr += x[index]; 
                        }
                    }
                }
              
                model.addConstr(expr == 1, "assign_i2_" + std::to_string(i2));
            }


            // set constraint for i3
            for (int i3 = 0; i3 < n; ++i3) {
                GRBLinExpr expr = 0.0;
                for (int i1 = 0; i1 < n; ++i1) {
                    for (int i2 = 0; i2 < n; ++i2) {
                        for (int i4 = 0; i4 < n; ++i4) {
                            int index = getIndex(i1, i2, i3, i4, n);
                            expr += x[index]; 
                        }
                    }
                }
                
                model.addConstr(expr == 1, "assign_i3_" + std::to_string(i3));
            }
            
            // set constraint for i4
            for (int i4 = 0; i4 < n; ++i4) {
                GRBLinExpr expr = 0.0;
                for (int i1 = 0; i1 < n; ++i1) {
                    for (int i2 = 0; i2 < n; ++i2) {
                        for (int i3 = 0; i3 < n; ++i3) {
                            int index = getIndex(i1, i2, i3, i4, n);
                            expr += x[index];  
                        }
                    }
                }
           
                model.addConstr(expr == 1, "assign_i4_" + std::to_string(i4));
            }

            model.optimize();

            // output
            if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
                
                std::cout << "Find solution, the minimized cost is: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;
            
                for (int i1 = 0; i1 < n; ++i1) {
                    for (int i2 = 0; i2 < n; ++i2) {
                        for (int i3 = 0; i3 < n; ++i3) {
                            for (int i4 = 0; i4 < n; ++i4) {
                                int index = getIndex(i1, i2, i3, i4, n);
                                double solutionValue = x[index].get(GRB_DoubleAttr_X);
                                if (solutionValue > 0.5) {
                                    double correspondingCost = cost[index];  
                                    std::cout << "x[" << i1+1 << "][" << i2+1 << "][" << i3+1 << "][" << i4+1 << "] = "
                                        << solutionValue << " (cost: " << correspondingCost << ")" << std::endl;
                                }
                            }
                        }
                    }
                }
            }
        
            else {
                std::cout << "No solution" << std::endl;
            }

        }
        catch (GRBException e) {
            std::cerr << "Gurobi error: " << e.getMessage() << std::endl;
        }
        catch (...) {
            std::cerr << "Unknown Error" << std::endl;
        }
    }
};


int main() {
    MultiAssignment assignment;
    assignment.read("multidimensional_assignment.dat");
    assignment.solve();
    return 0;
}
