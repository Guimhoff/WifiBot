#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Connection_clicked()
{
    robot.doConnect();
}


void MainWindow::on_Disconnection_clicked()
{
    robot.disConnect();
}
