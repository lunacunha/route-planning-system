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
#include <unordered_set>
using namespace std;
#define INF std::numeric_limits<double>::max()

class RoutePlanning {
    public:
        RoutePlanning(Graph<string>& graph);

        vector<string> dijkstra(const string& sourceLocation, const string& destinationLocation, double& totalTravelTime);
        vector<string> dijkstra(const string& sourceLocation, const string& destinationLocation, double& totalTravelTime, const unordered_set<string>& avoidNodes);
        pair<vector<string>,vector<string>> findIndependentRoutes(const string& sourceLocation, const string& destinationLocation, double& bestRouteTime, double& alternativeRouteTime);
        vector<string> findRestrictedRoute(const string& sourceLocation, const string& destinationLocation, const unordered_set<string>& avoidNodes, const vector<pair<string, string>>& avoidSegments, const string& includeNode, double& totalTravelTime);


    private:
        Graph<string>& graph;

};



#endif //ROUTEPLANNING_H
