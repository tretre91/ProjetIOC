#include "graph2.hpp"
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string.h>

void Graph2::initEmptyGraph(Vertex nbVertices) {
    nbVertex = nbVertices;
    matrix.clear();
    matrix.resize(nbVertex, {-1, -1});
    matrix.shrink_to_fit();
}

bool Graph2::isEdge(Vertex v1, Vertex v2) {
    return matrix[v1].first == v2 || matrix[v1].second == v2;
}

void Graph2::addEdge(Vertex v, Vertex v1) {
    if (matrix[v].first == -1) {
        matrix[v].first = v1;
    } else {
        matrix[v].second = v1;
    }
}

void Graph2::addEdgeFast(Vertex v, Vertex v1, Vertex v2) {
    matrix[v] = make_pair(v1, v2);
}

void Graph2::removeEdge(Vertex v1, Vertex v2) {
    if (matrix[v1].first == v2) {
        matrix[v1].first = -1;
    } else if (matrix[v1].second == v2) {
        matrix[v1].second = -1;
    }
}

bool Graph2::hasNoNeighbor(Vertex v1) {
    return matrix[v1].first == -1 && matrix[v1].second == -1;
}

/*void Graph2::toComplementGraph() {
   for(Vertex row =0; row < nbVertex; row++){
      for(Vertex col =row+1; col < nbVertex; col++){
        if (isEdge(row,col)) removeEdge(row,col);
        else addEdge(row,col);
      }
      std::cout << std::endl;
   }

}*/

void Graph2::display() {
    cout << endl;
    for (Vertex vertex = 0; vertex < nbVertex; vertex++) {
        cout << "Sommet " << vertex << " : " << endl;
        int val = 0;
        int v1;
        int v2;
        if (matrix[vertex].first != -1) {
            val++;
            v1 = matrix[vertex].first;
        } else {
            v1 = -1;
        }
        if (matrix[vertex].second != -1) {
            val++;
            v2 = matrix[vertex].first;
        } else {
            v2 = -1;
        }
        cout << v1 << " & " << v2 << " -> " << val << endl;
    }
    cout << endl;
}

// Même chose que getDegree, mais en arrêtant de chercher dès qu'on a 2 voisins
Vertex Graph2::getDegree(Vertex vertex) {
    int val = 0;
    if (matrix[vertex].first != -1) {
        val++;
    }
    if (matrix[vertex].second != -1) {
        val++;
    }
    return val;
}

std::pair<Vertex, Vertex> Graph2::getNeighbors(Vertex vertex) {
    return matrix[vertex];
}

void Graph2::intersectNeighbors(Vertex vertex, vector<Vertex>& remaining) {
    vector<Vertex> v;
    v.reserve(remaining.size());

    for (std::vector<Vertex>::iterator it = remaining.begin(); it != remaining.end(); ++it)
        if (isEdge(vertex, *it))
            v.push_back(*it);

    v.shrink_to_fit();
    remaining.resize(v.size());
    remaining = v;
    remaining.shrink_to_fit();
}

