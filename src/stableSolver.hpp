#ifndef STABLE_SOLVER_HPP
#define STABLE_SOLVER_HPP

#include <utility>
#include <vector>
#include "graph2.hpp"

typedef unsigned int Vertex;

class StableSolver
{
private:
    float currentCost;
    std::vector<bool> currentSolution;
    float bestCost;
    std::vector<bool> bestSolution;
    Graph2 graph;

    inline void resetSolution(unsigned int nbVertex) {
        currentCost = 0.0f;
        currentSolution.resize(nbVertex, false);
    }

    void solveComp(const composanteConnexe& comp);
    void solveLine(const std::vector<float>& weights);
    void solveCycle(const std::vector<float>& weights);

public:
    void displaySolution() const;

    float solve();

    float getCurrentCost() const {
        return currentCost;
    }

    float getBestCost() const {
        return bestCost;
    }
   
};

#endif // !STABLE_SOLVER_HPP
