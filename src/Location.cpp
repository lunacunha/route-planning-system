//
// Created by Luna on 22/02/2025.
//

#include "Location.h"

Location::Location(string location_name, int location_id, string location_code, bool parking)
        : name(location_name), id(location_id), code(location_code), hasParking(parking) {}

string Location::getLocationName() const {
    return name;
}
int Location::getLocationId() const {
    return id;
}
string Location::getLocationCode() const {
    return code;
}
bool Location::getHasParking() const {
    return hasParking;
}


void Location::setLocationName(const string& newName) {
    name = newName;
}
void Location::setLocationId(int newId) {
    id = newId;
}
void Location::setLocationCode(const string& newCode) {
    code = newCode;
}
void Location::setHasParking(bool newHasParking) {
    hasParking = newHasParking;
}