#ifndef GRAPH2_HPP
#define GRAPH2_HPP

#include <vector>
#include "graphNO.hpp"
using namespace std;

typedef unsigned int Vertex;

struct node {
    int id;
    std::pair<Vertex, Vertex> neighbors;
};

enum class typeGraphe
{
    SOLO,
    CHAINE,
    CYCLE
};

struct composanteConnexe {
    int size;
    std::vector<node> neighbors;
    std::vector<float> weights;
    typeGraphe type;
};

class Graph2 {

private:
   vector<std::pair<Vertex, Vertex>> matrix; //ou passer en vector, où les voisins de vertex sont à l'indice vertex et vertex+1
   Vertex nbVertex;
   vector<float> weights;
   void get_params(char * Preamble);
   void removeEdge(Vertex v1, Vertex v2);

public:
   void toDegree2(const vector<std::pair<Vertex, bool>>& list, GraphNO& g);
    std::vector<vector<pair<Vertex, Vertex>>> getComponents() {}
   void initEmptyGraph(Vertex nbVertices);
   bool isEdge(Vertex v, Vertex v1);
   bool hasNoNeighbor(Vertex v1);
   void addEdge(Vertex v, Vertex v1);
   void addEdgeFast(Vertex v, Vertex v1, Vertex v2);
   Vertex getNbVertices(){return nbVertex;};
   void deleteGraph(){matrix.clear();nbVertex=0;};
   void importGraphDIMACS( char *file);
   //void importWeights(char *file);
   void display();
   Vertex getDegree(Vertex vertex);
   vector<float> getWeights() {
       return weights;
   }

   std::vector<composanteConnexe> decoupeCompConnexe();
   void displayCompConnexe();

   //void toComplementGraph();

   std::pair<Vertex, Vertex> getNeighbors(Vertex vertex);

   void intersectNeighbors(Vertex vertex ,  vector<Vertex> & remaining);
};
#endif
