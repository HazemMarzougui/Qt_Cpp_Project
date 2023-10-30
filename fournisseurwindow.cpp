#include "fournisseurwindow.h"
#include "ui_fournisseurwindow.h"
#include "fournisseur.h"
#include <QMessageBox>
#include <QIntValidator>
#include <QTextBrowser>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QtPrintSupport/QPrinter>
#include <QPdfWriter>
#include <QDesktopServices>
#include <QPainter>
#include <QCheckBox>
#include "codeqr.h"
#include <QPixmap>
#include<QDirModel>
#include <QtSvg/QSvgRenderer>
#include <fstream>
#include<QRegularExpression>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include "webaxwidget.h"
#include "dialog.h"
#include "arduino.h"

using qrcodegen::QrCode ;
FournisseurWindow::FournisseurWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FournisseurWindow)
{

    ui->setupUi(this);
    ui->tableView->setModel(F.Afficher());
    ui->lineID->setValidator(new QIntValidator(0,9999999,this));
    ui->lineID_2->setValidator(new QIntValidator(0,9999999,this));


    int ret=A.connect_arduino(); // lancer la connexion à arduino
        switch(ret){
            case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
                break;
            case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
                break;
            case(-1):qDebug() << "arduino is not available";
            }




     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(ajouter_fournisseur()));

    //maps
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       QCoreApplication::organizationName(), QCoreApplication::applicationName());

    ui->WebBrowser->dynamicCall("Navigate(const QString&)", "https://www.google.com/maps/place/ESPRIT/@36.9016729,10.1713215,15z");

}

FournisseurWindow::~FournisseurWindow()
{
    delete ui;
}


void FournisseurWindow::on_pushButton_save_clicked()
{
    int Id_fournisseur=ui->lineID->text().toInt();
    QString Type=ui->comboBox_type->currentText();
    QString Nom=ui->lineEdit_name->text();
    QString Lieux=ui->lineEdit_place->text();
 Fournisseur F(Id_fournisseur,Type,Nom,Lieux);

bool test=F.Ajouter();

if (test)
 {
     ui->tableView->setModel(F.Afficher());
     QMessageBox :: information (nullptr, QObject ::tr("Insert Successfully"),
                  QObject ::tr("Insert Successfully\n"
                               "click cancel to exit"),
             QMessageBox:: Cancel);

}
 else
 {
     QMessageBox::critical(nullptr,QObject::tr("Insert Failed"),
                          QObject::tr("Try Again.\n"
                                      "click cancel to exit."),QMessageBox::Cancel);
 }
}



void FournisseurWindow::on_pushButton_delete_clicked()
{
    int Id_fournisseur=ui->lineID_2->text().toInt();
         bool test=F.Supprimer(Id_fournisseur);
         if (test)
         {     ui->tableView->setModel(F.Afficher());
            QMessageBox :: information (nullptr, QObject ::tr("OK"),
                          QObject ::tr("suppression effectué\n"
                                       "click cancel to exit"),
                     QMessageBox:: Cancel);

        }
         else
         {
             QMessageBox::critical(nullptr,QObject::tr("not ok"),
                                  QObject::tr("try again.\n"
                                              "click cancel to exit."),QMessageBox::Cancel);
         }
}


void FournisseurWindow::on_pushButton_update_clicked()
{
    int Id_Fournisseur=ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt(); // get the selected row id

    QString Type=ui->comboBox_type_2->currentText();
    QString Nom=ui->lineEdit_name_2->text();
    QString Lieux=ui->lineEdit_place_2->text();

                  Fournisseur  F(Id_Fournisseur,Type,Nom,Lieux);
               bool test=F.Modifier(Id_Fournisseur);

               if (test)

               {

               ui->tableView->setModel(F.Afficher());
                   QMessageBox :: information (nullptr, QObject ::tr("OK"),

                                QObject ::tr("modifier effectué\n"

                                             "click cancel to exit"),

                           QMessageBox:: Cancel);



           }

               else

               {

                   QMessageBox::critical(nullptr,QObject::tr("not ok"),

                                        QObject::tr("try again.\n"

                                                    "click cancel to exit."),QMessageBox::Cancel);
}
}

void FournisseurWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    ui->lineID_2->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString());
    ui->comboBox_type_2->setCurrentText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),1)).toString());
  ui->lineEdit_name_2->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),2)).toString());
   ui->lineEdit_place_2->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),3)).toString());

                            /*Code QR*/
   if(ui->tableView->currentIndex().row()==-1)
             QMessageBox::information(nullptr, QObject::tr("Warning"),
                                      QObject::tr("Please Choose a Provider from Table .\n"
                                                  "Click Ok to exit."), QMessageBox::Ok);
         else
         {




                     F.setidfournisseur(ui->lineID->text().toInt());
                     F.settype(ui->comboBox_type->currentText());
                     F.setnom(ui->lineEdit_name->text());
                     F.setlieux(ui->lineEdit_place ->text());







           QString  rawQr = "IDF:%1 TYPE:%2 NOM:%3 PLACE:%4 " ;
              rawQr = rawQr.arg(F.getidfournisseur()).arg(F.gettype()).arg(F.getnom()).arg(F.getlieux());
              QrCode qr = QrCode::encodeText(rawQr.toUtf8().constData(), QrCode::Ecc::HIGH);  // encoder la chaine formaté à partir des valeurs de classe en code QR



              std::ofstream myfile;
              myfile.open ("qrcode.svg") ;
              myfile << qr.toSvgString(1);
              myfile.close();
              QSvgRenderer svgRenderer(QString("qrcode.svg"));
              QPixmap pix( QSize(140, 140) );
              QPainter pixPainter( &pix );
              svgRenderer.render( &pixPainter );
              ui->label_codeQr->setPixmap(pix);
         }
}



/*LineEdit (Recherche)*/
void FournisseurWindow::on_lineEdit_search_textChanged(const QString &arg1)
{
  ui->tableView->setModel(F.Recherche(arg1));
}








void FournisseurWindow::on_lineEdit_name_textChanged(const QString &arg1)
{
    QString nom = ui->lineEdit_name->text().trimmed(); // Récupération du texte en supprimant les espaces inutiles au début et à la fin

    // Vérification si le champ est vide ou non
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Le champ du Nom est vide.");
        return;
    }

    // Vérification si le Nom contient des caractères non alphabétiques ou des chiffres
    QRegularExpression regex("[^a-zA-Z]");
    if (regex.match(nom).hasMatch() || nom.contains(QRegularExpression("\\d"))) {
        QMessageBox::warning(this, "Nom invalide", "Le Nom ne doit contenir que des lettres alphabétiques.");
        ui->lineEdit_name->setText(nom.remove(QRegularExpression("\\d"))); // Suppression des chiffres
        return;
    }
}




void FournisseurWindow::on_lineEdit_place_textEdited(const QString &arg1)
{
    QString place = ui->lineEdit_place->text().trimmed(); // Récupération du texte en supprimant les espaces inutiles au début et à la fin

    // Vérification si le champ est vide ou non
    if (place.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Le champ du Place est vide.");
        return;
    }

    // Vérification si le Place contient des caractères non alphabétiques ou des chiffres
    QRegularExpression regex("[^a-zA-Z]");
    if (regex.match(place).hasMatch() || place.contains(QRegularExpression("\\d"))) {
        QMessageBox::warning(this, "Place invalide", "Le Place ne doit contenir que des lettres alphabétiques.");
        ui->lineEdit_name->setText(place.remove(QRegularExpression("\\d"))); // Suppression des chiffres
        return;
    }
}


void FournisseurWindow::on_lineEdit_name_2_textEdited(const QString &arg1)
{
    QString nom = ui->lineEdit_name_2->text().trimmed(); // Récupération du texte en supprimant les espaces inutiles au début et à la fin

    // Vérification si le champ est vide ou non
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Le champ du Nom est vide.");
        return;
    }

    // Vérification si le Nom contient des caractères non alphabétiques ou des chiffres
    QRegularExpression regex("[^a-zA-Z]");
    if (regex.match(nom).hasMatch() || nom.contains(QRegularExpression("\\d"))) {
        QMessageBox::warning(this, "Nom invalide", "Le Nom ne doit contenir que des lettres alphabétiques.");
        ui->lineEdit_name->setText(nom.remove(QRegularExpression("\\d"))); // Suppression des chiffres
        return;
    }
}

