#include "graph2.hpp"
#include <stdlib.h>
#include <iostream>
#include <string.h>

void Graph2::initEmptyGraph(Vertex nbVertices) {
   nbVertex=nbVertices;
   matrix.clear();
   matrix.resize(nbVertex);
   matrix.shrink_to_fit();
   weights.clear();
   weights.resize(nbVertex);
   weights.shrink_to_fit();
}

bool Graph2::isEdge(Vertex v1, Vertex v2){
	return matrix[v1].first == v2 || matrix[v1].second == v2;
}

void Graph2::addEdge(Vertex v, Vertex v1){
	if(matrix[v].first == 4294967295){
		matrix[v].first = v1;
	}else{
		matrix[v].second = v1;
	}
}

void Graph2::addEdgeFast(Vertex v, Vertex v1, Vertex v2){
	matrix[v] = make_pair(v1,v2);
}

void Graph2::removeEdge(Vertex v1, Vertex v2){
	 if(matrix[v1].first == v2){
		 matrix[v1].first = 4294967295;
	 }else if(matrix[v1].second == v2){
		 matrix[v1].second = 4294967295;
	 }
}

bool Graph2::hasNoNeighbor(Vertex v1){
	return matrix[v1].first == 4294967295 && matrix[v1].second == 4294967295;
}

void Graph2::markingNeighbors(Vertex v1, std::vector<bool> &v) {
	for(Vertex row =0; row < nbVertex; row++){
		if (!v[row] && isEdge(row,v1)) v[row]=true;
	}
	v[v1]=true;
}


void Graph2::toComplementGraph() {
   for(Vertex row =0; row < nbVertex; row++){
      for(Vertex col =row+1; col < nbVertex; col++){
        if (isEdge(row,col)) removeEdge(row,col);
        else addEdge(row,col);
      }
      std::cout << std::endl;
   }

}

void Graph2::display() {
	cout << endl;
	for(int vertex = 0; vertex < nbVertex; vertex++){
		cout << "Sommet " << vertex << " : "<< endl;
		int val = 0;
		int v1;
		int v2;
		if (matrix[vertex].first != 4294967295) {val++; v1 = matrix[vertex].first;}else{v1 = -1;}
		if (matrix[vertex].second != 4294967295) {val++; v2 = matrix[vertex].first;}else{v2 = -1;}
		cout << v1 << " & " << v2 << " -> " << val << " | " << "Weight : " << weights[vertex] << endl;
	}
	cout << endl;
}

//Même chose que getDegree, mais en arrêtant de chercher dès qu'on a 2 voisins
Vertex Graph2::getDegree(Vertex vertex){
	int val = 0;
	if (matrix[vertex].first != 4294967295) {val++;}
	if (matrix[vertex].second != 4294967295) {val++;}
	return val;
}

Vertex Graph2::getDegreeWithUnmarked(Vertex vertex,	const std::vector<bool> &marked) {
	   Vertex deg=0;
	   for(Vertex row =0; row < nbVertex; row++){
	      if ((!marked[row] )&&isEdge(row,vertex)) deg++;
	   }
	   return deg;

}

std::pair<Vertex, Vertex> Graph2::getNeighbors(Vertex vertex){
	return matrix[vertex];
}

void Graph2::intersectNeighbors(Vertex vertex, vector<Vertex> &remaining) {
	vector<Vertex> v;
	v.reserve(remaining.size());

	for (std::vector<Vertex>::iterator it = remaining.begin() ; it != remaining.end(); ++it)
		if (isEdge(vertex,*it)) v.push_back(*it);

	v.shrink_to_fit();
	remaining.resize(v.size());
	remaining=v;
	remaining.shrink_to_fit();

}

void Graph2::get_params( char * Preamble)
                      /* getting Nr_vert and Nr_edge from the preamble string, 
						 containing Dimacs format "p ??? num num" */
{

	char c, *tmp;
	char * pp = Preamble;
	int stop = 0;
	tmp = (char *)calloc(100, sizeof(char));
	int Nr_vert, Nr_edges;
	Nr_vert = 0;
	
	while (!stop && (c = *pp++) != '\0'){
		switch (c)
		  {
			case 'c':
			  while ((c = *pp++) != '\n' && c != '\0');
			  break;
			  
			case 'p':
			  sscanf(pp, "%s %d %d\n", tmp, &Nr_vert, &Nr_edges);
			  stop = 1;
			  break;
			  
			default:
			  break;
		  }
	}
	
	free(tmp);
	
	std::cout << "import of DIMACS graph with " << Nr_vert << " vertices and " << Nr_edges  << " edges"<< std::endl;
	nbVertex = Nr_vert;

	
}

void Graph2::importGraphDIMACS( char *file)
{

	int MAX_PREAMBLE = 4000;
	char* Preamble = new char [MAX_PREAMBLE];
	int c, oc;
	char * pp = Preamble;
	int i,j;
	float k;
	FILE *fp;
	
	if ( (fp=fopen(file,"r"))==NULL )
	  { printf("ERROR: Cannot open infile\n"); exit(10); }

	for(oc = '\0' ;(c = fgetc(fp)) != EOF && (oc != '\n' || c != 'e')
		; oc = *pp++ = c); //?? 
 
	ungetc(c, fp); 
	*pp = '\0';
	get_params(Preamble);
	
	initEmptyGraph(nbVertex);

	int n = 0; //
	while ((c = fgetc(fp)) != EOF){
		switch (c)
		  {
			case 'e':
				if (!fscanf(fp, "%d %d %f", &i, &j, &k))
					{ printf("ERROR: corrupted inputfile\n"); exit(10); }
				//addEdge(i-1, j-1);
				//if( (i, j) != (0, 0)){ //jsp exactement ce qui fait ça mais tant pis
					addEdgeFast(n, i, j);
					weights[n] = k;
					n++; //
				//}
			  break;
			  
			case '\n':
			  
			default:
			  break;
		  }
	}
	/* */
	fclose(fp);
	delete[] Preamble;
	
	std::cout << "end of import of DIMACS graph with " << nbVertex << " vertices"<< std::endl;
	



}
