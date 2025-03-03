//
// Created by Luna on 22/02/2025.
//

#include <string>
#include <iostream>
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
    cout << "Best Route for Driving selected." << endl;
    // Chamar a função correspondente quando implementada
}

void Menu::restrictedRoute() {
    cout << "Restricted Route Planning selected." << endl;
    // Chamar a função correspondente quando implementada
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
