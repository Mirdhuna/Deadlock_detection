#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ResourceManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnInitialize_clicked();
    void on_Safety_clicked();
    void on_Request_clicked();
    void on_Release_clicked();
    void on_Crash_clicked();

private:
    Ui::MainWindow *ui;
    ResourceManager *manager;

    int processes;
    int resources;
};

#endif
