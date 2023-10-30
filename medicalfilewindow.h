#ifndef MEDICALFILEWINDOW_H
#define MEDICALFILEWINDOW_H
#include <QDebug>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include"dossier_medical.h"
#include "arduino.h"
#include <QCalendarWidget>
#include<utility>

QT_BEGIN_NAMESPACE
namespace Ui { class MedicalfileWindow; }
QT_END_NAMESPACE

class MedicalfileWindow : public QMainWindow
{
    Q_OBJECT

public:
     MedicalfileWindow(QWidget *parent = nullptr);

    ~MedicalfileWindow();
private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_delete_clicked();
    void update_label();

    void on_pushButton_update_clicked();
void on_pushButton_update_2_clicked();
 void on_pushButton_pdf_clicked(); //pdf
 //void on_pushButton_mail_clicked(); //mail
void on_lineEdit_recherche_textChanged(const QString &arg1); //recherche
//stat

void on_pushButton_statistics_clicked();
//calendrier
void on_pushButton_calendrier_clicked();
void on_calendarWidget_clicked(const QDate &date);

private:
 Ui::MedicalfileWindow *ui;
    dossier_medical Etmp;
    //calendrier
      QDate old_date_rendez_vous;

      QByteArray data; // variable contenant les données reçues

      Arduino A;

};
#endif // MedicalfileWindow_H
