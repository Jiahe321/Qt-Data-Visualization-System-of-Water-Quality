#include "GeographicalHotspots.hpp"
#include <QVBoxLayout>
#include <QQmlContext>
#include <QDebug>
GeographicalHotspots::GeographicalHotspots(WaterSampleDatabase* database, QWidget* parent)
    : QWidget(parent), db(database), engine(new QQmlApplicationEngine(this)) {

    QVBoxLayout* layout = new QVBoxLayout(this);

    // 加载地图
    loadMap();

    setLayout(layout);
}

// 加载 QML 文件并展示地图
void GeographicalHotspots::loadMap() {
    // 设置 QML 上下文数据（例如热点模型）
    QQmlContext* context = engine->rootContext();
    // context->setContextProperty("hotspotsModel", QVariant::fromValue(db->getSamples())); // 假设 getSamples 返回采样点数据

    qDebug() << "Current working directory: " << QDir::currentPath();

    // 加载 QML 文件
    engine->load(QUrl(QStringLiteral("qrc:/qml/MapView.qml")));
    // debug用
    if (engine->rootObjects().isEmpty()) {
        qWarning() << "Failed to load QML file!";
    }
}
