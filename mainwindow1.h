#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QMainWindow>
#include "fournisseurwindow.h"
#include "ui_indigentswindow.h"
#include "indigentswindow.h"
#include "logementswindow.h"
#include "ui_logementswindow.h"
#include "medicalfilewindow.h"
#include "ui_medicalfilewindow.h"
namespace Ui {
class MainWindow1;
}

class MainWindow1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow1(QWidget *parent = nullptr);
    ~MainWindow1();

private:
    Ui::MainWindow1 *ui;
    FournisseurWindow *f;
    IndigentsWindow *i;
    LogementsWindow *l;
    MedicalfileWindow *d;

    // Declare any private member variables or functions here

public slots:
    // Declare any public slots or signals here

private slots:
    void on_FournisseurButton_clicked();
    void on_Indifentbutton_clicked();
    void on_Employebutton_clicked();
    void on_Medicalfilebutton_clicked();
};

#endif // MAINWINDOW1_H