void FournisseurWindow::on_lineEdit_place_2_textEdited(const QString &arg1)
{
    QString place = ui->lineEdit_place_2->text().trimmed(); // Récupération du texte en supprimant les espaces inutiles au début et à la fin

    // Vérification si le champ est vide ou non
    if (place.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Le champ du Place est vide.");
        return;
    }

    // Vérification si le Place contient des caractères non alphabétiques ou des chiffres
    QRegularExpression regex("[^a-zA-Z]");
    if (regex.match(place).hasMatch() || place.contains(QRegularExpression("\\d"))) {
        QMessageBox::warning(this, "Place invalide", "Le Place ne doit contenir que des lettres alphabétiques.");
        ui->lineEdit_name->setText(place.remove(QRegularExpression("\\d"))); // Suppression des chiffres
        return;
    }
}


void FournisseurWindow::on_pushButton_sort_clicked()
{
    ui->tableView->setModel(F.triName());
}



void FournisseurWindow::on_printButton_clicked()
{
    QPdfWriter pdf("test.pdf");
                   QPainter painter(&pdf); // pour dessiner sur le PDF
                   int i = 4000;
                   painter.setPen(Qt::red);
                   painter.setFont(QFont("Arial", 30));
                   painter.drawText(2800,1200,"Liste des Fournisseurs");
                   painter.setPen(Qt::black);
                   painter.setFont(QFont("Arial", 50));
                   painter.drawRect(1500,200,7300,2600); // Dessin d'un rectangle pour encadrer les éléments suivants
                   painter.drawRect(0,3000,9600,500);    // Dessin d'un rectangle pour le pied de page
                   painter.setFont(QFont("Arial", 9));
                   // Ajout des en-têtes de colonnes
                   painter.drawText(1300,3300,"IDF");
                   painter.drawText(2600,3300,"TYPE");
                   painter.drawText(3900,3300,"NAME");
                   painter.drawText(5200,3300,"PLACE");

                   QSqlQuery query;
                   query.prepare("select * from FOURNISSEUR");
                   query.exec();
                   while (query.next())
                   {

                       // Ajout des données des fournisseurs dans le PDF
                       painter.drawText(1300,i,query.value(0).toString());
                       painter.drawText(2600,i,query.value(1).toString());
                       painter.drawText(3900,i,query.value(2).toString());
                       painter.drawText(5200,i,query.value(3).toString());
                       i = i +500;
                   }

                   int reponse = QMessageBox::question(this, "Génerer PDF", "Vous Voulez Affichez Le PDF ?", QMessageBox::Yes |  QMessageBox::No);
                   if (reponse == QMessageBox::Yes)
                   {
                       QDesktopServices::openUrl(QUrl::fromLocalFile("test.pdf"));

                       painter.end();
                   }
                   if (reponse == QMessageBox::No)
                   {
                       painter.end();

           }
}

void FournisseurWindow::on_pushButton_statistics_clicked()
{
    s=new Dialog();

       s->setWindowTitle("statistique ");
      // s->choix_pie();
       s->show();
}



