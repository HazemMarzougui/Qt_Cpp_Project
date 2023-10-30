#ifndef LOGEMENT_H
#define LOGEMENT_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QTextBrowser>


class logement
{
    int ID_LOGEMENT;
    QString ADRESSE , ETAT;

public:
    logement(){}
    logement(int,QString,QString);

    //getters
    int getID(){return ID_LOGEMENT;}
    QString getEtat(){return ETAT;}
    QString getAdresse(){return ADRESSE;}

    //setters
    void setID(int ID_LOGEMENT){this->ID_LOGEMENT=ID_LOGEMENT;}
    void setEtat(QString ETAT){this->ETAT=ETAT;}
    void setAdresse(QString ADRESSE){this->ADRESSE=ADRESSE;}

    //fonctions
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier_logement(int);


    //metiers
    QSqlQueryModel *recherche_logement(QString var) ;
     QSqlQueryModel *triadresse();
     void exporterpdf(QTextBrowser *text);
int calculerSommeEtat(QString ETAT) const;
void ajouterHistorique(QString action, QString type, int id);



};

#endif // LOGEMENT_H
