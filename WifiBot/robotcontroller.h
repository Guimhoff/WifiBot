#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include <QWidget>
#include <QWebEngineView>
#include <QNetworkAccessManager>
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

    void getData(const QByteArray Data);

    void on_viewLeft_pressed();

    void on_viewRight_pressed();

    void on_viewUp_pressed();

    void on_viewDown_pressed();

private:
    Ui::robotController *ui;
    MyRobot *robot;
};

#endif // ROBOTCONTROLLER_H
