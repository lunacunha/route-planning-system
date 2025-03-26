// Menu.cpp
#include "Menu.h"
#include "GraphInterface.h"
#include "RoutePlanning.h"
#include "DrivingWalkingRoutePlanning.h"
#include "InputValidation.h" // Contém as funções auxiliares de validação
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <cmath>
#include <memory>
using namespace std;

string Menu::trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos || end == string::npos) ? "" : s.substr(start, end - start + 1);
}

// Função auxiliar para converter uma string para uppercase.
string Menu::toUpper(const string &s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c) { return toupper(c); });
    return result;
}

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
            loadData(1);
            init();
        } else if (inp == "2") {
            loadData(2);
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

void Menu::loadData(int option) {
    switch(option) {
        case 1:
            graphInterface.loadLocations("smallDataset/shortLocations.csv");
        graphInterface.loadDistances("smallDataset/shortDistances.csv");
        break;
        case 2:
            graphInterface.loadLocations("dataset/Locations.csv");
        graphInterface.loadDistances("dataset/Distances.csv");
        break;
        default:
            cerr << "Invalid dataset option provided to loadData." << endl;
        exit(1);
    }

    // Atualiza os ponteiros de membro
    this->graph = &graphInterface.getGraph();
    this->parkingInfo = &graphInterface.getParkingInfo();
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
             <<"#   B -> BACK                  #" << "\n"
             <<"################################" << "\n"
             <<"Option: ";

        cin >> inp;

        if (inp == "1") {
            bestDrivingWalkingRoute();
        } else if (inp == "B" || inp == "b") init();
        else {
            cout << "Insert a valid input!" << endl;
            cin.clear();
            drivingWalkingMenu();
        }
    }
}

void Menu::independentRoute() {
    if (!graph || !parkingInfo) {
        cout << "Error: Graph data not loaded.\n";
        return;
    }
    RoutePlanning planner(*graph);

    // User Input for Source and Destination
    string sourceLocation, destinationLocation;
    cout << "Enter Source Location ID: ";
    cin >> sourceLocation;
    cout << "Enter Destination Location ID: ";
    cin >> destinationLocation;

    // Trim and uppercase
    sourceLocation = toUpper(trim(sourceLocation));
    destinationLocation = toUpper(trim(destinationLocation));

    if (sourceLocation.find(",") != string::npos || destinationLocation.find(",") != string::npos) {
        cout << "Error: Invalid source or destination format. Use single location IDs.\n";
        return;
    }

    // input.txt
    ofstream inputFile("input.txt", ios::trunc);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not write input.txt\n";
        return;
    }
    inputFile << "Mode:driving\n";
    inputFile << "Source:" << sourceLocation << "\n";
    inputFile << "Destination:" << destinationLocation << "\n";
    inputFile.close();

    // Run route planner
    double bestRouteTime, alternativeRouteTime;
    auto [bestRoute, alternativeRoute] = planner.findIndependentRoutes(sourceLocation, destinationLocation, bestRouteTime, alternativeRouteTime);

    // output content
    stringstream output;
    output << "===== Driving Route Results =====\n";
    output << "Source:" << sourceLocation << "\n";
    output << "Destination:" << destinationLocation << "\n";

    if (bestRoute.empty()) {
        output << "BestDrivingRoute:none\n";
        output << "AlternativeDrivingRoute:none\n";
    } else {
        output << "BestDrivingRoute:";
        for (size_t i = 0; i < bestRoute.size(); ++i) {
            output << bestRoute[i];
            if (i < bestRoute.size() - 1) output << ",";
        }
        output << "(" << static_cast<int>(round(bestRouteTime)) << ")\n";

        if (alternativeRoute.empty()) {
            output << "AlternativeDrivingRoute:none\n";
        } else {
            output << "AlternativeDrivingRoute:";
            for (size_t i = 0; i < alternativeRoute.size(); ++i) {
                output << alternativeRoute[i];
                if (i < alternativeRoute.size() - 1) output << ",";
            }
            output << "(" << static_cast<int>(round(alternativeRouteTime)) << ")\n";
        }
    }

    // Print to terminal
    cout << output.str();

    // Write to output.txt
    ofstream outputFile("output.txt", ios::trunc);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not write output.txt\n";
        return;
    }

    // output.txt only needs content after Mode, so skip the header
    outputFile << "Source:" << sourceLocation << "\n";
    outputFile << "Destination:" << destinationLocation << "\n";

    if (bestRoute.empty()) {
        outputFile << "BestDrivingRoute:none\n";
        outputFile << "AlternativeDrivingRoute:none\n";
    } else {
        outputFile << "BestDrivingRoute:";
        for (size_t i = 0; i < bestRoute.size(); ++i) {
            outputFile << bestRoute[i];
            if (i < bestRoute.size() - 1) outputFile << ",";
        }
        outputFile << "(" << static_cast<int>(round(bestRouteTime)) << ")\n";

        if (alternativeRoute.empty()) {
            outputFile << "AlternativeDrivingRoute:none\n";
        } else {
            outputFile << "AlternativeDrivingRoute:";
            for (size_t i = 0; i < alternativeRoute.size(); ++i) {
                outputFile << alternativeRoute[i];
                if (i < alternativeRoute.size() - 1) outputFile << ",";
            }
            outputFile << "(" << static_cast<int>(round(alternativeRouteTime)) << ")\n";
        }
    }

    outputFile.close();
    cout << "Results written to output.txt\n";
}



