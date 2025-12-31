#ifndef METRICCHARTVIEW_H
#define METRICCHARTVIEW_H

// STL includes
#include <vector>

// Qt includes
#include <QObject>

class MetricChartView : public QObject
{
    Q_OBJECT

public:
    explicit MetricChartView(QObject* parent = nullptr);

signals:

public slots:
    void setHistory(const std::vector<double>& history);

private:

};

#endif // METRICCHARTVIEW_H