#ifndef LOGEMENTSWINDOW_H
#define LOGEMENTSWINDOW_H
#include <QDebug>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include<QMessageBox>
#include"logement.h"
QT_BEGIN_NAMESPACE
namespace Ui { class LogementsWindow; }
QT_END_NAMESPACE

class LogementsWindow : public QMainWindow
{
    Q_OBJECT

public:
    LogementsWindow(QWidget *parent = nullptr);
    ~LogementsWindow();
private slots:
    void on_pushButton_3_clicked();//save
    void on_Button_Delete_clicked();

    void on_lineEdit_Search_textChanged(const QString &arg1);//recherche

    void on_Button_update_clicked();
       void on_pushButton_4_clicked();//save aprés modif
       void on_pushButton_pdf_clicked(); //pdf
       void on_pushButton_somme_clicked();
       void on_stat_clicked();
      QSqlQueryModel*afficherHistorique(QString query);
      void on_historique_clicked();

private:
    Ui::LogementsWindow *ui;
    logement Etmp;




};
#endif // LOGEMENTSWINDOW_H