void Menu::restrictedRoute() {

    if (!graph || !parkingInfo) {
        cout << "Error: Graph data not loaded.\n";
        return;
    }

    RoutePlanning planner(*graph);

    // User Input
    string sourceLocation, destinationLocation, includeNode;
    cout << "Enter Source Location ID: ";
    cin >> sourceLocation;
    cout << "Enter Destination Location ID: ";
    cin >> destinationLocation;
    cin.ignore();

    sourceLocation = toUpper(trim(sourceLocation));
    destinationLocation = toUpper(trim(destinationLocation));

    unordered_set<string> avoidNodesSet;
    vector<pair<string, string>> avoidSegments;

    cout << "Enter nodes to avoid (comma-separated, or leave empty): ";
    string avoidNodesInput;
    getline(cin, avoidNodesInput);
    if (!avoidNodesInput.empty()) {
        if (!isValidAvoidNodesFormat(avoidNodesInput)) {
            cout << "Error: Invalid format for avoid nodes.\n";
            return;
        }
        auto nodes = parseAvoidNodes(avoidNodesInput, *(new bool(true)));
        for (auto &n : nodes) avoidNodesSet.insert(toUpper(n));
    }

    cout << "Enter segments to avoid (format: (id,id),(id,id) or leave empty): ";
    string avoidSegmentsInput;
    getline(cin, avoidSegmentsInput);
    if (!avoidSegmentsInput.empty()) {
        if (!isValidAvoidSegmentsFormat(avoidSegmentsInput)) {
            cout << "Error: Invalid format for avoid segments.\n";
            return;
        }
        avoidSegments = parseAvoidSegments(avoidSegmentsInput, *(new bool(true)));
        for (auto &seg : avoidSegments) {
            seg.first = toUpper(seg.first);
            seg.second = toUpper(seg.second);
        }
    }

    cout << "Enter a mandatory node to include (or leave empty): ";
    getline(cin, includeNode);
    includeNode = toUpper(trim(includeNode));
    if (!includeNode.empty() && includeNode.find(",") != string::npos) {
        cout << "Error: Invalid format for include node.\n";
        return;
    }

    // Write input.txt
    ofstream inputFile("input.txt", ios::trunc);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not write input.txt\n";
        return;
    }
    inputFile << "Mode:driving\n";
    inputFile << "Source:" << sourceLocation << "\n";
    inputFile << "Destination:" << destinationLocation << "\n";
    inputFile << "AvoidNodes:" << avoidNodesInput << "\n";
    inputFile << "AvoidSegments:" << avoidSegmentsInput << "\n";
    inputFile << "IncludeNode:" << includeNode << "\n";
    inputFile.close();

    // Execute planning
    double totalTravelTime;
    auto restrictedRoute = planner.findRestrictedRoute(
        sourceLocation, destinationLocation, avoidNodesSet, avoidSegments, includeNode, totalTravelTime
    );

    // Build output
    stringstream output;
    output << "===== Restricted Route Results =====\n";
    output << "Source:" << sourceLocation << "\n";
    output << "Destination:" << destinationLocation << "\n";

    if (restrictedRoute.empty()) {
        output << "RestrictedDrivingRoute:none\n";
    } else {
        output << "RestrictedDrivingRoute:";
        for (size_t i = 0; i < restrictedRoute.size(); ++i) {
            output << restrictedRoute[i];
            if (i < restrictedRoute.size() - 1) output << ",";
        }
        output << "(" << static_cast<int>(round(totalTravelTime)) << ")\n";
    }

    // Terminal output
    cout << output.str();

    // Write to output.txt
    ofstream outputFile("output.txt", ios::trunc);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not write output.txt\n";
        return;
    }
    outputFile << "Source:" << sourceLocation << "\n";
    outputFile << "Destination:" << destinationLocation << "\n";
    if (restrictedRoute.empty()) {
        outputFile << "RestrictedDrivingRoute:none\n";
    } else {
        outputFile << "RestrictedDrivingRoute:";
        for (size_t i = 0; i < restrictedRoute.size(); ++i) {
            outputFile << restrictedRoute[i];
            if (i < restrictedRoute.size() - 1) outputFile << ",";
        }
        outputFile << "(" << static_cast<int>(round(totalTravelTime)) << ")\n";
    }

    outputFile.close();
    cout << "Results written to output.txt\n";
}


