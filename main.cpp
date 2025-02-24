//
// Created by Luna on 22/02/2025.
//

#include "src/GraphInterface.h"
#include <iostream>

using namespace std;

int main() {
    GraphInterface graphInterface;

    cout << "Test parser for locations...\n";
    graphInterface.loadLocations("smallDataset/shortLocations.csv");

    cout << "Teste concluído.\n";
    return 0;
}
