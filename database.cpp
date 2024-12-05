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

QChartView* WaterSampleDatabase::createPollutantTrendChart(const QString& pollutantName) {
    // Create SQL query
    QSqlQuery query(db);
    query.prepare(R"(
        SELECT sampleDateTime, result 
        FROM water_samples 
        WHERE determinandLabel = :pollutantName 
        ORDER BY sampleDateTime
    )");
    query.bindValue(":pollutantName", pollutantName);

    // Execute query
    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError();
        return nullptr;
    }

    // Create chart
    QChart* chart = new QChart();
    QLineSeries* lineSeries = new QLineSeries();

    // Query for data and add to series
    while (query.next()) {
        QDateTime sampleDateTime = QDateTime::fromString(query.value(0).toString(), "yyyy-MM-ddTHH:mm:ss");
        if (sampleDateTime.isValid()) {
            double result = query.value(1).toDouble();
            lineSeries->append(sampleDateTime.toMSecsSinceEpoch(), result);
        }
    }

    // Add series to chart
    chart->addSeries(lineSeries);

    // why not working！！！！！！！为什么！！！！！！why！！！！！
    // Configure X-axis as QDateTimeAxis
    QDateTimeAxis* axisX = new QDateTimeAxis();
    axisX->setFormat("yyyy-MM-dd HH:mm:ss");
    axisX->setTitleText("Time");
    axisX->setTickCount(10); // Adjust tick count as needed
    chart->addAxis(axisX, Qt::AlignBottom);
    lineSeries->attachAxis(axisX);

    // Configure Y-axis
    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("Result");
    chart->addAxis(axisY, Qt::AlignLeft);
    lineSeries->attachAxis(axisY);

    // Set chart title
    chart->setTitle("Pollutant Trend: " + pollutantName);

    // Create chart view
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}


// Maybe Useful later
QColor WaterSampleDatabase::getComplianceColor(double value) {
    if (value < 50) {
        return QColor("green");  // safe
    }
    else if (value < 100) {
        return QColor("amber");  // caution
    }
    else {
        return QColor("red");    // exceeding safe levels
    }
}

QChartView* WaterSampleDatabase::createPOPLevelsChart(const QString& pollutantName) {
    QSqlQuery query(db);
    // Group by samplingPointNotation and order by sampleDateTime
    query.prepare("SELECT samplingPointNotation, sampleDateTime, resultQualifierNotation, result, unitLabel FROM water_samples WHERE determinandLabel = :pollutantName ORDER BY samplingPointNotation, sampleDateTime");
    query.bindValue(":pollutantName", pollutantName);

    // execute query
    if (!query.exec()) {
        qDebug() << "Query failed：" << query.lastError();
        return nullptr;
    }

    // Create chart
    QChart* chart = new QChart();

    // Create a map to store series for each samplingPointNotation
    QMap<QString, QLineSeries*> seriesMap;

    // Query for data and add to series
    while (query.next()) {
        QString samplingPointNotation = query.value(0).toString();
        QDateTime sampleDateTime = QDateTime::fromString(query.value(1).toString(), "yyyy-MM-ddTHH:mm:ss");
        qDebug() << "Sample DateTime: " << sampleDateTime.toString();
        double result = query.value(3).toDouble();

        // If a series for this samplingPointNotation does not exist, create one
        if (!seriesMap.contains(samplingPointNotation)) {
            seriesMap[samplingPointNotation] = new QLineSeries();
        }

        // Append the data point to the appropriate series
        seriesMap[samplingPointNotation]->append(sampleDateTime.toMSecsSinceEpoch(), result);
    }

    // Add all series to the chart
    for (auto series : seriesMap) {
        chart->addSeries(series);
    }

    chart->createDefaultAxes();
    chart->setTitle("Pollutant Trend: " + pollutantName);

    // Set chart view and return 
    QChartView* chartView = new QChartView(chart);

    return chartView;
}

//代码是错的。应该显示所有污染物的合规情况（我们默认一种污染物的合规情况只有一种），然后可以按地区分类筛选出该地区的污染物
QChartView* WaterSampleDatabase::createComplianceChart(const QString& determinand) {
    // Create a chart
    QChart* chart = new QChart();
    chart->setTitle("Regulatory Compliance Overview for " + determinand);

    // Create bar sets for compliant and non-compliant counts
    QBarSet* compliantSet = new QBarSet("Compliant");
    QBarSet* nonCompliantSet = new QBarSet("Non-Compliant");

    // Execute query to get compliance data for the given determinand, grouped by samplingPointNotation

    QSqlQuery query;
    query.prepare(R"(
        SELECT 
            samplingPointNotation, 
            SUM(CASE WHEN isComplianceSample = 1 THEN 1 ELSE 0 END) AS compliant,
            SUM(CASE WHEN isComplianceSample = 0 THEN 1 ELSE 0 END) AS nonCompliant
        FROM water_samples
        WHERE determinandLabel = :determinand
        GROUP BY samplingPointNotation
    )");
    query.bindValue(":determinand", determinand);

    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError();
        delete chart;
        return nullptr;
    }

    // Prepare data for the chart
    QStringList categories; // Sampling point notations
    while (query.next()) {
        QString samplingPointNotation = query.value("samplingPointNotation").toString();
        int compliant = query.value("compliant").toInt();
        int nonCompliant = query.value("nonCompliant").toInt();

        categories << samplingPointNotation;
        *compliantSet << compliant;
        *nonCompliantSet << nonCompliant;
    }

    // Add data to a bar series
    QBarSeries* series = new QBarSeries();
    series->append(compliantSet);
    series->append(nonCompliantSet);
    chart->addSeries(series);

    // Add category axis for sampling point notations
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Add numeric axis for count
    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("Sample Count");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Customize chart appearance
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Create and return a QChartView
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}
