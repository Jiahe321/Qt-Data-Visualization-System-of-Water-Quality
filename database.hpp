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
    bool createTable();
    bool readCSV(const std::string& filename);
    // return a SQLTable to show data, defalt: query all data
    QSqlTableModel* getTableModel(const QString& determinand = QString());

private:
    void clearDatabase();
    bool stringToBool(const std::string& str);
    bool insertBatchIntoDatabase(const std::vector<WaterSample>& samples);

protected:
    QSqlDatabase db;
};
// Other method in methods.hpp
