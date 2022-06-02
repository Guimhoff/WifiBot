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

void MainWindow::on_label_2_linkActivated(const QString &link)
{
    ui->label_2->setTextFormat(Qt::RichText);
    ui->label_2->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label_2->setOpenExternalLinks(true);
}
