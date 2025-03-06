//
// Created by Luna on 22/02/2025.
//

#ifndef PROJETO1_DA_GRAPHINTERFACE_H
#define PROJETO1_DA_GRAPHINTERFACE_H

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Graph.h"
#include "Location.h"
using namespace std;

class GraphInterface {

private:
    Graph<string> graph;

public:
    GraphInterface();

    void loadLocations(const string &filename);
    void loadDistances(const string &filename);

    //unordered_map<string, Location*> getLocations();

    Graph<string>& getGraph() {
        return graph;
    }

    //void removeAll(); // aux function
};


#endif //PROJETO1_DA_GRAPHINTERFACE_H
