#pragma once

#include "database.hpp"
#include <QtCharts>

class Methods : public WaterSampleDatabase {
    // Notes:
    // This class is a child class of WaterSampleDatabase and inherits all its methods.
    // Since QSqlDatabase db is a protected member of the base class, we can access it directly.
public:
    // Constructor inheriting from the base class (WaterSampleDatabase)
    Methods(const std::string& dbName): WaterSampleDatabase(dbName) {}
    QChartView* createPollutantTrendChart(const QString& pollutantName);

public slots:
    void updateChart();

private:
    QColor getComplianceColor(double value);
};
