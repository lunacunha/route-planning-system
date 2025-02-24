//
// Created by Luna on 22/02/2025.
//

#include "GraphInterface.h"


GraphInterface::GraphInterface() {
    graph = Graph<string>(); // Inicializar o grafo
}

void GraphInterface::loadLocations(const string &filename) {
    ifstream fileToLoad(filename);
    if (!fileToLoad.is_open()) {
        cerr << "Couldn't open file:" << filename << endl;
        return;
    }
    string line;
    getline(fileToLoad, line); // skip header line
}



void GraphInterface::loadDistances(const string &filename) {
    ifstream fileToLoad(filename);
    if (!fileToLoad.is_open()) {
        cerr << "Couldn't open file:" << filename << endl;
        return;
    }
    string line;
    getline(fileToLoad, line); // skip header line

    while(getline(fileToLoad, line)) {
        istringstream iss(line);
        string element;
        vector<string> elements;

        // Splitting the line by commas
        while (getline(iss, element, ',')) {
            elements.push_back(element);
        }

        if (elements.size() == 4) {
            string location1 = elements[0];
            string location2 = elements[1];

            int drivingTime;
            int walkingTime;

            cout << "Read line: " << line << endl;

            cout << "Element[0]: " << elements[0] << ", Element[1]: " << elements[1]
                 << ", Element[2]: " << elements[2] << ", Element[3]: " << elements[3] << endl;


            try {
                if (elements[2] == "X") {
                    drivingTime = -1; // X is no driving
                } else {
                    drivingTime = stoi(elements[2]);
                }

                walkingTime = stoi(elements[3]);

                // add edge
                graph.addEdge(location1, location2, drivingTime, walkingTime);
            }
            catch (const exception &e) {
                cerr << "Error in conversion." << endl;
            }
        }
    }
}