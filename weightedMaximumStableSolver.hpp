#ifndef WEIGHTEDMAXIMUMSTABLESOLVER_HPP
#define WEIGHTEDMAXIMUMSTABLESOLVER_HPP

#include "graph2.hpp"
using namespace std;

class WeightedMaximumStableSolver {

private:
   Graph2 graph;
   Vertex nbVertex;
   //unsigned int currentCost;
   std::vector<float> weights;
   float currentCost;
   std::vector<bool> currentSolution;
   float bestCost;
   std::vector<bool> bestSolution;

   void initSolution();

protected:
   bool checkSolution();
   void updateBestSolution();

   enum class typeGraphe {SOLO, CHAINE, CYCLE};
   
public:
   Vertex getNbVertex() const {	return nbVertex;}

   struct node{
      int id;
      //float weight;
      std::pair<Vertex, Vertex> neighbors;
   };

   struct composanteConnexe{
      int size;
      vector<node> neighbors;
      //ou bien mettre un vector<float> weights; ici
      typeGraphe type;
   };

   vector<composanteConnexe> decoupeCompConnexe();
   void displayCompConnexe();

   void importGraphDIMACS( char *file);
   void importWeights( char *file);
   void displayGraph(){graph.display();};
   void displayBestSolution();
   void solvesmthing();
};
#endif
