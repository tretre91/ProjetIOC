#ifndef STABLE_SOLVER_HPP
#define STABLE_SOLVER_HPP

#include <utility>
#include <vector>
#include "graph2.hpp"

typedef unsigned int Vertex;

class StableSolver
{
private:
    float currentCost = 0;
    std::vector<bool> currentSolution;
    float bestCost = 0;
    std::vector<bool> bestSolution;
    Graph2 graph;

    inline void resetSolution(unsigned int nbVertex) {
        currentCost = 0.0f;
        currentSolution.resize(nbVertex);
        for (size_t i = 0; i < currentSolution.size(); i++) {
            currentSolution[i] = false;
        }
    }

    void solveComp(const composanteConnexe& comp);
    void solveLine(const std::vector<float>& weights);
    void solveCycle(const std::vector<float>& weights);

public:
    void displaySolution() const;

    void init(GraphNO& graph, const std::vector<std::pair<Vertex, bool>>& list);

    float solve();

    float getCurrentCost() const {
        return currentCost;
    }

    float getBestCost() const {
        return bestCost;
    }
   
};

#endif // !STABLE_SOLVER_HPP
