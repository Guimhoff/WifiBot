#include "robotcontroller.h"
#include "ui_robotcontroller.h"

robotController::robotController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::robotController)
{
    ui->setupUi(this);
    QWebEngineView *view = this->findChild<QWebEngineView*>("camView");
    view->load(QUrl("http://192.168.1.106:8080/?action=stream"));
    view->setZoomFactor(2.50);
    view->show();
}

robotController::~robotController()
{
    delete ui;
}

void robotController::setRobot(MyRobot *robot)
{
    this->robot = robot;
    connect(this->robot, SIGNAL(updateUI(QByteArray)), this, SLOT(getData(QByteArray)));
}

void robotController::on_disconnectButt_clicked()
{
    robot->disConnect();
    this->close();
}

void robotController::on_dirForward_pressed()
{
    qDebug("Avancée");
    robot->left_speed(.5);
    robot->right_speed(.5);
}

void robotController::on_dirForward_released()
{
    qDebug("Stop");
    robot->left_speed(0);
    robot->right_speed(0);
}



void robotController::on_dirLeft_pressed()
{
    robot->left_speed(0);
    robot->right_speed(.5);
}


void robotController::on_dirLeft_released()
{
    robot->left_speed(0);
    robot->right_speed(0);
}


void robotController::on_dirRight_pressed()
{
    robot->left_speed(.5);
    robot->right_speed(0);
}


void robotController::on_dirRight_released()
{
    robot->left_speed(0);
    robot->right_speed(0);
}


void robotController::on_dirBackward_pressed()
{
    robot->left_speed(-.5);
    robot->right_speed(-.5);
}


void robotController::on_dirBackward_released()
{
    robot->left_speed(0);
    robot->right_speed(0);
}

void robotController::getData(const QByteArray Data)
{
    qDebug("'coucou");
    ui->battery->setValue((unsigned char) Data[2]/2);
}
