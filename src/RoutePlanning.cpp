#include "RoutePlanning.h"
#include <queue>
#include <unordered_map>
#include <limits>
#include <iostream>

using namespace std;

RoutePlanning::RoutePlanning(Graph<std::string>& graph) : graph(graph) {}

vector<string> RoutePlanning::dijkstra(const string& sourceLocation, const string& destinationLocation, double& totalTravelTime) {
    unordered_map<string, double> minimumDistanceFromSource;
    unordered_map<string, string> previousLocationInPath;
    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> priorityQueue;

    // Check if source and destination exist
    Vertex<string>* sourceVertex = graph.findVertex(sourceLocation);
    Vertex<string>* destinationVertex = graph.findVertex(destinationLocation);

    if (!sourceVertex) {
        cerr << "Error: Source location " << sourceLocation << " does not exist in the graph!" << endl;
        return {};
    }
    if (!destinationVertex) {
        cerr << "Error: Destination location " << destinationLocation << " does not exist in the graph!" << endl;
        return {};
    }

    // Initialize distances
    for (auto vertex : graph.getVertexSet()) {
        minimumDistanceFromSource[vertex->getInfo()] = numeric_limits<double>::max();
        previousLocationInPath[vertex->getInfo()] = "";
    }

    minimumDistanceFromSource[sourceLocation] = 0.0;
    priorityQueue.push({0.0, sourceLocation});

    // Dijkstra's Algorithm
    while (!priorityQueue.empty()) {
        string currentLocation = priorityQueue.top().second;
        double currentMinimumDistance = priorityQueue.top().first;
        priorityQueue.pop();

        if (currentMinimumDistance > minimumDistanceFromSource[currentLocation]) continue;

        Vertex<string>* vertex = graph.findVertex(currentLocation);
        if (!vertex) continue;

        for (auto edge : vertex->getAdj()) {
            string neighborLocation = edge->getDest()->getInfo();
            double weight = edge->getDriving();

            // Ensure we don't consider edges marked as "no driving" (-1)
            if (weight < 0) {
                continue;
            }

            if (minimumDistanceFromSource[currentLocation] + weight < minimumDistanceFromSource[neighborLocation]) {
                minimumDistanceFromSource[neighborLocation] = minimumDistanceFromSource[currentLocation] + weight;
                previousLocationInPath[neighborLocation] = currentLocation;
                priorityQueue.push({minimumDistanceFromSource[neighborLocation], neighborLocation});
            }
        }
    }

    // Reconstruct the path
    vector<string> path;
    string current = destinationLocation;
    totalTravelTime = minimumDistanceFromSource[destinationLocation];

    if (totalTravelTime == numeric_limits<double>::max()) {
        return {}; // No possible route
    }

    while (!current.empty()) {
        path.insert(path.begin(), current);
        current = previousLocationInPath[current];
    }

    return path;
}

vector<string> RoutePlanning::dijkstra(const string& sourceLocation, const string& destinationLocation, double& totalTravelTime, const unordered_set<string>& avoidNodes) {
    unordered_map<string, double> minimumDistanceFromSource;
    unordered_map<string, string> previousLocationInPath;
    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> priorityQueue;

    // Check if source and destination exist
    Vertex<string>* sourceVertex = graph.findVertex(sourceLocation);
    Vertex<string>* destinationVertex = graph.findVertex(destinationLocation);

    if (!sourceVertex) {
        cerr << "Error: Source location " << sourceLocation << " does not exist in the graph!" << endl;
        return {};
    }
    if (!destinationVertex) {
        cerr << "Error: Destination location " << destinationLocation << " does not exist in the graph!" << endl;
        return {};
    }

    // Initialize distances
    for (auto vertex : graph.getVertexSet()) {
        if (avoidNodes.find(vertex->getInfo()) != avoidNodes.end()) {
            minimumDistanceFromSource[vertex->getInfo()] = numeric_limits<double>::max(); // Mark avoided nodes as unreachable
        } else {
            minimumDistanceFromSource[vertex->getInfo()] = numeric_limits<double>::max();
            previousLocationInPath[vertex->getInfo()] = "";
        }
    }

    minimumDistanceFromSource[sourceLocation] = 0.0;
    priorityQueue.push({0.0, sourceLocation});

    // Dijkstra's Algorithm
    while (!priorityQueue.empty()) {
        string currentLocation = priorityQueue.top().second;
        double currentMinimumDistance = priorityQueue.top().first;
        priorityQueue.pop();

        if (currentMinimumDistance > minimumDistanceFromSource[currentLocation]) continue;

        // Skip avoided nodes
        if (avoidNodes.find(currentLocation) != avoidNodes.end()) continue;

        Vertex<string>* vertex = graph.findVertex(currentLocation);
        if (!vertex) continue;

        for (auto edge : vertex->getAdj()) {
            string neighborLocation = edge->getDest()->getInfo();
            double weight = edge->getDriving();

            if (weight < 0 || avoidNodes.find(neighborLocation) != avoidNodes.end()) { // Avoid nodes and ignore "no driving" edges
                continue;
            }

            if (minimumDistanceFromSource[currentLocation] + weight < minimumDistanceFromSource[neighborLocation]) {
                minimumDistanceFromSource[neighborLocation] = minimumDistanceFromSource[currentLocation] + weight;
                previousLocationInPath[neighborLocation] = currentLocation;
                priorityQueue.push({minimumDistanceFromSource[neighborLocation], neighborLocation});
            }
        }
    }

    // Reconstruct the path
    vector<string> path;
    string current = destinationLocation;
    totalTravelTime = minimumDistanceFromSource[destinationLocation];

    if (totalTravelTime == numeric_limits<double>::max()) {
        return {}; // No possible route
    }

    while (!current.empty()) {
        path.insert(path.begin(), current);
        current = previousLocationInPath[current];
    }

    return path;
}


