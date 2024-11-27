#pragma once

#include <string>
#include <iostream>

// 用于存储一行数据的class
class WaterSample {
public:
    WaterSample(const std::string& id, const std::string& samplingPoint, const std::string& samplingPointNotation,
        const std::string& samplingPointLabel, const std::string& sampleDateTime,
        const std::string& determinandLabel, const std::string& determinandDefinition,
        int determinandNotation, char resultQualifierNotation,
        double result, const std::string& unitLabel, const std::string& sampledMaterialTypeLabel,
        bool isComplianceSample, const std::string& samplePurpose, int easting, int northing)
        : id(id), samplingPoint(samplingPoint), samplingPointNotation(samplingPointNotation),
        samplingPointLabel(samplingPointLabel), sampleDateTime(sampleDateTime), determinandLabel(determinandLabel),
        determinandDefinition(determinandDefinition), determinandNotation(determinandNotation),
        resultQualifierNotation(resultQualifierNotation), result(result), unitLabel(unitLabel),
        sampledMaterialTypeLabel(sampledMaterialTypeLabel), isComplianceSample(isComplianceSample),
        samplePurpose(samplePurpose), easting(easting), northing(northing) {}

    std::string getId() const { return id; }
    std::string getSamplingPoint() const { return samplingPoint; }
    std::string getSamplingPointNotation() const { return samplingPointNotation; }
    std::string getSamplingPointLabel() const { return samplingPointLabel; }
    std::string getSampleDateTime() const { return sampleDateTime; }
    std::string getDeterminandLabel() const { return determinandLabel; }
    std::string getDeterminandDefinition() const { return determinandDefinition; }
    int getDeterminandNotation() const { return determinandNotation; }
    char getResultQualifierNotation() const { return resultQualifierNotation; }
    double getResult() const { return result; }
    std::string getUnitLabel() const { return unitLabel; }
    std::string getSampledMaterialTypeLabel() const { return sampledMaterialTypeLabel; }
    bool getIsComplianceSample() const { return isComplianceSample; }
    std::string getSamplePurpose() const { return samplePurpose; }
    int getEasting() const { return easting; }
    int getNorthing() const { return northing; }

private:
    std::string id;  // Primary Key: @id
    std::string samplingPoint;  // FK1: samplingPoint.notation
    std::string samplingPointNotation;  // samplingPoint.notation
    std::string samplingPointLabel;  // samplingPoint.label
    std::string sampleDateTime;
    std::string determinandLabel;  // determinand.label
    std::string determinandDefinition;
    int determinandNotation;  // FK2: determinand.notation
    char resultQualifierNotation;  // resultQualifier.notation
    double result;
    std::string unitLabel;  // determinand.unit.label
    std::string sampledMaterialTypeLabel;  // sample.sampledMaterialType.label
    bool isComplianceSample;  // sample.isComplianceSample
    std::string samplePurpose;  // sample.purpose.label
    int easting;  // sample.samplingPoint.easting
    int northing;  // sample.samplingPoint.northing
};
