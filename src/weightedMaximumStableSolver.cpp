#include "weightedMaximumStableSolver.hpp"
#include <iostream>

void WeightedMaximumStableSolver::initSolution() {
    bestCost = 0;
    currentCost = 0;
    currentSolution.clear();
    currentSolution.resize(nbVertex);
    currentSolution.shrink_to_fit();
    bestSolution.clear();
    bestSolution.resize(nbVertex);
    bestSolution.shrink_to_fit();
    for (Vertex row = 0; row < nbVertex; row++)
        currentSolution[row] = 0;
    updateBestSolution();
}

void WeightedMaximumStableSolver::updateBestSolution() {
    if (!checkSolution())
        return;
    if (currentCost > bestCost) {
        bestCost = currentCost;
        bestSolution = currentSolution;
    }
}

bool WeightedMaximumStableSolver::checkSolution() {
    for (Vertex row1 = 0; row1 < nbVertex; row1++) {
        if (!currentSolution[row1])
            continue;
        for (Vertex row2 = row1 + 1; row2 < nbVertex; row2++) {
            if (currentSolution[row2] && graph.isEdge(row1, row2))
                return false;
        }
    }
    float temp = 0;
    for (Vertex r = 0; r < nbVertex; r++) {
        if (currentSolution[r])
            temp += weights[r];
    }
    if (temp != currentCost) {
        std::cout << "WARNING: different value, objective is " << currentCost << " different from the computed sum " << temp << std::endl;
        currentCost = temp;
    }
    return true;
}

void WeightedMaximumStableSolver::displayBestSolution() {
    std::cout << "Stable of weighted cost " << bestCost << " with  vertices : ";
    for (Vertex nod = 0; nod < nbVertex; nod++) {
        if (bestSolution[nod])
            std::cout << nod << " ";
    }
    std::cout << std::endl;
}

void WeightedMaximumStableSolver::importGraphDIMACS(char* file) {
    graph.importGraphDIMACS(file);
    nbVertex = graph.getNbVertices();
    bestCost = 0;
    initSolution();
}

