#pragma once

#include <QWidget>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include "database.hpp"

class GeographicalHotspots : public QWidget {
    Q_OBJECT
public:
    explicit GeographicalHotspots(WaterSampleDatabase* database, QWidget* parent = nullptr);

public slots:
    void updateGraph() {}; // 更新热点地图

private:
    void loadMap(); // 加载地图

    WaterSampleDatabase* db; // 数据库对象
    QQmlApplicationEngine* engine;
};
