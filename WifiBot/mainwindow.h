/**
 * @file mainwindow.h
 * @author IMHOFF Guillaume, COUTAREL  Allan, CHENY Valentin
 * @brief  Fichier header de mainwindow.cpp
 * @date 2022-06-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Librairies importées
#include <QMainWindow>

// Fichiers header importés
#include "myrobot.h"
#include "robotcontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Connection_clicked();

    void on_label_2_linkActivated(const QString &link);

private:
    // Instance de l'interface de connexion
    Ui::MainWindow *ui;
    // Instance du robot
    MyRobot robot;
    // Instance de l'interface de contrôle du robot
    robotController robotController;
};
#endif // MAINWINDOW_H
