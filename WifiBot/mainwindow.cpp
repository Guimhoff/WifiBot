/**
 * @file mainwindow.cpp
 * @author IMHOFF Guillaume, COUTAREL  Allan, CHENY Valentin
 * @brief  Fichier d'affichage de la fenêtre de connexion au Wifibot
 * @date 2022-06-17
 *
 * @copyright Copyright (c) 2022
 *
 */

// Fichiers header importés
#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief Construct a new Main Window:: Main Window object
 * 
 * @param parent 
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

/**
 * @brief Destroy the Main Window:: Main Window object
 * 
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::on_Connection_clicked
 * Déclenche une tentative de connexion au robot
 * et le changement d'interface
 */
void MainWindow::on_Connection_clicked()
{
    ui->Connection->setText("En cours de connexion...");
    this->repaint();
    robot.doConnect();
    this->close();
    robotController.show();
    robotController.setRobot(&robot);
}

/**
 * @brief MainWindow::on_label_2_linkActivated
 * Redirection vers le github du projet
 * @param link
 */
void MainWindow::on_label_2_linkActivated(const QString &link)
{
    ui->label_2->setTextFormat(Qt::RichText);
    ui->label_2->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label_2->setOpenExternalLinks(true);
}
