#include "logement.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
#include<QtSql/QSqlQueryModel>
#include<QtSql/QSqlQuery>
#include <QTextBrowser>
#include <QFileDialog>
#include<QMessageBox>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include<QPrinter>
#include <QSqlError>
#include<QDateTime>


logement::logement(int ID_LOGEMENT,QString ADRESSE,QString ETAT)
{
this->ID_LOGEMENT=ID_LOGEMENT;
this->ADRESSE=ADRESSE;
this->ETAT=ETAT;

}

bool logement::ajouter()
{
    QSqlQuery query;
    QString id_logement_String = QString::number(ID_LOGEMENT);
    query.prepare("INSERT INTO LOGEMENT(ID_LOGEMENT, ADRESSE, ETAT) VALUES (:ID_LOGEMENT, :ADRESSE, :ETAT)");
    query.bindValue(":ID_LOGEMENT", ID_LOGEMENT);
    query.bindValue(":ADRESSE", ADRESSE);
    query.bindValue(":ETAT", ETAT);
    if (!query.exec()) {
        qDebug() << "Failed to execute insert query: " << query.lastError().text();
        return false;
    }

    // Call ajouterHistorique after the query execution is successful
    ajouterHistorique("Ajout", "LOGEMENT", ID_LOGEMENT);

    return true;
}






QSqlQueryModel * logement:: afficher()
{
    QSqlQueryModel *  model=new QSqlQueryModel();
      model->setQuery("select * from LOGEMENT");


     model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID_LOGEMENT"));
       model->setHeaderData(1,Qt::Horizontal,QObject::tr("ADRESSE"));
       model->setHeaderData(2,Qt::Horizontal,QObject::tr("ETAT"));

       return model;
}

bool logement::supprimer(int id)
{
    QSqlQuery query;
    QString res = QString::number(id);

    query.prepare("DELETE FROM LOGEMENT WHERE ID_LOGEMENT = :id");
    query.bindValue(":id", res);

    bool success = query.exec();
    if (success) {
        ajouterHistorique("Suppression", "LOGEMENT", id);
    }

    return success;
}

bool logement::modifier_logement(int ID_LOGEMENT)
{

    QSqlQuery query;

    QString res = QString::number(ID_LOGEMENT);


    query.prepare("update  LOGEMENT set ADRESSE=:ADRESSE, ETAT=:ETAT where ID_LOGEMENT=:ID_LOGEMENT ");

    query.bindValue(":ADRESSE", ADRESSE);
    query.bindValue(":ETAT", ETAT);
    query.bindValue(":ID_LOGEMENT", ID_LOGEMENT);




    return query.exec();

}
//metiers

QSqlQueryModel * logement:: recherche_logement(QString var)
    {
        QSqlQueryModel * model= new QSqlQueryModel();

        model->setQuery("SELECT * FROM LOGEMENT WHERE ADRESSE LIKE '%"+var+"%' ");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_LOGEMENT"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("ADRESSE"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("ETAT"));

        return model;
    }
QSqlQueryModel * logement::triadresse(){
     QSqlQueryModel * model= new QSqlQueryModel();
     model->setQuery("SELECT * FROM LOGEMENT ORDER BY ADRESSE ");
     return model;

}
void logement::exporterpdf(QTextBrowser *text)
    {

    //exécuter des requêtes SQL
           QSqlQuery qry;
           //creation du tt pour sera utilisée pour stocker les données récupérées depuis la base de données
           std::list<QString> tt;

           // Ajout du titre "MEDICAL FILES" en orange
           tt.push_back("<span style='color: orange; font-size: 20px; font-weight: bold;'>HOUSING</span><br><br>");

           //récupérer toutes les données de la table
           qry.exec("select * from LOGEMENT");

           //création du tableau HTML
           QString table = "<table style='border-collapse: collapse; margin-top: 30px;'>";
           table += "<tr style='background-color: #d9d9d9;'>";
           table += "<th style='padding: 10px; text-align: left; width: 15%;'>ID_LOGEMENT</th>";
           table += "<th style='padding: 10px; text-align: left; width: 15%;'>ADRESSE</th>";
           table += "<th style='padding: 10px; text-align: left; width: 40%;'>ETAT</th>";
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
            QPixmap img("C:\\Users\\Tasnim ben Salah\\Desktop\\Logements\\Icons\\ApplicationHomeShare.png");
            int imgWidth = printer.pageRect().width() / 4;
            int imgHeight = imgWidth * img.height() / img.width();
            painter.drawPixmap(QRect(printer.pageRect().width() - imgWidth, 0, imgWidth, imgHeight), img);


            QMessageBox::information(nullptr, QObject::tr("Export PDF"),
                                      QObject::tr("Export avec succes .\n"
                                                  "Click OK to exit."), QMessageBox::Ok);


    }
int logement::calculerSommeEtat(QString etat) const {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM LOGEMENT WHERE ETAT = ?");
    query.bindValue(0, etat);
    if (!query.exec()) {
        // Une erreur s'est produite lors de l'exécution de la requête
        qDebug() << "Erreur lors de l'exécution de la requête SQL :" << query.lastError().text();
        return -1;
    }
    if (query.next()) {
        // La requête a réussi, récupérez le nombre de résultats
        int count = query.value(0).toInt();
        return count;
    } else {
        // La requête n'a retourné aucun résultat
        return 0;
    }
}
void logement::ajouterHistorique(QString action, QString type, int id)
{
    QSqlQuery query1;
    query1.prepare("select * from HISTORIQUE where TRUNC(HISTORIQUE_DATES) = TRUNC(SYSDATE)");
    query1.exec();

    QString date=QDateTime::currentDateTime().toString("dddd, dd MMMM yyyy");
    QString date1=QDateTime::currentDateTime().toString("dd/MM/yy");
    QString time=QDateTime::currentDateTime().toString("hh:mm");
    QString activite = "\n    " + date1 + "   -   " + time + " \t    " + action + " " + type + " d'identifiant:  " + QString::number(id) + " \n"; // correction ici pour convertir id en QString

    QSqlQuery query;
    query.prepare("INSERT INTO HISTORIQUE (activite, HISTORIQUE_DATES) VALUES (:activite, SYSDATE)");
    query.bindValue(":activite", activite);
    query.exec();

    activite = "\n\t\t\t\t" + date + "\n";
    if (!query1.next()) // if first activity of the day
    {
        query.prepare("INSERT INTO HISTORIQUE (activite, HISTORIQUE_DATES) VALUES (:activite, SYSDATE)");
        query.bindValue(":activite", activite);
        query.exec();
    }
    else
    {
        query.prepare("delete from HISTORIQUE where TRUNC(HISTORIQUE_DATES) = TRUNC(SYSDATE) and activite = :activite ");
        query.bindValue(":activite", activite);
        query.exec();

        query.prepare("INSERT INTO HISTORIQUE (activite, HISTORIQUE_DATES) VALUES (:activite, SYSDATE)");
        query.bindValue(":activite", activite);
        query.exec();
    }
}

