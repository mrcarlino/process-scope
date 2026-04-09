#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

// Structs
#include "CpuStats.h"
#include "MemoryStats.h"
#include "NetworkStats.h"
#include "ProcessInfo.h"

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
    void updateNetworkStats(const NetworkStats& stats);

    void updateProcessList(const std::vector<ProcessInfo>& processes);
    void updateProcessDetails(const ProcessDetails& details);

signals:
    void metricSelected(const QModelIndex& index); // TODO: is this needed? I can just color internally

private:
    void loadStylesheet();

    Ui::Viewer *ui;

    MetricChartView* mMetricChartView;
    ProcessDetailView* mProcessDetailView;

    QStandardItemModel* mProcessModel;
    QSortFilterProxyModel* mProcessProxyModel;
};

#endif // VIEWER_H
