#include "Viewer.h"
#include "ui_Viewer.h"

Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Viewer)
{
    ui->setupUi(this);

    resize(1200, 800);

    loadStylesheet();
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