void FournisseurWindow::on_tabWidget_currentChanged(int index)
{

    ui->label_stat->clear();
    // Clear the QLabel with ID 'label_stat'

    if (index == 2 )
    // If the current index of the QTabWidget is 2
    {
        // Remove all widgets from the QVBoxLayout with ID 'verticalLayout_2'
        QLayoutItem* child;
        while ((child = ui->verticalLayout_2->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }

        // Execute SQL queries to get the number of suppliers by type
        QSqlQuery query("SELECT COUNT(*) FROM FOURNISSEUR WHERE TYPE ='Pharmacy'");
        query.next();
        int nbPharmacy = query.value(0).toInt();
        query.exec("SELECT COUNT(*) FROM FOURNISSEUR WHERE TYPE='Restaurant'");
        query.next();
        int nbRestaurant = query.value(0).toInt();
        query.exec("SELECT COUNT(*) FROM FOURNISSEUR WHERE TYPE='Clothes Shop'");
        query.next();
        int nbShop = query.value(0).toInt();

        // Calculate the percentage of suppliers by type
        qreal total = nbPharmacy + nbRestaurant +nbShop;
        qreal pourcentagePharmacy = nbPharmacy / total * 100;
        qreal pourcentageRestaurant = nbRestaurant / total * 100;
        qreal pourcentageShop = nbShop / total * 100;

        // Create QBarSet objects for each type of supplier
        QBarSet *setPharmacy = new QBarSet("Pharmacy");
        *setPharmacy<< nbPharmacy;

        QBarSet *setRestaurant = new QBarSet("Restaurant");
        *setRestaurant << nbRestaurant;

        QBarSet *setClothesShop= new QBarSet("Clothes Shop");
        *setClothesShop << nbShop;

        // Create a QBarSeries object and append the QBarSet objects to it
        QBarSeries *series = new QBarSeries();
        series->append(setPharmacy);
        series->append(setRestaurant);
        series->append(setClothesShop);

        // Create a QChart object and add the QBarSeries object to it
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Répartition des FOURNISSEUR selon le TYPE");
        chart->createDefaultAxes();

        // Customize the X axis
        QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
        axisX->setTitleText("Type");
        axisX->append("Pharmacy");
        axisX->append("Restaurant");
        axisX->append("Clothes Shop");

        // Personnalisation de l'axe Y
        QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
        axisY->setLabelFormat("%d");
        axisY->setTitleText("Nombre de FOURNISSEUR");
        axisY->setRange(0, total + 1);
        axisY->setTickCount(total + 0);


        // Création de la vue du graphique
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        // Ajout de la vue dans la fenêtre principale
        ui->verticalLayout_2->addWidget(chartView);

        // Affichage des pourcentages dans un QLabel
        ui->label_stat->setText(QString("Pharmacy : %1 (%2 %)\nRestaurant : %3 (%4 %)\nClothes Shop : %5 (%6 %)")  // premier parametre pour le nbre et la deuxieme pour le pourcentage
                                   .arg(nbPharmacy)
                                   .arg(QString::number(pourcentagePharmacy, 'f', 2))
                                   .arg(nbRestaurant)
                                   .arg(QString::number(pourcentageRestaurant, 'f', 2))
                                   .arg(nbShop)
                                   .arg(QString::number(pourcentageShop,'f',2)));



    }
}




void FournisseurWindow::on_pushButton_clicked()
{
    QString n = QString::number(F.afficher_type("Pharmacy"));
    A.write_to_arduino(n.toUtf8());
}

void FournisseurWindow::on_pushButton_2_clicked()
{
    QString n = QString::number(F.afficher_type("Restaurant"));
    A.write_to_arduino(n.toUtf8());
}

void FournisseurWindow::on_pushButton_3_clicked()
{
    QString n = QString::number(F.afficher_type("Clothes Shop"));
    A.write_to_arduino(n.toUtf8());
}

void FournisseurWindow::ajouter_fournisseur(){
    int a = A.read_from_arduino().toInt();

    qDebug() << a;
    if(a==1){
        Fournisseur Fou(F.afficher_type("Pharmacy")+F.afficher_type("Restaurant")+F.afficher_type("Clothes Shop")+15000,"Pharmacy","","");
        Fou.Ajouter();
    }
    else if(a==2){
        Fournisseur Fou(F.afficher_type("Pharmacy")+F.afficher_type("Restaurant")+F.afficher_type("Clothes Shop")+15000,"Restaurant","","");
        Fou.Ajouter();
    }
    else if(a==3){
        Fournisseur Fou(F.afficher_type("Pharmacy")+F.afficher_type("Restaurant")+F.afficher_type("Clothes Shop")+15000,"Clothes Shop","","");
        Fou.Ajouter();
    }

}
