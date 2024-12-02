#pragma once

// All other pages for the tab
#include "ComplianceDashboard.hpp"
#include "PollutantOverview.hpp"
#include "POPs.hpp"
#include "EnvironmentalLitterIndicators.hpp"
#include "FluorinatedCompounds.hpp"
// Data source
#include "database.hpp"

// Qt libs
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
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <iostream>

class WaterSampleWindow : public QMainWindow {
    Q_OBJECT

public:
    WaterSampleWindow(QWidget* parent = nullptr);

private:
    // Data
    QString dbFilePath;
    QString csvFilePath;

    // All common widgets
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    QTableView* tableView;
    QTabWidget* tabWidget;

    // Tabs
    ComplianceDashboard* CD;
    PollutantOverview* PO;
    POPs* POPsTab;
    EnvironmentalLitterIndicators* ELI;
    FluorinatedCompounds* FC;

    // ²âÊÔËÑË÷¹¦ÄÜ
    QVBoxLayout* dataLayout;
    QWidget* dataPage;
    QLineEdit* searchInput;

    void createWidgets();
    void arrangeWidgets();
    void connectSlots();
    void setDataPage();
    void setTabs();
    void setStatusBarAndMenuBar();

private slots:
    void loadCSV();
    void searchData();

signals:
    // When data updated, tell other pages to update
    void dbUpdated();
};
