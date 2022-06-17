#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include <QWidget>
#include <QWebEngineView>

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

    void robotController::keyPressEvent(QKeyEvent *ev);
    void robotController::keyReleaseEvent(QKeyEvent *ev);

    void on_vMax_sliderMoved(int position);

    void on_rMax_sliderMoved(int position);

    void gamepadAxisConf();

    void ping();

    void batteryLevel(QByteArray Data);

    void distance(QByteArray Data);

    void vitesse(QByteArray Data);

    void on_sequenceButton_clicked();

private:
    Ui::robotController *ui;
    MyRobot *robot;

    float forwardAxe;
    float sideAxe;

    float vMax;
    float rMax;

    bool allowFront;
    bool allowBack;

    float current_odometrie;
    float previous_odometrie;
};

#endif // ROBOTCONTROLLER_H
