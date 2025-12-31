#include "ProcessTableView.h"

ProcessTableView::ProcessTableView(QObject *parent)
{
}

void ProcessTableView::onRowActivated(const QModelIndex& index)
{

}

int ProcessTableModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

int ProcessTableModel::columnCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant ProcessTableModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

void ProcessTableModel::setProcessTable(const std::vector<ProcessInfo>& list)
{

}
