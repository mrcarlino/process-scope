#include "Controller.h"

Controller::Controller(QObject *parent)
{
    // Create the main window
    mViewer = new Viewer();
    mViewer->show();

    setConnections();

    mTimer = new QTimer(this);
    mTimer->setInterval(1000); // 1 Hz
    connect(mTimer, &QTimer::timeout, this, &Controller::updateData);

    mTimer->start();

    std::string cpuModelName = mOsMetricsProvider.getCpuModelName();
    mViewer->updateCpuModelName(cpuModelName);
}

Controller::~Controller()
{
    mTimer->stop();
}

void Controller::setConnections()
{
    // These need signals to go back up
    connect(mViewer, &Viewer::metricSelected, this, [&](const QModelIndex &index){
        QModelIndex pidIndex = index.sibling(index.row(), 0);
        int pid = pidIndex.data().toInt();

        ProcessDetails details = mOsMetricsProvider.queryProcessDetails(pid);

        mViewer->updateProcessDetails(details);
    }); 
    // connect(); // ProcessTableView::processSelected to Controller::onProcessSelected
}

void Controller::updateData()
{
    // Collect and render process info
    std::vector<ProcessInfo> processList = mOsMetricsProvider.queryProcessTable();
    // TODO: add call to ProcessTableView

    // Collect and render memory stats
    CpuStats cpuStats = mOsMetricsProvider.queryCpuStats();
    mViewer->updateCpuStats(cpuStats);

    // Collect and render memory stats
    MemoryStats memoryStats = mOsMetricsProvider.queryMemoryStats();
    mViewer->updateMemoryStats(memoryStats);

    // Collect and render process list
    std::vector<ProcessInfo> processes = mOsMetricsProvider.queryProcessTable();
    mViewer->updateProcessList(processes);

    // Collect and render selected process details
    if (mCurrentSelectedMetricIndex != -1)
    {
        ProcessDetails processDetails = mOsMetricsProvider.queryProcessDetails(mCurrentSelectedMetricIndex);
        // TODO: add call to ProcessDetailView
    }
}

void Controller::onTotalMetricSelected(int metricIndex)
{

}

void Controller::onProcessSelected(int pid)
{

}
