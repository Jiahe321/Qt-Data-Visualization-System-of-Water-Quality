#pragma once

#include <QMainWindow>
#include <QString>

class QTableView;
class QVBoxLayout;
class QPushButton;
class QTabWidget;

class WaterSampleWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit WaterSampleWindow(QWidget* parent = nullptr);

private:
    void createWidgets();
    void arrangeWidgets();
    void connectSlots();

    QVBoxLayout* mainLayout;
    QTableView* tableView;
    QTabWidget* tabWidget;
    QPushButton* loadCSVButton;
    QString dbFilePath;
    QString csvFilePath;

private slots:
    void loadCSV();
};
