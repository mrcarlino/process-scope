#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>

// Forward declarations
class TotalMetricsView;
class MetricChartView;
class ProcessDetailView;
class ProcessTableView;

namespace Ui
{
    class Viewer;
}

class Viewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Viewer(QWidget *parent = nullptr);
    ~Viewer();

private:
    void loadStylesheet();

    Ui::Viewer *ui;

    TotalMetricsView* mTotalMetricsView;
    MetricChartView* mMetricChartView;
    ProcessDetailView* mProcessDetailView;
    ProcessTableView* mProcessTableView;
};

#endif // VIEWER_H
