#ifndef FOURNISSEURWINDOW_H
#define FOURNISSEURWINDOW_H
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMainWindow>
#include "fournisseur.h"
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QSettings>
#include "dialog.h"
#include "arduino.h"



QT_BEGIN_NAMESPACE
namespace Ui { class FournisseurWindow; }
QT_END_NAMESPACE

class FournisseurWindow : public QMainWindow
{
    Q_OBJECT

public:
    FournisseurWindow(QWidget *parent = nullptr);
    ~FournisseurWindow();

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_update_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index); /*Table View*/

    void on_lineEdit_search_textChanged(const QString &arg1);   /*Search Bar*/

    void on_lineEdit_name_textChanged(const QString &arg1);    /*Update Name*/

    void on_lineEdit_place_textEdited(const QString &arg1);    /*Update Place*/

    void on_lineEdit_name_2_textEdited(const QString &arg1);   /*Update Name*/

    void on_lineEdit_place_2_textEdited(const QString &arg1);  /*Update Place*/

    void on_pushButton_sort_clicked();    /*Tri Button*/

    void on_printButton_clicked();

    void on_pushButton_statistics_clicked();

    void on_tabWidget_currentChanged(int index);



    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void ajouter_fournisseur();

private:
    Ui::FournisseurWindow *ui;
    Fournisseur F ;    /* Create an instance of Class Fournisseur */
    Dialog *s;
    Arduino A;

};
#endif // FOURNISSEURWINDOW_H
