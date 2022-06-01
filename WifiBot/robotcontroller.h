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
    void on_dirForward_clicked();

private:
    Ui::robotController *ui;
    MyRobot *robot;
};

#endif // ROBOTCONTROLLER_H
