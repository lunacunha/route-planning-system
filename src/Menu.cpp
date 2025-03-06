//
// Created by Luna on 22/02/2025.
//

#include <string>
#include <iostream>
#include "GraphInterface.h"
#include "Graph.h"
#include <fstream>
#include "RoutePlanning.h"
#include "Menu.h"
#include "GraphInterface.h"
using namespace std;

Menu::Menu() {
    string inp;
    while (true) {
        cout <<"################################" << "\n"
             <<"#     Select a dataset:        #" << "\n"
             <<"#------------------------------#" << "\n"
             <<"#   1 -> Small dataset         #" << "\n"
             <<"#   2 -> Large dataset         #" << "\n"
             <<"#   E -> EXIT                  #" << "\n"
             <<"################################" << "\n"
             <<"Option: ";

        cin >> inp;

        if (inp == "1") {
            //loadData(1);
            init();
        } else if (inp == "2") {
            //loadData(2);
            init();
        } else if (inp == "e" || inp == "E") exit(0);
        else {
            cout << "Insert a valid input!" << endl;
            cin.clear();
            Menu();
        }
    }
}

void Menu::init() {
    string inp;
    while (true) {
        cout <<"################################" << "\n"
             <<"#      Route Planning Menu     #" << "\n"
             <<"#------------------------------#" << "\n"
             <<"#   1 -> Driving Only          #" << "\n"
             <<"#   2 -> Driving & Walking     #" << "\n"
             <<"#   B -> BACK                  #" << "\n"
             <<"#   E -> EXIT                  #" << "\n"
             <<"################################" << "\n"
             <<"Option: ";

        cin >> inp;

        if (inp == "1") {
            drivingOnlyMenu();
        } else if (inp == "2") {
            drivingWalkingMenu();
        } else if (inp == "B" || inp == "b") Menu();
        else if (inp == "e" || inp == "E") exit(0);
        else {
            cout << "Insert a valid input!" << endl;
            cin.clear();
            init();
        }
    }
}

void Menu::drivingOnlyMenu() {
    string inp;
    while (true) {
        cout <<"################################" << "\n"
             <<"#       Driving Only Menu      #" << "\n"
             <<"#------------------------------#" << "\n"
             <<"#   1 -> Best Route            #" << "\n"
             <<"#   2 -> Restricted Route      #" << "\n"
             <<"#   B -> BACK                  #" << "\n"
             <<"################################" << "\n"
             <<"Option: ";

        cin >> inp;

        if (inp == "1") {
            independentRoute();
        } else if (inp == "2") {
            restrictedRoute();
        } else if (inp == "B" || inp == "b") init();
        else {
            cout << "Insert a valid input!" << endl;
            cin.clear();
            drivingOnlyMenu();
        }
    }
}

void Menu::drivingWalkingMenu() {
    string inp;
    while (true) {
        cout <<"################################" << "\n"
             <<"#    Driving & Walking Menu    #" << "\n"
             <<"#------------------------------#" << "\n"
             <<"#   1 -> Best Route            #" << "\n"
             <<"#   2 -> Approximate Solutions #" << "\n"
             <<"#   B -> BACK                  #" << "\n"
             <<"################################" << "\n"
             <<"Option: ";

        cin >> inp;

        if (inp == "1") {
            bestDrivingWalkingRoute();
        } else if (inp == "2") {
            approximateSolutions();
        } else if (inp == "B" || inp == "b") init();
        else {
            cout << "Insert a valid input!" << endl;
            cin.clear();
            drivingWalkingMenu();
        }
    }
}

void Menu::independentRoute() {
    // Load the graph
    GraphInterface graphInterface;
    graphInterface.loadLocations("/home/martagfmartins/Desktop/faculdade/da/project1_DA/smallDataset/shortLocations.csv");
    graphInterface.loadDistances("/home/martagfmartins/Desktop/faculdade/da/project1_DA/smallDataset/shortDistances.csv");
    Graph<string>& graph = graphInterface.getGraph();
    RoutePlanning planner(graph);

    // User Input for Source and Destination
    string sourceLocation, destinationLocation;
    cout << "Enter Source Location ID: ";
    cin >> sourceLocation;
    cout << "Enter Destination Location ID: ";
    cin >> destinationLocation;

    // Trim spaces
    sourceLocation.erase(0, sourceLocation.find_first_not_of(" \t\r\n"));
    destinationLocation.erase(0, destinationLocation.find_first_not_of(" \t\r\n"));

    // Validate Source and Destination
    if (graph.findVertex(sourceLocation) == nullptr || graph.findVertex(destinationLocation) == nullptr) {
        cerr << "Error: Invalid source or destination!" << endl;
        return;
    }

    // Run Independent Route Planning
    double bestRouteTime, alternativeRouteTime;
    auto [bestRoute, alternativeRoute] = planner.findIndependentRoutes(sourceLocation, destinationLocation, bestRouteTime, alternativeRouteTime);

    // Display Results
    cout << "\n===== Driving Route Results =====" << endl;
    cout << "Source: " << sourceLocation << endl;
    cout << "Destination: " << destinationLocation << endl;

    if (bestRoute.empty()) {
        cout << "BestDrivingRoute: none" << endl;
        cout << "AlternativeDrivingRoute: none" << endl;
    } else {
        cout << "BestDrivingRoute: ";
        for (size_t i = 0; i < bestRoute.size(); i++) {
            cout << bestRoute[i];
            if (i < bestRoute.size() - 1) cout << ",";
        }
        cout << " (" << bestRouteTime << ")\n";

        if (alternativeRoute.empty()) {
            cout << "AlternativeDrivingRoute: none" << endl;
        } else {
            cout << "AlternativeDrivingRoute: ";
            for (size_t i = 0; i < alternativeRoute.size(); i++) {
                cout << alternativeRoute[i];
                if (i < alternativeRoute.size() - 1) cout << ",";
            }
            cout << " (" << alternativeRouteTime << ")\n";
        }
    }

    // Write results to output.txt
    ofstream outputFile("/home/martagfmartins/Desktop/faculdade/da/project1_DA/output.txt", ios::trunc);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not create output.txt" << endl;
        return;
    }

    outputFile << "Source:" << sourceLocation << endl;
    outputFile << "Destination:" << destinationLocation << endl;
    if (bestRoute.empty()) {
        outputFile << "BestDrivingRoute:none\n";
        outputFile << "AlternativeDrivingRoute:none\n";
    } else {
        outputFile << "BestDrivingRoute:";
        for (size_t i = 0; i < bestRoute.size(); i++) {
            outputFile << bestRoute[i];
            if (i < bestRoute.size() - 1) outputFile << ",";
        }
        outputFile << "(" << bestRouteTime << ")\n";

        if (alternativeRoute.empty()) {
            outputFile << "AlternativeDrivingRoute:none\n";
        } else {
            outputFile << "AlternativeDrivingRoute:";
            for (size_t i = 0; i < alternativeRoute.size(); i++) {
                outputFile << alternativeRoute[i];
                if (i < alternativeRoute.size() - 1) outputFile << ",";
            }
            outputFile << "(" << alternativeRouteTime << ")\n";
        }
    }
    outputFile.close();
    cout << "Results written to output.txt\n";
}


