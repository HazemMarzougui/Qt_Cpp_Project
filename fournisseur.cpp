#include "fournisseur.h"
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QFileDialog>
#include <QtPrintSupport/QPrinter>
#include <QPrinter>
#include <QPainter>
#include <QtPrintSupport/QPrintDialog>
#include <QLineEdit>
#include <QBoxLayout>
#include <QLineSeries>
#include <QtCharts>
#include <QChartView>
#include <QHBoxLayout>


Fournisseur::Fournisseur()
{

}
Fournisseur::Fournisseur(int id,QString type,QString nom,QString lieux){
    this->Id_fournisseur = id ;
    this->Type = type;
    this->Nom= nom ;
    this->Lieux= lieux ;

}


/*Getters*/
int Fournisseur::getidfournisseur(){return Id_fournisseur;}
QString Fournisseur::gettype(){return Type;}
QString Fournisseur::getnom(){return Nom;}
QString Fournisseur::getlieux(){return Lieux;}


/*Setters*/
void Fournisseur::setidfournisseur(int id){this->Id_fournisseur=id;}
void Fournisseur::settype(QString type){this->Type=type;}
void Fournisseur::setnom(QString nom){this->Nom=nom;}
void Fournisseur::setlieux(QString lieux){this->Lieux=lieux;}


/*CRUD*/

/*Ajouter*/
bool Fournisseur::Ajouter(){
    QSqlQuery query;
    QString res = QString :: number(Id_fournisseur);
         query.prepare("INSERT INTO FOURNISSEUR(IDF,TYPE,NOM,LIEUX)VALUES(:id,:type,:nom,:lieux)");
         query.bindValue(":id",res);    /* bindValue Pour lier le parametre id avec l'attribut IDF */
         query.bindValue(":type",Type);
         query.bindValue(":nom",Nom);
         query.bindValue(":lieux",Lieux);
         /*query.bindValue(":6",1);*/
     return query.exec();
}


/*Afficher*/
QSqlQueryModel * Fournisseur :: Afficher()
{
QSqlQueryModel * model=new QSqlQueryModel ();
model->setQuery("select * from FOURNISSEUR");
model->setHeaderData(0,Qt::Horizontal,QObject::tr("IDF"));
model->setHeaderData(1,Qt::Horizontal,QObject::tr("TYPE"));
model->setHeaderData(2,Qt::Horizontal,QObject::tr("NAME"));
model->setHeaderData(3,Qt::Horizontal,QObject::tr("LIEUX"));

return model;
}


/*Supprimer*/
bool Fournisseur ::Supprimer(int id)
{
QSqlQuery query;
QString res=QString ::number(id);
query.prepare("delete from FOURNISSEUR where IDF= :id");
query.bindValue(":id",res);
return query.exec();
}



/*Modifier*/
bool Fournisseur::Modifier(int id)
{
    QSqlQuery query;
    QString res=QString ::number(id);

    query.prepare("UPDATE FOURNISSEUR SET TYPE=:type,NOM=:nom,LIEUX=:lieux where IDF=:id");
    query.bindValue(":id",res);
    query.bindValue(":type",Type);
    query.bindValue(":nom",Nom);
    query.bindValue(":lieux",Lieux);

    return query.exec();
}




/*Recherche par Nom*/

QSqlQueryModel * Fournisseur:: Recherche(QString a){

    QSqlQueryModel *model=new QSqlQueryModel();
    model->setQuery("SELECT * FROM FOURNISSEUR WHERE ( NOM LIKE '%"+a+"%' OR LIEUX LIKE '%"+a+"%')");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("IDF"));   /*mettre le idf dans le premier colonne*/
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("TYPE"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("NOM"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("LIEUX"));



    return  model ;
}



QSqlQueryModel* Fournisseur::triName()
{
    QSqlQueryModel* model=new QSqlQueryModel();
    model->setQuery("SELECT* FROM FOURNISSEUR ORDER BY NOM");
    return model;
}


void Fournisseur::statistiqueSegment()
{
    // Requête SQL pour récupérer les données
    QSqlQuery query("SELECT COUNT(*) FROM FOURNISSEUR WHERE TYPE ='Pharmacy'");
    query.next();
    int nbPharmacy = query.value(0).toInt();
    query.exec("SELECT COUNT(*) FROM FOURNISSEUR WHERE TYPE='Restaurant'");
    query.next();
    int nbRestaurant = query.value(0).toInt();
    query.exec("SELECT COUNT(*) FROM FOURNISSEUR WHERE TYPE='Clothes Shop'");
    query.next();
    int nbShop = query.value(0).toInt();

    // Création de la série de données en courbe
    QLineSeries *series = new QLineSeries();  // presentation des données sous forme des lignes d'un graphique
    series->append(0, nbPharmacy);   // affecter nbpharmacy pour une serie
    series->append(1, nbRestaurant);
    series->append(1, nbShop);
    // Création du graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des fourniseurs selon le type");
    chart->createDefaultAxes();

    // Personnalisation de l'axe des X
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append("Pharmacy");
    axisX->append("Restaurant");
     axisX->append("Clothes Shop");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Personnalisation de l'axe des Y
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de Fournisseurs");
    chart->setAxisY(axisY, series);

    // Création de la vue du graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing); // graduation de charte

    // Ajout de la vue dans la fenêtre principale
   // ui->verticalLayout->addWidget(chartView);

}

int Fournisseur::afficher_type(QString type){

    QSqlQuery query("SELECT COUNT(*) FROM FOURNISSEUR WHERE TYPE ='"+type+"'");
    query.next();

    return query.value(0).toInt();

}

