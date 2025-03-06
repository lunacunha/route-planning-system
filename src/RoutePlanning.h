//
// Created by martagfmartins on 3/6/25.
//

#ifndef ROUTEPLANNING_H
#define ROUTEPLANNING_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include "Graph.h"
#include <string>
using namespace std;
#define INF std::numeric_limits<double>::max()

class RoutePlanning {
    public:
        RoutePlanning(Graph<string>& graph);

        std::vector<string> dijkstra(const string& source, const string& destination, double& totalTime);

    private:
        Graph<string>& graph;

};



#endif //ROUTEPLANNING_H
