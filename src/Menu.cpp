//
// Created by Luna on 22/02/2025.
//

#include <string>
#include <iostream>
#include "GraphInterface.h"
#include "Graph.h"
#include <fstream>
#include "RoutePlanning.h"
#include "DrivingWalkingRoutePlanning.h"
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
    graphInterface.loadLocations("/home/martagfmartins/Desktop/faculdade/da/project1_DA/smallDataset2/smallLocations2.csv");
    graphInterface.loadDistances("/home/martagfmartins/Desktop/faculdade/da/project1_DA/smallDataset2/smallDistances2.csv");
    Graph<string>& graph = graphInterface.getGraph();
    RoutePlanning planner(graph);

    // User Input for Source and Destination
    string sourceLocation, destinationLocation;
    cout << "Enter Source Location ID: ";
    cin >> sourceLocation;

    // Trim spaces
    sourceLocation.erase(0, sourceLocation.find_first_not_of(" \t\r\n"));
    sourceLocation.erase(sourceLocation.find_last_not_of(" \t\r\n") + 1);

    // Validate that only a single location ID was entered
    if (sourceLocation.find(",") != string::npos) {
        cout << "Error: Invalid source location format. Enter only a single location ID.\n";
        return;
    }

    cout << "Enter Destination Location ID: ";
    cin >> destinationLocation;

    // Trim spaces
    destinationLocation.erase(0, destinationLocation.find_first_not_of(" \t\r\n"));
    destinationLocation.erase(destinationLocation.find_last_not_of(" \t\r\n") + 1);

    // Validate that only a single location ID was entered
    if (destinationLocation.find(",") != string::npos) {
        cout << "Error: Invalid destination location format. Enter only a single location ID.\n";
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
    graphInterface.loadLocations("/home/martagfmartins/Desktop/faculdade/da/project1_DA/smallDataset2/smallLocations2.csv");
    graphInterface.loadDistances("/home/martagfmartins/Desktop/faculdade/da/project1_DA/smallDataset2/smallDistances2.csv");
    Graph<string>& graph = graphInterface.getGraph();
    RoutePlanning planner(graph);

    // User Input
    string sourceLocation, destinationLocation, includeNode;
    cout << "Enter Source Location ID: ";
    cin >> sourceLocation;
    cout << "Enter Destination Location ID: ";
    cin >> destinationLocation;

    cin.ignore();

    unordered_set<string> avoidNodes;
    vector<pair<string, string>> avoidSegments;

    // Ask user if they want to avoid nodes
    cout << "Enter nodes to avoid (comma-separated, or leave empty): ";
    string avoidNodesInput;
    getline(cin, avoidNodesInput);

    // Validate avoidNodes format: No parentheses allowed, only commas
    if (!avoidNodesInput.empty() && (avoidNodesInput.find("(") != string::npos || avoidNodesInput.find(")") != string::npos)) {
        cout << "Error: Invalid format for avoidNodes. Use comma-separated values without parentheses.\n";
        return; // Exit function and ask for input again
    }

    // Process avoidNodes input (only if it's valid)
    istringstream avoidNodesStream(avoidNodesInput);
    string node;
    while (getline(avoidNodesStream, node, ',')) {
        avoidNodes.insert(node);
    }

    // Ask user if they want to avoid segments
    cout << "Enter segments to avoid in format (id,id),(id,id) or leave empty: ";
    string avoidSegmentsInput;
    getline(cin, avoidSegmentsInput);
    if (!avoidSegmentsInput.empty()) {
        if (avoidSegmentsInput.find("(") == string::npos || avoidSegmentsInput.find(")") == string::npos) {
            cout << "Error: Invalid format for segments. Use (id,id),(id,id).\n";
            return; // Exit function
        }

        size_t pos = 0;
        while ((pos = avoidSegmentsInput.find("(")) != string::npos) {
            size_t endPos = avoidSegmentsInput.find(")");
            if (endPos == string::npos) {
                cout << "Error: Invalid format for segments. Ensure each segment is in (id,id) format.\n";
                return;
            }
            string segment = avoidSegmentsInput.substr(pos + 1, endPos - pos - 1);
            size_t commaPos = segment.find(",");

            if (commaPos == string::npos) {
                cout << "Error: Invalid segment format. Each segment must be (id,id).\n";
                return;
            }

            string from = segment.substr(0, commaPos);
            string to = segment.substr(commaPos + 1);
            avoidSegments.emplace_back(from, to);

            avoidSegmentsInput.erase(0, endPos + 1);
        }
    }

    // Ask for an include node
    cout << "Enter a mandatory node to include (or leave empty): ";
    getline(cin, includeNode);

    // Trim spaces
    includeNode.erase(0, includeNode.find_first_not_of(" \t\r\n"));
    includeNode.erase(includeNode.find_last_not_of(" \t\r\n") + 1);

    // Validate includeNode: It must be a single node (no commas)
    if (!includeNode.empty() && includeNode.find(",") != string::npos) {
        cout << "Error: Invalid format for node. Only enter a single node ID or leave empty.\n";
        return;
    }

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
    GraphInterface graphInterface;
    
    graphInterface.loadLocations("path/to/your/locations.csv");
    graphInterface.loadDistances("path/to/your/distances.csv");
    Graph<string>& graph = graphInterface.getGraph();
    const unordered_map<string, bool>& parkingInfo = graphInterface.getParkingInfo();
    
    DrivingWalkingRoutePlanning dwPlanner(graph, parkingInfo);
    
    string source, destination;
    int maxWalkTime;
    unordered_set<string> avoidNodes;
    vector<pair<string, string>> avoidSegments;
    
    cout << "Enter Source Location ID: ";
    cin >> source;
    cout << "Enter Destination Location ID: ";
    cin >> destination;
    cout << "Enter Maximum Walking Time: ";
    cin >> maxWalkTime;
    cin.ignore(); // Clear newline
    
    cout << "Enter avoid nodes (comma separated, or leave empty): ";
    string avoidNodesInput;
    getline(cin, avoidNodesInput);
    if (!avoidNodesInput.empty()) {
        istringstream iss(avoidNodesInput);
        string token;
        while(getline(iss, token, ',')) {
            avoidNodes.insert(token);
        }
    }
    
    cout << "Enter avoid segments (format: (id,id),(id,id) or leave empty): ";
    string avoidSegmentsInput;
    getline(cin, avoidSegmentsInput);
    if (!avoidSegmentsInput.empty()) {
        size_t pos = 0;
        while ((pos = avoidSegmentsInput.find("(")) != string::npos) {
            size_t endPos = avoidSegmentsInput.find(")", pos);
            if (endPos == string::npos)
                break;
            string segment = avoidSegmentsInput.substr(pos + 1, endPos - pos - 1);
            size_t commaPos = segment.find(",");
            if (commaPos != string::npos) {
                string from = segment.substr(0, commaPos);
                string to = segment.substr(commaPos + 1);
                avoidSegments.push_back({from, to});
            }
            avoidSegmentsInput.erase(0, endPos + 1);
        }
    }
    
    string errorMessage;
    auto route = dwPlanner.findBestRoute(source, destination, maxWalkTime, avoidNodes, avoidSegments, errorMessage);
    
    cout << "\n===== Driving & Walking Route Results =====" << endl;
    cout << "Source:" << source << endl;
    cout << "Destination:" << destination << endl;
    if (route.drivingRoute.empty() || route.walkingRoute.empty() || route.parkingNode.empty()) {
        cout << "DrivingRoute:none" << endl;
        cout << "ParkingNode:none" << endl;
        cout << "WalkingRoute:none" << endl;
        cout << "TotalTime:" << endl;
        cout << "Message:" << errorMessage << endl;
    } else {
        cout << "DrivingRoute:";
        for (size_t i = 0; i < route.drivingRoute.size(); i++) {
            cout << route.drivingRoute[i];
            if (i < route.drivingRoute.size() - 1)
                cout << ",";
        }
        cout << "(" << route.drivingTime << ")" << endl;
        
        cout << "ParkingNode:" << route.parkingNode << endl;
        
        cout << "WalkingRoute:";
        for (size_t i = 0; i < route.walkingRoute.size(); i++) {
            cout << route.walkingRoute[i];
            if (i < route.walkingRoute.size() - 1)
                cout << ",";
        }
        cout << "(" << route.walkingTime << ")" << endl;
        
        cout << "TotalTime:" << "(" << route.totalTime << ")" << endl;
    }
    
    // Write results to output file.
    ofstream outputFile("output.txt", ios::trunc);
    if (outputFile.is_open()) {
        outputFile << "Source:" << source << "\n";
        outputFile << "Destination:" << destination << "\n";
        if (route.drivingRoute.empty() || route.walkingRoute.empty() || route.parkingNode.empty()) {
            outputFile << "DrivingRoute:none\n";
            outputFile << "ParkingNode:none\n";
            outputFile << "WalkingRoute:none\n";
            outputFile << "TotalTime:\n";
            outputFile << "Message:" << errorMessage << "\n";
        } else {
            outputFile << "DrivingRoute:";
            for (size_t i = 0; i < route.drivingRoute.size(); i++) {
                outputFile << route.drivingRoute[i];
                if (i < route.drivingRoute.size() - 1)
                    outputFile << ",";
            }
            outputFile << "(" << route.drivingTime << ")\n";
            outputFile << "ParkingNode:" << route.parkingNode << "\n";
            outputFile << "WalkingRoute:";
            for (size_t i = 0; i < route.walkingRoute.size(); i++) {
                outputFile << route.walkingRoute[i];
                if (i < route.walkingRoute.size() - 1)
                    outputFile << ",";
            }
            outputFile << "(" << route.walkingTime << ")\n";
            outputFile << "TotalTime:" << "(" << route.totalTime << ")\n";
        }
        outputFile.close();
        cout << "Results written to output.txt" << endl;
    }
}

