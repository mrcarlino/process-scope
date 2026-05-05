#ifndef VIEWER_H
#define VIEWER_H

// STL includes
#include <deque>

// Qt includes
#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

// Structs
#include "CpuStats.h"
#include "MemoryStats.h"
#include "NetworkStats.h"
#include "ProcessInfo.h"

// Local includes
#include "ProcessDetails.h"

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
    void updateSelectedMetric(std::string name, const std::deque<float>& history);

    void updateProcessList(const std::vector<ProcessInfo>& processes);
    void updateProcessDetails(const ProcessDetails& details);
    void restoreTableSelection(int pid);

    void setupChart();
    void updateChartData(const std::deque<float>& historyData, double maxYValue);

signals:
    void processSelected(const QModelIndex& index);
    void totalMetricSelected(int index);

private:
    void loadStylesheet();

    Ui::Viewer *ui;

    QStandardItemModel* mProcessModel;
    QSortFilterProxyModel* mProcessProxyModel;
    QChart *mChart;
    QLineSeries *mSeries;
    QValueAxis *mAxisX;
    QValueAxis *mAxisY;
};

#endif // VIEWER_H
