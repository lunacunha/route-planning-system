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
        cerr << "Couldn't open file: " << filename << endl;
        return;
    }

    string line;
    getline(fileToLoad, line); // skip header line

    while (getline(fileToLoad, line)) {
        istringstream iss(line);
        string element;
        vector<string> elements;

        // splitting the line by commas
        while (getline(iss, element, ',')) {
            elements.push_back(element);
        }

        if (elements.size() == 4) {
            string locationName = elements[0];
            string locationId = elements[1];
            string locationCode = elements[2];
            bool hasParking;

            try {
                hasParking = (stoi(elements[3]) == 1);  // parking is 0 or 1
            } catch (exception &e) {
                cerr << "Error converting values: " << line << endl;
                continue;
            }

            graph.addVertex(locationId);
        }
    }
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

        // splitting the line by commas
        while (getline(iss, element, ',')) {
            elements.push_back(element);
        }

        if (elements.size() == 4) {
            string location1 = elements[0];
            string location2 = elements[1];

            int drivingTime;
            int walkingTime;

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


// auxiliar function to ensure the string does not have additional spaces
string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}
