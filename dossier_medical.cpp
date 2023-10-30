#include "dossier_medical.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
#include<QtSql/QSqlQueryModel>
#include<QtSql/QSqlQuery>
#include <QTextBrowser>
#include <QFileDialog>
#include<QMessageBox>
#include <QtPrintSupport/QPrinter>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QDate>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QUrl>
#include <QSqlError>
#include <QSqlDatabase>

#include <QFont>
//#include <QtCore/QDateTime>

#include<QStandardItemModel>
 dossier_medical:: dossier_medical()
 {

 }
dossier_medical::dossier_medical(int id_dossier,QString etat,QString description,QDate date_dernier_consol,QDate date_rendez_vous)
{
    this->id_dossier=id_dossier;
    this->etat=etat;
    this->description=description;
    this->date_dernier_consol=date_dernier_consol;
    this->date_rendez_vous=date_rendez_vous;
}
bool dossier_medical::ajouter()
{
    QSqlQuery query;
    QString id_dossier_String = QString::number(id_dossier);


    query.prepare("insert into DOSSIER_MEDICAL(ID_DOSSIER,ETAT,DESCRIPTION,DATE_DERNIER_CONSOL,DATE_RENDEZ_VOUS) "
                  "values(:id_dossier, :etat, :description, :date_dernier_consol, :date_rendez_vous)");

    // Création des variables liées
    query.bindValue(":id_dossier",id_dossier_String);
    query.bindValue(":etat",etat);
    query.bindValue(":description",description);
    query.bindValue(":date_dernier_consol",date_dernier_consol);
    query.bindValue(":date_rendez_vous",date_rendez_vous);
    return query.exec(); // exec() retourne la requête pour l'exécution
}


bool dossier_medical::supprimer(int id)
{
    QSqlQuery query;
    QString res = QString::number(id);

    query.prepare("Delete from DOSSIER_MEDICAL where ID_DOSSIER= :id");
    query.bindValue(":id",res);

    return query.exec();
}

QSqlQueryModel * dossier_medical::afficher()
{
    QSqlQueryModel *  model=new QSqlQueryModel();


   model->setQuery("select * from DOSSIER_MEDICAL");


   model->setHeaderData(3,Qt::Horizontal,QObject::tr("DATE_DERNIER_CONSOL"));
   model->setHeaderData(4,Qt::Horizontal,QObject::tr("DATE_RENDEZ_VOUS"));
   model->setHeaderData(2,Qt::Horizontal,QObject::tr("DESCRIPTION"));
     model->setHeaderData(1,Qt::Horizontal,QObject::tr("ETAT"));
     model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID_DOSSIER"));
   return model;

}


bool dossier_medical::modifier_dossier(int id,QString E,QString DES,QDate D, QDate R)
{
    QSqlQuery query;
    QString res = QString::number(id);
    query.prepare("Update DOSSIER_MEDICAL SET Etat=:etat, DESCRIPTION=:description, DATE_DERNIER_CONSOL=:date_dernier_consol, DATE_RENDEZ_VOUS=:date_rendez_vous  where ID_DOSSIER=:id_dossier");

    //creation variables liees


    query.bindValue(":date_dernier_consol",D);
    query.bindValue(":date_rendez_vous",R);
      query.bindValue(":description",DES);
      query.bindValue(":etat",E);
   query.bindValue(":id_dossier",res);

    return query.exec();
}


//metiers

