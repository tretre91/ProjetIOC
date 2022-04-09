#include "stableSolver.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>

void StableSolver::init(GraphNO& g) {
    graph = g; // TODO
    graph2.toDegree2(g);
    nbVertex = graph2.getNbVertices();
}

void StableSolver::init(GraphNO& g, const std::vector<bool>& marked) {
    graph = g;
    graph2.toDegree2(g, marked);
    nbVertex = graph2.getNbVertices();
    weights.clear();
    weights.resize(nbVertex, 1.0f);
}

void StableSolver::solve() {
    initSolution();
    currentSize = 0;
    for (auto& comp : graph2.decoupeCompConnexe2()) {
        solveComp(comp);
    }

    updateBestSolution();
}

void StableSolver::solveComp(const composanteConnexe& comp) {
    switch (comp.type) {
    case typeGraphe::SOLO:
        currentCost += weights[comp.neighbors[0].id];
        currentSolution[comp.neighbors[0].id] = true;
        currentSize++;
        break;
    case typeGraphe::CHAINE:
        solveLine(comp);
        break;
    case typeGraphe::CYCLE:
        solveCycle(comp);
        break;
    }
}

void StableSolver::solveLine(const composanteConnexe& comp) {
    std::vector<float> m_true(comp.size, 0.0f);
    std::vector<float> m_false(comp.size, 0.0f);

    m_true[0] = weights[comp.neighbors[0].id];
    m_false[0] = 0.0f; // TODO : inutile

    for (size_t i = 1; i < comp.size; i++) {
        m_true[i] = weights[comp.neighbors[i].id] + m_false[i - 1];
        m_false[i] = std::max(m_true[i - 1], m_false[i - 1]);
    }

    currentCost += std::max(m_true.back(), m_false.back());

    for (int i = comp.size - 1; i >= 0;) {
        if (m_true[i] >= m_false[i]) {
            currentSolution[comp.neighbors[i].id] = true;
            currentSize++;
            i -= 2;
        } else {
            i--;
        }
    }
}

struct Info { // TODO : trouver un nom plus expressif
private:
    float values[4] = {
      0.0f, // M faux faux
      0.0f, // M faux vrai
      0.0f, // M vrai faux
      0.0f  // M vrai vrai
    };

public:
    float operator()(bool take_first, bool take_current) const {
        return values[2 * take_first + take_current];
    }

    float& operator()(bool take_first, bool take_current) {
        return values[2 * take_first + take_current];
    }
};

void StableSolver::solveCycle(const composanteConnexe& comp) {
    std::vector<Info> m(comp.size);

    m[0](true, false) = std::numeric_limits<float>::lowest();
    m[0](false, true) = std::numeric_limits<float>::lowest();
    m[0](true, true) = weights[comp.neighbors[0].id];
    m[0](false, false) = 0.0f;

    m[1](true, true) = std::numeric_limits<float>::lowest();
    m[1](true, false) = weights[comp.neighbors[0].id];
    m[1](false, true) = weights[comp.neighbors[1].id];
    m[1](false, false) = 0.0f;

    for (size_t i = 2; i < m.size(); i++) {
        m[i](false, true) = weights[comp.neighbors[i].id] + m[i - 1](false, false);
        m[i](true, true) = weights[comp.neighbors[i].id] + m[i - 1](true, false);
        m[i](false, false) = std::max(m[i - 1](false, true), m[i - 1](false, false));
        m[i](true, false) = std::max(m[i - 1](true, true), m[i - 1](true, false));
    }

    Info& last = m.back();
    last(true, true) = std::numeric_limits<float>::lowest(); // le 1er et le dernier sommet sont voisins donc impossible

    const float comp_weight = std::max({last(false, false), last(false, true), last(true, false)});
    currentCost += comp_weight;

    const bool take_first = comp_weight == last(true, false);

    for (int i = m.size() - 1; i >= 0;) {
        if (m[i](take_first, true) >= m[i](take_first, false)) {
            currentSolution[comp.neighbors[i].id] = true;
            currentSize++;
            i -= 2;
        } else {
            i--;
        }
    }
}

void StableSolver::solveGreedy() {
    initSolution();
    std::vector<std::pair<Vertex, Vertex>> vertices(nbVertex);

    for (Vertex v = 0; v < vertices.size(); v++) {
        vertices[v] = std::make_pair(v, graph.getDegree(v));
    }

    auto compare = [&](const auto& p1, const auto& p2) { return p1.second < p2.second || (p1.second == p2.second && weights[p1.first] > weights[p2.first]); };

    std::sort(vertices.begin(), vertices.end(), compare);

    std::vector<Vertex> stable;
    for (pair<Vertex, Vertex>& p : vertices) {
        if (std::none_of(stable.begin(), stable.end(), [&](Vertex v) { return graph.isEdge(v, p.first); })) {
            stable.push_back(p.first);
            currentSolution[p.first] = true;
            currentCost += weights[p.first];
        }
    }

    currentSize = stable.size();
    updateBestSolution();
}

