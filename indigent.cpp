#include "indigent.h"
#include <QMessageBox>
#include <QDate>
#include <QString>
#include<QSqlQuery>
#include <QDebug>
#include <QObject>
#include <QFileDialog>
#include <QtPrintSupport/QPrinter>
#include <QPrinter>
#include <QPainter>
#include <QtPrintSupport/QPrintDialog>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QFile>
#include <QSqlError>
#include <QImage>
#include<QBuffer>
#include <QByteArray>
#include <QtSql>
#include <QtCharts>
#include <QChartView>
#include <QHBoxLayout>
#include"logement.h"
indigent::indigent(){}
indigent::indigent(int id_indigent,QString nom ,QString prenom,QDate dob,QString sexe)
{

     this->id_indigent=id_indigent;
     this->nom=nom;
     this->prenom=prenom;
     this->dob=dob;
     this->sexe=sexe;


}
int indigent::getid(){return id_indigent;}
QString indigent:: getnom(){return nom;}
QString indigent:: getprenom(){return prenom;}
QDate indigent:: getdate(){return dob;}
QString  indigent::getsexe(){return sexe;}
//
void indigent:: setid(int i){this->id_indigent=i;}
void indigent:: setnom(QString n){this->nom=n;}
void  indigent::setprenom(QString p){this->prenom=p;}
void  indigent::setdate(QDate d){this->dob=d;}
void indigent::setsexe(QString s){sexe=s;}
////ajouter /////
bool indigent ::ajouter()
{
    QSqlQuery query;
    QString res = QString :: number(id_indigent);
    //utilisé pour preparer une instruction sql pour l 'execution
    query.prepare("INSERT INTO  INDIGENT(ID_INDIGENT,NOM,PRENOM,DOB,SEXE)VALUES(:id,:nom,:prenom,:dob,:sexe)");
    query.bindValue(":id",res);//bch nhot l valeur f blastha res f id
    query.bindValue(":nom",nom);
    query.bindValue(":prenom",prenom);
    query.bindValue(":dob",dob);
    query.bindValue(":sexe",sexe);
    //query.bindValue(":id_logment",ID_LOGEMNT);//method is used to execute the query



    return query.exec();
}

////afficher////
QSqlQueryModel * indigent :: afficher()
{
    //class that provides a read-only data model
QSqlQueryModel * model=new QSqlQueryModel ();
model->setQuery("select * from INDIGENT");
model->setHeaderData(0,Qt::Horizontal,QObject::tr("id_indigent"));
model->setHeaderData(1,Qt::Horizontal,QObject::tr("nom"));
model->setHeaderData(2,Qt::Horizontal,QObject::tr("prenom"));
model->setHeaderData(3,Qt::Horizontal,QObject::tr("dob"));
model->setHeaderData(4,Qt::Horizontal,QObject::tr("sexe"));
model->setHeaderData(5,Qt::Horizontal,QObject::tr("logement"));
return model;
}

////supprimer////
bool indigent ::supprimer(int id)
{
QSqlQuery query; //créer un objet QSqlQuery pour exécuter des requêtes SQL queries
QString res=QString ::number(id); // convertir id m number l string
query.prepare("delete from INDIGENT where ID_INDIGENT= :id");//naamel preparation
query.bindValue(":id",res);
return query.exec(); // bch nexecuti the query
}
////modifier////

bool indigent::modifier(int id)
{
    QSqlQuery query;
    QString res=QString ::number(id);

    query.prepare("UPDATE INDIGENT SET NOM=:nom,PRENOM=:prenom,DOB=:dob,SEXE=:sexe where ID_INDIGENT=:id");
    query.bindValue(":id",res);
    query.bindValue(":nom",nom);
    query.bindValue(":prenom",prenom);
    query.bindValue(":dob",dob);
    query.bindValue(":sexe",sexe);


    return query.exec();
}



/*Recherche par Nom*/

QSqlQueryModel * indigent:: Recherche(QString a){

    QSqlQueryModel *model=new QSqlQueryModel();
    model->setQuery("SELECT * FROM INDIGENT WHERE ( NOM LIKE '%"+a+"%' OR PRENOM LIKE '%"+a+"%')");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID_INDIGENT"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("NOM"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("PRENOM"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("DOB"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("SEXE"));



    return  model ;
}


