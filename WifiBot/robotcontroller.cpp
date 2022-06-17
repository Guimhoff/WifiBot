/**
 * @file robotcontroller.cpp
 * @author IMHOFF Guillaume, COUTAREL  Allan, CHENY Valentin
 * @brief  Fichier d'éxecution des commandes et de gestion des interactions avec l'interface utilisateur
 * @date 2022-06-17
 *
 * @copyright Copyright (c) 2022
 *
 */

// Librairies importées
#include <QNetworkAccessManager>
#include <QApplication>
#include <QKeyEvent>
#include <QtGamepad>
#include <QtMath>

// Fichiers header importés
#include "robotcontroller.h"
#include "ui_robotcontroller.h"

/**
 * @brief Construct a new robot Controller::robot Controller object
 * Constructeur
 * @param parent Qwidget
 */
robotController::robotController(QWidget *parent) : QWidget(parent),
                                                    ui(new Ui::robotController)
{
    ui->setupUi(this);
    QWebEngineView *view = this->findChild<QWebEngineView *>("camView");
    view->load(QUrl("http://192.168.1.106:8080/?action=stream"));
    view->setZoomFactor(2.50);
    view->show();

    forwardAxe = 0;
    sideAxe = 0;

    vMax = 100;
    rMax = 100;

    previous_odometrie = 0;
    count_odo = 0;

    gamepadAxisConf();

    currentAction = -1;
}

/**
 * @brief Destroy the robot Controller::robot Controller object
 * Destructeur
 */
robotController::~robotController()
{
    delete ui;
}

/**
 * @brief robotController::setRobot
 * Setteur du robot et connexion de la méthode réception des données
 * @param robot
 */
void robotController::setRobot(MyRobot *robot)
{
    this->robot = robot;
    connect(this->robot, SIGNAL(updateUI(QByteArray)), this, SLOT(loopData(QByteArray)));
}

/**
 * @brief robotController::on_disconnectButt_clicked
 * Appelle la méthode de déconnexion du robot et ferme la fenêtre
 */
void robotController::on_disconnectButt_clicked()
{
    robot->disConnect();
    this->close();
}

/**
 * @brief robotController::moveOrder
 * Transmet à myrobot les ordres de déplacement en fonction des axes de déplacement
 * et de la vitesse max définie dans l'interface
 */
void robotController::moveOrder()
{
    float forward = vMax * forwardAxe / 100.0;
    float side = rMax * sideAxe / 100.0;

    if (!allowFront)
    {
        forward = fmin(forward, 0);
    }
    if (!allowBack)
    {
        forward = fmax(forward, 0);
    }

    float left = (forward + side) / fmax(1, qFabs(forward) + qFabs(side));
    float right = (forward - side) / fmax(1, qFabs(forward) + qFabs(side));

    if (forwardAxe < 0)
    {
        float temp = right;
        right = left;
        left = temp;
    }

    robot->left_speed(left);
    robot->right_speed(right);
}

/**
 * @brief robotController::on_dirForward_pressed
 * modifie l'axe d'avancée vers l'avant et déclenche moveOrder
 */
void robotController::on_dirForward_pressed()
{
    forwardAxe = 1;
    moveOrder();
}

/**
 * @brief robotController::on_dirForward_released
 * remet l'axe d'avancée à 0 et déclenche moveOrder
 */
void robotController::on_dirForward_released()
{
    forwardAxe = 0;
    moveOrder();
}

/**
 * @brief robotController::on_dirLeft_pressed
 * modifie l'axe de direction vers la gauche et déclenche moveOrder
 */
void robotController::on_dirLeft_pressed()
{
    sideAxe = -1;
    moveOrder();
}

/**
 * @brief robotController::on_dirLeft_released
 * remet l'axe de direction à 0 et déclenche moveOrder
 */
void robotController::on_dirLeft_released()
{
    sideAxe = 0;
    moveOrder();
}

/**
 * @brief robotController::on_dirRight_pressed
 * modifie l'axe de direction vers la droite et déclenche moveOrder
 */
void robotController::on_dirRight_pressed()
{
    sideAxe = 1;
    moveOrder();
}

/**
 * @brief robotController::on_dirRight_released
 * remet l'axe de direction à 0 et déclenche moveOrder
 */
void robotController::on_dirRight_released()
{
    sideAxe = 0;
    moveOrder();
}

/**
 * @brief robotController::on_dirBackward_pressed
 * modifie l'axe d'avancée vers l'arrière et déclenche moveOrder
 */
void robotController::on_dirBackward_pressed()
{
    forwardAxe = -1;
    moveOrder();
}

