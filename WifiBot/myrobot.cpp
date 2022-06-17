/**
 * @file myrobot.cpp
 * @author GINHAC DOMINIQUE, IMHOFF Guillaume, COUTAREL  Allan, CHENY Valentin
 * @brief  Fichier de fonctions de communication avec le robot
 * @date 2022-06-17
 *
 * @copyright Copyright (c) 2022
 *
 */

// Librairies importées
#include <QtMath>
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QTimer>
#include <QMutex>

// Fichier header importé
#include "myrobot.h"

/**
 * @brief MyRobot::MyRobot
 * Constructeur
 * @param parent
 */
MyRobot::MyRobot(QObject *parent) : QObject(parent)
{

    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = 0x0;
    DataToSend[3] = 0x0;
    DataToSend[4] = 0x0;
    DataToSend[5] = 0x0;
    DataToSend[6] = 0x0;
    DataToSend[7] = 0x0;
    DataToSend[8] = 0x0;

    DataReceived.resize(21);
    TimerEnvoi = new QTimer();
    // setup signal and slot
    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot())); // Send data to wifibot timer
}


/**
 * @brief MyRobot::doConnect
 * Initie le protocole de connexion au robot
 */
void MyRobot::doConnect()
{
    socket = new QTcpSocket(this); // socket creation
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    qDebug() << "connecting..."; // this is not blocking call
    // socket->connectToHost("LOCALHOST", 15020);
    socket->connectToHost("192.168.1.106", 15020); // connection to wifibot
    // we need to wait...
    if (!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
        return;
    }
    TimerEnvoi->start(75);
}

/**
 * @brief MyRobot::disConnect
 * Déconnecte le robot
 */
void MyRobot::disConnect()
{
    TimerEnvoi->stop();
    socket->close();
}

/**
 * @brief MyRobot::connected
 * Information dans la console d'une connexion réussie
 */
void MyRobot::connected()
{
    qDebug() << "connected..."; // Hey server, tell me about you.
}

/**
 * @brief MyRobot::disconnected
 * Information dans la console de la fin de connexion
 */
void MyRobot::disconnected()
{
    qDebug() << "disconnected...";
}

/**
 * @brief MyRobot::bytesWritten*
 * Information dans la console de l'envoi d'informations au robot
 * @param bytes
 */
void MyRobot::bytesWritten(qint64 bytes)
{
    // qDebug() << bytes << " bytes written...";
}

/**
 * @brief MyRobot::readyRead
 * Protocole de réception des données envoyées par le roboto
 */
void MyRobot::readyRead()
{
    // qDebug() << "reading..."; // read the data from the socket
    DataReceived = socket->readAll();
    emit updateUI(DataReceived);
    qDebug() << (unsigned char)DataReceived[3] << (unsigned char)DataReceived[4] << (unsigned char)DataReceived[11] << (unsigned char)DataReceived[12];
}

/**
 * @brief MyRobot::MyTimerSlot
 * Envoi récurrent des données
 */
void MyRobot::MyTimerSlot()
{
    // qDebug() << "Timer...";

    unsigned char *add = (unsigned char *)DataToSend.data();
    short crc = Crc16(add + 1, 6);
    DataToSend[7] = (char)crc;        // low byte
    DataToSend[8] = (char)(crc >> 8); // heigh byte

    while (Mutex.tryLock())
        ;
    socket->write(DataToSend);
    Mutex.unlock();
}

/**
 * @brief MyRobot::Crc16
 * Calcul du crc
 * @param Adresse_tab
 * @param Taille_max
 * @return crc
 */
short MyRobot::Crc16(unsigned char *Adresse_tab, unsigned char Taille_max)
{
    unsigned int Crc = 0xFFFF;
    unsigned int Polynome = 0xA001;
    unsigned int CptOctet = 0;
    unsigned int CptBit = 0;
    unsigned int Parity = 0;
    Crc = 0xFFFF;
    Polynome = 0xA001;
    for (CptOctet = 0; CptOctet < Taille_max; CptOctet++)
    {
        Crc ^= *(Adresse_tab + CptOctet);
        for (CptBit = 0; CptBit <= 7; CptBit++)
        {
            Parity = Crc;
            Crc >>= 1;
            if (Parity % 2 == true)
                Crc ^= Polynome;
        }
    }
    return (Crc);
}

/**
 * @brief MyRobot::left_speed
 * Contrôle la vitesse du train gauche du robot
 * @param speed (de -1 à 1)
 */
void MyRobot::left_speed(float speed)
{
    unsigned int intSpeed = (int)(qFabs(speed) * MAXSPEED);
    bool forward = (speed > 0);

    DataToSend[2] = intSpeed;
    int rest = DataToSend[6];
    if (forward)
        rest |= 1UL << 6;
    else
        rest &= ~(1UL << 6);
    DataToSend[6] = rest;
}

/**
 * @brief MyRobot::right_speed
 * Contrôle la vitesse du train droit du robot
 * @param speed (de -1 à 1)
 */
void MyRobot::right_speed(float speed)
{
    unsigned int intSpeed = (int)(qFabs(speed) * MAXSPEED);
    bool forward = (speed > 0);

    DataToSend[4] = intSpeed;
    int rest = DataToSend[6];
    if (forward)
        rest |= 1UL << 4;
    else
        rest &= ~(1UL << 4);
    DataToSend[6] = rest;
}
