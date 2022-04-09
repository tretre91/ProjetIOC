#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stableSolver.hpp>

namespace fs = std::filesystem;

static constexpr char sep = ',';

template<typename Function>
void bench(std::ofstream& csv, StableSolver& solver, Function function, int nb_iter = 5) {
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;
    using duration = std::chrono::duration<double>;

    double total_cost = 0;
    unsigned int total_size = 0;
    double total_time = 0.0;

    time_point start, end;

    for (int i = 0; i < nb_iter; i++) {
        start = clock::now();
        function(solver);
        end = clock::now();
        total_cost += solver.getCost();
        total_size += solver.getSize();
        total_time += duration(end - start).count();
    }

    csv << sep << (total_cost / nb_iter) << sep << (total_size / (double)nb_iter) << sep << (total_time / nb_iter);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <dossier d'instances> [fichier de sortie]\n";
        return 1;
    }

    fs::path instances_folder(argv[1]);
    if (!fs::exists(instances_folder) || !fs::is_directory(instances_folder)) {
        std::cerr << "Le dossier d'instance " << instances_folder.string() << " n'existe pas\n";
        return 1;
    }

    const char* filename = argc > 2 ? argv[2] : "out.csv";
    std::ofstream csv(filename);
    if (!csv.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier " << filename << '\n';
        return 1;
    }

    // nom, nb sommets, nb arêtes, densité, puis pour chaque solveur poids, taille du stable et temps d'exec
    // clang-format off
    csv << "name" << sep << "vertices" << sep << "edges" << sep << "density" << sep
        << "cost_0.1" << sep << "size_0.1" << sep << "time_0.1" << sep
        << "cost_1" << sep << "size_1" << sep << "time_1" << sep
        << "cost_0" << sep << "size_0" << sep << "time_0" << sep
        << "cost_greedy" << sep << "size_greedy" << sep << "time_greedy\n";
    // clang-format on

    StableSolver solver;

    for (const auto& entry : fs::directory_iterator(instances_folder)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        auto [nbVertex, nbEdges] = solver.importWCol(entry.path().string());
        if (nbVertex > 0) {
            csv << entry.path().filename().string() << sep << nbVertex << sep << nbEdges << sep << (nbEdges / (double)nbVertex);
            bench(csv, solver, [](StableSolver& solver) { solver.solveRandomizedHeuristic(0.1f); });
            bench(csv, solver, [](StableSolver& solver) { solver.solveRandomizedHeuristic(1.0f); });
            bench(csv, solver, [](StableSolver& solver) { solver.solveRandomizedHeuristic(0.0f); });
            bench(csv, solver, [](StableSolver& solver) { solver.solveGreedy(); }, 1);
            csv << '\n';
        }
    }

    csv.close();
}
