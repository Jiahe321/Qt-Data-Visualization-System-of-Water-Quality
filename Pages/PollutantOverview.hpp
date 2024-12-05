#pragma once

#include <QWidget>
#include "database.hpp"


class PollutantOverview : public QWidget {
public:
    PollutantOverview(WaterSampleDatabase* database, QWidget* parent = nullptr);

public slots:
    void updateChart(); // 更新图表槽函数

private:
    WaterSampleDatabase* db; // 共享的数据库对象
    QLabel* infoLabel;
    QChartView* chartView;
};