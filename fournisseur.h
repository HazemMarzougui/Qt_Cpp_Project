#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H
#include <QString>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTextBrowser>
#include <QTextBrowser>
#include <QtPrintSupport/QPrinter>
#include <QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDialog>

QT_CHARTS_USE_NAMESPACE


class Fournisseur
{
private:
    int Id_fournisseur ;
    QString Type ;
    QString Nom ;
    QString Lieux ;
public:
    Fournisseur();
    Fournisseur(int,QString,QString,QString);
     /*Getters*/


    int getidfournisseur();
    QString gettype();
    QString getnom();
    QString getlieux();
    /*Setters*/


    void setidfournisseur(int);
    void settype(QString);
    void setnom(QString);
    void setlieux(QString);



    /*Fonctions*/
    bool Ajouter();
    QSqlQueryModel * Afficher();
    QSqlQueryModel *Recherche(QString );    /*Recherche*/
    void Controle(QString); /*Controle de saisie*/
    QSqlQueryModel  * triName();   /*Tri Par Nom*/

    int afficher_type(QString type);


    bool Supprimer (int);
    bool Modifier(int );
      void statistiqueSegment();
};

#endif // FOURNISSEUR_H
