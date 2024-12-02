#include "methods.hpp"

QChartView* Methods::createPollutantTrendChart(const QString& pollutantName) {
    // Create SQL query
    QSqlQuery query(db);
    query.prepare("SELECT sampleDateTime, resultQualifierNotation, result, unitLabel, isComplianceSample FROM water_samples WHERE determinandLabel = :pollutantName ORDER BY sampleDateTime");
    query.bindValue(":pollutantName", pollutantName);

    // execute query
    if (!query.exec()) {
        qDebug() << "Query failed：" << query.lastError();
        return nullptr;
    }

    // Create chart
    QChart* chart = new QChart();

    QLineSeries* lineSeries = new QLineSeries();

    // Create and set X axis (DateTime axis)
    QDateTimeAxis* axisX = new QDateTimeAxis();
    axisX->setFormat("yyyy-MM-dd hh:mm:ss"); // 时间格式
    axisX->setTitleText("DateTime");  // 设置 X 轴标题
    chart->addAxis(axisX, Qt::AlignBottom);
    lineSeries->attachAxis(axisX);

    // Query for unitLabel
    QString unitLabel;
    if (query.next()) {
        unitLabel = query.value(3).toString();
    }
    else {
        unitLabel = "Unknown";
    }

    // Create and set Y axis (Value axis)
    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("Pollutant Level (" + unitLabel + ")");  // Y 轴的标题加单位
    axisY->setLabelFormat("%.f"); // 格式化数值，可以调整精度
    chart->addAxis(axisY, Qt::AlignLeft);
    lineSeries->attachAxis(axisY);

    // Query for data and add to series
    while (query.next()){
        QDateTime sampleDateTime = QDateTime::fromString(query.value(0).toString(), "yyyy-MM-ddTHH:mm:ss");
        double result = query.value(2).toDouble();

        lineSeries->append(sampleDateTime.toMSecsSinceEpoch(), result);
    } 

    // Add series to chart
    chart->addSeries(lineSeries);

    // Set chart title
    chart->setTitle("Pollutant Trend: " + pollutantName);

    // Set chart view and return
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

// Maybe Useful later
QColor Methods::getComplianceColor(double value) {
    if (value < 50) {
        return QColor("green");  // safe
    }
    else if (value < 100) {
        return QColor("amber");  // caution
    }
    else {
        return QColor("red");    // exceeding safe levels
    }
}
