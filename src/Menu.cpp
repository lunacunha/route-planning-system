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
    ofstream inputFile("/home/martagfmartins/Desktop/faculdade/da/project1_DA/input.txt", ios::trunc);
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
    ofstream outputFile("/home/martagfmartins/Desktop/faculdade/da/project1_DA/output.txt", ios::trunc);
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
    // Load graph
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
    ofstream inputFile("/home/martagfmartins/Desktop/faculdade/da/project1_DA/input.txt", ios::trunc);
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
    ofstream outputFile("/home/martagfmartins/Desktop/faculdade/da/project1_DA/output.txt", ios::trunc);
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
    GraphInterface graphInterface;
    graphInterface.loadLocations("/home/martagfmartins/Desktop/faculdade/da/project1_DA/smallDataset2/smallLocations2.csv");
    graphInterface.loadDistances("/home/martagfmartins/Desktop/faculdade/da/project1_DA/smallDataset2/smallDistances2.csv");
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
    ofstream inputFile("/home/martagfmartins/Desktop/faculdade/da/project1_DA/input.txt", ios::trunc);
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

    // strings separadas
    stringstream terminalOutput;
    stringstream fileOutput;

    terminalOutput << "\n ===== Driving & Walking Route Results =====\n";
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
    } else {
        // Driving route
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
    }

    // print no terminal
    cout << terminalOutput.str();
    cout << "Results written to output.txt" << endl;

    // guardar em output.txt
    ofstream outputFile("/home/martagfmartins/Desktop/faculdade/da/project1_DA/output.txt", ios::trunc);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not write output.txt" << endl;
        return;
    }
    outputFile << fileOutput.str();
    outputFile.close();
}


void Menu::approximateSolutions() {
    cout << "Approximate Solutions selected." << endl;
    GraphInterface graphInterface;

    // Load dataset
    graphInterface.loadLocations("/home/martagfmartins/Desktop/faculdade/da/project1_DA/smallDataset2/smallLocations2.csv");
    graphInterface.loadDistances("/home/martagfmartins/Desktop/faculdade/da/project1_DA/smallDataset2/smallDistances2.csv");
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
    cin.ignore(); // clear newline

    source = toUpper(source);
    destination = toUpper(destination);

    cout << "Enter avoid nodes (comma separated, or leave empty): ";
    string avoidNodesInput;
    getline(cin, avoidNodesInput);
    if (!avoidNodesInput.empty()) {
        if (!isValidAvoidNodesFormat(avoidNodesInput)) {
            cout << "Error: Invalid format for avoid nodes." << endl;
            return;
        }
        vector<string> nodes = parseAvoidNodes(avoidNodesInput, *(new bool(true)));
        for (auto &token : nodes) {
            avoidNodes.insert(toUpper(token));
        }
    }

    cout << "Enter avoid segments (format: (id,id),(id,id) or leave empty): ";
    string avoidSegmentsInput;
    getline(cin, avoidSegmentsInput);
    if (!avoidSegmentsInput.empty()) {
        if (!isValidAvoidSegmentsFormat(avoidSegmentsInput)) {
            cout << "Error: Invalid format for avoid segments." << endl;
            return;
        }
        avoidSegments = parseAvoidSegments(avoidSegmentsInput, *(new bool(true)));
        for (auto &seg : avoidSegments) {
            seg.first = toUpper(seg.first);
            seg.second = toUpper(seg.second);
        }
    }

    // Write input.txt
    ofstream inputFile("/home/martagfmartins/Desktop/faculdade/da/project1_DA/input.txt", ios::trunc);
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

    // Process
    auto alternatives = dwPlanner.findApproximateRoutes(source, destination, maxWalkTime, avoidNodes, avoidSegments);

    // Build output
    stringstream output;
    output << "\n===== Approximate Routes =====\n";
    if (alternatives.empty()) {
        output << "No approximate routes found.\n";
    } else {
        for (size_t i = 0; i < alternatives.size(); i++) {
            output << "DrivingRoute" << i + 1 << ":";
            for (size_t j = 0; j < alternatives[i].drivingRoute.size(); j++) {
                output << alternatives[i].drivingRoute[j];
                if (j < alternatives[i].drivingRoute.size() - 1) output << ",";
            }
            output << "(" << static_cast<int>(alternatives[i].drivingTime) << ")\n";

            output << "ParkingNode" << i + 1 << ":" << alternatives[i].parkingNode << "\n";

            output << "WalkingRoute" << i + 1 << ":";
            for (size_t j = 0; j < alternatives[i].walkingRoute.size(); j++) {
                output << alternatives[i].walkingRoute[j];
                if (j < alternatives[i].walkingRoute.size() - 1) output << ",";
            }
            output << "(" << static_cast<int>(alternatives[i].walkingTime) << ")\n";

            output << "TotalTime" << i + 1 << ":(" << static_cast<int>(alternatives[i].totalTime) << ")\n";
        }
    }

    // Output to terminal
    cout << output.str();

    // Write output.txt
    ofstream outputFile("/home/martagfmartins/Desktop/faculdade/da/project1_DA/output.txt", ios::trunc);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not write output.txt" << endl;
        return;
    }
    outputFile << "Source:" << source << "\n";
    outputFile << "Destination:" << destination << "\n";
    if (alternatives.empty()) {
        outputFile << "DrivingRoute1:none\n";
        outputFile << "ParkingNode1:none\n";
        outputFile << "WalkingRoute1:none\n";
        outputFile << "TotalTime1:\n";
        outputFile << "DrivingRoute2:none\n";
        outputFile << "ParkingNode2:none\n";
        outputFile << "WalkingRoute2:none\n";
        outputFile << "TotalTime2:\n";
    } else {
        for (size_t i = 0; i < alternatives.size(); i++) {
            outputFile << "DrivingRoute" << i + 1 << ":";
            for (size_t j = 0; j < alternatives[i].drivingRoute.size(); j++) {
                outputFile << alternatives[i].drivingRoute[j];
                if (j < alternatives[i].drivingRoute.size() - 1) outputFile << ",";
            }
            outputFile << "(" << static_cast<int>(alternatives[i].drivingTime) << ")\n";

            outputFile << "ParkingNode" << i + 1 << ":" << alternatives[i].parkingNode << "\n";

            outputFile << "WalkingRoute" << i + 1 << ":";
            for (size_t j = 0; j < alternatives[i].walkingRoute.size(); j++) {
                outputFile << alternatives[i].walkingRoute[j];
                if (j < alternatives[i].walkingRoute.size() - 1) outputFile << ",";
            }
            outputFile << "(" << static_cast<int>(alternatives[i].walkingTime) << ")\n";

            outputFile << "TotalTime" << i + 1 << ":(" << static_cast<int>(alternatives[i].totalTime) << ")\n";
        }
    }

    outputFile.close();
    cout << "Results written to output.txt" << endl;
}


void Menu::end() {
    std::cout << "Exiting program..." << std::endl;
}
