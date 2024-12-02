#include "database.hpp"
#include <iostream>
#include <stdexcept>

// Initialize SQLite database connection
WaterSampleDatabase::WaterSampleDatabase(const std::string& dbName) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QString::fromStdString(dbName));

    if (!db.open()) {
        std::cerr << "Error: Unable to open database!" << std::endl;
    }
}

// Clear the database before reading new data
void WaterSampleDatabase::clearDatabase() {
    QSqlQuery query(db);
    query.exec("DELETE FROM water_samples");
}

// Create table
bool WaterSampleDatabase::createTable() {
    QSqlQuery query;
    const QString createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS water_samples (
            id TEXT PRIMARY KEY, 
            samplingPoint TEXT, 
            samplingPointNotation TEXT, 
            samplingPointLabel TEXT, 
            sampleDateTime TEXT, 
            determinandLabel TEXT, 
            determinandDefinition TEXT, 
            determinandNotation TEXT, 
            resultQualifierNotation TEXT, 
            result REAL, 
            unitLabel TEXT, 
            sampledMaterialTypeLabel TEXT, 
            isComplianceSample INTEGER, 
            samplePurpose TEXT, 
            easting INTEGER, 
            northing INTEGER
        )
    )";

    if (!query.exec(createTableSQL)) {
        std::cerr << "Failed to create or verify table: "
            << query.lastError().text().toStdString() << std::endl;
        return false;
    }
    return true;
}

// Read data from a CSV file into the WaterSample vector
bool WaterSampleDatabase::readCSV(const std::string& filename) {
    clearDatabase(); // Clear the database before inserting new data

    csv::CSVReader reader(filename);
    std::vector<WaterSample> samples; // Vector to hold parsed WaterSample objects

    // Iterate over each row in the CSV and store the data
    for (const auto& row : reader) {
        // Parse values from the row based on your updated column names
        std::string id = row["@id"].get<>();
        std::string samplingPoint = row["sample.samplingPoint"].get<>();
        std::string samplingPointNotation = row["sample.samplingPoint.notation"].get<>();
        std::string samplingPointLabel = row["sample.samplingPoint.label"].get<>();
        std::string sampleDateTime = row["sample.sampleDateTime"].get<>();
        std::string determinandLabel = row["determinand.label"].get<>();
        std::string determinandDefinition = row["determinand.definition"].get<>();
        int determinandNotation = row["determinand.notation"].get<int>();
        char resultQualifierNotation = row["resultQualifier.notation"].get<>()[0];
        double result = row["result"].get<double>();
        std::string unitLabel = row["determinand.unit.label"].get<>();
        std::string sampledMaterialTypeLabel = row["sample.sampledMaterialType.label"].get<>();
        bool isComplianceSample = stringToBool(row["sample.isComplianceSample"].get<>());
        std::string samplePurpose = row["sample.purpose.label"].get<>();
        int easting = row["sample.samplingPoint.easting"].get<int>();
        int northing = row["sample.samplingPoint.northing"].get<int>();

        // Create a WaterSample object and add it to the vector
        WaterSample sample(id, samplingPoint, samplingPointNotation, samplingPointLabel, sampleDateTime,
            determinandLabel, determinandDefinition, determinandNotation, resultQualifierNotation,
            result, unitLabel, sampledMaterialTypeLabel,
            isComplianceSample, samplePurpose, easting, northing);
        samples.push_back(sample);
    }

    // Now insert all data into the database
    return insertBatchIntoDatabase(samples);
}

// Insert all WaterSample objects into the database
bool WaterSampleDatabase::insertBatchIntoDatabase(const std::vector<WaterSample>& samples) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO water_samples (id, samplingPoint, samplingPointNotation, samplingPointLabel, "
        "sampleDateTime, determinandLabel, determinandDefinition, determinandNotation, resultQualifierNotation, "
        "result, unitLabel, sampledMaterialTypeLabel, isComplianceSample, "
        "samplePurpose, easting, northing) "
        "VALUES (:id, :samplingPoint, :samplingPointNotation, :samplingPointLabel, :sampleDateTime, "
        ":determinandLabel, :determinandDefinition, :determinandNotation, :resultQualifierNotation, "
        ":result, :unitLabel, :sampledMaterialTypeLabel, :isComplianceSample, "
        ":samplePurpose, :easting, :northing)");

    // Begin a transaction for batch insert
    db.transaction();

    for (const auto& sample : samples) {
        query.bindValue(":id", QString::fromStdString(sample.getId()));
        query.bindValue(":samplingPoint", QString::fromStdString(sample.getSamplingPoint()));
        query.bindValue(":samplingPointNotation", QString::fromStdString(sample.getSamplingPointNotation()));
        query.bindValue(":samplingPointLabel", QString::fromStdString(sample.getSamplingPointLabel()));
        query.bindValue(":sampleDateTime", QString::fromStdString(sample.getSampleDateTime()));
        query.bindValue(":determinandLabel", QString::fromStdString(sample.getDeterminandLabel()));
        query.bindValue(":determinandDefinition", QString::fromStdString(sample.getDeterminandDefinition()));
        query.bindValue(":determinandNotation", sample.getDeterminandNotation());

        char resultQualifier = sample.getResultQualifierNotation();
        if (resultQualifier == '\0' || resultQualifier == '0') { // check for empty value
            query.bindValue(":resultQualifierNotation", QString(""));
        }
        else {
            query.bindValue(":resultQualifierNotation", QString(resultQualifier));
        }

        query.bindValue(":result", sample.getResult());
        query.bindValue(":unitLabel", QString::fromStdString(sample.getUnitLabel()));
        query.bindValue(":sampledMaterialTypeLabel", QString::fromStdString(sample.getSampledMaterialTypeLabel()));
        query.bindValue(":isComplianceSample", sample.getIsComplianceSample());
        query.bindValue(":samplePurpose", QString::fromStdString(sample.getSamplePurpose()));
        query.bindValue(":easting", sample.getEasting());
        query.bindValue(":northing", sample.getNorthing());

        if (!query.exec()) {
            std::cerr << "Failed to insert sample: " << query.lastError().text().toStdString() << std::endl;
            db.rollback();  // Rollback if insertion fails
            return false;
        }
    }

    // Commit the transaction if all insertions succeed
    db.commit();
    return true;
}

QSqlTableModel* WaterSampleDatabase::getTableModel(const QString& determinand) {
    QSqlTableModel* model = new QSqlTableModel();
    model->setTable("water_samples");

    // Search for certain row
    if (!determinand.isEmpty()) {
        model->setFilter(QString("determinandLabel = '%1'").arg(determinand));
    }

    // Load data
    model->select();
    return model;
}

// Change string isComplianceSample to bool
bool WaterSampleDatabase::stringToBool(const std::string& str) {
    if (str == "TRUE" || str == "true") return true;
    if (str == "FALSE" || str == "false") return false;
    throw std::invalid_argument("Invalid boolean value: " + str);
}
