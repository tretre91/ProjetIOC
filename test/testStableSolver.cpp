#include <stableSolver.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <nom de l'instance>\n";
        return 0;
    }

    StableSolver solver;
    GraphNO g;
    g.importGraphDIMACS(argv[1]);
    solver.init(g);
    g.display();
    std::cout << '\n';

    solver.updateFixed({});
    solver.solve();
    std::cout << "Stable size : " << solver.getSize() << '\n';
    solver.displayBestSolution();
    
    // Decommenter cette partie si test avec custom.col
    // on a une chaine 5 - 6 - 7, si on fixe 6, 5 et 7 ne sont plus inclus dans la solution optimale
    
    /* std::cout << "\nWith vertex 6 included :\n";
    solver.updateFixed({{6u, true}});
    solver.solve();
    std::cout << "Stable size : " << solver.getSize() << '\n';
    solver.displayBestSolution(); */
    
    return 0;
}
