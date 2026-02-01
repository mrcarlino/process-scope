#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>

#include "CpuStats.h"
#include "MemoryStats.h"

#include "ProcessTableView.h"
#include "ProcessDetailView.h"
#include "MetricChartView.h"

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

    void updateCpuModelName(const std::string& label);
    void updateCpuStats(const CpuStats& stats);
    void updateMemoryStats(const MemoryStats& stats);

signals:
    void metricSelected(int metricIndex); // TODO: is this needed? I can just color internally

private:
    void loadStylesheet();

    Ui::Viewer *ui;

    MetricChartView* mMetricChartView;
    ProcessDetailView* mProcessDetailView;
    ProcessTableView* mProcessTableView;
};

#endif // VIEWER_H
