#ifndef GRAPH2_HPP
#define GRAPH2_HPP

#include "graphNO.hpp"
#include <vector>
using namespace std;

typedef unsigned int Vertex;

struct node {
    Vertex id;
    std::pair<Vertex, Vertex> neighbors;
};

enum class typeGraphe
{
    SOLO,
    CHAINE,
    CYCLE
};

struct composanteConnexe {
    unsigned int size = 0;
    std::vector<node> neighbors;
    typeGraphe type = typeGraphe::SOLO;

    composanteConnexe() = default;
    composanteConnexe(typeGraphe type) : type(type), size(0) {}
    composanteConnexe(typeGraphe type, const node& first) : type(type), size(1), neighbors({first}){};
};

class Graph2
{
private:
    vector<std::pair<Vertex, Vertex>> matrix; // ou passer en vector, où les voisins de vertex sont à l'indice vertex et vertex+1
    Vertex nbVertex;
    void get_params(char* Preamble);
    void removeEdge(Vertex v1, Vertex v2);
    vector<float> weights;

    template<typename Function>
    composanteConnexe explore(typeGraphe type, std::vector<bool>& explored, const node& first, Function stopCondition);

public:
    void toDegree2(GraphNO& g);
    void toDegree2General(GraphNO& g);
    void selectRandomNeighborsGNO(GraphNO& g, Vertex v);
    void toDegree2Alea(GraphNO& g);
    void toDegree2Max(GraphNO& g);
    void sortByRatio(vector<std::pair<Vertex, float>> &list);
    void initEmptyGraph(Vertex nbVertices);
    bool isEdge(Vertex v, Vertex v1);
    bool hasNoNeighbor(Vertex v1);
    void addEdge(Vertex v, Vertex v1);
    void addEdgeFast(Vertex v, Vertex v1, Vertex v2);
    void addEdgeBi(Vertex v1, Vertex v2);
    Vertex getNbVertices() {
        return nbVertex;
    };
    vector<float> getWeights(){return weights;}
    void deleteGraph() {
        matrix.clear();
        nbVertex = 0;
    };
    void importGraphDIMACS(char* file);
    // void importWeights(char *file);
    void displayWithDegree();
    void display2();
    Vertex getDegree(Vertex vertex);

    std::vector<composanteConnexe> decoupeCompConnexe();
    std::vector<composanteConnexe> decoupeCompConnexe2();
    void displayCompConnexe();

    // void toComplementGraph();

    std::pair<Vertex, Vertex> getNeighbors(Vertex vertex);

    void intersectNeighbors(Vertex vertex, vector<Vertex>& remaining);
};

template<typename Function>
composanteConnexe Graph2::explore(typeGraphe type, std::vector<bool>& explored, const node& first, Function stopCondition) {
    composanteConnexe comp(type);
    auto& nodes = comp.neighbors;

    node current = first;
    Vertex next = current.neighbors.first;
    Vertex prev;

    do {
        explored[current.id] = true;
        nodes.push_back(current);
        prev = current.id;
        current.id = next;
        current.neighbors = getNeighbors(current.id);
        next = current.neighbors.first != prev ? current.neighbors.first : current.neighbors.second;
    } while (!stopCondition(current));

    if (type == typeGraphe::CHAINE) { // TODO
        explored[current.id] = true;
        nodes.push_back(current);
    }
    comp.size = comp.neighbors.size();

    return comp;
}

#endif
