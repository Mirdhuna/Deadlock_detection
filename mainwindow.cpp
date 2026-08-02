
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <vector>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Safety->setEnabled(false);
    ui->Request->setEnabled(false);
    ui->Release->setEnabled(false);
    ui->Crash->setEnabled(false);

    /* Prevent manual editing of NEED table */
    ui->tableNeed->setEditTriggers(QAbstractItemView::NoEditTriggers);
    statusBar()->showMessage("System Ready");
  }

MainWindow::~MainWindow()
{
    delete ui;
}

/* ---------------- INITIALIZE ---------------- */

void MainWindow::on_btnInitialize_clicked()
{
    int processes = ui->SpinProcesses->value();
    int resources = ui->spinResources->value();

    ui->tableAllocation->setRowCount(processes);
    ui->tableAllocation->setColumnCount(resources);

    ui->tableMax->setRowCount(processes);
    ui->tableMax->setColumnCount(resources);

    ui->tableNeed->setRowCount(processes);
    ui->tableNeed->setColumnCount(resources);

    ui->tableAvailable->setRowCount(1);
    ui->tableAvailable->setColumnCount(resources);

    QStringList resourceHeaders;

    for(int j=0;j<resources;j++)
        resourceHeaders << "R" + QString::number(j);

    ui->tableAllocation->setHorizontalHeaderLabels(resourceHeaders);
    ui->tableMax->setHorizontalHeaderLabels(resourceHeaders);
    ui->tableNeed->setHorizontalHeaderLabels(resourceHeaders);
    ui->tableAvailable->setHorizontalHeaderLabels(resourceHeaders);

    QStringList processHeaders;

    for(int i=0;i<processes;i++)
        processHeaders << "P" + QString::number(i);

    ui->tableAllocation->setVerticalHeaderLabels(processHeaders);
    ui->tableMax->setVerticalHeaderLabels(processHeaders);
    ui->tableNeed->setVerticalHeaderLabels(processHeaders);

    ui->tableAllocation->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableMax->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableNeed->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableAvailable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /* Request table */

    ui->tableRequest->setRowCount(1);
    ui->tableRequest->setColumnCount(resources);
    ui->tableRequest->setHorizontalHeaderLabels(resourceHeaders);
    ui->tableRequest->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->SpinRequestProcess->setMaximum(processes-1);
    ui->Safety->setEnabled(true);
    ui->Request->setEnabled(true);
    ui->Release->setEnabled(true);
    ui->Crash->setEnabled(true);

    statusBar()->showMessage("System Initialized");
}

/* ---------------- SAFETY CHECK ---------------- */

