//
// Created by Luna on 22/02/2025.
//

#include "GraphInterface.h"


GraphInterface::GraphInterface() {
    graph = Graph<string>(); // Inicializar o grafo
}


// auxiliar function to ensure the string does not have additional spaces
string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}


void GraphInterface::loadLocations(const string &filename) {
    ifstream fileToLoad(filename);
    if (!fileToLoad.is_open()) {
        return;
    }

    string line;
    getline(fileToLoad, line); // Ignorar cabeçalho

    while (getline(fileToLoad, line)) {
        istringstream iss(line);
        string element;
        vector<string> elements;

        while (getline(iss, element, ',')) {
            elements.push_back(trim(element));
        }

        if (elements.size() != 4) {
            continue;
        }

        string locationCode = elements[2];
        bool hasParking;

        if (elements[3] != "0" && elements[3] != "1") {
            continue;
        }
        hasParking = (elements[3] == "1");

        graph.addVertex(locationCode);
    }
}


void GraphInterface::loadDistances(const string &filename) {
    ifstream fileToLoad(filename);
    if (!fileToLoad.is_open()) {
        return;
    }

    string line;
    getline(fileToLoad, line); // Ignorar cabeçalho

    while (getline(fileToLoad, line)) {
        istringstream iss(line);
        string element;
        vector<string> elements;

        while (getline(iss, element, ',')) {
            elements.push_back(trim(element));
        }

        if (elements.size() != 4) {
            continue;
        }

        string location1 = elements[0];
        string location2 = elements[1];
        int drivingTime = -1, walkingTime = -1;

        if (graph.findVertex(location1) == nullptr || graph.findVertex(location2) == nullptr) {
            continue;
        }

        if (elements[2] == "X") {
            drivingTime = -1;
        } else if (all_of(elements[2].begin(), elements[2].end(), ::isdigit)) {
            drivingTime = stoi(elements[2]);
        } else {
            continue;
        }

        if (all_of(elements[3].begin(), elements[3].end(), ::isdigit)) {
            walkingTime = stoi(elements[3]);
        } else {
            continue;
        }

        graph.addEdge(location1, location2, drivingTime, walkingTime);
    }
}