void StableSolver::solveRandomizedHeuristic(float fix_probability) {
    std::random_device dev;
    std::mt19937 engine(dev());
    std::uniform_int_distribution<unsigned int> random(0, 100);
    const unsigned int threshold = std::lroundf(fix_probability * 100);

    std::vector<std::pair<Vertex, Vertex>> vertices(nbVertex);

    // décrémente le degré de tous les voisins d'un sommet
    auto remove_vertex = [&](Vertex v) {
        for (auto& vertex : vertices) {
            if (graph.isEdge(v, vertex.first)) {
                vertex.second--;
            }
        }
    };

    for (Vertex v = 0; v < vertices.size(); v++) {
        vertices[v] = std::make_pair(v, graph.getDegree(v));
    }

    // fonction de tri par degré croissant, et poids décroissant
    auto compare = [&](const auto& p1, const auto& p2) { return p1.second < p2.second || (p1.second == p2.second && weights[p1.first] > weights[p2.first]); };

    std::sort(vertices.begin(), vertices.end(), compare);
    // itérateur qui pointe juste après la fin de la zone des sommets gardés
    auto begin = vertices.begin();
    std::vector<Vertex> to_remove;

    while (vertices.back().second > 2) {
        if (random(engine) < threshold) { // cas constructif
            to_remove.clear();
            to_remove.reserve(begin->second);

            auto end = std::remove_if(begin + 1, vertices.end(), [&, kept = begin->first](const std::pair<Vertex, Vertex>& v) {
                if (graph.isEdge(v.first, kept)) {
                    to_remove.push_back(v.first);
                    return true;
                }
                return false;
            });

            vertices.erase(end, vertices.end());

            for (Vertex v : to_remove) {
                remove_vertex(v);
            }

            begin++;
        } else { // cas destructif
            Vertex removed = vertices.back().first;
            vertices.pop_back();
            remove_vertex(removed);
        }

        std::sort(vertices.begin(), vertices.end(), compare);
    }

    // on crée un GraphNO qui ne contiendra que les sommets et arêtes en degré 2
    GraphNO reduced_graph;
    reduced_graph.initEmptyGraph(vertices.size());
    std::vector<float> reduced_weights;
    reduced_weights.reserve(vertices.size());

    // index du début des sommets non fixés
    size_t offset = std::distance(vertices.begin(), begin);

    for (size_t i = 0; i < vertices.size(); i++) {
        reduced_weights.push_back(weights[vertices[i].first]);
        if (i >= offset) { // les sommets fixés ont un degré 0, on économise quelques itérations
            for (size_t j = offset; j < vertices.size(); j++) {
                if (graph.isEdge(vertices[i].first, vertices[j].first)) {
                    reduced_graph.addEdge(i, j);
                }
            }
        }
    }

    // on resout sur le graphe réduit
    std::swap(graph, reduced_graph);
    std::swap(weights, reduced_weights);
    graph2.toDegree2(graph);
    nbVertex = graph2.getNbVertices();

    solve();
    std::vector<bool> solution = currentSolution;

    // on remet les variables à leur valeur de base
    std::swap(graph, reduced_graph);
    std::swap(weights, reduced_weights);
    nbVertex = graph.getNbVertices();

    // on met à jour la "vraie" solution
    currentSolution.clear();
    currentSolution.resize(nbVertex);
    for (size_t i = 0; i < solution.size(); i++) {
        if (solution[i]) {
            currentSolution[vertices[i].first] = true;
        }
    }
    checkSolution();
}

std::pair<unsigned, unsigned> StableSolver::importWCol(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "ERROR: Failed to open instance file " << filename << '\n';
        return make_pair(0, 0);
    }

    char line_type;
    std::string tmp;

    nbVertex = 0;
    unsigned int nbEdges = 0;
    unsigned int index = -1;
    Vertex v1, v2;

    while (file >> line_type) {
        switch (line_type) {
        case 'c': // comment
            file.ignore();
            std::getline(file, tmp);
            break;
        case 'p': // preamble
            if (nbVertex != 0) {
                std::cerr << "The instance file " << filename << " is ill-formed\n";
                return std::make_pair(0, 0);
            }
            file >> tmp >> nbVertex >> nbEdges;
            std::cout << "Importing DIMACS graph with " << nbVertex << " vertices and " << nbEdges << " edges\n";
            graph.initEmptyGraph(nbVertex);
            weights.resize(nbVertex);
            break;
        case 'v': // weight line
            file >> index;
            file >> weights[index - 1];
            break;
        case 'e': // edge line
            file >> v1 >> v2;
            graph.addEdge(v1 - 1, v2 - 1);
            break;
        default:
            std::cerr << "WARNING: unknown line type '" << line_type << "'\n";
            file.ignore();
            std::getline(file, tmp);
            std::cerr << "Ignored line '" << tmp << "'\n";
            break;
        }
    }

    std::cout << "End of import\n";
    file.close();
    return std::make_pair(nbVertex, nbEdges);
}