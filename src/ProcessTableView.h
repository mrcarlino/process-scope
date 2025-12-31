#ifndef PROCESSTABLEVIEW_H
#define PROCESSTABLEVIEW_H

// STL includes
#include <vector>

// Qt includes
#include <QObject>
#include <QTableView>

// Local includes
#include "ProcessInfo.h"

class ProcessTableView : public QTableView
{
    Q_OBJECT
public:
    explicit ProcessTableView(QObject* parent = nullptr);

signals:
    void processSelected(int pid);

private slots:
    void onRowActivated(const QModelIndex& index);
};

class ProcessTableModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

public slots:
    void setProcessTable(const std::vector<ProcessInfo>& list);

private:
    std::vector<ProcessInfo> mRows;
};

#endif // PROCESSTABLEVIEW_H