void Menu::restrictedRoute() {
    // Load Graph
    GraphInterface graphInterface;
    graphInterface.loadLocations("/home/martagfmartins/Desktop/faculdade/da/project1_DA/smallDataset/shortLocations.csv");
    graphInterface.loadDistances("/home/martagfmartins/Desktop/faculdade/da/project1_DA/smallDataset/shortDistances.csv");
    Graph<string>& graph = graphInterface.getGraph();
    RoutePlanning planner(graph);

    // User Input
    string sourceLocation, destinationLocation, includeNode;
    cout << "Enter Source Location ID: ";
    cin >> sourceLocation;
    cout << "Enter Destination Location ID: ";
    cin >> destinationLocation;

    unordered_set<string> avoidNodes;
    vector<pair<string, string>> avoidSegments;

    // Ask user if they want to avoid nodes
    cout << "Enter nodes to avoid (comma-separated, or leave empty): ";
    string avoidNodesInput;
    cin.ignore();
    getline(cin, avoidNodesInput);

    if (!avoidNodesInput.empty()) {
        istringstream ss(avoidNodesInput);
        string node;
        while (getline(ss, node, ',')) {
            avoidNodes.insert(node);
        }
    }

    // Ask user if they want to avoid segments
    cout << "Enter segments to avoid in format (id,id),(id,id) or leave empty: ";
    string avoidSegmentsInput;
    getline(cin, avoidSegmentsInput);

    if (!avoidSegmentsInput.empty()) {
        size_t pos = 0;
        while ((pos = avoidSegmentsInput.find("(")) != string::npos) {
            size_t endPos = avoidSegmentsInput.find(")");
            string segment = avoidSegmentsInput.substr(pos + 1, endPos - pos - 1);
            size_t commaPos = segment.find(",");
            if (commaPos != string::npos) {
                string from = segment.substr(0, commaPos);
                string to = segment.substr(commaPos + 1);
                avoidSegments.emplace_back(from, to);
            }
            avoidSegmentsInput.erase(0, endPos + 1);
        }
    }

    // Ask for an include node
    cout << "Enter a mandatory node to include (or leave empty): ";
    getline(cin, includeNode);

    // Run restricted route planning
    double totalTravelTime;
    vector<string> restrictedRoute = planner.findRestrictedRoute(
        sourceLocation, destinationLocation, avoidNodes, avoidSegments, includeNode, totalTravelTime);

    // Display Results
    cout << "\n===== Restricted Route Results =====" << endl;
    cout << "Source: " << sourceLocation << endl;
    cout << "Destination: " << destinationLocation << endl;

    if (restrictedRoute.empty()) {
        cout << "RestrictedDrivingRoute: none" << endl;
    } else {
        cout << "RestrictedDrivingRoute: ";
        for (size_t i = 0; i < restrictedRoute.size(); i++) {
            cout << restrictedRoute[i];
            if (i < restrictedRoute.size() - 1) cout << ",";
        }
        cout << " (" << totalTravelTime << ")\n";
    }

    // Write results to output.txt
    ofstream outputFile("/home/martagfmartins/Desktop/faculdade/da/project1_DA/output.txt", ios::trunc);
    outputFile << "Source:" << sourceLocation << endl;
    outputFile << "Destination:" << destinationLocation << endl;
    if (restrictedRoute.empty()) {
        outputFile << "RestrictedDrivingRoute:none\n";
    } else {
        outputFile << "RestrictedDrivingRoute:";
        for (size_t i = 0; i < restrictedRoute.size(); i++) {
            outputFile << restrictedRoute[i];
            if (i < restrictedRoute.size() - 1) outputFile << ",";
        }
        outputFile << "(" << totalTravelTime << ")\n";
    }
    outputFile.close();
    cout << "Results written to output.txt\n";
}


void Menu::bestDrivingWalkingRoute() {
    cout << "Best Route for Driving & Walking selected." << endl;
    // Chamar a função correspondente quando implementada
}

void Menu::approximateSolutions() {
    cout << "Approximate Solutions selected." << endl;
    // Chamar a função correspondente quando implementada
}

void Menu::end() {
    cout << "Exiting program..." << endl;
}