QSqlQueryModel* indigent::triSexe()
{

    QSqlQueryModel* model=new QSqlQueryModel();

    model->setQuery("SELECT* FROM INDIGENT ORDER BY SEXE");

    return model;
}
QSqlQueryModel* indigent::triDob()
{

    QSqlQueryModel* model=new QSqlQueryModel();

    model->setQuery("SELECT* FROM INDIGENT ORDER BY DOB");

    return model;
}
void indigent::exporterpdf(QTextBrowser *text)
    {
    // Exécuter des requêtes SQL
       QSqlQuery qry;

       // Création du tableau qui sera utilisé pour stocker les données récupérées depuis la base de données
       QStringList headers = {"ID_INDIGENT", "NOM", "PRENOM", "DOB", "SEXE"};
       QStandardItemModel* tableModel = new QStandardItemModel();
       tableModel->setHorizontalHeaderLabels(headers);

       // Ajout des données dans le tableau
       qry.exec("SELECT * FROM INDIGENT");
       while (qry.next())
       {
           QList<QStandardItem*> row;
           row << new QStandardItem(qry.value(0).toString());
           row << new QStandardItem(qry.value(1).toString());
           row << new QStandardItem(qry.value(2).toString());
           row << new QStandardItem(qry.value(3).toString());
           row << new QStandardItem(qry.value(4).toString());
           tableModel->appendRow(row);
       }

       // Création de la vue de tableau et ajout du modèle de tableau
       QTableView tableView;
       tableView.setModel(tableModel);
       tableView.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

       // Ajout du titre "INDIGENT FILES" en orange
       QString htmlText = "<span style='color: orange; font-size: 20px; font-weight: bold;'>INDIGENT FILES</span><br><br>";

       // Convertir le modèle de tableau en HTML
       QString tableHtml = "<table>" + tableView.horizontalHeader()->sectionViewportPosition(0);
       for (int i = 0; i < tableModel->rowCount(); i++)
       {
           tableHtml += "<tr>";
           for (int j = 0; j < tableModel->columnCount(); j++)
           {
               QString value = tableModel->item(i, j)->text();
               tableHtml += "<td>" + value + "</td>";
           }
           tableHtml += "</tr>";
       }
       tableHtml += "</table>";

       // Ajouter le tableau au texte HTML
       htmlText += tableHtml;

       // Ouvre une boîte de dialogue pour permettre à l'utilisateur de choisir l'emplacement et le nom du fichier PDF généré
       QString fileName = QFileDialog::getSaveFileName((QWidget*)0, "Export PDF", QString(), "*.pdf");
       if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }

       // Créer une nouvelle instance de la classe QPrinter qui est utilisée pour configurer l'impression
       QPrinter printer(QPrinter::PrinterResolution);

       // Le format de sortie de l'impression
       printer.setOutputFormat(QPrinter::PdfFormat);

       // La taille de papier pour l'impression
       printer.setPaperSize(QPrinter::A4);

       // Le nom de fichier de sortie pour l'impression
       printer.setOutputFileName(fileName);

       // Impression des données au format HTML
       QTextDocument doc;
       doc.setHtml(htmlText);

       // Dessiner le contenu du QTextDocument sur l'imprimante
       QPainter painter(&printer);
       doc.drawContents(&painter);

            // Ajout de l'image à droite de la page
            QPixmap img("C:\\Users\\ADMIN\\Desktop\\Interface Qt (1)\\Indigents\\Icons\\ApplicationHomeShare.png");
            int imgWidth = printer.pageRect().width() / 4;
            int imgHeight = imgWidth * img.height() / img.width();
            painter.drawPixmap(QRect(printer.pageRect().width() - imgWidth, 0, imgWidth, imgHeight), img);

            QMessageBox::information(nullptr, QObject::tr("Export PDF"),
                                      QObject::tr("Export avec succes .\n"
                                                  "Click OK to exit."), QMessageBox::Ok);


        }


bool indigent::insertImageToDatabase(const QString& imagePath, const QString& imageName, const int& id)
{
    // Read the image file
    QFile file(imagePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open image file: " << file.errorString();
        return false;
    }

    QByteArray imageBytes = file.readAll();
    file.close();

    // Insert the image into the database
    QSqlQuery query;
    query.prepare("INSERT INTO INDIGENT (ID_LOGEMNT, NAME, DATA) VALUES (:id, :name, :data)");
    query.bindValue(":id", 1);
    query.bindValue(":name", imageName);
    query.bindValue(":data", imageBytes);
    if (!query.exec()) {
        qDebug() << "Failed to insert image into database: " << query.lastError().text();
        return false;
    }

    qDebug() << "Image inserted successfully!";
    return true;
}



int indigent::alerte()
{
    QSqlQuery query;
        int nbIndigents = 0;

        if (query.exec("SELECT COUNT(*) FROM INDIGENT")) {
            if (query.next()) {
                nbIndigents = query.value(0).toInt();
            }
        } else {
            qWarning() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
        }

        qDebug() << "Nombre d'indigents : " << nbIndigents;
        return nbIndigents;
}
void indigent::statistiqueSegment()
{

    // Requête SQL pour récupérer les données
    QSqlQuery query("SELECT COUNT(*) FROM INDIGENT WHERE sexe='Femme'");
    query.next();
    int nbFemmes = query.value(0).toInt();
    query.exec("SELECT COUNT(*) FROM INDIGENT WHERE sexe='Homme'");
    query.next();
    int nbHommes = query.value(0).toInt();

    // Création de la série de données en courbe
    QLineSeries *series = new QLineSeries();
    series->append(0, nbFemmes);
    series->append(1, nbHommes);

    // Création du graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des INDIGENTS selon le sexe");
    chart->createDefaultAxes();

    // Personnalisation de l'axe des X
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append("Femmes");
    axisX->append("Hommes");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Personnalisation de l'axe des Y
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Valeurs");

    axisY->setTitleText("Nombre de INDIGENT");
    chart->setAxisY(axisY, series);

    // Création de la vue du graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Ajout de la vue dans la fenêtre principale
   // ui->verticalLayout->addWidget(chartView);

}
void indigent::imprimer(QTextEdit* widget)
{
    QPrinter printer;

      QPrintDialog dialog(&printer);
      if (dialog.exec() != QDialog::Accepted) {
          return;
      }

      QPainter painter(&printer);
      QRect rect = painter.viewport();
      QSize size = widget->document()->size().toSize();
      size.scale(rect.size(), Qt::KeepAspectRatio);
      painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
      painter.setWindow(0, 0, widget->document()->pageSize().width(), widget->document()->pageSize().height());
      painter.setRenderHint(QPainter::Antialiasing, true);
      widget->document()->drawContents(&painter);
}
