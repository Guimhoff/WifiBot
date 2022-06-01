#include "robotcontroller.h"
#include "ui_robotcontroller.h"

robotController::robotController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::robotController)
{
    ui->setupUi(this);
}

robotController::~robotController()
{
    delete ui;
}

void robotController::setRobot(MyRobot *robot)
{
    this->robot = robot;
}


void robotController::on_disconnectButt_clicked()
{
    robot->disConnect();
}

void robotController::on_dirForward_clicked()
{
    qDebug("coucou");
}
