#pragma once

#include <string>
#include <iostream>

class SamplingPoint {
public:
    WaterSample(const std::string id, const std::string samplingPoint, const std::string sampleDateTime,
                const std::string determinand, const char resultQualifier, double result):
                id(id), samplingPoint(samplingPoint), sampleDateTime(sampleDateTime),
                determinand(determinand), resultQualifier(resultQualifier), result(result){}

    std::string getId() const { return id; }
    std::string getSamplingPoint() const { return samplingPoint; }
    std::string getSamplDateTime() const { return sampleDateTime; }
    std::string getDeterminand() const { return determinand; }
    char getResultQualifier() const { return resultQualifier; }
    double getResult() const { return result; }

private:
    std::string id;
    std::string samplingPoint; // FK1: samplingPoint.notation
    std::string sampleDateTime;
    std::string determinand; // FK2: determinand.notation
    char resultQualifier;
    double result;
};

std::ostream& operator<<(std::ostream& os, const WaterSample& sample);
