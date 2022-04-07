#include <chrono>
#include <iostream>
#include <stableSolver.hpp>

int main(int argc, char* argv[]) {
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;
    using duration = std::chrono::duration<double>;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <instance file>\n";
        return 0;
    }

    StableSolver solver;
    solver.importWCol(argv[1]);

    for (int i = 0; i < 10; i++) {
        time_point start = clock::now();
        solver.solveRandomizedHeuristic(0.1f);
        time_point end = clock::now();

        std::cout << "Time elapsed: " << duration(end - start).count() << "s\n";
        std::cout << "Cost: " << solver.getCost() << '\n';
        std::cout << "Size: " << solver.getSize() << '\n';
    }
}
