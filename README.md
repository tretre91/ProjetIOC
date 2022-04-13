# ProjetIOC

Code pour le projet d'IOC
Auteurs : Brice Pointal, Trévis Morvany

## Structure

- src : dossier contanant lee classes utilisées
- test : dossier contenant les instances et les fichiers sources de test / benchmark
- csv : dossier contenant les [tableaux de résultats](csv/randomizedHeuristic2.csv)

La classe `Graph2` ([.hpp](src/graph2.hpp), [.cpp](src/graph2.cpp)) contient l'implémentation des graphes de degré 2, ainsi que le découpage de composante connexe

La classe `StableSolver` ([.hpp](src/stableSolver.hpp), [.cpp](src/stableSolver.cpp)) contient les méthodes de résolution (résolution exacte en degré 2, heuristique degré n)

Le fichier [benchmark.cpp](test/benchmark.cpp) contient le code utilisé pour genérer le fichier csv

Pour compiler utiliser cmake
```bash
cmake -S . -B build
cd build
cmake --build . --target all # ou juste 'make' si sous linux
```

### cibles

- `testStableSolver` : test du solveur degré 2 (prend en argument l'instance à tester)
- `testSolver` : test du solveur degré n (prend en argument l'instance ponderée à tester)
- `benchmark` : test de l'heuristique sur toutes les instances pondérées d'un dossier spécifié (prend en argument le dossier d'intances, et optionnelement le nom du fichier de sortie)
