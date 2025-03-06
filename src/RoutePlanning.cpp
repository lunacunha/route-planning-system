#include "RoutePlanning.h"
#include <queue>
#include <unordered_map>
#include <limits>

RoutePlanning::RoutePlanning(Graph<std::string>& graph) : graph(graph) {}

std::vector<std::string> RoutePlanning::dijkstra(const std::string& source, const std::string& destination, double& totalTime) {
  std::unordered_map<std::string, double> dist;
  std::unordered_map<std::string, std::string> pred;
  std::priority_queue<std::pair<double, std::string>, std::vector<std::pair<double, std::string>>, std::greater<>> pq;

  // Inicializar distâncias
  for (auto vertex : graph.getVertexSet()) {
    dist[vertex->getInfo()] = std::numeric_limits<double>::max();
    pred[vertex->getInfo()] = "";
  }
  dist[source] = 0.0;
  pq.push({0.0, source});

  // Algoritmo de Dijkstra
  while (!pq.empty()) {
    std::string u = pq.top().second;
    double d_u = pq.top().first;
    pq.pop();

    if (d_u > dist[u]) continue;

    Vertex<std::string>* vertex = graph.findVertex(u);
    if (!vertex) continue;

    for (auto edge : vertex->getAdj()) {
      std::string v = edge->getDest()->getInfo();
      double weight = edge->getDriving(); // Usamos o tempo de condução como peso

      if (dist[u] + weight < dist[v]) {
        dist[v] = dist[u] + weight;
        pred[v] = u;
        pq.push({dist[v], v});
      }
    }
  }

  // Reconstruir caminho
  std::vector<std::string> path;
  std::string current = destination;
  totalTime = dist[destination];

  if (totalTime == std::numeric_limits<double>::max()) {
    return {}; // Sem caminho possível
  }

  while (current != "") {
    path.insert(path.begin(), current);
    current = pred[current];
  }

  return path;
}
