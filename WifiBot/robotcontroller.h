/**
 * @file robotcontroller.h
 * @author IMHOFF Guillaume, COUTAREL  Allan, CHENY Valentin
 * @brief  Fichier header de robotcontroller.h
 * @date 2022-06-17
 *
 * @copyright Copyright (c) 2022
 *
 */


#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

// Librairies importées
#include <QWidget>
#include <QWebEngineView>

// Fichier header importé
#include "myrobot.h"

namespace Ui
{
    class robotController;
}

class robotController : public QWidget
{
    Q_OBJECT

public:
    explicit robotController(QWidget *parent = nullptr);
    ~robotController();
    void setRobot(MyRobot *robot);
    QWebEngineView *view;

private slots:
    void on_disconnectButt_clicked();

    void on_dirForward_released();

    void on_dirForward_pressed();

    void on_dirLeft_pressed();

    void on_dirLeft_released();

    void on_dirRight_pressed();

    void on_dirRight_released();

    void on_dirBackward_pressed();

    void on_dirBackward_released();

    void loopData(QByteArray Data);

    void on_viewLeft_pressed();

    void on_viewRight_pressed();

    void on_viewUp_pressed();

    void on_viewDown_pressed();

    void moveOrder();

    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    void on_vMax_sliderMoved(int position);

    void on_rMax_sliderMoved(int position);

    void gamepadAxisConf();

    void ping();

    void batteryLevel(QByteArray Data);

    void distance(QByteArray Data);

    void vitesse(QByteArray Data);

    void on_sequenceButton_clicked();

    void testAction();

private:
    // Instance de l'interface
    Ui::robotController *ui;
    // Instance de myrobot (interface avec le robot)
    MyRobot *robot;

    // Axes d'ordres de déplacement (avant/arrière et gauche/droite)
    float forwardAxe;
    float sideAxe;

    // Vitesse maximales de marche avant/arrière
    float vMax;
    // Vitesse maximale de rotation
    float rMax;

    // Le robot peut avancer
    bool allowFront;
    // Le robot peut reculer
    bool allowBack;

    // Odométrie actuelle
    float current_odometrie;
    // Odométrie précédente (rafraichissement précédent)
    float previous_odometrie;
    // Position de la dernière vitesse entrée dans le speed_tab
    int count_odo;
    // Les 10 dernières vitesses reçues du robot
    int speed_tab[10] = {0,0,0,0,0,0,0,0,0,0};

    // Temps auquel s'exécutera la prochaine action
    qint64 nextAction;
    // index de l'action à effectuer
    int currentAction;
};

#endif // ROBOTCONTROLLER_H
