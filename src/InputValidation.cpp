//
// Created by martagfmartins on 3/24/25.
//

#include "InputValidation.h"
#include <sstream>
#include <algorithm>
#include <cctype>

// Verifica se avoid nodes NÃO contém parênteses.
bool isValidAvoidNodesFormat(const std::string &input) {
    return (input.find('(') == std::string::npos && input.find(')') == std::string::npos);
}

std::vector<std::string> parseAvoidNodes(const std::string &input, bool &valid) {
    std::vector<std::string> result;
    valid = isValidAvoidNodesFormat(input);
    if (!valid) return result;

    std::istringstream iss(input);
    std::string token;
    while (getline(iss, token, ',')) {
        // Remove espaços extras no início e fim.
        token.erase(token.begin(), std::find_if(token.begin(), token.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        token.erase(std::find_if(token.rbegin(), token.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), token.end());
        if (!token.empty())
            result.push_back(token);
    }
    return result;
}

// Verifica se a string de avoid segments está no formato: (id,id),(id,id),...
bool isValidAvoidSegmentsFormat(const std::string &input) {
    if (input.empty())
        return true;
    // Deve conter pelo menos um '(' e um ')'
    if (input.find('(') == std::string::npos || input.find(')') == std::string::npos)
        return false;

    size_t pos = 0;
    while (pos < input.size()) {
        size_t openPos = input.find('(', pos);
        if (openPos == std::string::npos)
            break;
        size_t closePos = input.find(')', openPos);
        if (closePos == std::string::npos)
            return false;
        std::string segment = input.substr(openPos + 1, closePos - openPos - 1);
        size_t commaPos = segment.find(',');
        if (commaPos == std::string::npos)
            return false;
        // Verifica se há mais de uma vírgula.
        if (segment.find(',', commaPos + 1) != std::string::npos)
            return false;
        pos = closePos + 1;
        // Pula vírgulas que se seguem.
        if (pos < input.size() && input[pos] == ',')
            pos++;
    }
    return true;
}

std::vector<std::pair<std::string, std::string>> parseAvoidSegments(const std::string &input, bool &valid) {
    std::vector<std::pair<std::string, std::string>> result;
    valid = isValidAvoidSegmentsFormat(input);
    if (!valid)
        return result;

    size_t pos = 0;
    std::string working = input;
    while ((pos = working.find("(")) != std::string::npos) {
        size_t endPos = working.find(")", pos);
        if (endPos == std::string::npos) {
            valid = false;
            return result;
        }
        std::string segment = working.substr(pos + 1, endPos - pos - 1);
        size_t commaPos = segment.find(",");
        if (commaPos == std::string::npos) {
            valid = false;
            return result;
        }
        std::string from = segment.substr(0, commaPos);
        std::string to = segment.substr(commaPos + 1);
        // Remove espaços extras
        from.erase(from.begin(), std::find_if(from.begin(), from.end(), [](unsigned char ch){ return !std::isspace(ch); }));
        from.erase(std::find_if(from.rbegin(), from.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), from.end());
        to.erase(to.begin(), std::find_if(to.begin(), to.end(), [](unsigned char ch){ return !std::isspace(ch); }));
        to.erase(std::find_if(to.rbegin(), to.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), to.end());

        result.push_back({from, to});
        working.erase(0, endPos + 1);
    }
    return result;
}