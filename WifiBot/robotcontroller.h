#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include <QWidget>
#include <QWebEngineView>
#include <QApplication>
#include "myrobot.h"

namespace Ui {
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

    void moveOrder();

    void robotController::keyPressEvent(QKeyEvent *ev);
    void robotController::keyReleaseEvent(QKeyEvent *ev);

private:
    Ui::robotController *ui;
    MyRobot *robot;

    float forwardAxe;
    float sideAxe;
};

#endif // ROBOTCONTROLLER_H
