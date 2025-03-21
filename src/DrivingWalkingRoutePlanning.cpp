#include "DrivingWalkingRoutePlanning.h"
#include "RoutePlanning.h"
#include <limits>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <cmath>
#include <tuple>
#include <algorithm>
using namespace std;

DrivingWalkingRoutePlanning::DrivingWalkingRoutePlanning(Graph<string>& graph, const unordered_map<string, bool>& parkingInfo)
    : graph(graph), parkingInfo(parkingInfo)
{}

// Implementation of Dijkstra for walking segments (using walking weights)
vector<string> DrivingWalkingRoutePlanning::dijkstraWalking(const string& src, const string& dest, double& walkingTime, const unordered_set<string>& avoidNodes) {
    unordered_map<string, double> minDist;
    unordered_map<string, string> prev;
    auto vertexSet = graph.getVertexSet();
    for (auto vertex : vertexSet) {
        string info = vertex->getInfo();
        minDist[info] = numeric_limits<double>::max();
        prev[info] = "";
    }
    if (avoidNodes.find(src) != avoidNodes.end()) {
        walkingTime = numeric_limits<double>::max();
        return {};
    }
    minDist[src] = 0.0;
    
    // Priority queue: (distance, vertex)
    using pii = pair<double, string>;
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0.0, src});
    
    while (!pq.empty()) {
        auto [dist, u] = pq.top();
        pq.pop();
        if (dist > minDist[u])
            continue;
        if (u == dest)
            break;
        auto vertex = graph.findVertex(u);
        if (!vertex)
            continue;
        for (auto edge : vertex->getAdj()) {
            string v = edge->getDest()->getInfo();
            if (avoidNodes.find(v) != avoidNodes.end())
                continue;
            double w = edge->getWalking();
            if (w < 0)
                continue;
            if (minDist[u] + w < minDist[v]) {
                minDist[v] = minDist[u] + w;
                prev[v] = u;
                pq.push({minDist[v], v});
            }
        }
    }
    
    walkingTime = minDist[dest];
    if (walkingTime == numeric_limits<double>::max())
        return {};
    
    // Reconstruct the walking path.
    vector<string> path;
    string cur = dest;
    while (!cur.empty()) {
        path.insert(path.begin(), cur);
        cur = prev[cur];
    }
    return path;
}

// Helper for driving route: reuses the existing RoutePlanning::dijkstra.
// Note: We use a temporary RoutePlanning object.
vector<string> DrivingWalkingRoutePlanning::dijkstraDriving(const string& src, const string& dest, double& drivingTime, const unordered_set<string>& avoidNodes) {
    RoutePlanning rp(graph);
    return rp.dijkstra(src, dest, drivingTime, avoidNodes);
}

