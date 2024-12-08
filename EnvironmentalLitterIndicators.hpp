#pragma once

#include <QWidget>
#include <QLabel>
#include <QtCharts>
#include "database.hpp"

class EnvironmentalLitterIndicators : public QWidget {
    Q_OBJECT

public:
    EnvironmentalLitterIndicators(WaterSampleDatabase* database, QWidget* parent = nullptr);

public slots:
    void updateChart();
    void populateDropdowns(); // Populate dropdown options

private:
    WaterSampleDatabase* db;
    QLabel* infoLabel;
    QChartView* chartView;
    QComboBox* locationDropdown;   // Dropdown for locations
    QComboBox* litterTypeDropdown; // Dropdown for litter types
    QString selectedLocation;    // Current selected location
    QString selectedLitterType; // Current selected litter type
    QVector<QPair<QString, double>> getLitterData(const QString& location, const QString& litterType); // Fetch data

private slots:
    void onFilterChanged();
    void onPointHovered(const QPointF& point, bool state); // Slot for point hover
    void onPointClicked(const QPointF& point);            // Slot for point click
    void onLitterTypeChanged();// Slot for litter type change, update location combobox
};