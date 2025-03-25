//
// Created by Luna on 22/02/2025.
//

#ifndef PROJETO1_DA_MENU_H
#define PROJETO1_DA_MENU_H

#include "GraphInterface.h"

class Menu {
public:
    Menu();

    void init();

    void drivingOnlyMenu();

    void drivingWalkingMenu();

    void independentRoute();

    void restrictedRoute();

    void bestDrivingWalkingRoute();

    void approximateSolutions();

    static void end();

    static string toUpper(const std::string &s);

    static string trim(const string &s);
};


#endif //PROJETO1_DA_MENU_H
