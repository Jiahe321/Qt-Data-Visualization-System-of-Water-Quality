#pragma once

#include <QWidget>

class QVBoxLayout;
class QLabel;

class EnvironmentalLitterIndicators : public QWidget {
    Q_OBJECT

public:
    EnvironmentalLitterIndicators(QWidget* parent = nullptr);
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
