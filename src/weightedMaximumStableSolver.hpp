#ifndef WEIGHTEDMAXIMUMSTABLESOLVER_HPP
#define WEIGHTEDMAXIMUMSTABLESOLVER_HPP

#include "graphNO.hpp"
using namespace std;

class WeightedMaximumStableSolver
{
protected:
    GraphNO graph;
    Vertex nbVertex;
    std::vector<float> weights;
    float currentCost;
    std::vector<bool> currentSolution;
    float bestCost;
    std::vector<bool> bestSolution;

    void initSolution();

protected:
    bool checkSolution();
    void updateBestSolution();

public:
    void importGraphDIMACS(char* file);
    void importWeights(char* file);
    void displayBestSolution();
};
#endif
