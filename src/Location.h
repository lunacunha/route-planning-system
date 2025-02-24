//
// Created by Luna on 22/02/2025.
//

#ifndef PROJETO1_DA_LOCATION_H
#define PROJETO1_DA_LOCATION_H

#include <iostream>
using namespace std;

class Location {

private:
    string name;
    int id;
    string code;
    bool hasParking;


public:
    Location(string location_name, int location_id, string location_code, bool parking);

    // getter functions
    string getLocationName() const;
    int getLocationId() const;
    string getLocationCode() const;
    bool getHasParking() const;

    // setter functions
    void setLocationName(const string& name);
    void setLocationId(int id);
    void setLocationCode(const string& code);
    void setHasParking(bool p);
};

#endif //PROJETO1_DA_LOCATION_H