pair<vector<string>, vector<string>> RoutePlanning::findIndependentRoutes(const string& sourceLocation, const string& destinationLocation, double& bestRouteTime, double& alternativeRouteTime) {
    // Verifica se source e destination existem
    Vertex<string>* sourceVertex = graph.findVertex(sourceLocation);
    Vertex<string>* destinationVertex = graph.findVertex(destinationLocation);

    if (!sourceVertex || !destinationVertex) {
        cerr << "Error: One or both locations do not exist in the graph!" << endl;
        return { {}, {} };
    }

    // Encontra a melhor rota (mais rápida)
    vector<string> bestRoute = dijkstra(sourceLocation, destinationLocation, bestRouteTime);
    if (bestRoute.empty()) {
        cerr << "No valid best route found!" << endl;
        return { {}, {} };
    }

    // Cria um conjunto com os nós intermediários da melhor rota (exclui source e destination)
    unordered_set<string> avoidNodes;
    for (size_t i = 1; i < bestRoute.size() - 1; ++i) {
        avoidNodes.insert(bestRoute[i]);
    }

    // Encontra rota alternativa, evitando os nós da melhor rota
    vector<string> alternativeRoute = dijkstra(sourceLocation, destinationLocation, alternativeRouteTime, avoidNodes);

    // Verifica se a rota alternativa é válida e tem tempo >= tempo da principal
    if (!alternativeRoute.empty() && alternativeRouteTime < bestRouteTime) {
        // Se for mais rápida, então não é válida -> rejeitar
        alternativeRoute.clear();
        alternativeRouteTime = 0.0;
    }

    return { bestRoute, alternativeRoute };
}

vector<string> RoutePlanning::findRestrictedRoute(const string& sourceLocation, const string& destinationLocation,
                                                  const unordered_set<string>& avoidNodes,
                                                  const vector<pair<string, string>>& avoidSegments,
                                                  const string& includeNode, double& totalTravelTime) {
    // Ensure source and destination exist
    Vertex<string>* sourceVertex = graph.findVertex(sourceLocation);
    Vertex<string>* destinationVertex = graph.findVertex(destinationLocation);

    if (!sourceVertex || !destinationVertex) {
        cerr << "Error: Source or Destination does not exist!" << endl;
        return {};
    }

    // Temporarily store removed edges with original weights
    vector<tuple<string, string, double, double>> removedEdges;
    for (const auto& segment : avoidSegments) {
        Vertex<string>* v1 = graph.findVertex(segment.first);
        if (v1) {
            for (auto edge : v1->getAdj()) {
                if (edge->getDest()->getInfo() == segment.second) {
                    removedEdges.push_back(make_tuple(segment.first, segment.second, edge->getDriving(), edge->getWalking()));
                    graph.removeEdge(segment.first, segment.second);
                    break;
                }
            }
        }


        Vertex<string>* v2 = graph.findVertex(segment.second);
        if (v2) {
            for (const auto edge : v2->getAdj()) {
                if (edge->getDest()->getInfo() == segment.first) {
                    removedEdges.push_back(make_tuple(segment.second, segment.first, edge->getDriving(), edge->getWalking()));
                    graph.removeEdge(segment.second, segment.first);
                    break;
                }
            }
        }
    }

    vector<string> finalRoute;
    totalTravelTime = 0.0;

    if (!includeNode.empty()) {
        // If an includeNode is specified, split the search into two parts
        double firstPartTime, secondPartTime;
        vector<string> firstPart = dijkstra(sourceLocation, includeNode, firstPartTime, avoidNodes);
        vector<string> secondPart = dijkstra(includeNode, destinationLocation, secondPartTime, avoidNodes);

        if (firstPart.empty() || secondPart.empty()) {
            cerr << "No valid route passing through includeNode!" << endl;
            finalRoute = {}; // No valid route
        } else {
            // Merge the two routes (avoiding duplicate includeNode)
            firstPart.pop_back();
            finalRoute.insert(finalRoute.end(), firstPart.begin(), firstPart.end());
            finalRoute.insert(finalRoute.end(), secondPart.begin(), secondPart.end());
            totalTravelTime = firstPartTime + secondPartTime;
        }
    } else {
        // Normal Dijkstra search while avoiding nodes
        finalRoute = dijkstra(sourceLocation, destinationLocation, totalTravelTime, avoidNodes);
    }

    // Restore removed edges
    for (const auto& segment : removedEdges) {
        graph.addEdge(get<0>(segment), get<1>(segment), get<2>(segment), get<3>(segment));
    }

    return finalRoute;
}


