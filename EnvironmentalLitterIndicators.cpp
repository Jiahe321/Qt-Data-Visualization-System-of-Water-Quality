#include "EnvironmentalLitterIndicators.hpp"
#include <QMessageBox>
#include <QToolTip>

// Constructor
EnvironmentalLitterIndicators::EnvironmentalLitterIndicators(WaterSampleDatabase* database, QWidget* parent)
    : QWidget(parent), db(database), chartView(nullptr) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Dropdown Layout
    QHBoxLayout* dropdownLayout = new QHBoxLayout();

    QLabel* locationLabel = new QLabel("Location:");
    locationDropdown = new QComboBox(this);
    locationDropdown->addItem("All Locations"); // Default option


    QLabel* litterTypeLabel = new QLabel("Litter Type:");
    litterTypeDropdown = new QComboBox(this);
    litterTypeDropdown->addItem("All Litter Types"); // Default option
    litterTypeDropdown->addItems({ "Bathing Water Profile : Other Litter (incl. plastics)",
                                   "Sewage debris", "Tarry residues" });

    locationDropdown->setEnabled(false);
    litterTypeDropdown->setEnabled(false);

    dropdownLayout->addWidget(locationLabel);
    dropdownLayout->addWidget(locationDropdown);
    dropdownLayout->addWidget(litterTypeLabel);
    dropdownLayout->addWidget(litterTypeDropdown);

    mainLayout->addLayout(dropdownLayout);

    // Info Label
    infoLabel = new QLabel("No data available for the selected filters.", this);
    infoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(infoLabel);

    setLayout(mainLayout);

    // Connect dropdown changes to update the chart
    connect(locationDropdown, &QComboBox::currentTextChanged, this, &EnvironmentalLitterIndicators::updateChart);
    connect(litterTypeDropdown, &QComboBox::currentTextChanged, this, &EnvironmentalLitterIndicators::updateChart);
    connect(litterTypeDropdown, &QComboBox::currentTextChanged, this, &EnvironmentalLitterIndicators::onLitterTypeChanged);
}

void EnvironmentalLitterIndicators::populateDropdowns() {
    // Clear current items and add default option
    locationDropdown->clear();
    locationDropdown->addItem("All Locations");

    // Use WaterSampleDatabase to fetch unique locations
    QStringList locations = db->getUniqueEntries("", "location", "");
    if (locations.isEmpty()) {
        qWarning() << "No unique locations found.";
        return;
    }

    // Add retrieved locations to the dropdown
    locationDropdown->addItems(locations);

    // Populate litter type dropdown with predefined options
    litterTypeDropdown->clear();
    litterTypeDropdown->addItem("All Litter Types");
    litterTypeDropdown->addItems({ "Bathing Water Profile : Other Litter (incl. plastics)",
                                   "Sewage debris", "Tarry residues" });
}

void EnvironmentalLitterIndicators::onLitterTypeChanged() {
    QString selectedLitterType = litterTypeDropdown->currentText();
    locationDropdown->clear();
    locationDropdown->addItem("All Locations");

    // Mapping determinantDefinition to Label
    QMap<QString, QString> litterTypeMap = {
        {"All Litter Types", ""},
        {"Bathing Water Profile : Other Litter (incl. plastics)", "BWP - O.L."},
        {"Sewage debris", "SewageDebris"},
        {"Tarry residues", "TarryResidus"}
        // Add more mappings as needed
    };

    QString litterLabel = litterTypeMap.value(selectedLitterType, "");

    // Filter locations by selected litter label
    QStringList locations = db->getUniqueEntries(litterLabel, "location", "");

    if (!locations.isEmpty()) {
        locationDropdown->addItems(locations);
    }
    else {
        qWarning() << "No locations found for selected litter type:" << selectedLitterType;
        QMessageBox::information(this, "No Locations Found",
            "No locations are available for the selected litter type.");
    }
}


QVector<QPair<QString, double>> EnvironmentalLitterIndicators::getLitterData(const QString& selectedLocation, const QString& selectedLitterType) {
    QVector<QPair<QString, double>> data;

    QString queryStr = "SELECT samplingPointLabel, AVG(result) AS avgResult FROM water_samples WHERE 1=1";

    if (selectedLocation != "All Locations") {
        queryStr += " AND samplingPointLabel = :location";
    }

    if (selectedLitterType != "All Litter Types") {
        queryStr += " AND determinandDefinition = :litterType";
    }

    queryStr += " GROUP BY samplingPointLabel";

    QSqlQuery query;
    query.prepare(queryStr);

    if (selectedLocation != "All Locations") {
        query.bindValue(":location", selectedLocation);
    }

    if (selectedLitterType != "All Litter Types") {
        query.bindValue(":litterType", selectedLitterType);
    }

    if (!query.exec()) {
        qDebug() << "Database query failed:" << query.lastError();
        return data;
    }

    while (query.next()) {
        QString label = query.value("samplingPointLabel").toString();
        double avgResult = query.value("avgResult").toDouble();
        data.append(qMakePair(label, avgResult));
    }

    return data;
}

void EnvironmentalLitterIndicators::updateChart() {
    locationDropdown->setEnabled(true);
    litterTypeDropdown->setEnabled(true);
    // Clear previous chart view
    if (chartView) {
        layout()->removeWidget(chartView);
        delete chartView;
        chartView = nullptr;
    }

    // Get selected dropdown values
    QString selectedLocation = locationDropdown->currentText();
    QString selectedLitterType = litterTypeDropdown->currentText();

    // Check if both filters are set to "All"
    if (selectedLocation == "All Locations" && selectedLitterType == "All Litter Types") {
        infoLabel->setText("Please select a location or litter type.");
        infoLabel->show();
        return;
    }

    // Get litter data based on the selected filters
    QVector<QPair<QString, double>> data = getLitterData(selectedLocation, selectedLitterType);

    if (data.isEmpty()) {
        infoLabel->setText("No data available for the selected filters.");
        infoLabel->show();
        return;
    }

    // Create a bar series for litter data
    QBarSeries* series = new QBarSeries();
    QBarSet* set = new QBarSet(selectedLitterType);

    QStringList categories;
    for (const auto& [location, value] : data) {
        categories << location;
        *set << value;
    }

    series->append(set);

    // Create a chart
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Litter Comparison");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Configure X-axis with location categories
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Locations");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Configure Y-axis with litter levels
    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("Average Litter Levels");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Create a new chart view and replace the previous one
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout()->addWidget(chartView);

    infoLabel->hide();
}

void EnvironmentalLitterIndicators::onFilterChanged() {
    selectedLocation = locationDropdown->currentText();
    selectedLitterType = litterTypeDropdown->currentText();
    updateChart(); // Update chart when the filter changes
}

void EnvironmentalLitterIndicators::onPointHovered(const QPointF& point, bool state) {
    if (state) {
        QString details = QString("Location: %1\nLitter Level: %2").arg(point.x()).arg(point.y());
        QToolTip::showText(QCursor::pos(), details, this);
    }
    else {
        QToolTip::hideText();
    }
}

void EnvironmentalLitterIndicators::onPointClicked(const QPointF& point) {
    QString details = QString("Location: %1\nLitter Level: %2").arg(point.x()).arg(point.y());
    QMessageBox::information(this, "Litter Details", details);
}
