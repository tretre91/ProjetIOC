#ifndef STABLE_SOLVER_HPP
#define STABLE_SOLVER_HPP

#include <vector>
#include <utility>

typedef unsigned int Vertex;

struct Node {
    Vertex id = 0;
    std::pair<Vertex, Vertex> p = { 0, 0 };
};

class StableSolver
{
private:
    float currentCost;
    std::vector<bool> currentSolution;
    float bestCost;
    std::vector<bool> bestSolution;

    inline void resetSolution(unsigned int nbVertex) {
        currentCost = 0.0f;
        currentSolution.resize(nbVertex, false);
    }

    void solveLine(const std::vector<Node>& component, const std::vector<float>& weights);
    void solveCycle(const std::vector<Node>& component, const std::vector<float>& weights);

public:
    void displaySolution() const;

    void solveLine(const std::vector<float>& weights) {
        resetSolution(weights.size());
        std::vector<Node> nodes(weights.size());
        solveCycle(nodes, weights);
    }

    void solve(const std::vector<Node>& component) {}

    float getCurrentCost() const {
        return currentCost;
    }

    float getBestCost() const {
        return bestCost;
    }
};

#endif // !STABLE_SOLVER_HPP
