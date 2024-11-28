#pragma once

// All other pages for the tab
#include "ComplianceDashboard.hpp"
#include "PollutantOverview.hpp"
#include "POPs.hpp"
#include "EnvironmentalLitterIndicators.hpp"
#include "FluorinatedCompounds.hpp"
// Data source
#include "database.hpp"

#include <QMainWindow>
#include <QString>
#include <QApplication>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <iostream>

class QTableView;
class QVBoxLayout;
class QTabWidget;

class WaterSampleWindow : public QMainWindow {
    Q_OBJECT

public:
    WaterSampleWindow(QWidget* parent = nullptr);

private:
    // All widgets
    QVBoxLayout* mainLayout;
    QTableView* tableView;
    QTabWidget* tabWidget;
    QString dbFilePath;
    QString csvFilePath;

    QWidget* centralWidget;
    QWidget* dataPage;
    QVBoxLayout* dataLayout;

    QMenu* fileMenu;
    QAction* loadCSVAction;

    // Tabs
    ComplianceDashboard* CD;
    PollutantOverview* PO;
    POPs* POPsTab;
    EnvironmentalLitterIndicators* ELI;
    FluorinatedCompounds* FC;

    void createWidgets();
    void arrangeWidgets();
    void connectSlots();

private slots:
    void loadCSV();
};
