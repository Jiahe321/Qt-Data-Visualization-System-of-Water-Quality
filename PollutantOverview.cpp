#include "PollutantOverview.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QChartView>

PollutantOverview::PollutantOverview(WaterSampleDatabase* database, QWidget* parent)
    : QWidget(parent), db(database), chartView(nullptr) {

    QVBoxLayout* layout = new QVBoxLayout(this);

    // 提示标签
    infoLabel = new QLabel("No CSV file selected", this);

    // 添加初始组件
    layout->addWidget(infoLabel);

    setLayout(layout);
}


void PollutantOverview::updateChart() {
    // 清理旧的图表
    if (chartView) {
        layout()->removeWidget(chartView);
        delete chartView;
        chartView = nullptr;
    }

    // Test sample
    QString determinand = "1,1,1,2 -TET";

    // 检查数据库是否可用
    if (!db) {
        infoLabel->setText("Database not available");
        infoLabel->show();
        return;
    }

    // 使用共享的数据库对象生成图表
    chartView = db->createPollutantTrendChart(determinand);

    if (chartView) {
        layout()->addWidget(chartView); // 添加图表到布局
        infoLabel->hide(); // 隐藏提示信息
    } else {
        infoLabel->setText("No data available for the selected pollutant");
        infoLabel->show();
    }
}