void Menu::bestDrivingWalkingRoute() {
    if (!graph || !parkingInfo) {
        cout << "Error: Graph or parking data not loaded.\n";
        return;
    }

    DrivingWalkingRoutePlanning dwPlanner(*graph, *parkingInfo);

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

    source = toUpper(source);
    destination = toUpper(destination);

    cout << "Enter avoid nodes (comma separated, or leave empty): ";
    string avoidNodesInput;
    getline(cin, avoidNodesInput);
    if (!avoidNodesInput.empty()) {
        if (!isValidAvoidNodesFormat(avoidNodesInput)) {
            cout << "Error: Invalid format for avoid nodes.\n";
            return;
        }
        vector<string> nodes = parseAvoidNodes(avoidNodesInput, *(new bool(true)));
        for (auto &token : nodes)
            avoidNodes.insert(toUpper(token));
    }

    cout << "Enter avoid segments (format: (id,id),(id,id) or leave empty): ";
    string avoidSegmentsInput;
    getline(cin, avoidSegmentsInput);
    if (!avoidSegmentsInput.empty()) {
        if (!isValidAvoidSegmentsFormat(avoidSegmentsInput)) {
            cout << "Error: Invalid format for avoid segments.\n";
            return;
        }
        avoidSegments = parseAvoidSegments(avoidSegmentsInput, *(new bool(true)));
        for (auto &seg : avoidSegments) {
            seg.first = toUpper(seg.first);
            seg.second = toUpper(seg.second);
        }
    }

    // input.txt
    ofstream inputFile("input.txt", ios::trunc);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not write input.txt" << endl;
        return;
    }
    inputFile << "Mode:driving-walking\n";
    inputFile << "Source:" << source << "\n";
    inputFile << "Destination:" << destination << "\n";
    inputFile << "MaxWalkTime:" << maxWalkTime << "\n";
    inputFile << "AvoidNodes:" << avoidNodesInput << "\n";
    inputFile << "AvoidSegments:" << avoidSegmentsInput << "\n";
    inputFile.close();

    // route
    string errorMessage;
    auto route = dwPlanner.findBestRoute(source, destination, maxWalkTime, avoidNodes, avoidSegments, errorMessage);

    stringstream terminalOutput;
    stringstream fileOutput;

    terminalOutput << "\n===== Driving & Walking Route Results =====\n";
    terminalOutput << "Source:" << source << "\n";
    terminalOutput << "Destination:" << destination << "\n";

    fileOutput << "Source:" << source << "\n";
    fileOutput << "Destination:" << destination << "\n";

    if (route.drivingRoute.empty() || route.walkingRoute.empty() || route.parkingNode.empty()) {
        terminalOutput << "DrivingRoute:none\n";
        terminalOutput << "ParkingNode:none\n";
        terminalOutput << "WalkingRoute:none\n";
        terminalOutput << "TotalTime:\n";
        terminalOutput << "Message:" << errorMessage << "\n";

        fileOutput << "DrivingRoute:none\n";
        fileOutput << "ParkingNode:none\n";
        fileOutput << "WalkingRoute:none\n";
        fileOutput << "TotalTime:\n";
        fileOutput << "Message:" << errorMessage << "\n";

        cout << terminalOutput.str();
        cout << "Results written to output.txt" << endl;

        // Write failure result to file
        ofstream outputFile("output.txt", ios::trunc);
        if (!outputFile.is_open()) {
            cerr << "Error: Could not write output.txt" << endl;
            return;
        }
        outputFile << fileOutput.str();
        outputFile.close();

        // ✅ Chama automaticamente a função de aproximação
        cout << "\nNo suitable route found. Looking for approximate solutions...\n";
        approximateSolutions(source, destination, maxWalkTime, avoidNodes, avoidSegments);
        return;
    }

    // Success case
    terminalOutput << "DrivingRoute:";
    fileOutput << "DrivingRoute:";
    for (size_t i = 0; i < route.drivingRoute.size(); i++) {
        terminalOutput << route.drivingRoute[i];
        fileOutput << route.drivingRoute[i];
        if (i < route.drivingRoute.size() - 1) {
            terminalOutput << ",";
            fileOutput << ",";
        }
    }
    terminalOutput << "(" << static_cast<int>(route.drivingTime) << ")\n";
    fileOutput << "(" << static_cast<int>(route.drivingTime) << ")\n";

    terminalOutput << "ParkingNode:" << route.parkingNode << "\n";
    fileOutput << "ParkingNode:" << route.parkingNode << "\n";

    terminalOutput << "WalkingRoute:";
    fileOutput << "WalkingRoute:";
    for (size_t i = 0; i < route.walkingRoute.size(); i++) {
        terminalOutput << route.walkingRoute[i];
        fileOutput << route.walkingRoute[i];
        if (i < route.walkingRoute.size() - 1) {
            terminalOutput << ",";
            fileOutput << ",";
        }
    }
    terminalOutput << "(" << static_cast<int>(route.walkingTime) << ")\n";
    fileOutput << "(" << static_cast<int>(route.walkingTime) << ")\n";

    terminalOutput << "TotalTime:(" << static_cast<int>(route.totalTime) << ")\n";
    fileOutput << "TotalTime:(" << static_cast<int>(route.totalTime) << ")\n";

    cout << terminalOutput.str();
    cout << "Results written to output.txt" << endl;

    ofstream outputFile("output.txt", ios::trunc);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not write output.txt" << endl;
        return;
    }
    outputFile << fileOutput.str();
    outputFile.close();
}