void Graph2::get_params(char* Preamble)
/* getting Nr_vert and Nr_edge from the preamble string,
   containing Dimacs format "p ??? num num" */
{
    char c, *tmp;
    char* pp = Preamble;
    int stop = 0;
    tmp = (char*)calloc(100, sizeof(char));
    int Nr_vert, Nr_edges;
    Nr_vert = 0;

    while (!stop && (c = *pp++) != '\0') {
        switch (c) {
        case 'c':
            while ((c = *pp++) != '\n' && c != '\0')
                ;
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

    std::cout << "import of DIMACS graph with " << Nr_vert << " vertices and " << Nr_edges << " edges" << std::endl;
    nbVertex = Nr_vert;
}

void Graph2::toDegree2(GraphNO& g) {
    initEmptyGraph(g.getNbVertices());

    for (Vertex v = 0; v < g.getNbVertices(); v++) {
        std::vector<Vertex> voisins = g.getNeighbors(v);

        if (voisins.size() == 1) {
            addEdge(v, voisins[0]);
        } else if (voisins.size() == 2) {
            addEdgeFast(v, voisins[0], voisins[1]);
        }
    }
}

void Graph2::importGraphDIMACS(char* file) {
    int MAX_PREAMBLE = 4000;
    char* Preamble = new char[MAX_PREAMBLE];
    int c, oc;
    char* pp = Preamble;
    int i, j;
    float k;
    FILE* fp;

    if ((fp = fopen(file, "r")) == NULL) {
        printf("ERROR: Cannot open infile\n");
        exit(10);
    }

    for (oc = '\0'; (c = fgetc(fp)) != EOF && (oc != '\n' || c != 'e'); oc = *pp++ = c)
        ; //??

    ungetc(c, fp);
    *pp = '\0';
    get_params(Preamble);

    initEmptyGraph(nbVertex);

    int n = 0; //
    while ((c = fgetc(fp)) != EOF) {
        switch (c) {
        case 'e':
            if (!fscanf(fp, "%d %d %f", &i, &j, &k)) {
                printf("ERROR: corrupted inputfile\n");
                exit(10);
            }
            // addEdge(i-1, j-1);
            // if( (i, j) != (0, 0)){ //jsp exactement ce qui fait ça mais tant pis
            addEdgeFast(n, i, j);
            // weights[n] = k;
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

    std::cout << "end of import of DIMACS graph with " << nbVertex << " vertices" << std::endl;
}

// note 1 : *peut* être worth de trier les sommets par degré (simplifierait surtout les disjonctions de cas (un tout petit peu))
// note 2 : on sait que le nombre max de voisins est 2, peut modifier graphNO.getNeighbors pour prendre ça en compte (done)
// note 3 : on pourrait ne pas gérer les poids dans cette fonction, les sommets gardent leur numéro, le poids est donc récupérable après
vector<composanteConnexe> Graph2::decoupeCompConnexe() {
    // on récupère le premier sommet, on suit la matrice d'adjacence jusqu'à ne plus avoir de nouveau sommet
    vector<Vertex> remaining(nbVertex, false);
    // remaining.shrink_to_fit();

    vector<int> degree(nbVertex);
    // degree.shrink_to_fit();

    vector<composanteConnexe> graphesConnexes;
    // graphesConnexes.shrink_to_fit();

    for (Vertex i = 0; i < nbVertex; i++) {
        remaining[i] = true; // normalement on peut faire ça à l'init mais il semblerait que je sois nul
        degree[i] = getDegree(i);
    }

    unsigned int vertexHandled = 0; // pas indispensable, ça raccourci juste le temps d'exec à la toute dernière composante connexe
    int idmin = 0;

    while (vertexHandled < nbVertex) {
        composanteConnexe currentComposanteConnexe;
        currentComposanteConnexe.size = 1;

        while (!remaining[idmin]) {
            idmin++;
        }

        if (degree[idmin] == 0) {
            // suppression du sommet
            remaining[idmin] = false;
            vertexHandled++;

            // setup de la node
            node currentNode;
            currentNode.neighbors = make_pair(-1, -1);
            currentNode.id = idmin;
            // currentComposanteConnexe.weight.push_back(weights[idmin]);

            // setup de la composante connexe
            currentComposanteConnexe.type = typeGraphe::SOLO;
            currentComposanteConnexe.neighbors.push_back(currentNode);
        } else if (degree[idmin] == 1) {
            currentComposanteConnexe.type = typeGraphe::CHAINE;
            // on prend l'unique voisin et on parcourt
            Vertex currentVertex = getNeighbors(idmin).first;

            node leaderNode;
            leaderNode.id = idmin;
            // currentComposanteConnexe.weight.push_back(weights[idmin]);
            leaderNode.neighbors = make_pair(currentVertex, -1);
            currentComposanteConnexe.neighbors.push_back(leaderNode);
            Vertex previousVertex = idmin;

            remaining[idmin] = false;
            vertexHandled++;
            bool over = false;
            while (!over) {
                // on considère le cas déjà déjà réglé
                remaining[currentVertex] = false;
                vertexHandled++;

                // setup de la node
                node currentNode;
                currentNode.id = currentVertex;
                // currentComposanteConnexe.weight.push_back(weights[currentVertex]); // a mettre plus tard

                // maj des current et previous vertex
                if (degree[currentVertex] > 1) {
                    Vertex tmp = currentVertex;                                      // stockage pour faire un échange de valeur
                    std::pair<Vertex, Vertex> voisins = getNeighbors(currentVertex); // les voisins du sommet acutel
                    if (voisins.first == previousVertex) {                           // on choisi le bon prochain sommet à regarder
                        currentVertex = voisins.second;
                    } else {
                        currentVertex = voisins.first;
                    }
                    // création de la paire de voisins dans la node
                    currentNode.neighbors = make_pair(previousVertex, currentVertex);
                    previousVertex = tmp;
                } else { // cas fin de chaîne
                    over = true;
                    currentNode.neighbors = make_pair(previousVertex, -1);
                }
                // maj de la composante connexe
                currentComposanteConnexe.size++;
                currentComposanteConnexe.neighbors.push_back(currentNode);
            }
        } else { // cas degré 2
            // sommet de départ déjà stocké dans startingVertex, si on retombe dessus on arrête, si on tombe sur un sommet de degré 1 on repart dans l'autre
            // sens on itère comme dans le cas degree = 1
            std::pair<Vertex, Vertex> startingNeighbors = getNeighbors(idmin);
            Vertex currentVertex = startingNeighbors.first;

            node leaderNode;
            leaderNode.id = idmin;
            // leaderNode.weight = weights[idmin];
            leaderNode.neighbors = getNeighbors(idmin);
            currentComposanteConnexe.neighbors.push_back(leaderNode);
            Vertex previousVertex = idmin;

            remaining[idmin] = false;
            vertexHandled++;

            bool over = false;              // condition d'arrêt
            bool switchedDirection = false; // sens de parcours
            while (!over) {
                remaining[currentVertex] = false;
                vertexHandled++;

                node currentNode;
                currentNode.id = currentVertex;
                // currentComposanteConnexe.weight.push_back(weights[currentVertex]);

                if (degree[currentVertex] == 2) {
                    if (currentVertex == idmin) { // cas "on retombe sur le sommet de départ"
                        currentComposanteConnexe.type = typeGraphe::CYCLE;
                        // currentComposanteConnexe.size--; //le sommet avait déjà été pris en compte
                        over = true;
                    } else {
                        Vertex tmp = currentVertex;
                        std::pair<Vertex, Vertex> voisins = getNeighbors(currentVertex);
                        if (voisins.first == previousVertex) {
                            currentVertex = voisins.second;
                        } else {
                            currentVertex = voisins.first;
                        }
                        currentNode.neighbors = make_pair(previousVertex, currentVertex);
                        previousVertex = tmp;
                    }
                } else { // cas sommet de degré 1
                    currentNode.neighbors = make_pair(previousVertex, -1);
                    previousVertex = idmin;
                    currentVertex = startingNeighbors.first;
                    if (switchedDirection) {
                        over = true;
                    } else {
                        currentComposanteConnexe.type = typeGraphe::CHAINE;
                        switchedDirection = true;
                    }
                }
                if (!over) {
                    currentComposanteConnexe.size++;
                    currentComposanteConnexe.neighbors.push_back(currentNode);
                }
            }
        }
        graphesConnexes.push_back(currentComposanteConnexe);
    }
    return graphesConnexes;
}

vector<composanteConnexe> Graph2::decoupeCompConnexe2() {
    std::vector<bool> explored(nbVertex, false);
    std::vector<unsigned int> degrees(nbVertex);

    for (Vertex i = 0; i < nbVertex; i++) {
        degrees[i] = getDegree(i);
    }

    std::vector<composanteConnexe> comps;

    for (Vertex v = 0; v < nbVertex; v++) {
        if (!explored[v]) {
            if (degrees[v] == 0) {
                composanteConnexe comp(typeGraphe::SOLO, node{v, make_pair(-1, -1)});
                comps.emplace_back(std::move(comp));
                explored[v] = true;
            } else if (degrees[v] == 1) {
                node first = {v, getNeighbors(v)};
                composanteConnexe comp = explore(typeGraphe::CHAINE, explored, first, [](const node& n) { return n.neighbors.second == -1; });
                comps.emplace_back(std::move(comp));
            }
        }
    }

    for (Vertex v = 0; v < nbVertex; v++) {
        if (!explored[v]) {
            node first = {v, getNeighbors(v)};
            composanteConnexe comp = explore(typeGraphe::CYCLE, explored, first, [initial = v](const node& n) { return n.id == initial; });
            comps.emplace_back(std::move(comp));
        }
    }

    return comps;
}

void Graph2::displayCompConnexe() {
    cout << endl;
    display();
    vector<composanteConnexe> list = decoupeCompConnexe();
    for (const composanteConnexe& c : list) {
        cout << c.size << " ";
        cout << "Type : ";
        if (c.type == typeGraphe::SOLO) {
            cout << "Solo";
        } else if (c.type == typeGraphe::CHAINE) {
            cout << "Chaine";
        } else {
            cout << "Cycle";
        }
        cout << endl;
        for (node n : c.neighbors) {
            cout << n.id << " - ";
        }
        cout << endl << endl;
    }
    cout << endl;
}
