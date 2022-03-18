#ifndef GRAPHNO_HPP
#define GRAPHNO_HPP

#include <vector>
using namespace std;

typedef unsigned int Vertex;

class Graph2 {

private:
   vector<std::pair<Vertex, Vertex>> matrix; //ou passer en vector, où les voisins de vertex sont à l'indice vertex et vertex+1
   Vertex nbVertex;
   vector<int> weights;
   void get_params(char * Preamble);
   void removeEdge(Vertex v1, Vertex v2);

public:
   void initEmptyGraph(Vertex nbVertices);
   bool isEdge(Vertex v, Vertex v1);
   bool hasNoNeighbor(Vertex v1);
   void markingNeighbors(Vertex v1, std::vector<bool> & v);
   void addEdge(Vertex v, Vertex v1);
   void addEdgeFast(Vertex v, Vertex v1, Vertex v2);
   Vertex getNbVertices(){return nbVertex;};
   void deleteGraph(){matrix.clear();nbVertex=0;};
   void importGraphDIMACS( char *file);
   void importWeights(char *file);
   void display();
   Vertex getDegree(Vertex vertex);
   Vertex getDegreeWithUnmarked(Vertex vertex,  const vector<bool> & marked);
   void toComplementGraph();

   std::pair<Vertex, Vertex> getNeighbors(Vertex vertex);

   void intersectNeighbors(Vertex vertex ,  vector<Vertex> & remaining);
};
#endif
