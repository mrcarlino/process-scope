#include "Viewer.h"
#include "ui_Viewer.h"
#include <sstream>

Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Viewer)
{
    ui->setupUi(this);

    resize(1200, 800);

    loadStylesheet();

    setupChart();

    // Initialize the Process Table model
    mProcessModel = new QStandardItemModel(this);
    mProcessModel->setColumnCount(2);
    mProcessModel->setHorizontalHeaderLabels(QStringList() << "PID" << "Name");

    // Connect the View to the Model
    ui->processTableView->setModel(mProcessModel);
    ui->processTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->processTableView->verticalHeader()->setVisible(false);

    // Set up table filtering
    mProcessProxyModel = new QSortFilterProxyModel(this);
    mProcessProxyModel->setSourceModel(mProcessModel);
    mProcessProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->processTableView->setModel(mProcessProxyModel);
    ui->processTableView->setSortingEnabled(true);

    // Set connections
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, [&](const QString& str) {
        mProcessProxyModel->setFilterKeyColumn(1); 
        mProcessProxyModel->setFilterFixedString(str);
    });

    connect(ui->totalCpuFrame, &ClickableCard::clicked, this, [this](){
        emit totalMetricSelected(1);
    });

    connect(ui->totalMemoryFrame, &ClickableCard::clicked, this, [this](){
        emit totalMetricSelected(2);
    });

    // Process list row click
    connect(ui->processTableView, &QTableView::clicked, this, [&](const QModelIndex& index) { emit processSelected(index); });
}

Viewer::~Viewer()
{
    delete ui;
}

void Viewer::updateCpuModelName(const std::string& label)
{
    ui->totalCpuSubLabel->setText(QString::fromStdString(label));
}

void Viewer::updateCpuStats(const CpuStats& stats)
{
    QString label = QString::number(stats.percent, 'f', 2) + "%";

    ui->totalCpuValueLabel->setText(label);
}

void Viewer::updateMemoryStats(const MemoryStats& stats)
{
    QString label = QString::number(stats.percent, 'f', 2) + "%";
    QString sublabel = QString::number(stats.usedMb, 'f', 0) + " / " + QString::number(stats.totalMb, 'f', 0) + " MB";

    ui->totalMemoryValueLabel->setText(label);
    ui->totalMemorySubLabel->setText(sublabel);
}

void Viewer::updateNetworkStats(const NetworkStats &stats)
{
    std::ostringstream d_out;
    d_out << std::fixed << std::setprecision(1);

    if (stats.download < 1024)
    {
        d_out << static_cast<int>(stats.download) << " B/s";
    }
    else if (stats.download < 1048576)
    {
        d_out << (stats.download / 1024.0) << " KB/s";
    }
    else
    {
        d_out << (stats.download / 1048576.0) << " MB/s"; // Does the division here!
    }

    std::ostringstream u_out;
    u_out << std::fixed << std::setprecision(1);

    if (stats.upload < 1024)
    {
        u_out << static_cast<int>(stats.upload) << " B/s";
    }
    else if (stats.upload < 1048576)
    {
        u_out << (stats.upload / 1024.0) << " KB/s";
    }
    else
    {
        u_out << (stats.upload / 1048576.0) << " MB/s"; // Does the division here!
    }

    QString label = "↓ " + QString::fromStdString(d_out.str()) + " | ↑ " + QString::fromStdString(u_out.str());
    QString sublabel = "Adapter: " + QString::fromStdString(stats.activeAdapter);

    ui->networkBandwidthValueLabel->setText(label);
    ui->networkBandwidthSubLabel->setText(sublabel);
}

void Viewer::updateSelectedMetric(std::string name, const std::deque<float>& history)
{
    ui->selectedMetricSectionLabel->setText(QString::fromStdString(name));

    // Build the new data points in memory
    QList<QPointF> points;
    
    // We want the newest data point at X = 0, and the oldest at X = -60.
    // If the deque has 45 items, we start at X = -45.
    int currentX = -history.size() + 1; 
    
    for (double value : history) 
    {
        points.append(QPointF(currentX, value));
        currentX++;
    }

    // Swap the data into the GPU in one atomic operation
    mSeries->replace(points);
}

void Viewer::updateProcessList(const std::vector<ProcessInfo> &processes)
{
    mProcessModel->removeRows(0, mProcessModel->rowCount());

    for (const ProcessInfo& process : processes)
    {
        QList<QStandardItem*> rowItems;

        QStandardItem* pidItem = new QStandardItem(QString::number(process.pid));
        pidItem->setTextAlignment(Qt::AlignCenter);
        rowItems << pidItem;

        QStandardItem* nameItem = new QStandardItem(QString::fromStdString(process.name));
        rowItems << nameItem;

        mProcessModel->appendRow(rowItems);
    }
}

