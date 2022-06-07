#include "robotcontroller.h"
#include "ui_robotcontroller.h"
#include <QKeyEvent>
#include <QtMath>

robotController::robotController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::robotController)
{
    ui->setupUi(this);
    QWebEngineView *view = this->findChild<QWebEngineView*>("camView");
    view->load(QUrl("http://192.168.1.106:8080/?action=stream"));
    view->setZoomFactor(2.50);
    view->show();

    forwardAxe = 0;
    sideAxe = 0;
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

void robotController::moveOrder()
{
    float left = (forwardAxe + sideAxe)/fmax(1, qFabs(forwardAxe) + qFabs(sideAxe));
    float right = (forwardAxe - sideAxe)/fmax(1, qFabs(forwardAxe) + qFabs(sideAxe));

    if(forwardAxe < 0){
        float temp = right;
        right = left;
        left = temp;
    }

    robot->left_speed(left);
    robot->right_speed(right);
    qDebug("======");
    qDebug() << forwardAxe;
    qDebug() << sideAxe;
    qDebug() << left;
    qDebug() << right;

}

void robotController::on_dirForward_pressed()
{
    forwardAxe = 1;
    moveOrder();
}

void robotController::on_dirForward_released()
{
    forwardAxe = 0;
    moveOrder();
}

void robotController::on_dirLeft_pressed()
{
    sideAxe = -1;
    moveOrder();
}


void robotController::on_dirLeft_released()
{
    sideAxe = 0;
    moveOrder();
}


void robotController::on_dirRight_pressed()
{
    sideAxe = 1;
    moveOrder();
}


void robotController::on_dirRight_released()
{
    sideAxe = 0;
    moveOrder();
}


void robotController::on_dirBackward_pressed()
{
    forwardAxe = -1;
    moveOrder();
}


void robotController::on_dirBackward_released()
{
    forwardAxe = 0;
    moveOrder();
}




void robotController::keyPressEvent(QKeyEvent *ev)
{
    QString key = (QString)ev->key();

    //qDebug() << ("You Pressed Key " + key);

    QStringList keys;
    keys << "Z" << "Q" << "S" << "D";

    switch (keys.indexOf(key)) {
        case 0:
            on_dirForward_pressed();
            break;
        case 1:
            on_dirLeft_pressed();
            break;
        case 2:
            on_dirBackward_pressed();
            break;
        case 3:
            on_dirRight_pressed();
            break;
    }
}

void robotController::keyReleaseEvent(QKeyEvent *ev)
{
    QString key = (QString)ev->key();

    //qDebug() << ("You Release Key " + key);

    QStringList keys;
    keys << "Z" << "Q" << "S" << "D";

    switch (keys.indexOf(key)) {
        case 0:
            on_dirForward_released();
            break;
        case 1:
            on_dirLeft_released();
            break;
        case 2:
            on_dirBackward_released();
            break;
        case 3:
            on_dirRight_released();
            break;
    }
}