DrivingWalkingRoute DrivingWalkingRoutePlanning::findBestRoute(const string& source, const string& destination, 
                                      int maxWalkTime, 
                                      const unordered_set<string>& avoidNodes, 
                                      const vector<pair<string, string>>& avoidSegments,
                                      string& errorMessage) {
    DrivingWalkingRoute bestRoute;
    bestRoute.totalTime = numeric_limits<double>::max();
    errorMessage = "";
    
    // Validate that source and destination exist.
    auto srcVertex = graph.findVertex(source);
    auto destVertex = graph.findVertex(destination);
    if (!srcVertex || !destVertex) {
        errorMessage = "Source or destination does not exist.";
        return bestRoute;
    }
    
    // Source and destination must not be parking nodes.
    if (parkingInfo.find(source) != parkingInfo.end() && parkingInfo.at(source)) {
        errorMessage = "Source is a parking node.";
        return bestRoute;
    }
    if (parkingInfo.find(destination) != parkingInfo.end() && parkingInfo.at(destination)) {
        errorMessage = "Destination is a parking node.";
        return bestRoute;
    }
    
    // They also must not be adjacent (to ensure both driving and walking segments exist).
    bool adjacent = false;
    for (auto edge : srcVertex->getAdj()) {
        if (edge->getDest()->getInfo() == destination) {
            adjacent = true;
            break;
        }
    }
    if (adjacent) {
        errorMessage = "Source and destination are adjacent.";
        return bestRoute;
    }
    
    // Process avoidSegments: temporarily remove these edges.
    vector<tuple<string, string, double, double>> removedEdges;
    for (auto seg : avoidSegments) {
        auto v1 = graph.findVertex(seg.first);
        if (v1) {
            for (auto edge : v1->getAdj()) {
                if (edge->getDest()->getInfo() == seg.second) {
                    removedEdges.push_back(make_tuple(seg.first, seg.second, edge->getDriving(), edge->getWalking()));
                    graph.removeEdge(seg.first, seg.second);
                    break;
                }
            }
        }
    }
    
    // Iterate through candidate parking nodes (only those with parking available).
    auto vertices = graph.getVertexSet();
    bool routeFound = false;
    for (auto vertex : vertices) {
        string node = vertex->getInfo();
        if (node == source || node == destination)
            continue;
        if (parkingInfo.find(node) == parkingInfo.end() || !parkingInfo.at(node))
            continue;
        
        // Compute driving route from source to candidate parking node.
        double drivingTime = 0.0;
        vector<string> drivingRoute = dijkstraDriving(source, node, drivingTime, avoidNodes);
        if (drivingRoute.empty())
            continue;
        
        // Compute walking route from parking node to destination.
        double walkingTime = 0.0;
        vector<string> walkingRoute = dijkstraWalking(node, destination, walkingTime, avoidNodes);
        if (walkingRoute.empty())
            continue;
        
        // Must include both segments and walking time must be within user limits.
        if (walkingTime > maxWalkTime)
            continue;
        
        double totalTime = drivingTime + walkingTime;
        // Choose the route with smallest total time; in case of tie, pick the one with a longer walking segment.
        if (totalTime < bestRoute.totalTime || (fabs(totalTime - bestRoute.totalTime) < 1e-6 && walkingTime > bestRoute.walkingTime)) {
            bestRoute.drivingRoute = drivingRoute;
            bestRoute.parkingNode = node;
            bestRoute.walkingRoute = walkingRoute;
            bestRoute.drivingTime = drivingTime;
            bestRoute.walkingTime = walkingTime;
            bestRoute.totalTime = totalTime;
            routeFound = true;
        }
    }
    
    // Restore temporarily removed avoid-segment edges.
    for (auto seg : removedEdges) {
        string from, to;
        double d, w;
        tie(from, to, d, w) = seg;
        graph.addEdge(from, to, d, w);
    }
    
    if (!routeFound)
        errorMessage = "No valid driving-walking route found that meets the requirements.";
    
    return bestRoute;
}

vector<DrivingWalkingRoute> DrivingWalkingRoutePlanning::findApproximateRoutes(const string& source, const string& destination, 
                                                      int maxWalkTime, 
                                                      const unordered_set<string>& avoidNodes, 
                                                      const vector<pair<string, string>>& avoidSegments) {
    vector<DrivingWalkingRoute> suggestions;
    // Relax the maximum walking time constraint by fixed increments (for example, +5 and +10).
    int increments[2] = { maxWalkTime + 5, maxWalkTime + 10 };
    
    for (int newMaxWalk : increments) {
        string dummyError;
        DrivingWalkingRoute route = findBestRoute(source, destination, newMaxWalk, avoidNodes, avoidSegments, dummyError);
        if (!route.drivingRoute.empty() && !route.walkingRoute.empty() && !route.parkingNode.empty()) {
            suggestions.push_back(route);
            if (suggestions.size() == 2)
                break;
        }
    }
    // Sort suggestions by overall total time.
    sort(suggestions.begin(), suggestions.end(), [](const DrivingWalkingRoute& a, const DrivingWalkingRoute& b) {
        return a.totalTime < b.totalTime;
    });
    return suggestions;
}
