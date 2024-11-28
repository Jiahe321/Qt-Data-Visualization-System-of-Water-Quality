#include "PollutantOverview.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QTableView>

// Change it! You can try to use the code structure of main window
PollutantOverview::PollutantOverview(QWidget* parent)
    : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* title = new QLabel("PollutantOverview", this);
    QTableView* table = new QTableView(this);

    layout->addWidget(title);
    layout->addWidget(table);

    setLayout(layout);
}
