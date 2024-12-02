#include "PollutantOverview.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QChartView>

PollutantOverview::PollutantOverview(QWidget* parent)
    : QWidget(parent) {

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Title
    QLabel* title = new QLabel("PollutantOverview", this);

    // Chart (initially hidden)
    Methods method = Methods("water_samples.db");
    QString determinand = "1, 1, 2 - Trichloroethane";
    QChartView* chart = method.createPollutantTrendChart(determinand);

    // Add widgets to layout
    layout->addWidget(title);
    layout->addWidget(chart);

    setLayout(layout);

}
