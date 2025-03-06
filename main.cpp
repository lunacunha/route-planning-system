//
// Created by Luna on 22/02/2025.
//

#include "src/GraphInterface.h"
#include "src/Graph.h"
#include "src/RoutePlanning.h"
#include "src/Menu.h"
#include <iostream>

using namespace std;

int main() {
    GraphInterface graphInterface;
    graphInterface.loadLocations("smallDataset/shortLocations.csv");
    graphInterface.loadDistances("smallDataset/shortDistances.csv");

    Graph<std::string> graph = graphInterface.getGraph();
    // Criar a classe RoutePlanning usando o grafo carregado
    RoutePlanning planner(graphInterface.getGraph());

    // Definir nós de teste
    string start = "LD3372";  // Origem
    string end = "PR2490";       // Destino

    // Executar o Dijkstra
    double totalTime;  // Declaração correta da variável
    vector<string> path = planner.dijkstra(start, end, totalTime); // Corrigida a chamada


    // Exibir o resultado
    cout << "\n===== RESULTADO DO DIJKSTRA =====\n";
    if (path.empty()) {
        cout << "Nenhum caminho encontrado entre " << start << " e " << end << endl;
    } else {
        cout << "Melhor caminho: ";
        for (const auto &node : path) {
            cout << node << " -> ";
        }
        cout << "Fim (Tempo total: " << totalTime << " minutos)\n";
    }

    return 0;


    /*Menu menu;
    menu.init();
    Menu::end();
    return 0;*/
}
