//
// Created by martagfmartins on 3/24/25.
//

#include "InputValidation.h"
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// checks if avoid nodes do not have parentheses
bool isValidAvoidNodesFormat(const string &input) {
    return (input.find('(') == string::npos && input.find(')') == string::npos);
}

vector<string> parseAvoidNodes(const string &input, bool &valid) {
    vector<string> result;
    valid = isValidAvoidNodesFormat(input);
    if (!valid) {
        return result;
    }

    istringstream iss(input);
    string token;
    while (getline(iss, token, ',')) {
        // removes extra spaces
        token.erase(token.begin(), find_if(token.begin(), token.end(), [](unsigned char ch) {
            return !isspace(ch);
        }));
        token.erase(find_if(token.rbegin(), token.rend(), [](unsigned char ch) {
            return !isspace(ch);
        }).base(), token.end());
        if (!token.empty())
            result.push_back(token);
    }
    return result;
}

// checks if string avoid segments is in the right format -> (id,id),(id,id),...
bool isValidAvoidSegmentsFormat(const string &input) {
    if (input.empty())
        return true;
    // it needs to have at least 1 '(' and 1 ')'
    if (input.find('(') == string::npos || input.find(')') == string::npos)
        return false;

    size_t pos = 0;
    while (pos < input.size()) {
        size_t openPos = input.find('(', pos);
        if (openPos == string::npos)
            break;

        size_t closePos = input.find(')', openPos);
        if (closePos == string::npos)
            return false;

        string segment = input.substr(openPos + 1, closePos - openPos - 1);
        size_t commaPos = segment.find(',');
        if (commaPos == string::npos)
            return false;
        // checks if it has more than 1 comma
        if (segment.find(',', commaPos + 1) != string::npos)
            return false;
        pos = closePos + 1;
        // ignores next commas
        if (pos < input.size() && input[pos] == ',')
            pos++;
    }
    return true;
}

vector<pair<string, string>> parseAvoidSegments(const string &input, bool &valid) {
    vector<pair<string, string>> result;
    valid = isValidAvoidSegmentsFormat(input);
    if (!valid)
        return result;

    size_t pos = 0;
    string working = input;
    while ((pos = working.find("(")) != string::npos) {
        size_t endPos = working.find(")", pos);
        if (endPos == string::npos) {
            valid = false;
            return result;
        }
        string segment = working.substr(pos + 1, endPos - pos - 1);
        size_t commaPos = segment.find(",");
        if (commaPos == string::npos) {
            valid = false;
            return result;
        }
        string from = segment.substr(0, commaPos);
        string to = segment.substr(commaPos + 1);

        // removes extra spaces
        from.erase(from.begin(), find_if(from.begin(), from.end(), [](unsigned char ch){ return !isspace(ch); }));
        from.erase(find_if(from.rbegin(), from.rend(), [](unsigned char ch){ return !isspace(ch); }).base(), from.end());
        to.erase(to.begin(), find_if(to.begin(), to.end(), [](unsigned char ch){ return !isspace(ch); }));
        to.erase(find_if(to.rbegin(), to.rend(), [](unsigned char ch){ return !isspace(ch); }).base(), to.end());

        result.push_back({from, to});
        working.erase(0, endPos + 1);
    }
    return result;
}