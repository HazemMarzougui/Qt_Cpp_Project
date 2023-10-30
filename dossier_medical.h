#ifndef DOSSIER_MEDICAL_H
#define DOSSIER_MEDICAL_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QDate>
#include <QTextBrowser>
#include"ui_mainwindow.h"

class dossier_medical
{
    QString etat,description;
    int id_dossier;
    QDate date_dernier_consol,date_rendez_vous;
public:

    //constructeurs
    dossier_medical();
    dossier_medical(int,QString,QString,QDate,QDate);

     //getters
    QString getetat(){return etat;}
    QString getdescription(){return description;}
    int getid_dossier(){return id_dossier;}
    QDate getdate_dernier_consol(){return date_dernier_consol; }
    QDate getdate_rendez_vous(){return date_rendez_vous; }

    //setters
    void setetat(QString e){etat=e;}
    void setdescription(QString d){description=d;}
    void setid_dossier(int id){this->id_dossier=id;}
    void setdate_dernier_consol(QDate dernier_consol){date_dernier_consol=dernier_consol;}
    void setdate_rendez_vous(QDate rendez_vous){date_rendez_vous=rendez_vous;}

    //fonctions
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier_dossier(int id,QString E,QString DES,QDate D, QDate R);

//metiers
     QSqlQueryModel *recherche_dossier(QString var) ;
   QSqlQueryModel* trirendez_vous();

    void exporterpdf(QTextBrowser *text);


private:
    QList<dossier_medical> dossiers;  // Variable membre qui contient la liste des dossiers médicaux


};

#endif // DOSSIER_MEDICAL_H
