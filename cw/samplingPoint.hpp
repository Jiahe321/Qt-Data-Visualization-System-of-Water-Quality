#pragma once

#include <string>
#include <iostream>

class SamplingPoint {
public:
    SamplingPoint(const sdt::string notation, const std::string label,const std::string MaterialType,
                  bool isCompliance, const std::string purpose, long int easting, long int northing):
                  notation(notation), label(label), MaterialType(MaterialType), isCompliance(isCompliance),
                  purpose(purpose), easting(easting), northing(northing) {}

    std::string getNotation() const { return notation };
    std::string getLabel() const { return label };
    std::string getMaterialType() const { return MaterialType };
    bool getisCompliance() const { return isCompliance };
    std::string getPurpose() const { return purpose };
    long int getEasting() const { return easting };
    long int getNorthing() const { return northing };

private:

};
