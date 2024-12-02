#pragma once

#include <QWidget>
#include "methods.hpp"

class QVBoxLayout;
class QLabel;

class PollutantOverview : public QWidget {
    Q_OBJECT

public:
    PollutantOverview(QWidget* parent = nullptr);
    // Add all your public methods here
    // Remenber to delete the brackets and write the method in cpp file
    void createWidgets() {};
    void arrangeWidgets() {};

private:
    // Create all your widgets here

    // Add all your private methods here

private slots:
    // connect all slots here, you can do it later
    void connectSlots() {};
};
