#include "mainwindow1.h"
#include "ui_mainwindow1.h"
#include "fournisseurwindow.h"
#include "ui_fournisseurwindow.h"
#include "indigentswindow.h"
#include "ui_indigentswindow.h"
#include "medicalfilewindow.h"
#include "ui_medicalfilewindow.h"
#include"employe.h"
#include "QMessageBox"
#include<QDebug>
#include <QPoint>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QTextDocument>
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QtCharts>
#include <QPieSeries>
#include<QPainter>
#include <random>
#include "sessionuser.h"
#include "chat.h"
#include "arduino.h"
#include "mainwindow.h"
#include <QByteArray>
#include <QDebug>


MainWindow1::MainWindow1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow1)
{
    ui->setupUi(this);

    // Connect signals and slots here
}

MainWindow1::~MainWindow1()
{
    delete ui;
}








void MainWindow1::on_FournisseurButton_clicked()
{
    close();
     f=new FournisseurWindow(this);
     f->show();
}



void MainWindow1::on_Indifentbutton_clicked()
{
    close();
     i=new IndigentsWindow(this);
     i->show();
}

void MainWindow1::on_Employebutton_clicked()
{
    close();
     l=new LogementsWindow(this);
     l->show();
}

void MainWindow1::on_Medicalfilebutton_clicked()
{
    close();
     d=new MedicalfileWindow(this);
     d->show();
}
