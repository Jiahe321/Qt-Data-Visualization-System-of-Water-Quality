#include "Window.hpp"

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

    // Set up central widget and layout
    centralWidget = new QWidget();

    // Initialize other page widgets
    dataPage = new QWidget();
    CD = new ComplianceDashboard(this);
    PO = new PollutantOverview(this);
    POPsTab = new POPs(this);
    ELI = new EnvironmentalLitterIndicators(this);
    FC = new FluorinatedCompounds(this);
}

void WaterSampleWindow::arrangeWidgets() {
    // Arrange central widget and layout
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Add TableView to a new tab
    dataLayout = new QVBoxLayout(dataPage);
    dataLayout->addWidget(tableView);
    tabWidget->addTab(dataPage, "Data View");

    // Other widgets wait to be done
    tabWidget->addTab(CD, "Compliance Dashboard");
    tabWidget->addTab(PO, "Pollutant Overview");
    tabWidget->addTab(POPsTab, "Persistent Organic Pollutants (POPs)");
    tabWidget->addTab(ELI, "Environmental Litter Indicators");
    tabWidget->addTab(FC, "Fluorinated Compounds");

    // Add TabWidget to the main layout
    mainLayout->addWidget(tabWidget);

    // Set up status bar
    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->showMessage("Ready");

    // Create Menu Bar
    fileMenu = menuBar()->addMenu("File");
    loadCSVAction = new QAction("Load CSV", this);
    fileMenu->addAction(loadCSVAction);
}

void WaterSampleWindow::connectSlots() {
    // Connect Load CSV Action
    connect(loadCSVAction, &QAction::triggered, this, &WaterSampleWindow::loadCSV);
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
