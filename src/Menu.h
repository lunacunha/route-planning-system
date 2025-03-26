#ifndef PROJETO1_DA_MENU_H
#define PROJETO1_DA_MENU_H

#include "GraphInterface.h"
#include <unordered_set>
#include <vector>
#include <string>

class Menu {
private:
    GraphInterface graphInterface;
    Graph<std::string>* graph;
    const std::unordered_map<std::string, bool>* parkingInfo;

public:
    Menu();

    void init();
    void loadData(int option);
    void drivingOnlyMenu();
    void drivingWalkingMenu();
    void independentRoute();
    void restrictedRoute();
    void bestDrivingWalkingRoute();

    // ✅ Atualizada com os parâmetros corretos
    void approximateSolutions(const std::string& source,
                              const std::string& destination,
                              int maxWalkTime,
                              const std::unordered_set<std::string>& avoidNodes,
                              const std::vector<std::pair<std::string, std::string>>& avoidSegments);

    static void end();
    static std::string toUpper(const std::string &s);
    static std::string trim(const std::string &s);
};

#endif //PROJETO1_DA_MENU_H
