#pragma once

#include <string>
#include <vector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDateTime>
#include "csv.hpp"
#include "sample.hpp"

class WaterSampleDatabase {
public:
    WaterSampleDatabase(const std::string& dbName);

    void clearDatabase();
    bool createTable();
    bool readCSV(const std::string& filename);
    bool insertBatchIntoDatabase(const std::vector<WaterSample>& samples);
    QSqlTableModel* getTableModel(); // return a SQLTable to show data
    // other methods waiting to be completed

private:
    QSqlDatabase db;
    bool stringToBool(const std::string& str);
};
