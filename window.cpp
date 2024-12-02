#include "window.hpp"

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

    // Set up central widget and layout
    centralWidget = new QWidget();

    // Initialize Tab
    tabWidget = new QTabWidget();

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

    setDataPage();
    setTabs();
    setStatusBarAndMenuBar();
}

void WaterSampleWindow::setDataPage() {
    // Add TableView to a new tab
    QVBoxLayout* dataLayout = new QVBoxLayout(dataPage);

    // ²âÊÔËÑË÷¹¦ÄÜ
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLabel* searchLabel = new QLabel("Search by Determinand Label:");
    searchInput = new QLineEdit();
    QPushButton* searchButton = new QPushButton("Search");
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchInput);
    searchLayout->addWidget(searchButton);

    dataLayout->addLayout(searchLayout);

    // Connect Search Button
    connect(searchButton, &QPushButton::clicked, this, &WaterSampleWindow::searchData);

    // Table
    dataLayout->addWidget(tableView);
}

void WaterSampleWindow::setTabs() {
    tabWidget->addTab(dataPage, "Data View");
    tabWidget->addTab(PO, "Pollutant Overview");
    tabWidget->addTab(POPsTab, "Persistent Organic Pollutants (POPs)");
    tabWidget->addTab(ELI, "Environmental Litter Indicators");
    tabWidget->addTab(FC, "Fluorinated Compounds");
    tabWidget->addTab(CD, "Compliance Dashboard");

    // Add TabWidget to the main layout
    mainLayout->addWidget(tabWidget);
}

void WaterSampleWindow::setStatusBarAndMenuBar() {
    // Create status bar
    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->showMessage("Ready");

    // Create Menu Bar
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* loadCSVAction = new QAction("Load CSV", this);
    connect(loadCSVAction, &QAction::triggered, this, &WaterSampleWindow::loadCSV);
    fileMenu->addAction(loadCSVAction);
}

void WaterSampleWindow::connectSlots() {
    // Wait to edit
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
        // Inform other pages that the database has been updated
        emit dbUpdated();
    }
    else {
        QMessageBox::critical(this, "Error", "Error loading CSV data.");
    }
}

void WaterSampleWindow::searchData() {
    QString filterLabel = searchInput->text(); // Get the input text for filtering
    WaterSampleDatabase db(dbFilePath.toStdString());

    // Get the filtered model
    QSqlTableModel* filteredModel = db.getTableModel(filterLabel);

    if (filteredModel) {
        tableView->setModel(filteredModel); // Update the TableView with the filtered model
        statusBar()->showMessage(filterLabel.isEmpty() ? "Showing all data" : "Filtered by: " + filterLabel);
    }
    else {
        QMessageBox::warning(this, "Error", "No data found for the given filter.");
    }
}