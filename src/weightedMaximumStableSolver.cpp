#include "weightedMaximumStableSolver.hpp"
#include <iostream>

void WeightedMaximumStableSolver::initSolution() {
    bestCost = 0;
    currentCost = 0;
    currentSolution.clear();
    currentSolution.resize(nbVertex);
    currentSolution.shrink_to_fit();
    bestSolution.clear();
    bestSolution.resize(nbVertex);
    bestSolution.shrink_to_fit();
    for (Vertex row = 0; row < nbVertex; row++)
        currentSolution[row] = 0;
    updateBestSolution();
}

void WeightedMaximumStableSolver::updateBestSolution() {
    if (!checkSolution())
        return;
    if (currentCost > bestCost) {
        bestCost = currentCost;
        bestSolution = currentSolution;
    }
}

bool WeightedMaximumStableSolver::checkSolution() {
    for (Vertex row1 = 0; row1 < nbVertex; row1++) {
        if (!currentSolution[row1])
            continue;
        for (Vertex row2 = row1 + 1; row2 < nbVertex; row2++) {
            if (currentSolution[row2] && graph.isEdge(row1, row2))
                return false;
        }
    }
    float temp = 0;
    for (Vertex r = 0; r < nbVertex; r++) {
        if (currentSolution[r])
            temp += weights[r];
    }
    if (temp != currentCost) {
        std::cout << "WARNING: different value, objective is " << currentCost << " different from the computed sum " << temp << std::endl;
        currentCost = temp;
    }
    return true;
}

void WeightedMaximumStableSolver::displayBestSolution() {
    std::cout << "Stable of weighted cost " << bestCost << " with  vertices : ";
    for(Vertex nod =0; nod < nbVertex; nod++){
        if (bestSolution[nod]) std::cout <<  nod << " ";
    }
    std::cout << std::endl;
}

void WeightedMaximumStableSolver::importGraphDIMACS( char *file){
    graph.importGraphDIMACS(file);
    nbVertex = graph.getNbVertices();
    bestCost=0;
    initSolution();
}
