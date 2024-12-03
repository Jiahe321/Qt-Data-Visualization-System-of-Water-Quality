#include "EnvironmentalLitterIndicators.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QChartView>

// Constructor
EnvironmentalLitterIndicators::EnvironmentalLitterIndicators(WaterSampleDatabase* database, QWidget* parent)
    : QWidget(parent), db(database), chartView(nullptr) {

    // Create and set the layout for the widget
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Initialize info label to display a message
    infoLabel = new QLabel("No CSV file selected", this);

    // Add infoLabel to the layout
    layout->addWidget(infoLabel);

    // Set the layout for the widget
    setLayout(layout);
}

// Slot to update the chart
void EnvironmentalLitterIndicators::updateChart() {
    // If there's an existing chart, remove and delete it
    if (chartView) {
        layout()->removeWidget(chartView);
        delete chartView;
        chartView = nullptr;
    }

    // Check if the database is available
    if (!db) {
        infoLabel->setText("Database not available");
        infoLabel->show();
        return;
    }

    // Define the pollutant of interest
    QString determinand = "PCBs";

    // Generate a chart for the selected pollutant
    chartView = db->createELIChart(determinand);

    // If the chart is created, add it to the layout
    if (chartView) {
        layout()->addWidget(chartView); // Add chart to the layout
        infoLabel->hide(); // Hide the info label
    }
    else {
        // If no data is available for the selected pollutant, show a message
        infoLabel->setText("No data available for the selected pollutant");
        infoLabel->show();
    }
}
