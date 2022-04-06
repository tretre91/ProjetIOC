#include "../src/stableSolver.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <chrono>
#include <cmath>
#include <string>
#include <random>
#include <iomanip>

vector<std::string> initInstancesDegree2(){
    int nbInstances = 10;
    vector<std::string> listFile;
    std::string path = "../test/RandomInstances/";
    //srand((unsigned int) time(0)); //c'est peut être pas si mal d'avoir les même instances à chaque génération
    for(int i = 1; i < nbInstances; i++){
        std::string name = "instanceD2_" + to_string(i) + ".col";
        std::ofstream file(path + name);

        if(file.is_open()){
            //c name
            //c Source
            file << "c FILE: " << name <<endl;
            file << "c SOURCE: initInstancesDegree2" << endl;
            
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

vector<std::string> initInstances(){
    int nbInstances = 10;
    vector<std::string> listFile;
    std::string path = "../test/RandomInstances/";
    //srand((unsigned int) time(0)); //c'est peut être pas si mal d'avoir les même instances à chaque génération
    for(int i = 1; i <= nbInstances; i++){
        std::string name = "instance_" + to_string(i) + ".col";
        std::ofstream file(path + name);

        if(file.is_open()){
            //c name
            //c Source
            file << "c FILE: " << name <<endl;
            file << "c SOURCE: initInstances" << endl;
            
            int nbVertex = 10 + rand()%10;
            int nbEdge = rand()%(4*nbVertex);

            //p carac
            file << "p edge " << nbVertex << " " << nbEdge << endl;
            
            //e edges
            //std::string edges;
            //tant qu'on a pas placé nbEdge edge, on fait
            int vertexGenerated = 1;
            int edgePlaced = 0;

            while(vertexGenerated <= nbVertex){
                float r = rand()/(float) RAND_MAX; //ma chance de générer doit baisser à chaque edge ajouté
                //si je veux 10 edge par vertex, il me faut une chance de drop le vertex en cours exponentielle, qui est significative à partir de 6, et spike autour de 11-12
                //random_device rd{};
                //normal_distribution<> d{ev, ev/log(ev)}
                for(int i = vertexGenerated + 1; i <= nbVertex; i++){
                    if(r < 1. - (nbVertex/ (float) nbEdge)){
                        //edges += "e " + to_string(vertexGenerated) + " " + to_string(i) + '\n';
                        //cout << "Added bond : " << vertexGenerated << " - " << i << endl;
                        file << "e " << vertexGenerated << " " << i << endl;
                        edgePlaced++;
                    }
                }
                vertexGenerated++;
            }

            //file << edges;
        }else{
            printf("ERROR: Cannot open outfile\n");
        }
        //cout << path+name <<endl;
        //listFile
        listFile.emplace_back(path + name);
    }
    return listFile;
}

int main(int argc, char* argv[]) {
    vector<string> listInstancesD2 =  initInstancesDegree2();
    vector<string> listInstances = initInstances();

    StableSolver solver;
    
    GraphNO g;

    auto start = std::chrono::high_resolution_clock::now();

    /*for(string pathname : listInstancesD2){
        cout << pathname <<endl;
        g.importGraphDIMACS(const_cast<char*>(pathname.c_str()));
        
        //cout << "-----Regular Display-----" << endl;
        //g.display();
        //cout << "-----INIT-----" << endl;
        solver.init(g);
        //cout << "-----ToDegree2 Display-----" << endl;
        //solver.getGraph2().display2();
        solver.updateFixed({});

        //fout << pathname << ", ";
        //solver.getGraph2().display();
        auto start = std::chrono::high_resolution_clock::now();
        solver.solve();
        std::chrono::duration<double> tempsSeq = std::chrono::high_resolution_clock::now() - start;
        cout << "Solved in " << tempsSeq.count() << " s" << '\n';
        std::cout << "Backtracking : " << solver.getSize() << '\n';
    }*/

    for(string pathname : listInstances){
        cout << pathname <<endl;
        g.importGraphDIMACS(const_cast<char*>(pathname.c_str()));
        //cout << "-----Regular Display-----" << endl;
        g.display();
        //cout << "-----INIT-----" << endl;
        solver.init(g);
        //cout << "-----ToDegree2 Display-----" << endl;
        //solver.getGraph2().display2();
        solver.updateFixed({});

         auto start = std::chrono::high_resolution_clock::now();
        solver.solve();
        std::chrono::duration<double> tempsSeq = std::chrono::high_resolution_clock::now() - start;
        cout << "Solved in " << tempsSeq.count() << " s" << '\n';
        std::cout << "Backtracking : " << solver.getSize() << '\n';
    }

    //g.importGraphDIMACS("/home/brice/Bureau/IOC/ProjetIOC/test/instances/custom.col");
    //fout.close();
    return 0;
}
