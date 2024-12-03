#include "POPs.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QTableView>

// Change it! You can try to use the code structure of main window
POPs::POPs(WaterSampleDatabase* database, QWidget* parent)
    : QWidget(parent), db(database), chartView(nullptr) {

    QVBoxLayout* layout = new QVBoxLayout(this);

    // 提示标签
    infoLabel = new QLabel("No CSV file selected", this);

    // 添加初始组件
    layout->addWidget(infoLabel);

    setLayout(layout);
}

void POPs::updateChart() {
    if (chartView) {
        layout()->removeWidget(chartView);
        delete chartView;
        chartView = nullptr;
    }
    if (!db) {
        infoLabel->setText("Database not available");
        infoLabel->show();
        return;
    }

    chartView = db->createPOPLevelsChart();

   if (chartView) {
        layout()->addWidget(chartView); // 添加图表到布局
        infoLabel->hide(); // 隐藏提示信息
    } else {
        infoLabel->setText("No data available for the selected pollutant");
        infoLabel->show();
    }

}