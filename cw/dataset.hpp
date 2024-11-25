#pragma once

#include <vector>
#include "sample.hpp"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class WaterSampleDataset {
public:
    WaterSampleDataset(const QString& dbName);
    bool createTables();
    void loadFromCSV(const std::string& filename);
    std::vector<WaterSample> getSamples() const;

private:
    QSqlDatabase db;
    void insertSample(const WaterSample& sample);
};