<<<<<<< HEAD
//note 1 : *peut* être worth de trier les sommets par degré (simplifierait surtout les disjonctions de cas (un tout petit peu))
//note 2 : on sait que le nombre max de voisins est 2, peut modifier graphNO.getNeighbors pour prendre ça en compte (done)
//note 3 : on pourrait ne pas gérer les poids dans cette fonction, les sommets gardent leur numéro, le poids est donc récupérable après
/*vector<WeightedMaximumStableSolver::composanteConnexe> WeightedMaximumStableSolver::decoupeCompConnexe(){
	// on récupère le premier sommet, on suit la matrice d'adjacence jusqu'à ne plus avoir de nouveau sommet
    vector<Vertex> remaining  (nbVertex, true);
    // remaining.shrink_to_fit();

    vector<int> degree (nbVertex, 0);
	//degree.shrink_to_fit();

	vector<composanteConnexe> graphesConnexes;
	//graphesConnexes.shrink_to_fit();

	int vertexHandled = 0; //pas indispensable, ça raccourci juste le temps d'exec à la toute dernière composante connexe
	int idmin = 0;

	for(int i = 0; i < nbVertex; i++){
		if(graph.getDegree(i) == 0){
			remaining[i] = false;
			vertexHandled++;
			node n = (i, graph.matrix[i]); //mdr je sais pas si c'est ça la syntaxe
			graphesConnexes.neighbors.emplace_back(1, {n}, {}, SOLO);
		}else if (graph.getDegree(i) == 1 && remaining[i]){
			composanteConnexe currentComposanteConnexe = (1, {} , {}, CHAINE);
			
			//on prend l'unique voisin et on parcourt
			Vertex currentVertex = graph.getNeighbors(i).first;

			node leaderNode = (i, make_pair(currentVertex, -1));
			//leaderNode.id = idmin;
			//currentComposanteConnexe.weight = weights[idmin];
			//leaderNode.neighbors = make_pair(currentVertex, -1);
			currentComposanteConnexe.neighbors.emplace_back(leaderNode);
			Vertex previousVertex = i;

			remaining[i] = false;
			vertexHandled++;
			bool over = false;
			while(!over){
				//on considère le cas déjà déjà réglé
				remaining[currentVertex] = false;
				vertexHandled++;

				//setup de la node
				node currentNode;
				currentNode.id = currentVertex;
				//currentComposanteConnexe.weight.push_back() = weights[currentVertex]; //a mettre plus tard

				//maj des current et previous vertex
				if(degree[currentVertex] > 1){
					Vertex tmp = currentVertex; //stockage pour faire un échange de valeur
					std::pair<Vertex, Vertex> voisins = graph.getNeighbors(currentVertex); //les voisins du sommet acutel
					if(voisins.first == previousVertex){ //on choisi le bon prochain sommet à regarder
						currentVertex = voisins.second;
					}else{
						currentVertex = voisins.first;
					}
					//création de la paire de voisins dans la node
					currentNode.neighbors = make_pair(previousVertex, currentVertex);
					previousVertex = tmp;
				}else{ //cas fin de chaîne
					over = true;
					currentNode.neighbors = make_pair(previousVertex, -1);
				}
				//maj de la composante connexe
				currentComposanteConnexe.size++;
				currentComposanteConnexe.neighbors.emplace_back(currentNode);
		}
	}

	while(vertexHandled < nbVertex){
		composanteConnexe currentComposanteConnexe;
		currentComposanteConnexe.size = 1;

		while(!remaining[idmin]){
			idmin++;
		}*/

		/*if(degree[idmin] == 0){
			//suppression du sommet
			remaining[idmin] = false;
			vertexHandled++;

			//setup de la node
			node currentNode;
			currentNode.neighbors = make_pair(-1, -1);
			currentNode.id = idmin;
			//currentNode.weight = weights[idmin];

			//setup de la composante connexe
			currentComposanteConnexe.type = WeightedMaximumStableSolver::typeGraphe::SOLO;
			currentComposanteConnexe.neighbors.push_back(currentNode);
		}else*/ /*if(degree[idmin] == 1){
			currentComposanteConnexe.type = WeightedMaximumStableSolver::typeGraphe::CHAINE;
			//on prend l'unique voisin et on parcourt
			Vertex currentVertex = graph.getNeighbors(idmin).first;

			node leaderNode;
			leaderNode.id = idmin;
			currentComposanteConnexe.weight = weights[idmin];
			leaderNode.neighbors = make_pair(currentVertex, -1);
			currentComposanteConnexe.neighbors.push_back(leaderNode);
			Vertex previousVertex = idmin;

			remaining[idmin] = false;
			vertexHandled++;
			bool over = false;
			while(!over){
				//on considère le cas déjà déjà réglé
				remaining[currentVertex] = false;
				vertexHandled++;

				//setup de la node
				node currentNode;
				currentNode.id = currentVertex;
				currentComposanteConnexe.weight.push_back() = weights[currentVertex]; //a mettre plus tard

				//maj des current et previous vertex
				if(degree[currentVertex] > 1){
					Vertex tmp = currentVertex; //stockage pour faire un échange de valeur
					std::pair<Vertex, Vertex> voisins = graph.getNeighbors(currentVertex); //les voisins du sommet acutel
					if(voisins.first == previousVertex){ //on choisi le bon prochain sommet à regarder
						currentVertex = voisins.second;
					}else{
						currentVertex = voisins.first;
					}
					//création de la paire de voisins dans la node
					currentNode.neighbors = make_pair(previousVertex, currentVertex);
					previousVertex = tmp;
				}else{ //cas fin de chaîne
					over = true;
					currentNode.neighbors = make_pair(previousVertex, -1);
				}
				//maj de la composante connexe
				currentComposanteConnexe.size++;
				currentComposanteConnexe.neighbors.push_back(currentNode);
			}
		}else*//*if(degree[idmin] == 2{ //cas degré 2
			//sommet de départ déjà stocké dans startingVertex, si on retombe dessus on arrête, si on tombe sur un sommet de degré 1 on repart dans l'autre sens
			//on itère comme dans le cas degree = 1
			std::pair<Vertex, Vertex> startingNeighbors = graph.getNeighbors(idmin);
			Vertex currentVertex = startingNeighbors.first;
			
			node leaderNode;
			leaderNode.id = idmin;
			//leaderNode.weight = weights[idmin];
			leaderNode.neighbors = graph.getNeighbors(idmin);
			currentComposanteConnexe.neighbors.emplace_back(leaderNode);
			Vertex previousVertex = idmin;

			remaining[idmin] = false;
			vertexHandled++;

			bool over = false; //condition d'arrêt
			bool switchedDirection = false; //sens de parcours
			while(!over){
				remaining[currentVertex] = false;
				vertexHandled++;

				node currentNode;
				currentNode.id = currentVertex;
				//currentNode.weight = weights[currentVertex];

				if(degree[currentVertex] == 2){
					if(currentVertex == idmin){ //cas "on retombe sur le sommet de départ"
						currentComposanteConnexe.type = WeightedMaximumStableSolver::typeGraphe::CYCLE;
						//currentComposanteConnexe.size--; //le sommet avait déjà été pris en compte
						over = true;
					}else{
						Vertex tmp = currentVertex;
						std::pair<Vertex, Vertex> voisins = graph.getNeighbors(currentVertex);
						if(voisins.first == previousVertex){
							currentVertex = voisins.second;
						}else{
							currentVertex = voisins.first;
						}
						currentNode.neighbors = make_pair(previousVertex, currentVertex);
						previousVertex = tmp;
					}
				}else{ //cas sommet de degré 1
					currentNode.neighbors = make_pair(previousVertex, -1);
					previousVertex = idmin;
					currentVertex = startingNeighbors.first;
					if(switchedDirection){
						over = true;	
					}else{
						currentComposanteConnexe.type = WeightedMaximumStableSolver::typeGraphe::CHAINE;
						switchedDirection = true;
					}
				}
				if(!over){
					currentComposanteConnexe.size++;
					currentComposanteConnexe.neighbors.emplace_back(currentNode);
				}
			}
		}
		graphesConnexes.emplace_back(currentComposanteConnexe);
	}
	return graphesConnexes;
}*/

void WeightedMaximumStableSolver::displayCompConnexe(){
	cout << endl;
	graph.display();
	vector<WeightedMaximumStableSolver::composanteConnexe> list = decoupeCompConnexe();
	for(WeightedMaximumStableSolver::composanteConnexe c : list){
		cout << c.size << " ";
		cout << "Type : ";
		if(c.type == WeightedMaximumStableSolver::typeGraphe::SOLO){
			cout << "Solo";
		}else if(c.type == WeightedMaximumStableSolver::typeGraphe::CHAINE){
			cout << "Chaine";
		}else{
			cout << "Cycle";
		}
		cout << endl;
		for(node n : c.neighbors){
			cout << n.id << " - ";
		}
		cout << endl << endl;
	}
	cout << endl;
=======
void WeightedMaximumStableSolver::importWeights(char* file) {
    // TODO
>>>>>>> 06122fb7301d69d8c0723711a5965cc9ed68ff39
}