void MainWindow::on_Safety_clicked()
{
    int processes = ui->tableAllocation->rowCount();
    int resources = ui->tableAllocation->columnCount();

    std::vector<std::vector<int>> alloc(processes, std::vector<int>(resources));
    std::vector<std::vector<int>> max(processes, std::vector<int>(resources));
    std::vector<std::vector<int>> need(processes, std::vector<int>(resources));
    std::vector<int> available(resources);

    for(int i=0;i<processes;i++)
    {
        for(int j=0;j<resources;j++)
        {
            alloc[i][j] = ui->tableAllocation->item(i,j)
                    ? ui->tableAllocation->item(i,j)->text().toInt() : 0;

            max[i][j] = ui->tableMax->item(i,j)
                    ? ui->tableMax->item(i,j)->text().toInt() : 0;

            if(max[i][j] < alloc[i][j])
            {
                QMessageBox::warning(this,"Error",
                                     "Max cannot be less than Allocation");
                return;
            }

            need[i][j] = max[i][j] - alloc[i][j];

            ui->tableNeed->setItem(i,j,
                new QTableWidgetItem(QString::number(need[i][j])));
        }
    }

    for(int j=0;j<resources;j++)
    {
        available[j] = ui->tableAvailable->item(0,j)
                ? ui->tableAvailable->item(0,j)->text().toInt() : 0;
    }
    qDebug() << "Available resources:";
    for(int j=0;j<resources;j++)
        qDebug() << available[j];
    std::vector<bool> finish(processes,false);
    std::vector<int> safeSequence;
    std::vector<int> work = available;

    for(int count=0; count<processes; count++)
    {
        bool found=false;

        for(int i=0;i<processes;i++)
        {
            if(!finish[i])
            {
                bool possible=true;

                for(int j=0;j<resources;j++)
                {
                    if(need[i][j] > work[j])
                    {
                        possible=false;
                        break;
                    }
                }

                if(possible)
                {
                    for(int j=0;j<resources;j++)
                        work[j]+=alloc[i][j];

                    safeSequence.push_back(i);
                    finish[i]=true;
                    found=true;
                }
            }
        }

        if(!found)
            break;
    }

    if(safeSequence.size()==processes)
    {
        QString seq="Safe Sequence: ";

        for(int p : safeSequence){
            seq += "P" + QString::number(p) + " ";

            for(int j=0;j<resources;j++)
            {
                ui->tableAllocation->item(p,j)->setBackground(Qt::green);
                ui->tableMax->item(p,j)->setBackground(Qt::green);
                ui->tableNeed->item(p,j)->setBackground(Qt::green);
            }
        }
        QMessageBox::information(this,"System State","SAFE\n"+seq);
    }
    else
    {
        for(int i=0;i<processes;i++)
        {
            for(int j=0;j<resources;j++)
            {
                ui->tableAllocation->item(i,j)->setBackground(Qt::red);
            }
        }
        QMessageBox::warning(this,"System State",
                             "System is NOT SAFE (Deadlock Possible)");
    }
    QString summary="System Summary\n";

    for(int j=0;j<resources;j++)
    {
        int allocated=0;

        for(int i=0;i<processes;i++)
            allocated += alloc[i][j];

        summary += "R" + QString::number(j) +
                   " Allocated: " + QString::number(allocated) +
                   " Available: " + QString::number(available[j]) + "\n";
    }

    QMessageBox::information(this,"Resource Summary",summary);
}

/* ---------------- REQUEST ---------------- */

