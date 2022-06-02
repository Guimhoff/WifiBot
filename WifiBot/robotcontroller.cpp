#include "robotcontroller.h"
#include "ui_robotcontroller.h"

robotController::robotController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::robotController)
{
    ui->setupUi(this);
    QWebEngineView *view = this->findChild<QWebEngineView*>("camView");
    view->load(QUrl("http://192.168.1.106:8080/?action=stream"));
    view->setZoomFactor(2.00);
    view->show();
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
    this->close();
}

void robotController::on_dirForward_clicked()
{
    qDebug("coucou");
}
