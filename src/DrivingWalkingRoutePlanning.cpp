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
#include <numeric>
#include <string>
using namespace std;

DrivingWalkingRoutePlanning::DrivingWalkingRoutePlanning(Graph<string>& graph, const unordered_map<string, bool>& parkingInfo)
    : graph(graph), parkingInfo(parkingInfo)
{}

// Dijkstra for walking segments using walking weights
vector<string> DrivingWalkingRoutePlanning::dijkstraWalking(const string& src, const string& dest, double& walkingTime, const unordered_set<string>& avoidNodes) {
    unordered_map<string, double> minDist;
    unordered_map<string, string> prev;
    auto vertexSet = graph.getVertexSet();
    for (const auto& vertex : vertexSet) {
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
        for (const auto edge : vertex->getAdj()) {
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

    // Ensure that the walking route has at least 1 edge
    if (path.size() < 2)
        return {};

    return path;
}

// Helper for driving route: reuses the existing RoutePlanning::dijkstra
// Note: we use a temporary RoutePlanning object
vector<string> DrivingWalkingRoutePlanning::dijkstraDriving(const string& src, const string& dest, double& drivingTime, const unordered_set<string>& avoidNodes) {
    RoutePlanning rp(graph);
    vector<string> route = rp.dijkstra(src, dest, drivingTime, avoidNodes);
    // Ensure that the driving route has at least one edge.
    if (route.size() < 2)
        route.clear();
    return route;
}

DrivingWalkingRoute DrivingWalkingRoutePlanning::findBestRoute(const string& source, const string& destination,
                                      int maxWalkTime,
                                      const unordered_set<string>& avoidNodes,
                                      const vector<pair<string, string>>& avoidSegments,
                                      string& errorMessage) {
    DrivingWalkingRoute bestRoute;
    bestRoute.totalTime = numeric_limits<double>::max();
    errorMessage = "";

    // validates the existence of source and destination
    auto srcVertex = graph.findVertex(source);
    auto destVertex = graph.findVertex(destination);
    if (!srcVertex || !destVertex) {
        errorMessage = "Source or destination does not exist.";
        return bestRoute;
    }

    // source and destination cannot have parking
    if (parkingInfo.find(source) != parkingInfo.end() && parkingInfo.at(source)) {
        errorMessage = "Source is a parking node.";
        return bestRoute;
    }
    if (parkingInfo.find(destination) != parkingInfo.end() && parkingInfo.at(destination)) {
        errorMessage = "Destination is a parking node.";
        return bestRoute;
    }

    // source and destination cannot be adj
    bool adjacent = false;
    for (const auto edge : srcVertex->getAdj()) {
        if (edge->getDest()->getInfo() == destination) {
            adjacent = true;
            break;
        }
    }
    if (adjacent) {
        errorMessage = "Source and destination are adjacent.";
        return bestRoute;
    }

    // processes avoidSegments: removes those edges temporarily
    vector<tuple<string, string, double, double>> removedEdges;
    for (const auto& seg : avoidSegments) {
        auto v1 = graph.findVertex(seg.first);
        if (v1) {
            for (const auto edge : v1->getAdj()) {
                if (edge->getDest()->getInfo() == seg.second) {
                    removedEdges.push_back(make_tuple(seg.first, seg.second, edge->getDriving(), edge->getWalking()));
                    graph.removeEdge(seg.first, seg.second);
                    break;
                }
            }
        }
    }

    // goes through possible parking nodes (those with parking available)
    auto vertices = graph.getVertexSet();
    bool routeFound = false;
    for (const auto vertex : vertices) {
        string node = vertex->getInfo();
        if (node == source || node == destination)
            continue;
        // it has to have parking available
        if (parkingInfo.find(node) == parkingInfo.end() || !parkingInfo.at(node))
            continue;

        // route from source to candidate
        double drivingTime = 0.0;
        vector<string> drivingRoute = dijkstraDriving(source, node, drivingTime, avoidNodes);
        if (drivingRoute.empty())
            continue;

        // route cannot go through destination
        if (find(drivingRoute.begin(), drivingRoute.end(), destination) != drivingRoute.end())
            continue;

        // route from candidate (parking) to destination
        double walkingTime = 0.0;
        vector<string> walkingRoute = dijkstraWalking(node, destination, walkingTime, avoidNodes);
        if (walkingRoute.empty())
            continue;

        // checks if both segments have at leats 1 edge and if walking time is within the limit
        if (drivingRoute.size() < 2 || walkingRoute.size() < 2 || walkingTime > maxWalkTime)
            continue;

        double totalTime = drivingTime + walkingTime;
        // chooses the route with the lowest total time
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

    // recoves the edges that were temporarily removed (avoid segments)
    for (const auto& seg : removedEdges) {
        string from, to;
        double d, w;
        tie(from, to, d, w) = seg;
        graph.addEdge(from, to, d, w);
    }

    if (!routeFound)
        errorMessage = "No possible route with max. walking time of "+ to_string(maxWalkTime) + " minutes";

    return bestRoute;
}


vector<DrivingWalkingRoute> DrivingWalkingRoutePlanning::findApproximateRoutes(
    const string& source, const string& destination,
    int maxWalkTime,
    const unordered_set<string>& avoidNodes,
    const vector<pair<string, string>>& avoidSegments) {

    vector<DrivingWalkingRoute> suggestions;
    unordered_set<string> seenHashes;
    vector<int> increments = { maxWalkTime + 5, maxWalkTime + 10, maxWalkTime + 15, maxWalkTime + 20 };

    for (int newMaxWalkTime : increments) {
        auto candidates = findAllValidRoutes(source, destination, newMaxWalkTime, avoidNodes, avoidSegments);
        for (const auto& route : candidates) {
            string hash = route.parkingNode + "|" +
                          accumulate(route.drivingRoute.begin(), route.drivingRoute.end(), string()) + "|" +
                          accumulate(route.walkingRoute.begin(), route.walkingRoute.end(), string());

            if (seenHashes.count(hash) == 0) {
                seenHashes.insert(hash);
                suggestions.push_back(route);
                if (suggestions.size() == 2) break;
            }
        }
        if (suggestions.size() == 2) break;
    }

    sort(suggestions.begin(), suggestions.end(), [](const DrivingWalkingRoute& a, const DrivingWalkingRoute& b) {
        return a.totalTime < b.totalTime;
    });

    return suggestions;
}


vector<DrivingWalkingRoute> DrivingWalkingRoutePlanning::findAllValidRoutes(
    const string& source, const string& destination,
    int maxWalkTime,
    const unordered_set<string>& avoidNodes,
    const vector<pair<string, string>>& avoidSegments) {

    vector<DrivingWalkingRoute> validRoutes;

    // Validation
    auto srcVertex = graph.findVertex(source);
    auto destVertex = graph.findVertex(destination);
    if (!srcVertex || !destVertex) return validRoutes;

    if ((parkingInfo.find(source) != parkingInfo.end() && parkingInfo.at(source)) ||
        (parkingInfo.find(destination) != parkingInfo.end() && parkingInfo.at(destination)))
        return validRoutes;

    // checks if source and destination are adjacent
    for (const auto& edge : srcVertex->getAdj()) {
        if (edge->getDest()->getInfo() == destination)
            return validRoutes;
    }

    // removes avoidSegments temporarily
    vector<tuple<string, string, double, double>> removedEdges;
    for (const auto& seg : avoidSegments) {
        auto v1 = graph.findVertex(seg.first);
        if (v1) {
            for (const auto& edge : v1->getAdj()) {
                if (edge->getDest()->getInfo() == seg.second) {
                    removedEdges.emplace_back(seg.first, seg.second, edge->getDriving(), edge->getWalking());
                    graph.removeEdge(seg.first, seg.second);
                    break;
                }
            }
        }
    }

    for (const auto& vertex : graph.getVertexSet()) {
        string candidate = vertex->getInfo();
        if (candidate == source || candidate == destination) continue;
        if (parkingInfo.find(candidate) == parkingInfo.end() || !parkingInfo.at(candidate)) continue;

        double drivingTime = 0.0;
        auto drivingRoute = dijkstraDriving(source, candidate, drivingTime, avoidNodes);
        if (drivingRoute.empty()) continue;
        if (find(drivingRoute.begin(), drivingRoute.end(), destination) != drivingRoute.end()) continue;

        double walkingTime = 0.0;
        auto walkingRoute = dijkstraWalking(candidate, destination, walkingTime, avoidNodes);
        if (walkingRoute.empty()) continue;

        if (drivingRoute.size() < 2 || walkingRoute.size() < 2 || walkingTime > maxWalkTime) continue;

        DrivingWalkingRoute route;
        route.drivingRoute = drivingRoute;
        route.parkingNode = candidate;
        route.walkingRoute = walkingRoute;
        route.drivingTime = drivingTime;
        route.walkingTime = walkingTime;
        route.totalTime = drivingTime + walkingTime;
        validRoutes.push_back(route);
    }

    // restore edges
    for (const auto& edge : removedEdges) {
        string from, to;
        double d, w;
        tie(from, to, d, w) = edge;
        graph.addEdge(from, to, d, w);
    }

    return validRoutes;
}