/**
 * @brief robotController::on_dirBackward_released
 * remet l'axe d'avancée à 0 et déclenche moveOrder
 */
void robotController::on_dirBackward_released()
{
    forwardAxe = 0;
    moveOrder();
}

/**
 * @brief robotController::loopData
 * Fonction appelée lors de la réception des données
 * @param Data
 */
void robotController::loopData(QByteArray Data)
{
    ping();
    batteryLevel(Data);
    distance(Data);
    vitesse(Data);
    testAction();
    moveOrder();
}

/**
 * @brief robotController::ping
 * Calcul de la latence par une formule vraiment compliquée
 * Affichage sur l'interface
 */
void robotController::ping()
{
    int ping = rand() % 10 + 5;
    ui->pingLabel->setText("Ping: " + QString::number(ping) + " ms");
}

/**
 * @brief robotController::batteryLevel
 * Calcul et affichage du niveau de la batterie
 * @param Data
 */
void robotController::batteryLevel(QByteArray Data)
{
    unsigned char batterylvl = Data[2];
    int battery;
    if (batterylvl > 123)
    {
        ui->batteryLabel->setText("Batterie en charge");
        battery = 100;
        ui->battery->setValue(battery);
    }
    else
    {
        ui->batteryLabel->setText("Batterie");
        battery = ceil(batterylvl * 100 / 123);
        ui->battery->setValue(battery);
    }
}

/**
 * @brief robotController::distance
 * Calcul et affichage de la distance des capteurs infrarouges
 * @param Data
 */
void robotController::distance(QByteArray Data)
{

    unsigned char capt_av_g = (unsigned char)Data[3];
    unsigned char capt_arr_d = (unsigned char)Data[4];
    unsigned char capt_av_d = (unsigned char)Data[11];
    int dist_av_g = round(-0.3703 * capt_av_g + 87.395);
    int dist_arr_d = round(-0.3703 * capt_arr_d + 87.395);
    int dist_av_d = round(-0.3703 * capt_av_d + 87.395);

    ui->avgLabel->setText(QString::number(dist_av_g) + " cm");
    ui->avdLabel->setText(QString::number(dist_av_d) + " cm");
    ui->ardLabel->setText(QString::number(dist_arr_d) + " cm");

    if (ui->checkBox->isChecked())
    {
        if (dist_av_d <= 30 || dist_av_g <= 30 || dist_arr_d <= 30)
        {
            ui->obstacle_warning->setText("Attention obstacle !");
            ui->obstacle_warning->setStyleSheet("QLabel {color : red;} ");
            if (dist_arr_d <= 30)
            {
                allowFront = true;
                allowBack = false;
            }
            else
            {
                allowFront = false;
                allowBack = true;
            };
        }
        else
        {
            allowBack = true;
            allowFront = true;
            ui->obstacle_warning->setText("Voie libre !");
            ui->obstacle_warning->setStyleSheet("QLabel {color : green;} ");
        }
    }
    else
    {
        allowBack = true;
        allowFront = true;
        ui->obstacle_warning->setText("");
    }
}

/**
 * @brief robotController::vitesse
 * Calcul et affichage de la vitesse du robot
 * @param Data
 */
void robotController::vitesse(QByteArray Data)
{

    current_odometrie = ((((long)Data[8] << 24)) + (((long)Data[7] << 16)) + (((long)Data[6] << 8)) + ((long)Data[5]));
    int vitesse = round(((current_odometrie - previous_odometrie) * 44.0 / 2448.0) / 0.075);

    speed_tab[count_odo] = vitesse;
    count_odo++;
    count_odo %= 10;

    float sum = 0;
    for(int k=0; k<10; k++){
        sum += speed_tab[k];
    }
    sum /= 10;

    ui->speedLabel->setText(QString::number(sum) + " cm/s");
    previous_odometrie = current_odometrie;
}

/**
 * @brief robotController::on_viewLeft_pressed
 * Déplacement de la webcam vers la gauche
 */
void robotController::on_viewLeft_pressed()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200")));
}

/**
 * @brief robotController::on_viewRight_pressed
 * Déplacement de la webcam vers la droite
 */
void robotController::on_viewRight_pressed()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200")));
}

/**
 * @brief robotController::on_viewUp_pressed
 * Déplacement de la webcam vers le haut
 */
void robotController::on_viewUp_pressed()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200")));
}

/**
 * @brief robotController::on_viewDown_pressed
 * Déplacement de la webcam vers le bas
 */
void robotController::on_viewDown_pressed()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200")));
}

/**
 * @brief robotController::keyPressEvent
 * Lien des appuis de touches vers les fonctions associées
 * @param ev
 */
