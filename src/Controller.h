#ifndef CONTROLLER_H
#define CONTROLLER_H

// STL includes
#include <vector>

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

signals:
    // Update the views
    void processTableUpdated(const std::vector<ProcessInfo>& list);
    void systemTotalsUpdated(const MemoryStats& totals);
    void selectedProcessUpdated(const ProcessDetails& details);
    void metricHistoryUpdated(const std::vector<double>& history);

public slots:
    void updateData(); // On timer
    void onTotalMetricSelected(int metricIndex);
    void onProcessSelected(int pid);

private:
    void setConnections();

    Viewer* mViewer;

    OsMetricsProvider mOsMetricsProvider;
    QTimer* mTimer;

    int mCurrentSelectedMetricIndex; // TODO: add to initializer list
    int mCurrentSelectedPid; // TODO: add to initializer list

    std::vector<double> memHistory;
};

#endif // CONTROLLER_H
