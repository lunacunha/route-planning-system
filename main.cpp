//
// Created by Luna on 22/02/2025.
//

#include "src/GraphInterface.h"
#include <iostream>

using namespace std;

int main() {
    GraphInterface graphInterface;

    cout << "Test parser for distances...\n";
    graphInterface.loadDistances("\"../smallDataset/shortDistances.csv\"");

    cout << "Teste concluído.\n";
    return 0;
}
