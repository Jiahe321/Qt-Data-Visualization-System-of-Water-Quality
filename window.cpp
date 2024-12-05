#include "window.hpp"

WaterSampleWindow::WaterSampleWindow(QWidget* parent)
    : QMainWindow(parent), dbFilePath("water_samples.db"), csvFilePath("") {
    // shared db
    db = new WaterSampleDatabase(dbFilePath.toStdString());
    createWidgets();
    arrangeWidgets();
    connectSlots();
    setWindowTitle(tr("Water Sample Data"));
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
    CD = new ComplianceDashboard(db,this);
    PO = new PollutantOverview(db,this);
    POPsTab = new POPs(db, this);
    ELI = new EnvironmentalLitterIndicators(db, this);
    FC = new FluorinatedCompounds(db, this);
    GHS = new GeographicalHotspots(db, this);
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
    QLabel* searchLabel = new QLabel(tr("Search by Determinand Label:"));
    searchInput = new QLineEdit();
    QPushButton* searchButton = new QPushButton(tr("Search"));
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
    tabWidget->addTab(dataPage, tr("Data View"));
    tabWidget->addTab(PO, tr("Pollutant Overview"));
    tabWidget->addTab(POPsTab, tr("Persistent Organic Pollutants (POPs)"));
    tabWidget->addTab(ELI, tr("Environmental Litter Indicators"));
    tabWidget->addTab(FC, tr("Fluorinated Compounds"));
    tabWidget->addTab(CD, tr("Compliance Dashboard"));
    tabWidget->addTab(GHS, tr("Geographical Hotspots"));

    // Add TabWidget to the main layout
    mainLayout->addWidget(tabWidget);
}

void WaterSampleWindow::setStatusBarAndMenuBar() {
    // Create status bar
    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->showMessage("Ready");

    // Create Menu Bar
    // File menu
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    QAction* loadCSVAction = new QAction(tr("Load CSV"), this);
    loadCSVAction->setShortcut(QKeySequence::Open);
    loadCSVAction->setStatusTip(tr("Load CSV data into the database"));
    connect(loadCSVAction, &QAction::triggered, this, &WaterSampleWindow::loadCSV);
    fileMenu->addAction(loadCSVAction);

    QAction* close = new QAction(tr("Quit"), this);
    close->setShortcut(QKeySequence::Close);
    close->setStatusTip(tr("Quit the application"));
    connect(close, &QAction::triggered, this, &WaterSampleWindow::close);
    fileMenu->addAction(close);

    // Help menu
    QMenu* helpMenu = menuBar()->addMenu(tr("Help"));

    QAction* aboutAction = new QAction(tr("&About"), this);
    // TODO: set shortcut
    // aboutAction->setShortcut(QKeySequence::HelpContents);
    aboutAction->setStatusTip(tr("Show information about this application"));
    connect(aboutAction, &QAction::triggered, this, &WaterSampleWindow::about);
    helpMenu->addAction(aboutAction);

    QAction* aboutQtAction = new QAction(tr("About &Qt"), this);
    // TODO: set shortcut
    // aboutQtAction->setShortcut(QKeySequence::AboutQt);
    aboutQtAction->setStatusTip(tr("Show information about the Qt library"));
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
    helpMenu->addAction(aboutQtAction);
}

// Slot implementation for About
void WaterSampleWindow::about() {
    QMessageBox::about(this, "About <> Application",
        "line "
        "line"
        "line");
}

void WaterSampleWindow::connectSlots() {
    // Wait to edit
    connect(this, &WaterSampleWindow::dbUpdated, PO, &PollutantOverview::updateChart);
    connect(this, &WaterSampleWindow::dbUpdated, POPsTab, &POPs::updateChart);
    connect(this, &WaterSampleWindow::dbUpdated, ELI, &EnvironmentalLitterIndicators::updateChart);
	connect(this, &WaterSampleWindow::dbUpdated, CD, &ComplianceDashboard::updateChart);
	connect(this, &WaterSampleWindow::dbUpdated, FC, &FluorinatedCompounds::updateChart);
    // connect(this, &WaterSampleWindow::dbUpdated, GHS, &GeographicalHotspots::updateGraph);
}

void WaterSampleWindow::loadCSV() {
    // Open a file dialog to select the CSV file
    csvFilePath = QFileDialog::getOpenFileName(
        this, tr("Select CSV File"), QString(), tr("CSV Files (*.csv)"));

    if (csvFilePath.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No file selected."));
        return;
    }

    // Database operations
    if (!db->createTable()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to create or verify the database table."));
        return;
    }

    if (db->readCSV(csvFilePath.toStdString())) {
        tableView->setModel(db->getTableModel());
        statusBar()->showMessage(tr("CSV data loaded successfully."));
        // Inform other pages that the database has been updated
        emit dbUpdated();
    }
    else {
        QMessageBox::critical(this, tr("Error"), tr("Error loading CSV data."));
    }
}

void WaterSampleWindow::searchData() {
    QString filterLabel = searchInput->text(); // Get the input text for filtering
    WaterSampleDatabase db(dbFilePath.toStdString());

    // Get the filtered model
    QSqlTableModel* filteredModel = db.getTableModel(filterLabel);

    if (filteredModel) {
        tableView->setModel(filteredModel); // Update the TableView with the filtered model
        statusBar()->showMessage(filterLabel.isEmpty() ? tr("Showing all data") : tr("Filtered by: ") + filterLabel);
    }
    else {
        QMessageBox::warning(this, tr("Error"), tr("No data found for the given filter."));
    }
}