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

// Dijkstra for walking segments using walking weights.
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

    // Ensure that the walking route has at least one edge.
    if (path.size() < 2)
        return {};

    return path;
}

// Helper for driving route: reuses the existing RoutePlanning::dijkstra.
// Note: We use a temporary RoutePlanning object.
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

    // Validação da existência de source e destination.
    auto srcVertex = graph.findVertex(source);
    auto destVertex = graph.findVertex(destination);
    if (!srcVertex || !destVertex) {
        errorMessage = "Source or destination does not exist.";
        return bestRoute;
    }

    // A origem e o destino não podem ser nós com estacionamento.
    if (parkingInfo.find(source) != parkingInfo.end() && parkingInfo.at(source)) {
        errorMessage = "Source is a parking node.";
        return bestRoute;
    }
    if (parkingInfo.find(destination) != parkingInfo.end() && parkingInfo.at(destination)) {
        errorMessage = "Destination is a parking node.";
        return bestRoute;
    }

    // A origem e o destino não podem ser nós adjacentes.
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

    // Processa os avoidSegments: remove temporariamente essas arestas.
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

    // Itera pelos nós candidatos para estacionamento (apenas os que têm estacionamento disponível).
    auto vertices = graph.getVertexSet();
    bool routeFound = false;
    for (const auto vertex : vertices) {
        string node = vertex->getInfo();
        if (node == source || node == destination)
            continue;
        // O nó candidato deve ter estacionamento disponível.
        if (parkingInfo.find(node) == parkingInfo.end() || !parkingInfo.at(node))
            continue;

        // Calcula a rota de condução de source até o nó candidato.
        double drivingTime = 0.0;
        vector<string> drivingRoute = dijkstraDriving(source, node, drivingTime, avoidNodes);
        if (drivingRoute.empty())
            continue;

        // Garante que a rota de condução não passe pelo destination.
        if (find(drivingRoute.begin(), drivingRoute.end(), destination) != drivingRoute.end())
            continue;

       /* // Verifica se há algum nó de estacionamento intermediário na rota de condução.
        bool intermediateParkingFound = false;
        for (size_t i = 1; i < drivingRoute.size() - 1; i++) {
            string intermediate = drivingRoute[i];
            if (parkingInfo.find(intermediate) != parkingInfo.end() && parkingInfo.at(intermediate)) {
                intermediateParkingFound = true;
                break;
            }
        }
        if (intermediateParkingFound)
            continue;
        */
        // Calcula a rota de caminhada do nó candidato (parking) até o destination.
        double walkingTime = 0.0;
        vector<string> walkingRoute = dijkstraWalking(node, destination, walkingTime, avoidNodes);
        if (walkingRoute.empty())
            continue;

        // Verifica se ambos os segmentos possuem pelo menos uma aresta e se o tempo de caminhada está dentro do limite.
        if (drivingRoute.size() < 2 || walkingRoute.size() < 2 || walkingTime > maxWalkTime)
            continue;

        double totalTime = drivingTime + walkingTime;
        // Seleciona a rota com o menor tempo total; em caso de empate, escolhe a com maior tempo de caminhada.
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

    // Restaura as arestas removidas temporariamente (avoidSegments).
    for (const auto& seg : removedEdges) {
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
    // Relax the maximum walking time constraint by fixed increments (e.g., +5 and +10).
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
