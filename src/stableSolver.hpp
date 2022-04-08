#ifndef STABLE_SOLVER_HPP
#define STABLE_SOLVER_HPP

#include <utility>
#include <vector>
#include "graph2.hpp"
#include "weightedMaximumStableSolver.hpp"

typedef unsigned int Vertex;

class StableSolver : public WeightedMaximumStableSolver
{
private:
    unsigned int currentSize;
    Graph2 graph2;

    void solveComp(const composanteConnexe& comp);
    void solveLine(const composanteConnexe& comp);
    void solveCycle(const composanteConnexe& comp);

public:
    void init(GraphNO& graph, const std::vector<bool>& marked);

    void solve();

    float getCost() const {
        return bestCost;
    }

    unsigned int getSize() const {
        return currentSize;
    }
   
};

#endif // !STABLE_SOLVER_HPP
