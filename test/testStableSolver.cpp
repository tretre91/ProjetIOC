#include "../src/stableSolver.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

vector<std::string> initInstances(){
    int nbInstances = 10;
    vector<std::string> listFile;
    std::string path = "/home/brice/Bureau/IOC/ProjetIOC/test/RandomInstances/";
    //srand((unsigned int) time(0)); //c'est peut être pas si mal d'avoir les même instances à chaque génération
    for(int i = 1; i < nbInstances; i++){
        std::string name = "instance" + to_string(i) + ".col";
        std::ofstream file(path + name);

        if(file.is_open()){
            //c name
            //c Source
            file << "c FILE: " << name <<endl;
            file << "c SOURCE: RANDOM GENERATION" << endl;
            
            int nbVertex = 20 + rand()%180;
            int nbEdge = rand()%nbVertex;

            //p carac
            file << "p edge " << nbVertex << " " << nbEdge << endl;
            
            //e edges
            
            vector<int> degree(nbVertex, 0);
            //tant qu'on a pas placé nbEdge edge, on fait
            int edgePlaced = 0;
            while(edgePlaced < nbEdge){
                Vertex firstSelectedVertex;
                //on prend un sommet aléatoire n'ayant pas déjà 2 edge dans la liste
                do{
                    firstSelectedVertex = 1 + rand()%(nbVertex-1);
                }while(degree[firstSelectedVertex] > 1);
                //on prend un deuxième sommet aléatoire n'ayant pas déjà 2 edge dans la liste ET n'étant pas déjà voisin du premier
                Vertex secondSelectedVertex;
                do{
                    secondSelectedVertex = 1 + rand()%(nbVertex-1);
                }while(degree[secondSelectedVertex] > 1 || secondSelectedVertex == firstSelectedVertex);

                degree[firstSelectedVertex]++;
                degree[secondSelectedVertex]++;
                edgePlaced++;

                file << "e " << firstSelectedVertex << " " << secondSelectedVertex << endl;
            }
        }else{
            printf("ERROR: Cannot open outfile\n");
        }
        //cout << path+name <<endl;
        //listFile
        listFile.emplace_back(path + name);
    }
    return listFile;
}

std::vector<float> importFile(const std::string& filename) {
    std::ifstream file(filename);

    if (file.is_open()) {
        char c;
        size_t size;
        file >> c >> size;
        std::vector<float> values(size);
        for (float& v : values) {
            file >> v;
        }
        return values;
    }
    return {};
}

int main() {
    vector<string> listInstances = initInstances();
    
    StableSolver solver;
    GraphNO g;
    for(string pathname : listInstances){
        cout << pathname <<endl;
        g.importGraphDIMACS(const_cast<char*>(pathname.c_str()));
        solver.init(g, {});
        float cost = solver.solve();
        std::cout << cost << '\n';
    }
    //g.importGraphDIMACS("/home/brice/Bureau/IOC/ProjetIOC/test/instances/custom.col");

    return 0;
}
