#include "Controller.h"

Controller::Controller(QObject *parent)
{
    // Create the main window
    mViewer = new Viewer();
    mViewer->show();

    setConnections();
}

Controller::~Controller()
{
}

void Controller::setConnections()
{
    // connect(); // TotalMetricsView::metricSelected to Controller::onTotalMetricSelected 
    // connect(); // ProcessTableView::processSelected to Controller::onProcessSelected
    // connect(); // Controller::systemTotalsUpdated to TotalMetricsView::updateTotals
    // connect(); // Controller::metricHistoryUpdated to MetricChartView::setHistory
    // connect(); // Controller::processTableUpdated to ProcessTableView:: ?
    // connect(); // Controller::selectedProcessUpdated to ProcessDetailView::setDetails
}

void Controller::updateData()
{

}

void Controller::onTotalMetricSelected(int metricIndex)
{

}

void Controller::onProcessSelected(int pid)
{

}
