#ifndef STABLE_SOLVER_HPP
#define STABLE_SOLVER_HPP

#include <utility>
#include <vector>
#include "graph2.hpp"
#include "weightedMaximumStableSolver.hpp"
#include <string>

typedef unsigned int Vertex;

class StableSolver : public WeightedMaximumStableSolver
{
private:
    unsigned int currentSize;
    Graph2 graph2;

    void solveComp(const composanteConnexe& comp);
    void solveLine(const composanteConnexe& comp);
    void solveCycle(const composanteConnexe& comp);

    void solveRandomizedHeuristic(float fix_probability);
    std::pair<unsigned, unsigned> importWCol(const std::string& filename);

public:
    void updateFixed(const vector<std::pair<Vertex, bool>>& list);

    void init(GraphNO& graph);

    void solve();

    float getCost() const {
        return bestCost;
    }

    Graph2 getGraph2(){ return graph2;}

    unsigned int getSize() const {
        return currentSize;
    }
};

#endif // !STABLE_SOLVER_HPP