void MainWindow::on_Request_clicked()
{
    int processes = ui->tableAllocation->rowCount();
    int resources = ui->tableAllocation->columnCount();
    int p = ui->SpinRequestProcess->value();

    std::vector<std::vector<int>> alloc(processes, std::vector<int>(resources));
    std::vector<std::vector<int>> max(processes, std::vector<int>(resources));
    std::vector<std::vector<int>> need(processes, std::vector<int>(resources));
    std::vector<int> available(resources);
    std::vector<int> request(resources);

    for(int i=0;i<processes;i++)
    {
        for(int j=0;j<resources;j++)
        {
            alloc[i][j] = ui->tableAllocation->item(i,j)
                    ? ui->tableAllocation->item(i,j)->text().toInt() : 0;

            max[i][j] = ui->tableMax->item(i,j)
                    ? ui->tableMax->item(i,j)->text().toInt() : 0;

            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    for(int j=0;j<resources;j++)
    {
        available[j] = ui->tableAvailable->item(0,j)
                ? ui->tableAvailable->item(0,j)->text().toInt() : 0;

        request[j] = ui->tableRequest->item(0,j)
                ? ui->tableRequest->item(0,j)->text().toInt() : 0;
    }

    for(int j=0;j<resources;j++)
    {
        if(request[j] > need[p][j])
        {
            QMessageBox::warning(this,"Error",
                                 "Request exceeds process NEED.");
            return;
        }

        if(request[j] > available[j])
        {
            QMessageBox::warning(this,"Wait",
                                 "Resources not available.");
            return;
        }
    }

    for(int j=0;j<resources;j++)
    {
        available[j] -= request[j];
        alloc[p][j] += request[j];
        need[p][j] -= request[j];
    }

    std::vector<bool> finish(processes,false);
    std::vector<int> safeSequence;
    std::vector<int> work = available;

    for(int count=0; count<processes; count++)
    {
        bool found=false;

        for(int i=0;i<processes;i++)
        {
            if(!finish[i])
            {
                bool possible=true;

                for(int j=0;j<resources;j++)
                {
                    if(need[i][j] > work[j])
                    {
                        possible=false;
                        break;
                    }
                }

                if(possible)
                {
                    for(int j=0;j<resources;j++)
                        work[j]+=alloc[i][j];

                    safeSequence.push_back(i);
                    finish[i]=true;
                    found=true;
                }
            }
        }

        if(!found) break;
    }

    if(safeSequence.size()==processes)
    {
        for(int j=0;j<resources;j++)
        {
            ui->tableAvailable->setItem(0,j,
                new QTableWidgetItem(QString::number(available[j])));

            ui->tableAllocation->setItem(p,j,
                new QTableWidgetItem(QString::number(alloc[p][j])));

            ui->tableNeed->setItem(p,j,
                new QTableWidgetItem(QString::number(need[p][j])));
        }

        QMessageBox::information(this,"Request Granted","System remains SAFE.");
    }
    else
    {
        QMessageBox::warning(this,"Request Denied",
                             "Request would make system UNSAFE.");
    }

    /* Clear request table */

    for(int j=0;j<resources;j++)
        ui->tableRequest->setItem(0,j,new QTableWidgetItem("0"));
    for(int j=0;j<resources;j++)
    {
        ui->tableRequest->setItem(0,j,new QTableWidgetItem("0"));
    }
    for(int i=0;i<processes;i++)
    {
        for(int j=0;j<resources;j++)
        {
            int alloc = ui->tableAllocation->item(i,j)->text().toInt();
            int max = ui->tableMax->item(i,j)->text().toInt();

            ui->tableNeed->setItem(i,j,
                new QTableWidgetItem(QString::number(max-alloc)));
        }
    }
}

/* ---------------- RELEASE ---------------- */

void MainWindow::on_Release_clicked()
{
    int p = ui->SpinRequestProcess->value();
    int resources = ui->tableAllocation->columnCount();

    for(int j=0;j<resources;j++)
    {
        int release = ui->tableRequest->item(0,j)
                ? ui->tableRequest->item(0,j)->text().toInt() : 0;

        int alloc = ui->tableAllocation->item(p,j)->text().toInt();
        int avail = ui->tableAvailable->item(0,j)->text().toInt();
        int need  = ui->tableNeed->item(p,j)->text().toInt();

        if(release > alloc)
        {
            QMessageBox::warning(this,"Error",
                                 "Cannot release more than allocated");
            return;
        }

        ui->tableAllocation->setItem(p,j,
            new QTableWidgetItem(QString::number(alloc-release)));

        ui->tableAvailable->setItem(0,j,
            new QTableWidgetItem(QString::number(avail+release)));

        ui->tableNeed->setItem(p,j,
            new QTableWidgetItem(QString::number(need+release)));
    }
    statusBar()->showMessage("Resources released from process P" + QString::number(p));
    for(int i=0;i<processes;i++)
    {
        for(int j=0;j<resources;j++)
        {
            int alloc = ui->tableAllocation->item(i,j)->text().toInt();
            int max = ui->tableMax->item(i,j)->text().toInt();

            ui->tableNeed->setItem(i,j,
                new QTableWidgetItem(QString::number(max-alloc)));
        }
    }
}

/* ---------------- CRASH ---------------- */

void MainWindow::on_Crash_clicked()
{
    int p = ui->SpinRequestProcess->value();
    int resources = ui->tableAllocation->columnCount();

    for(int j=0;j<resources;j++)
    {
        int alloc = ui->tableAllocation->item(p,j)->text().toInt();
        int avail = ui->tableAvailable->item(0,j)->text().toInt();
        int max   = ui->tableMax->item(p,j)->text().toInt();

        ui->tableAvailable->setItem(0,j,
            new QTableWidgetItem(QString::number(avail+alloc)));

        ui->tableAllocation->setItem(p,j,new QTableWidgetItem("0"));

        ui->tableNeed->setItem(p,j,
            new QTableWidgetItem(QString::number(max)));
    }
    statusBar()->showMessage("Process crashed: P" + QString::number(p));
    for(int i=0;i<processes;i++)
    {
        for(int j=0;j<resources;j++)
        {
            int alloc = ui->tableAllocation->item(i,j)->text().toInt();
            int max = ui->tableMax->item(i,j)->text().toInt();

            ui->tableNeed->setItem(i,j,
                new QTableWidgetItem(QString::number(max-alloc)));
        }
    }
}
