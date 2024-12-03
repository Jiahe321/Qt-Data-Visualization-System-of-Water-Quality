#pragma once

#include <string>
#include <vector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDateTime>
#include <QtCharts>
#include "csv.hpp"
#include "sample.hpp"

class WaterSampleDatabase {
public:
    WaterSampleDatabase(const std::string& dbName);
    bool createTable();
    bool readCSV(const std::string& filename);
    // return a SQLTable to show data, defalt: query all data
    QSqlTableModel* getTableModel(const QString& determinand = QString());
    // For different pages
    QChartView* createPollutantTrendChart(const QString& pollutantName);
    QChartView* createPOPLevelsChart() {return nullptr;};

private:
    void clearDatabase();
    bool stringToBool(const std::string& str);
    bool insertBatchIntoDatabase(const std::vector<WaterSample>& samples);
    // For different pages
    // 存在问题：y轴没法加载，数据没法显示
    QColor getComplianceColor(double value);
    QSqlDatabase db;
};

