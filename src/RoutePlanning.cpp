#include "RoutePlanning.h"
#include <queue>
#include <unordered_map>
#include <limits>
using namespace std;

RoutePlanning::RoutePlanning(Graph<std::string>& graph) : graph(graph) {}

vector<string> RoutePlanning::dijkstra(const string& source, const string& destination, double& totalTime) {
  unordered_map<string, double> dist;
  unordered_map<string, string> pred;
  priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> pq;

  // Inicializar distâncias
  for (auto vertex : graph.getVertexSet()) {
    dist[vertex->getInfo()] = numeric_limits<double>::max();
    pred[vertex->getInfo()] = "";
  }
  dist[source] = 0.0;
  pq.push({0.0, source});

  // Algoritmo de Dijkstra
  while (!pq.empty()) {
    string u = pq.top().second;
    double d_u = pq.top().first;
    pq.pop();

    if (d_u > dist[u]) continue;

    Vertex<string>* vertex = graph.findVertex(u);
    if (!vertex) continue;

    for (auto edge : vertex->getAdj()) {
      string v = edge->getDest()->getInfo();
      double weight = edge->getDriving(); // Usamos o tempo de condução como peso

      if (dist[u] + weight < dist[v]) {
        dist[v] = dist[u] + weight;
        pred[v] = u;
        pq.push({dist[v], v});
      }
    }
  }

  // Reconstruir caminho
  vector<string> path;
  string current = destination;
  totalTime = dist[destination];

  if (totalTime == numeric_limits<double>::max()) {
    return {}; // Sem caminho possível
  }

  while (current != "") {
    path.insert(path.begin(), current);
    current = pred[current];
  }

  return path;
}
