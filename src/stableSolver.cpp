#include "stableSolver.hpp"
#include <algorithm>

void StableSolver::updateFixed(const vector<std::pair<Vertex, bool>>& list) {
    weights.clear();
    weights.resize(nbVertex, 1.0f);
    for (const pair<Vertex, bool>& p : list) {
        weights[p.first] = p.second ? 10.f : -10.f;
    }
}

void StableSolver::init(GraphNO& g) {
    graph = g; // TODO
    graph2.toDegree2(g);
    nbVertex = graph2.getNbVertices();
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