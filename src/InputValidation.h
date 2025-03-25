//
// Created by martagfmartins on 3/24/25.
//

#ifndef INPUTVALIDATION_H
#define INPUTVALIDATION_H

#include <string>
#include <vector>
using namespace std;



bool isValidAvoidNodesFormat(const std::string &input);
std::vector<std::string> parseAvoidNodes(const std::string &input, bool &valid);

bool isValidAvoidSegmentsFormat(const std::string &input);
std::vector<std::pair<std::string, std::string>> parseAvoidSegments(const std::string &input, bool &valid);


#endif //INPUTVALIDATION_H
