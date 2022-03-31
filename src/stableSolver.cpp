#include "stableSolver.hpp"
#include <algorithm>
#include <iostream>

void StableSolver::displaySolution() const {
    std::cout << "Selected vertices:";
    for (size_t i = 0; i < currentSolution.size(); i++) {
        if (currentSolution[i]) {
            std::cout << ' ' << i;
        }
    }
    std::cout << "\nTotal weigth: " << currentCost << '\n';
}

void StableSolver::init(GraphNO& g, const std::vector<std::pair<Vertex, bool>>& list) {
    graph.toDegree2(list, g);

}

float StableSolver::solve() {
    bestCost = 0;
    for (auto& comp : graph.decoupeCompConnexe2()) {
        solveComp(comp);
        bestCost += currentCost;
    }
    return bestCost;
}

void StableSolver::solveComp(const composanteConnexe& comp) {
    switch (comp.type) {
    case typeGraphe::SOLO:
        currentCost = comp.weights[0];
        break;
    case typeGraphe::CHAINE:
        solveLine(comp.weights);
        break;
    case typeGraphe::CYCLE:
        solveCycle(comp.weights);
        break;
    }
}

void StableSolver::solveLine(const std::vector<float>& weights) {
    resetSolution(weights.size());
    std::vector<float> m_true(weights.size(), 0.0f);
    std::vector<float> m_false(weights.size(), 0.0f);

    m_true[0] = weights[0];
    m_false[0] = 0.0f; // TODO : inutile

    for (size_t i = 1; i < weights.size(); i++) {
        m_true[i] = weights[i] + m_false[i - 1];
        m_false[i] = std::max(m_true[i - 1], m_false[i - 1]);
    }

    currentCost = std::max(m_true.back(), m_false.back());

    for (int i = weights.size() - 1; i >= 0;) {
        if (m_true[i] >= m_false[i]) {
            currentSolution[i] = true;
            i -= 2;
        } else {
            i--;
        }
    }
}

struct Info {
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

void StableSolver::solveCycle(const std::vector<float>& weights) {
    resetSolution(weights.size());
    std::vector<Info> m(weights.size());

    m[0](true, false) = std::numeric_limits<float>::lowest();
    m[0](false, true) = std::numeric_limits<float>::lowest();
    m[0](true, true) = weights[0];
    m[0](false, false) = 0.0f;

    m[1](true, true) = std::numeric_limits<float>::lowest();
    m[1](true, false) = weights[0];
    m[1](false, true) = weights[1];
    m[1](false, false) = 0.0f;

    for (size_t i = 2; i < m.size(); i++) {
        m[i](false, true) = weights[i] + m[i - 1](false, false);
        m[i](true, true) = weights[i] + m[i - 1](true, false);
        m[i](false, false) = std::max(m[i - 1](false, true), m[i - 1](false, false));
        m[i](true, false) = std::max(m[i - 1](true, true), m[i - 1](true, false));
    }

    Info& last = m.back();
    last(true, true) = std::numeric_limits<float>::lowest();
    currentCost = std::max({last(false, false), last(false, true), last(true, false)});

    const bool take_first = currentCost == last(true, false);

    for (int i = m.size() - 1; i >= 0;) {
        if (m[i](take_first, true) >= m[i](take_first, false)) {
            currentSolution[i] = true;
            i -= 2;
        } else {
            i--;
        }
    }
}
