#ifndef PROCESSDETAILVIEW_H
#define PROCESSDETAILVIEW_H

// Qt includes
#include <QObject>

// Local includes
#include "ProcessDetails.h"

class ProcessDetailView : public QObject
{
    Q_OBJECT

public:
    explicit ProcessDetailView(QObject* parent = nullptr);

signals:

public slots:
    void setDetails(const ProcessDetails& details);

private:

};

#endif // PROCESSDETAILVIEW_H