void Viewer::updateProcessDetails(const ProcessDetails& details)
{
    QString nameLabel = details.name == "" ? "Process Details" : QString::fromStdString(details.name);
    QString pidLabel = details.pid < 0 ? "--" : QString::number(details.pid);
    QString threadLabel = details.threads < 0 ? "--" : QString::number(details.threads);
    QString memRssLabel = details.memoryRSS < 0.0 ? "--" : QString::number(details.memoryRSS, 'f', 2) + " MB";
    QString memVszLabel = details.memoryVSZ < 0.0 ? "--" : QString::number(details.memoryVSZ, 'f', 2) + " MB";

    ui->processDetailsSectionLabel->setText(nameLabel);
    ui->processPIDValueLabel->setText(pidLabel);
    ui->processThreadsValueLabel->setText(threadLabel);
    ui->processMemRSSValueLabel->setText(memRssLabel);
    ui->processMemVSZValueLabel->setText(memVszLabel);
}

void Viewer::restoreTableSelection(int pid) 
{
    if (pid == -1) return;

    QAbstractItemModel* model = ui->processTableView->model();
    if (!model) return;

    // Convert the int to a string so it matches the QStandardItem text
    QString pidString = QString::number(pid);

    QModelIndexList matches = model->match(
        model->index(0, 0),        // Starting index
        Qt::DisplayRole,           // Searching the displayed text
        pidString,                 // <--- Pass the STRING here
        1,                         
        Qt::MatchFixedString | Qt::MatchCaseSensitive // String matching flags
    );

    if (!matches.isEmpty()) {
        ui->processTableView->selectionModel()->select(
            matches.first(), 
            QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows
        );
    }
}

void Viewer::setupChart()
{
    // Create the data series
    mSeries = new QLineSeries();
    
    // Make the line look like a professional monitor
    QPen pen(QColor("#0078D7")); 
    pen.setWidth(2);
    mSeries->setPen(pen);

    // Create the chart and attach the series
    mChart = new QChart();
    mChart->addSeries(mSeries);
    mChart->legend()->hide();
    mChart->setMargins(QMargins(0, 0, 0, 0)); // Remove dead white space
    mChart->setBackgroundRoundness(0);

    // Setup the X Axis (Time: -60 seconds to 0)
    mAxisX = new QValueAxis();
    mAxisX->setRange(-60, 0);
    mAxisX->setLabelFormat("%d s");
    mChart->addAxis(mAxisX, Qt::AlignBottom);
    mSeries->attachAxis(mAxisX);

    // Setup the Y Axis (Values: 0 to Max)
    mAxisY = new QValueAxis();
    mAxisY->setRange(0, 100); // Default to 100%
    mAxisY->setLabelFormat("%d");
    mAxisY->setTickCount(3);
    mChart->addAxis(mAxisY, Qt::AlignLeft);
    mSeries->attachAxis(mAxisY);

    // Create the View and inject it into your UI layout
    QChartView *chartView = new QChartView(mChart);
    chartView->setRenderHint(QPainter::Antialiasing); // Smooth lines

    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *existingLayout = qobject_cast<QVBoxLayout*>(ui->selectedMetricFrame->layout());
    existingLayout->addWidget(chartView, 1);
}

void Viewer::loadStylesheet()
{
    setStyleSheet(R"(
        QMainWindow {
            background: #f5f5f5;
        }

        /* Card frames */
        QFrame[card="true"] {
            background: #ffffff;
            border-radius: 12px;
            border: 1px solid #e0e0e0;
        }

        /* Header title */
        QLabel#programName {
            font-size: 28px;
            font-weight: 600;
        }

        /* Search box */
        QLineEdit#searchLineEdit {
            border-radius: 6px;
            border: 1px solid #d0d0d0;
            padding: 4px 8px;
            background: #ffffff;
        }

        /* Header buttons */
        QPushButton#settingsButton,
        QPushButton#saveButton {
            background: #000000;
            color: #ffffff;
            border-radius: 6px;
            padding: 6px 14px;
            border: none;
        }

        QPushButton#settingsButton:hover,
        QPushButton#saveButton:hover {
            background: #333333;
        }

        QFrame[selectableCard="true"]:hover {
            border: 1px solid #bbbbbb;
            background: #fafafa;
        }

        QFrame[selectableCard="true"][selected="true"] {
            border: 2px solid #0078d4;
        }

        QLabel[objectName$="SectionLabel"] {
            font-size: 18px;
            font-weight: 500;
        }
        
        QLabel[objectName$="CardLabel"] {
            font-size: 22px;
            font-weight: 400;
        }

        QLabel[objectName$="ValueLabel"] {
            font-size: 26px;
            font-weight: 600;
        }
    )");
}
