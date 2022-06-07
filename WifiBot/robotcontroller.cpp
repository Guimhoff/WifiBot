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

void robotController::getData(QByteArray Data)
{
    unsigned char batterylvl = Data[2];
    int battery;
    if(batterylvl > 123) {
        ui->batteryLabel->setText("Batterie en charge");
        battery = batterylvl*100/183;
        ui->battery->setValue(battery);
    }
    else {
        ui->batteryLabel->setText("Batterie");
        battery = batterylvl*100/123;
        ui->battery->setValue(battery);
    }
}

void robotController::on_viewLeft_pressed()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200")));
}


void robotController::on_viewRight_pressed()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200")));
}


void robotController::on_viewUp_pressed()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200")));
}


void robotController::on_viewDown_pressed()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200")));
}
