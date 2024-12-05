import QtQuick 2.15
import QtQuick.Controls 2.15
import QtLocation 5.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Geographical Hotspots"

    Plugin {
        id: mapPlugin
        name: "osm" // 使用 OpenStreetMap 插件
    }

    Map {
        id: map
        width: parent.width
        height: parent.height
        plugin: mapPlugin
        center: QtPositioning.coordinate(51.5074, -0.1278) // 设置地图中心（伦敦的坐标）
        zoomLevel: 10
        Component.onCompleted: {
                console.log("Map loaded!")
            }

        // 标记热点
        MapItemView {
            model: hotspotsModel // 绑定热点数据模型
            delegate: MapQuickItem {
                coordinate: QtPositioning.coordinate(model.latitude, model.longitude)
                sourceItem: Rectangle {
                    width: 10
                    height: 10
                    color: "red"
                    radius: 5
                }
            }
        }
    }
}