void Menu::approximateSolutions() {
    cout << "Approximate Solutions selected." << endl;
    GraphInterface graphInterface;
    
    graphInterface.loadLocations("path/to/your/locations.csv");
    graphInterface.loadDistances("path/to/your/distances.csv");
    Graph<string>& graph = graphInterface.getGraph();
    const unordered_map<string, bool>& parkingInfo = graphInterface.getParkingInfo();
    
    DrivingWalkingRoutePlanning dwPlanner(graph, parkingInfo);
    
    
    string source, destination;
    int maxWalkTime;
    unordered_set<string> avoidNodes;
    vector<pair<string, string>> avoidSegments;
    
    cout << "Enter Source Location ID: ";
    cin >> source;
    cout << "Enter Destination Location ID: ";
    cin >> destination;
    cout << "Enter Maximum Walking Time: ";
    cin >> maxWalkTime;
    cin.ignore();
    
    cout << "Enter avoid nodes (comma separated, or leave empty): ";
    string avoidNodesInput;
    getline(cin, avoidNodesInput);
    if (!avoidNodesInput.empty()) {
        istringstream iss(avoidNodesInput);
        string token;
        while(getline(iss, token, ',')) {
            avoidNodes.insert(token);
        }
    }
    
    cout << "Enter avoid segments (format: (id,id),(id,id) or leave empty): ";
    string avoidSegmentsInput;
    getline(cin, avoidSegmentsInput);
    if (!avoidSegmentsInput.empty()) {
        size_t pos = 0;
        while ((pos = avoidSegmentsInput.find("(")) != string::npos) {
            size_t endPos = avoidSegmentsInput.find(")", pos);
            if (endPos == string::npos)
                break;
            string segment = avoidSegmentsInput.substr(pos + 1, endPos - pos - 1);
            size_t commaPos = segment.find(",");
            if (commaPos != string::npos) {
                string from = segment.substr(0, commaPos);
                string to = segment.substr(commaPos + 1);
                avoidSegments.push_back({from, to});
            }
            avoidSegmentsInput.erase(0, endPos + 1);
        }
    }
    
    auto alternatives = dwPlanner.findApproximateRoutes(source, destination, maxWalkTime, avoidNodes, avoidSegments);
    cout << "\n===== Approximate Routes =====" << endl;
    if (alternatives.empty()) {
        cout << "No approximate routes found." << endl;
    } else {
        for (size_t i = 0; i < alternatives.size(); i++) {
            cout << "DrivingRoute" << i+1 << ":";
            for (size_t j = 0; j < alternatives[i].drivingRoute.size(); j++) {
                cout << alternatives[i].drivingRoute[j];
                if (j < alternatives[i].drivingRoute.size() - 1)
                    cout << ",";
            }
            cout << "(" << alternatives[i].drivingTime << ")" << endl;
            
            cout << "ParkingNode" << i+1 << ":" << alternatives[i].parkingNode << endl;
            
            cout << "WalkingRoute" << i+1 << ":";
            for (size_t j = 0; j < alternatives[i].walkingRoute.size(); j++) {
                cout << alternatives[i].walkingRoute[j];
                if (j < alternatives[i].walkingRoute.size() - 1)
                    cout << ",";
            }
            cout << "(" << alternatives[i].walkingTime << ")" << endl;
            
            cout << "TotalTime" << i+1 << ":" << "(" << alternatives[i].totalTime << ")" << endl;
        }
    }
}

void Menu::end() {
    cout << "Exiting program..." << endl;
}
