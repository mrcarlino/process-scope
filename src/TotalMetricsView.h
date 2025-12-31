#ifndef TOTALMETRICSVIEW_H
#define TOTALMETRICSVIEW_H

// Qt includes
#include <QObject>

// Local includes
#include  "SystemTotals.h"

class TotalMetricsView : public QObject
{
    Q_OBJECT

public:
    explicit TotalMetricsView(QObject* parent = nullptr);
    ~TotalMetricsView();

signals:
    void metricSelected(int metricIndex); // TODO: is this needed? I can just color internally

public slots:
    void updateTotals(const SystemTotals& totals);

private:

};

#endif // TOTALMETRICSVIEW_H