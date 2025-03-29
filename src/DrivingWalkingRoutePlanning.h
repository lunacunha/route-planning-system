#ifndef DRIVING_WALKING_ROUTE_PLANNING_H
#define DRIVING_WALKING_ROUTE_PLANNING_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "Graph.h"  // Make sure the include path is correct

using namespace std;

struct DrivingWalkingRoute {
    vector<string> drivingRoute;
    string parkingNode;
    vector<string> walkingRoute;
    double drivingTime;
    double walkingTime;
    double totalTime;
    int usedMaxWalkTime;
};

class DrivingWalkingRoutePlanning {
public:

    // constructor receives a reference to the graph and a parking info mapping
    DrivingWalkingRoutePlanning(Graph<string>& graph, const unordered_map<string, bool>& parkingInfo);
    
    // finds the best route based on overall travel time (with tie-breaker on walking time)
    // if failure -> errorMessage is set and returned route segments will be empty
    DrivingWalkingRoute findBestRoute(const string& source, const string& destination, 
                                      int maxWalkTime, 
                                      const unordered_set<string>& avoidNodes, 
                                      const vector<pair<string, string>>& avoidSegments,
                                      string& errorMessage);
    
    // provides up to 2 alternative routes (approximate solutions) that relax the walking time constraint
    vector<DrivingWalkingRoute> findApproximateRoutes(const string& source, const string& destination, 
                                                      int maxWalkTime, 
                                                      const unordered_set<string>& avoidNodes, 
                                                      const vector<pair<string, string>>& avoidSegments);
    vector<DrivingWalkingRoute> findAllValidRoutes(
    const string& source, const string& destination,
    int maxWalkTime,
    const unordered_set<string>& avoidNodes,
    const vector<pair<string, string>>& avoidSegments) ;

    
private:
    Graph<string>& graph;
    const unordered_map<string, bool>& parkingInfo;
    
    // Helper: Dijkstra for walking segments using walking edge weights
    vector<string> dijkstraWalking(const string& src, const string& dest, double& walkingTime, const unordered_set<string>& avoidNodes);
    
    // Helper: Uses the driving cost to compute driving segments
    vector<string> dijkstraDriving(const string& src, const string& dest, double& drivingTime, const unordered_set<string>& avoidNodes);
};

#endif // DRIVING_WALKING_ROUTE_PLANNING_H
