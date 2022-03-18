#include <stableSolver.hpp>
#include <iostream>
#include <fstream>

std::vector<float> importFile(const std::string& filename) {
    std::ifstream file(filename);

    if (file.is_open()) {
        char c;
        size_t size;
        file >> c >> size;
        std::vector<float> values(size);
        for (float& v : values) {
            file >> v;
        }
        return values;
    }
    return {};
}

int main() {
    StableSolver solver;
    solver.solveLine(importFile("C:/Users/Tretr/Desktop/Université/L3-S6/IOC/projet/test/instances/test_chaine.txt"));
    solver.displaySolution();
    return 0;
}
