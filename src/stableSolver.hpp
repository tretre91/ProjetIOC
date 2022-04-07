#ifndef STABLE_SOLVER_HPP
#define STABLE_SOLVER_HPP

#include "graph2.hpp"
#include "weightedMaximumStableSolver.hpp"
#include <string>
#include <utility>
#include <vector>

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
    void updateFixed(const vector<std::pair<Vertex, bool>>& list);

    void init(GraphNO& graph);

    void solve();

    void solveRandomizedHeuristic(float fix_probability);

    float getCost() const {
        return bestCost;
    }

    unsigned int getSize() const {
        return currentSize;
    }

    /**
     * @brief Importe un graphe DIMACS .wcol et renvoie le nombre de sommets et d'ar�tes (ou {0, 0} en cas d'erreur)
     */
    std::pair<unsigned, unsigned> importWCol(const std::string& filename);
};

#endif // !STABLE_SOLVER_HPP