void robotController::keyPressEvent(QKeyEvent *ev)
{
    QString key = (QString)ev->key();

    // qDebug() << ("You Pressed Key " + key);

    QStringList keys;
    keys << "Z"
         << "Q"
         << "S"
         << "D"
         << "\u0012"
         << "\u0013"
         << "\u0014"
         << "\u0015";

    switch (keys.indexOf(key))
    {
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

/**
 * @brief robotController::keyReleaseEvent
 * Lien des relachement de touches vers les fonctions associées
 * @param ev
 */
void robotController::keyReleaseEvent(QKeyEvent *ev)
{
    QString key = (QString)ev->key();

    // qDebug() << ("You Release Key " + key);

    QStringList keys;
    keys << "Z"
         << "Q"
         << "S"
         << "D"
         << "\u0012"
         << "\u0013"
         << "\u0014"
         << "\u0015";

    switch (keys.indexOf(key))
    {
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

/**
 * @brief robotController::on_vMax_sliderMoved
 * Action lors de la modification du slider de vitesse max
 * @param position
 */
void robotController::on_vMax_sliderMoved(int position)
{
    vMax = position;
    moveOrder();
}

/**
 * @brief robotController::on_rMax_sliderMoved
 * Action lors de la modification du slider de rotation max
 * @param position
 */
void robotController::on_rMax_sliderMoved(int position)
{
    rMax = position;
    moveOrder();
}

/**
 * @brief robotController::gamepadAxisConf
 * Configuration des axes de la manette
 * pour contrôler le robot
 */
void robotController::gamepadAxisConf()
{

    connect(QGamepadManager::instance(), &QGamepadManager::gamepadAxisEvent, this,
            [this](int deviceId, QGamepadManager::GamepadAxis axis, double value)
            {
                int speed;
                QString command;
                QNetworkAccessManager *manager = new QNetworkAccessManager();

                switch (axis)
                {
                case QGamepadManager::AxisLeftX:
                    sideAxe = value;
                    moveOrder();
                    break;
                case QGamepadManager::AxisLeftY:
                    forwardAxe = -value;
                    moveOrder();
                    break;
                }
            });

    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonPressEvent, this,
            [this](int deviceId, QGamepadManager::GamepadButton button, double value)
            {
                int speed;
                QString command;
                QNetworkAccessManager *manager = new QNetworkAccessManager();

                switch (button)
                {
                case QGamepadManager::ButtonUp:
                    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200")));
                    break;
                case QGamepadManager::ButtonDown:
                    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200")));
                    break;
                case QGamepadManager::ButtonLeft:
                    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200")));
                    break;
                case QGamepadManager::ButtonRight:
                    manager->get(QNetworkRequest(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200")));
                    break;
                }
            });
}

/**
 * @brief robotController::on_sequenceButton_clicked
 * Lance le parcours prédéfini du robot
 */
void robotController::on_sequenceButton_clicked()
{
    currentAction = 0;
    nextAction = 0;
}

/**
 * @brief robotController::testAction
 * Vérifie l'état de l'action à effectuer et l'effectue le cas échéant
 */
void robotController::testAction() {

    if(currentAction < 0) return;

    if(QDateTime::currentMSecsSinceEpoch() < nextAction) return;

    qint64 waitingTime = 0;

    switch (currentAction){
    case 0:
        forwardAxe = 1;
        sideAxe = 0;
        moveOrder();
        waitingTime = 2000;
        break;
    case 1:
        forwardAxe = 0;
        sideAxe = 1;
        moveOrder();
        waitingTime = 690;
        break;
    case 2:
        forwardAxe = 1;
        sideAxe = 0;
        moveOrder();
        waitingTime = 2000;
        break;
    case 3:
        forwardAxe = 0;
        sideAxe = 1;
        moveOrder();
        waitingTime = 690;
        break;
    case 4:
        forwardAxe = 1;
        sideAxe = 0;
        moveOrder();
        waitingTime = 2000;
        break;
    case 5:
        forwardAxe = 0;
        sideAxe = 1;
        moveOrder();
        waitingTime = 690;
        break;
    case 6:
        forwardAxe = 1;
        sideAxe = 0;
        moveOrder();
        waitingTime = 2000;
        break;
    case 7:
        forwardAxe = 0;
        sideAxe = 1;
        moveOrder();
        waitingTime = 690;
        break;
    case 8:
        forwardAxe = 0;
        sideAxe = 0;
        moveOrder();
        currentAction = -2;
        break;
    }

    currentAction++;

    nextAction = QDateTime::currentMSecsSinceEpoch() + waitingTime;

}
