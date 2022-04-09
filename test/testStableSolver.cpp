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
    int nbInstances = 1;
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
            
            random_device dev;
            mt19937 engine(dev());
            uniform_int_distribution<unsigned int> random(2000, 10000);
            int nbVertex = 10 + (int) random(engine);
            int nbEdge = (int) (random(engine))%(4*nbVertex);

            //p carac
            file << "p edge " << nbVertex << " " << 0 << endl;
            
            normal_distribution<float> distribEdge( nbEdge / (float) nbVertex, 1);
            vector<int> degree (nbVertex, 0);
            int vertexGenerated = 1;

            while(vertexGenerated < nbVertex - 1){
                uniform_int_distribution<unsigned int> distribVertex(vertexGenerated+1, nbVertex);
                int toPlace = ceil(distribEdge(engine));
                while(degree[vertexGenerated] < toPlace){
                    Vertex selected = ceil(distribVertex(engine));
                    file << "e " << vertexGenerated << " " << selected << endl;
                    degree[vertexGenerated]++;
                    degree[selected]++;
                }
                vertexGenerated++;
            }
        }else{
            printf("ERROR: Cannot open outfile\n");
        }
        listFile.emplace_back(path + name);
    }
    return listFile;
}

int main(int argc, char* argv[]) {
    //vector<string> listInstancesD2 =  initInstancesDegree2();
    vector<string> listInstances = initInstances();
    cout << "Instances generated" << endl;
    StableSolver solver;
    
    GraphNO g;

    auto start = std::chrono::high_resolution_clock::now();

    for(string pathname : listInstances){
        cout << pathname <<endl;
        g.importGraphDIMACS(const_cast<char*>(pathname.c_str()));

        solver.init(g);

        solver.updateFixed({});

        auto start = std::chrono::high_resolution_clock::now();
        solver.solve();
        std::chrono::duration<double> tempsSeq = std::chrono::high_resolution_clock::now() - start;
        cout << "Solved in " << tempsSeq.count() << " s" << '\n';
        std::cout << "Backtracking : " << solver.getSize() << '\n';
    }
    return 0;
}
