#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include <QWidget>
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



private slots:
    void on_disconnectButt_clicked();

private:
    Ui::robotController *ui;
    MyRobot *robot;
};

#endif // ROBOTCONTROLLER_H
