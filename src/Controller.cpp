#include "Controller.h"

Controller::Controller(QObject *parent) :
    mCurrentSelectedPid(-1)
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
    // TODO: disconnect if process ends or unclicked
    
    // These need signals to go back up
    connect(mViewer, &Viewer::metricSelected, this, [&](const QModelIndex &index){
        QModelIndex pidIndex = index.sibling(index.row(), 0);
        int newSelectedPid = pidIndex.data().toInt();
        mCurrentSelectedPid = mCurrentSelectedPid == pidIndex.data().toInt() ? -1 : newSelectedPid;

        mViewer->restoreTableSelection(mCurrentSelectedPid);

        if (mCurrentSelectedPid < 0)
        {
            ProcessDetails details;
            mViewer->updateProcessDetails(details);
        }

        updateData();
    });
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

    // Collect and render network stats
    NetworkStats networkStats = mOsMetricsProvider.queryNetworkStats();
    mViewer->updateNetworkStats(networkStats);

    // Collect and render process list
    std::vector<ProcessInfo> processes = mOsMetricsProvider.queryProcessTable();
    mViewer->updateProcessList(processes);

    // Collect and render selected process details
    if (mCurrentSelectedPid != -1)
    {
        ProcessDetails details = mOsMetricsProvider.queryProcessDetails(mCurrentSelectedPid);
        mViewer->updateProcessDetails(details);
    }

    mViewer->restoreTableSelection(mCurrentSelectedPid);
}

void Controller::onTotalMetricSelected(int metricIndex)
{

}

void Controller::onProcessSelected(int pid)
{

}
