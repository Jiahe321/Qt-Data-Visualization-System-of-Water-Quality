#include "Window.hpp"
#include "database.hpp"
#include <QApplication>
#include <QPushButton>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

WaterSampleWindow::WaterSampleWindow(QWidget* parent)
    : QMainWindow(parent), dbFilePath("water_samples.db"), csvFilePath("") {
    createWidgets();
    arrangeWidgets();
    connectSlots();
    setWindowTitle("Water Sample Data");
}

void WaterSampleWindow::createWidgets() {
    // Initialize main layout
    mainLayout = new QVBoxLayout();

    // Initialize TableView
    tableView = new QTableView();

    // Initialize TabWidget
    tabWidget = new QTabWidget();

    // Initialize Buttons
    loadCSVButton = new QPushButton("Load CSV");
}

void WaterSampleWindow::arrangeWidgets() {
    // Set up central widget and layout
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Add TableView to a new tab
    QWidget* dataPage = new QWidget();
    QVBoxLayout* dataLayout = new QVBoxLayout(dataPage);
    dataLayout->addWidget(tableView);
    tabWidget->addTab(dataPage, "Data View");

    // Add TabWidget to the main layout
    mainLayout->addWidget(tabWidget);

    // Add a toolbar with Load CSV Button
    QToolBar* toolbar = addToolBar("Main Toolbar");
    toolbar->addWidget(loadCSVButton);

    // Set up status bar
    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->showMessage("Ready");
}

void WaterSampleWindow::connectSlots() {
    connect(loadCSVButton, &QPushButton::clicked, this, &WaterSampleWindow::loadCSV);
}

void WaterSampleWindow::loadCSV() {
    // Open a file dialog to select the CSV file
    csvFilePath = QFileDialog::getOpenFileName(
        this, "Select CSV File", QString(), "CSV Files (*.csv)");

    if (csvFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file selected.");
        return;
    }

    // Database operations
    WaterSampleDatabase db(dbFilePath.toStdString());
    if (!db.createTable()) {
        QMessageBox::critical(this, "Error", "Failed to create or verify the database table.");
        return;
    }

    if (db.readCSV(csvFilePath.toStdString())) {
        tableView->setModel(db.getTableModel());
        statusBar()->showMessage("CSV data loaded successfully.");
    }
    else {
        QMessageBox::critical(this, "Error", "Error loading CSV data.");
    }
}
