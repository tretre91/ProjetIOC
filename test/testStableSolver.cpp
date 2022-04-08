#include <stableSolver.hpp>
#include <iostream>

int main() {
	StableSolver solver;
	GraphNO g;

	g.initEmptyGraph(6);
	g.addEdge(0, 1);
	g.addEdge(1, 2);
	g.addEdge(2, 3);
	g.addEdge(3, 4);
	g.addEdge(4, 5);
	g.addEdge(5, 0);
	g.addEdge(0, 3);

	solver.init(g, { false, true, true, true, true, true });

	g.display();
	std::cout << '\n';

	solver.solve();
	std::cout << "Stable size : " << solver.getSize() << '\n';
	solver.displayBestSolution();

	return 0;
}