//recherche
QSqlQueryModel * dossier_medical:: recherche_dossier(QString var)
    {
    QSqlQueryModel* model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM DOSSIER_MEDICAL WHERE ETAT LIKE '%" + var + "%' ORDER BY DATE_RENDEZ_VOUS");
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("date_dernier_consol"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("date_rendez_vous"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("description"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("etat"));
        return model;
    }

//tri
QSqlQueryModel* dossier_medical::trirendez_vous()
{

    QSqlQueryModel* model=new QSqlQueryModel();

   model->setQuery("SELECT * FROM DOSSIER_MEDICAL ORDER BY DATE_RENDEZ_VOUS");

    return model;
}



//pdf


void dossier_medical::exporterpdf(QTextBrowser *text)
    {

    //exécuter des requêtes SQL
       QSqlQuery qry;
       //creation du tt pour sera utilisée pour stocker les données récupérées depuis la base de données
       std::list<QString> tt;

       // Ajout du titre "MEDICAL FILES" en orange
       tt.push_back("<span style='color: orange; font-size: 20px; font-weight: bold;'>MEDICAL FILES</span><br><br>");

       //récupérer toutes les données de la table
       qry.exec("select * from DOSSIER_MEDICAL");

       //création du tableau HTML
       QString table = "<table style='border-collapse: collapse; margin-top: 30px;'>";
       table += "<tr style='background-color: #d9d9d9;'>";
       table += "<th style='padding: 10px; text-align: left; width: 15%;'>ID_DOSSIER</th>";
       table += "<th style='padding: 10px; text-align: left; width: 15%;'>ETAT</th>";
       table += "<th style='padding: 10px; text-align: left; width: 40%;'>DESCRIPTION</th>";
       table += "<th style='padding: 10px; text-align: left; width: 15%;'>DATE DERNIER CONSOL</th>";
       table += "<th style='padding: 10px; text-align: left; width: 15%;'>DATE RENDEZ-VOUS</th>";
       table += "</tr>";

       while(qry.next())
       {
           table += "<tr>";
           table += "<td style='padding: 10px; border: 1px solid #ddd;'>" + qry.value(0).toString() + "</td>";
           table += "<td style='padding: 10px; border: 1px solid #ddd;'>" + qry.value(1).toString() + "</td>";
           table += "<td style='padding: 10px; border: 1px solid #ddd;'>" + qry.value(2).toString() + "</td>";
           table += "<td style='padding: 10px; border: 1px solid #ddd;'>" + qry.value(3).toDate().toString(Qt::ISODate) + "</td>";
           table += "<td style='padding: 10px; border: 1px solid #ddd;'>" + qry.value(4).toDate().toString(Qt::ISODate) + "</td>";
           table += "</tr>";
       }

       table += "</table>";

       //ajout du tableau au contenu HTML
       tt.push_back("<br>" + table + "<br>");



       for(std::list<QString>::iterator it =tt.begin();it!=tt.end();++it)
       {
           text->setText(text->toHtml()+*it);
       }



      //ouvre une boîte de dialogue pour permettre à l'utilisateur de choisir l'emplacement et le nom du fichier PDF généré
      QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
      if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }

      //crée une nouvelle instance de la classe QPrinter qui est utilisée pour configurer l'impression.
      QPrinter printer(QPrinter::PrinterResolution);

      //le format de sortie de l'impression
      printer.setOutputFormat(QPrinter::PdfFormat);
      //la taille de papier pour l'impression
      printer.setPaperSize(QPrinter::A4);
      //le nom de fichier de sortie pour l'impression
      printer.setOutputFileName(fileName);

      // Impression des données au format HTML
      QTextDocument doc;
      doc.setDefaultStyleSheet("table { border-collapse: collapse; } th, td { border: 1px solid black; padding: 5px; font-size: 10px; } th { background-color: #cccccc; }");
      doc.setHtml(text->toHtml());

      // Draw the contents of the QTextDocument to the printer
      QPainter painter(&printer);
      doc.drawContents(&painter);


        // Ajout de l'image à droite de la page
        QPixmap img("C:\\Users\\souha\\Desktop\\Dossier Medicale\\Icons\\ApplicationHomeShare.png");
        int imgWidth = printer.pageRect().width() / 4;
        int imgHeight = imgWidth * img.height() / img.width();
        painter.drawPixmap(QRect(printer.pageRect().width() - imgWidth, 0, imgWidth, imgHeight), img);


        QMessageBox::information(nullptr, QObject::tr("Export PDF"),
                                  QObject::tr("Export avec succes .\n"
                                              "Click OK to exit."), QMessageBox::Ok);


    }



