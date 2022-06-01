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
    ui->Connection->setText("En cours de connexion...");
    this->repaint();
    robot.doConnect();
    this->close();
    robotController.show();
    robotController.setRobot(&robot);
}