void Menu::approximateSolutions(const string& source, const string& destination,
                                int maxWalkTime,
                                const unordered_set<string>& avoidNodes,
                                const vector<pair<string, string>>& avoidSegments) {
    if (!graph || !parkingInfo) {
        cerr << "Error: Graph or parking data not loaded.\n";
        return;
    }

    DrivingWalkingRoutePlanning dwPlanner(*graph, *parkingInfo);

    // Obter até 2 rotas alternativas
    auto alternatives = dwPlanner.findApproximateRoutes(source, destination, maxWalkTime, avoidNodes, avoidSegments);

    // Terminal header
    stringstream terminalOutput;
    terminalOutput << "\n===== Approximate Routes =====\n";

    // Write header to file
    ofstream outputFile("output.txt", ios::trunc);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not write output.txt" << endl;
        return;
    }

    outputFile << "Source:" << source << "\n";
    outputFile << "Destination:" << destination << "\n";

    if (alternatives.empty()) {
        terminalOutput << "No approximate routes found.\n";

        for (int i = 1; i <= 2; ++i) {
            terminalOutput << "DrivingRoute" << i << ":none\n";
            terminalOutput << "ParkingNode" << i << ":none\n";
            terminalOutput << "WalkingRoute" << i << ":none\n";
            terminalOutput << "TotalTime" << i << ":\n";

            outputFile << "DrivingRoute" << i << ":none\n";
            outputFile << "ParkingNode" << i << ":none\n";
            outputFile << "WalkingRoute" << i << ":none\n";
            outputFile << "TotalTime" << i << ":\n";
        }
    } else {
        for (size_t i = 0; i < alternatives.size(); ++i) {
            const auto& alt = alternatives[i];

            // Driving Route
            terminalOutput << "DrivingRoute" << i + 1 << ":";
            outputFile << "DrivingRoute" << i + 1 << ":";
            for (size_t j = 0; j < alt.drivingRoute.size(); ++j) {
                terminalOutput << alt.drivingRoute[j];
                outputFile << alt.drivingRoute[j];
                if (j < alt.drivingRoute.size() - 1) {
                    terminalOutput << ",";
                    outputFile << ",";
                }
            }
            terminalOutput << "(" << static_cast<int>(alt.drivingTime) << ")\n";
            outputFile << "(" << static_cast<int>(alt.drivingTime) << ")\n";

            // Parking Node
            terminalOutput << "ParkingNode" << i + 1 << ":" << alt.parkingNode << "\n";
            outputFile << "ParkingNode" << i + 1 << ":" << alt.parkingNode << "\n";

            // Walking Route
            terminalOutput << "WalkingRoute" << i + 1 << ":";
            outputFile << "WalkingRoute" << i + 1 << ":";
            for (size_t j = 0; j < alt.walkingRoute.size(); ++j) {
                terminalOutput << alt.walkingRoute[j];
                outputFile << alt.walkingRoute[j];
                if (j < alt.walkingRoute.size() - 1) {
                    terminalOutput << ",";
                    outputFile << ",";
                }
            }
            terminalOutput << "(" << static_cast<int>(alt.walkingTime) << ")\n";
            outputFile << "(" << static_cast<int>(alt.walkingTime) << ")\n";

            // Total Time
            terminalOutput << "TotalTime" << i + 1 << ":(" << static_cast<int>(alt.totalTime) << ")\n";
            outputFile << "TotalTime" << i + 1 << ":(" << static_cast<int>(alt.totalTime) << ")\n";
        }

        // Caso apenas uma solução tenha sido encontrada, ainda assim mostrar a segunda como none
        if (alternatives.size() == 1) {
            terminalOutput << "DrivingRoute2:none\n";
            terminalOutput << "ParkingNode2:none\n";
            terminalOutput << "WalkingRoute2:none\n";
            terminalOutput << "TotalTime2:\n";

            outputFile << "DrivingRoute2:none\n";
            outputFile << "ParkingNode2:none\n";
            outputFile << "WalkingRoute2:none\n";
            outputFile << "TotalTime2:\n";
        }
    }

    // Mostrar no terminal e finalizar
    cout << terminalOutput.str();
    cout << "Results written to output.txt" << endl;
    outputFile.close();
}




void Menu::end() {
    std::cout << "Exiting program..." << std::endl;
}
