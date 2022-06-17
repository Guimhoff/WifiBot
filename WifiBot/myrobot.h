/**
 * @file myrobot.h
 * @author GINHAC DOMINIQUE, IMHOFF Guillaume, COUTAREL  Allan, CHENY Valentin
 * @brief  Fichier header de myrobot.cpp
 * @date 2022-06-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MYROBOT_H
#define MYROBOT_H

// Librairies importées
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QTimer>
#include <QMutex>

class MyRobot : public QObject {
    Q_OBJECT
public:
    explicit MyRobot(QObject *parent = 0);
    void doConnect();
    void disConnect();
    short Crc16(unsigned char *Adresse_tab, unsigned char Taille_max);

    void left_speed(float speed);
    void right_speed(float speed);

    QByteArray DataToSend;
    QByteArray DataReceived;
    QMutex Mutex;

signals:
    void updateUI(const QByteArray Data);
public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void MyTimerSlot();

private:
    // instance du socket liée à la connexion
    QTcpSocket *socket;
    // Délai d'envoi des données
    QTimer *TimerEnvoi;
    // constante définissant la vitesse max à envoyer au robot
    const int MAXSPEED = 254;
};

#endif // MYROBOT_H
