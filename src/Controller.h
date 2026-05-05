#ifndef CONTROLLER_H
#define CONTROLLER_H

// STL includes
#include <vector>
#include <deque>

// Qt includes
#include <QObject>
#include <QTimer>

// Local includes
#include "Viewer.h"
#include "OsMetricsProvider.h"
#include "ProcessInfo.h"
#include "ProcessDetails.h"

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

public slots:
    void updateData(); // On timer
    void onTotalMetricSelected(int metricIndex);

private:
    void setConnections();
    void cpuHistoryHandler(float latestValue);
    void memHistoryHandler(float latestValue);
    void selectedMetricHandler();

    Viewer* mViewer;

    OsMetricsProvider mOsMetricsProvider;
    QTimer* mTimer;

    int mCurrentSelectedPid;
    int mCurrentSelectedTotalMetric;

    std::deque<float> mTotalCpuHistory;
    std::deque<float> mTotalMemHistory;
};

#endif // CONTROLLER_H
