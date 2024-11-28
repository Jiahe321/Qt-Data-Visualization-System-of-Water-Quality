#include "EnvironmentalLitterIndicators.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QTableView>

// Change it! You can try to use the code structure of main window
EnvironmentalLitterIndicators::EnvironmentalLitterIndicators(QWidget* parent)
    : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* title = new QLabel("EnvironmentalLitterIndicators", this);
    QTableView* table = new QTableView(this);

    layout->addWidget(title);
    layout->addWidget(table);

    setLayout(layout);
}
