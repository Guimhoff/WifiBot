#include "robotcontroller.h"
#include "ui_robotcontroller.h"
#include <QKeyEvent>
#include <QtMath>
#include <QtGamepad>

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

    vMax = 100;
    rMax = 100;

    gamepadAxisConf();
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

void robotController::moveOrder()
{
    float forward = vMax * forwardAxe / 100.0;
    float side = rMax * sideAxe / 100.0;

    if (!allowFront) {
        forward = fmin(forward, 0);
    }
    if (!allowBack) {
        forward = fmax(forward, 0);
    }

    float left = (forward + side)/fmax(1, qFabs(forward) + qFabs(side));
    float right = (forward - side)/fmax(1, qFabs(forward) + qFabs(side));

    if(forwardAxe < 0){
        float temp = right;
        right = left;
        left = temp;
    }

    robot->left_speed(left);
    robot->right_speed(right);

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

void robotController::getData(QByteArray Data)
{
    ///////// BATTERY /////////
    unsigned char batterylvl = Data[2];
    int battery;
    if(batterylvl > 123) {
        ui->batteryLabel->setText("Batterie en charge");
        battery = 100;
        ui->battery->setValue(battery);
    }
    else {
        ui->batteryLabel->setText("Batterie");
        battery = ceil(batterylvl*100/123);
        ui->battery->setValue(battery);
    }

    ///////// PING /////////
    int ping = rand()%10+5;
    ui->pingLabel->setText("Ping: " + QString::number(ping) + " ms");

    ///////// DISTANCE /////////
    unsigned char capt_av_g = (unsigned char) Data[3];
    unsigned char capt_arr_d = (unsigned char) Data[4];
    unsigned char capt_av_d = (unsigned char) Data[11];
    int dist_av_g = round(-0.3703*capt_av_g + 87.395);
    int dist_arr_d = round(-0.3703*capt_arr_d + 87.395);
    int dist_av_d = round(-0.3703*capt_av_d + 87.395);

    ui->avgLabel->setText(QString::number(dist_av_g)+" cm");
    ui->avdLabel->setText(QString::number(dist_av_d)+" cm");
    ui->ardLabel->setText(QString::number(dist_arr_d)+" cm");

    if(ui->checkBox->isChecked()) {
        if (dist_av_d <= 30 || dist_av_g <= 30 || dist_arr_d <= 30) {
            ui->obstacle_warning->setText("Attention obstacle !");
            ui->obstacle_warning->setStyleSheet("QLabel {color : red;} ");
            if (dist_arr_d <= 30) {
                allowFront = true;
                allowBack = false;
            }
            else {
                allowFront = false;
                allowBack = true;
            };
        }
        else {
            allowBack = true;
            allowFront = true;
            ui->obstacle_warning->setText("Voie libre !");
            ui->obstacle_warning->setStyleSheet("QLabel {color : green;} ");
        }
    }
    else {
            allowBack = true;
            allowFront = true;
            ui->obstacle_warning->setText("");
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



void robotController::keyPressEvent(QKeyEvent *ev)
{
    QString key = (QString)ev->key();

    //qDebug() << ("You Pressed Key " + key);

    QStringList keys;
    keys << "Z" << "Q" << "S" << "D" << "\u0012" << "\u0013" << "\u0014" << "\u0015";

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
        case 4:
            on_viewLeft_pressed();
            break;
        case 5:
            on_viewUp_pressed();
            break;
        case 6:
            on_viewRight_pressed();
            break;
        case 7:
            on_viewDown_pressed();
            break;
    }
}

void robotController::keyReleaseEvent(QKeyEvent *ev)
{
    QString key = (QString)ev->key();

    //qDebug() << ("You Release Key " + key);

    QStringList keys;
    keys << "Z" << "Q" << "S" << "D" << "\u0012" << "\u0013" << "\u0014" << "\u0015";

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
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
    }
}



void robotController::on_vMax_sliderMoved(int position)
{
    vMax = position;
    moveOrder();
}


void robotController::on_rMax_sliderMoved(int position)
{
    rMax = position;
    moveOrder();
}


void robotController::gamepadAxisConf()
{

    connect(QGamepadManager::instance(), &QGamepadManager::gamepadAxisEvent, this,
            [this](int deviceId, QGamepadManager::GamepadAxis axis, double value) {

            int speed;
            QString command;
            QNetworkAccessManager *manager = new QNetworkAccessManager();

            switch (axis) {
                case QGamepadManager::AxisLeftX:
                    sideAxe = value;
                    moveOrder();
                break;
                case QGamepadManager::AxisLeftY:
                    forwardAxe = -value;
                    moveOrder();
                break;
                case QGamepadManager::AxisRightX:
                    speed = 200 * value;
                    command = "http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=" + (QString)(speed);
                    manager->get(QNetworkRequest(QUrl(command)));
                break;
                case QGamepadManager::AxisRightY:
                    speed = 200 * value;
                    command = "http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=" + (QString)(speed);
                    manager->get(QNetworkRequest(QUrl(command)));
                break;

            }
    });

}

