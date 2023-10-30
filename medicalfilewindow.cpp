#include "medicalfilewindow.h"
#include "ui_medicalfilewindow.h"
#include <QPixmap>
#include <QtWidgets>
#include"dossier_medical.h"
#include <QMessageBox>
#include"QtPrintSupport/QPrinter"
#include"QPdfWriter"
#include <QTextBrowser>
#include <QPrinter>
#include <QPainter>
//stat
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QColor>
#include <QtGui>
#include<QGraphicsSimpleTextItem>
#include<QPieSlice>
//mail

#include "smtp.h"

#include <QtNetwork>

bool kk=true;
MedicalfileWindow::MedicalfileWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MedicalfileWindow)
{
    ui->setupUi(this);
//calendrier
    //L'appel connect() nécessaire pour connecter les signaux émis par les widgets à leurs slots correspondants
    // Récupérer les dates de rendez-vous depuis la base de données
        QSqlQuery query;
        query.exec("SELECT DATE_RENDEZ_VOUS FROM DOSSIER_MEDICAL");

        // Définir le format de texte pour chaque date de rendez-vous
        QTextCharFormat format;
        format.setBackground(Qt::red);
        format.setForeground(Qt::white);

        while (query.next()) {
            QDate date = query.value(0).toDate();
            ui->calendarWidget->setDateTextFormat(date, format);
        }

        // Connecter le signal clicked(const QDate &) du widget calendarWidget à la fonction on_calendarWidget_clicked(const QDate &)
        connect(ui->calendarWidget, SIGNAL(clicked(const QDate &)), this, SLOT(on_calendarWidget_clicked(const QDate &)));

    //CONTROLE DE SAISIE ID_DOSSIER
    ui->lineEdit_id->setValidator(new QIntValidator(0,999, this));


    //controle de saisie description
    QLineEdit* lineEdit = new QLineEdit;
    lineEdit->setMaxLength(100);
    ui->lineEdit_description->setMaxLength(100);

    //etat
     ui->lineEdit_condition->setMaxLength(10);

    //appel afficher et tri
    ui->tableView_list->setModel(Etmp.afficher());
    ui->tableView_list->setModel(Etmp.trirendez_vous());

         on_pushButton_statistics_clicked();
         on_pushButton_calendrier_clicked();

         int ret=A.connect_arduino(); // lancer la connexion à arduino
         switch(ret){
         case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
             break;
         case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
            break;
         case(-1):qDebug() << "arduino is not available";
         }
          QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
          //le slot update_label suite à la reception du signal readyRead (reception des données).

}

MedicalfileWindow::~MedicalfileWindow()
{
    delete ui;
}


void MedicalfileWindow::on_pushButton_save_clicked()
{
    // Récupération des informations saisies dans les champs
    int id_dossier = ui->lineEdit_id->text().toInt();
    QString etat = ui->lineEdit_condition->text();
    QString description = ui->lineEdit_description->text();
    QDate date_dernier_consol = ui->dateEdit_last_consultation->date();
    QDate date_rendez_vous = ui->dateEdit_next_meeting->date();



    // Vérification que les champs "etat" et "description" ne contiennent que des alphabets
    QRegExp regExp("^[A-Za-z ]+$");
    if (!etat.contains(regExp) || !description.contains(regExp)) {
        // Afficher un message d'erreur à l'utilisateur
        QMessageBox::warning(this, "Erreur", "Veuillez saisir des alphabets seulement.");
        return;
    }
    // Vérification que l'état est soit "bien" soit "malade"
        if (etat != "bien" && etat != "malade") {
            // Afficher un message d'erreur à l'utilisateur
            QMessageBox::warning(this, "Erreur", "L'état doit être soit 'bien' soit 'malade'.");
            return;
        }
    if ((date_rendez_vous < date_dernier_consol) ) {
        // Afficher un message d'erreur à l'utilisateur
        QMessageBox::warning(this, "Erreur", "La date de rendez-vous doit être supérieure à la date de la dernière consultation .");

        return;
    }
    //************mailllll
    // Formater le corps du message avec les valeurs saisies
    QString message = QString("Bonjour,\n\n" // Ajouter ici le mot "Bonjour" au début du mail
                               "Le dossier avec l'ID %1 a été ajouté avec succès.\n"
                               "État : %2\n"
                               "Description : %3\n"
                               "Dernière consultation : %4\n"
                               "Prochain rendez-vous : %5\n\n"
                              "Cordialement,") // Ajouter ici le mot "Cordialement" à la fin du mail
                        .arg(id_dossier)
                        .arg(etat)
                        .arg(description)
                        .arg(date_dernier_consol.toString(Qt::ISODate))
                        .arg(date_rendez_vous.toString(Qt::ISODate));

    Smtp* smtp = new Smtp("souha.habibi@esprit.tn", "211JFT0086S", "smtp.gmail.com", 465);

    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));

    smtp->sendMail("souha.habibi@esprit.tn", "souha.habibi@esprit.tn","DOSSIER MEDICAL", message);

    //*************


    dossier_medical D(id_dossier, etat, description, date_dernier_consol, date_rendez_vous);
    bool test = D.ajouter();

    if (test)
    {
        // Actualiser la vue
        ui->tableView_list->setModel(Etmp.afficher());

        // Appeler la fonction de tri
        ui->tableView_list->setModel(Etmp.trirendez_vous());


        // Afficher un message de confirmation
        QMessageBox::information(nullptr, QObject::tr("Ok"),
            QObject::tr("Ajout effectué.\n"
                        "Cliquez sur Annuler pour quitter."),
                        QMessageBox::Cancel);
         on_pushButton_calendrier_clicked();
    }
    else
    {
        // Afficher un message d'erreur
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
            QObject::tr("Ajout non effectué.\n"
                        "Cliquez sur Annuler pour quitter."),
                        QMessageBox::Cancel);
    }
    ui->lineEdit_id->clear();
    ui->lineEdit_condition->clear();
    ui->lineEdit_description->clear();
    ui->dateEdit_last_consultation->setDate(QDate::currentDate());
    ui->dateEdit_next_meeting->setDate(QDate::currentDate().addDays(7));
    on_pushButton_statistics_clicked();

}
void MedicalfileWindow::on_pushButton_delete_clicked()
{
    dossier_medical d;
    // Obtenez les données de cette cellule dans le modèle de données
    QVariant rowData = ui->tableView_list->model()->data(ui->tableView_list->currentIndex());

    // Affichez les données de la ligne sélectionnée dans la console
    qDebug() << "La ligne sélectionnée est : " << rowData.toString();

    bool test=d.supprimer(rowData.toInt());

    if(test)
    {
        // Enlever le format de texte rouge pour cette date particulière
        QDate date = ui->tableView_list->model()->index(ui->tableView_list->currentIndex().row(), 4).data().toDate();
        ui->calendarWidget->setDateTextFormat(date, QTextCharFormat());

        QMessageBox::information(nullptr, QObject::tr("ok"),
                    QObject::tr("Suppression effectuee.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

        ui->tableView_list->setModel(d.afficher());
          //appel fonction tri
         ui->tableView_list->setModel(Etmp.trirendez_vous());




    }
    else
        QMessageBox::critical(nullptr, QObject::tr("not ok"),
                    QObject::tr("Suppression non effectuee.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
    on_pushButton_statistics_clicked();
}

void MedicalfileWindow::on_pushButton_update_clicked()
{


        QModelIndexList selectedList=ui->tableView_list->selectionModel()->selectedRows();
        if(!(selectedList.isEmpty())){
        ui->lineEdit_id_2->setText(ui->tableView_list->model()->data(ui->tableView_list->model()->index(selectedList.at(0).row(),0)).toString());
        ui->lineEdit_condition_2->setText(ui->tableView_list->model()->data(ui->tableView_list->model()->index(selectedList.at(0).row(),1)).toString());
        ui->lineEdit_description_2->setText(ui->tableView_list->model()->data(ui->tableView_list->model()->index(selectedList.at(0).row(),2)).toString());
        ui->dateEdit_last_consultation_2->setDate(ui->tableView_list->model()->data(ui->tableView_list->model()->index(selectedList.at(0).row(),3)).toDate());
        ui->dateEdit_next_meeting_2->setDate(ui->tableView_list->model()->data(ui->tableView_list->model()->index(selectedList.at(0).row(),4)).toDate());



    }
        else {
            QMessageBox::information(nullptr, QObject::tr("ok"),
                        QObject::tr("Select row to Update please!!.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);
    }
    }
void MedicalfileWindow::on_pushButton_update_2_clicked()
{

    // Récupération des informations saisies dans les champs
        int id_dossier = ui->lineEdit_id_2->text().toInt();
        QString etat = ui->lineEdit_condition_2->text();
        QString description = ui->lineEdit_description_2->text();
        QDate date_dernier_consol = ui->dateEdit_last_consultation_2->date();
        QDate date_rendez_vous = ui->dateEdit_next_meeting_2->date();

        // Vérification que les champs "etat" et "description" ne contiennent que des alphabets
        QRegExp regExp("^[A-Za-z ]+$");

        if (!etat.contains(regExp) || !description.contains(regExp)) {
            // Afficher un message d'erreur à l'utilisateur
            QMessageBox::warning(this, "Erreur", "Veuillez saisir des alphabets seulement.");
            return;
        }
        // Vérification que l'état est soit "bien" soit "malade"
        if (etat != "bien" && etat != "malade") {
            // Afficher un message d'erreur à l'utilisateur
            QMessageBox::warning(this, "Erreur", "L'état doit être soit 'bien' soit 'grave'.");
            return;
        }

        // Vérification que la date de rendez-vous est supérieure à la date de la dernière consultation
        if (date_rendez_vous <= date_dernier_consol) {
            // Afficher un message d'erreur à l'utilisateur
            QMessageBox::warning(this, "Erreur", "La date de rendez-vous doit être supérieure à la date de la dernière consultation.");
            return;


        }

        dossier_medical d;
        bool test = d.modifier_dossier(id_dossier, etat, description, date_dernier_consol, date_rendez_vous);


        if (test) {
            // Actualiser la vue
            ui->tableView_list->setModel(Etmp.afficher());

            // Appeler la fonction de tri
            ui->tableView_list->setModel(Etmp.trirendez_vous());

            // Afficher un message de confirmation
            QMessageBox::information(nullptr, QObject::tr("Ok"),
                QObject::tr("Modification effectuée.\n"
                            "Cliquez sur Annuler pour quitter."),
                            QMessageBox::Cancel);

            // Mettre à jour la couleur de la date de rendez-vous dans le calendrier
            QTextCharFormat format;
            format.setBackground(Qt::red);
            format.setForeground(Qt::white);
            ui->calendarWidget->setDateTextFormat(date_rendez_vous, format);

            // Mettre à jour la couleur de l'ancienne date de rendez-vous dans le calendrier
            QTextCharFormat old_format;
            old_format.setBackground(Qt::white);
            old_format.setForeground(Qt::black);
            ui->calendarWidget->setDateTextFormat(old_date_rendez_vous, old_format);

            // Rafraîchir l'interface utilisateur
            ui->calendarWidget->update();
            on_pushButton_calendrier_clicked();
        }

        else {
            // Afficher un message d'erreur
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                QObject::tr("Modification non effectuée.\n"
                            "Cliquez sur Annuler pour quitter."),
                            QMessageBox::Cancel);
        }

    ui->lineEdit_id_2->clear();
    ui->lineEdit_condition_2->clear();
    ui->lineEdit_description_2->clear();
    ui->dateEdit_last_consultation_2->setDate(QDate::currentDate());
    ui->dateEdit_next_meeting_2->setDate(QDate::currentDate().addDays(7));

}


//recherrche
void MedicalfileWindow::on_lineEdit_recherche_textChanged(const QString &arg1)
{
  ui->tableView_list->setModel(Etmp.recherche_dossier(arg1));
}




//pdf
void MedicalfileWindow::on_pushButton_pdf_clicked()
{
   Etmp.exporterpdf(ui->textBrowser);
}


//stat

void MedicalfileWindow::on_pushButton_statistics_clicked()
{
    QSqlQuery q1, q2, q3; //requette
    qreal tot = 0, c1 = 0, c2 = 0;

    q1.prepare("SELECT * FROM DOSSIER_MEDICAL");
    q1.exec(); //La requête récupère toutes les lignes de la table,

    q2.prepare("SELECT * FROM DOSSIER_MEDICAL WHERE ETAT='bien'");
    q2.exec();

    q3.prepare("SELECT * FROM DOSSIER_MEDICAL WHERE ETAT='malade'");
    q3.exec();
    // les deux autres requêtes récupèrent uniquement les lignes où le champ "ETAT" est égal à "bien" et "malade",

    while (q1.next()) { tot++; } //compter nbr totale dossiers
    while (q2.next()) { c1++; } //compter nbr bien
    while (q3.next()) { c2++; } //compter nbr malade
    // la fonction utilise une boucle while pour compter le nombre total dossiers et le nombre dossiers dans chaque catégorie de etat.

    c1 = c1 / tot;
    c2 = c2 / tot; //calculer les pourcentages

    //Définir les tranches et le pourcentage de l'ensemble qu'elles occupent
    QPieSeries *series = new QPieSeries();//QPieSeries pour stocker les données du graphique circulaire

    QString bienStr = QString("bien (%1%)").arg(QString::number(c1*100, 'f', 2));
    QPieSlice *slice1 = series->append(bienStr,c1);

    slice1->setColor(QColor(255,165,0)); // set orange color for the "bien" slice

    QString maladeStr = QString("malade (%1%)").arg(QString::number(c2*100, 'f', 2));
    QPieSlice *slice2 = series->append(maladeStr,c2);

    slice2->setColor(QColor(Qt::black)); // définir la couleur noire pour la tranche "grave"

    // Afficher les étiquettes de pourcentage à l'extérieur de chaque tranche
    slice1->setLabelVisible();
    slice1->setLabelPosition(QPieSlice::LabelOutside);
    slice2->setLabelVisible();
    slice2->setLabelPosition(QPieSlice::LabelOutside);

    // Create the chart(graphique) widget
    QChart* chart = new QChart();

    // Add data to chart with title and show legend
    chart->addSeries(series);
    chart->legend()->show();

    // Used to display the chart
    QChartView* chartView = new QChartView(chart, ui->graphicsView);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(700, 500);
    chartView->show();
}


void MedicalfileWindow::on_pushButton_calendrier_clicked()
{

    // Définir le format de texte pour chaque date de rendez-vous
    QTextCharFormat format;
    //arriere plan rouge
    format.setBackground(Qt::red);
    //text blanc
    format.setForeground(Qt::white);

    // Récupérer les dates de rendez-vous depuis la base de données et définir le format de texte pour chaque date
    QSqlQuery query;
    query.exec("SELECT DATE_RENDEZ_VOUS FROM DOSSIER_MEDICAL");
    while (query.next()) {
        //extrait la date_rendez_vous de la requete avec toDate
        QDate date = query.value(0).toDate();
        if(ui->calendarWidget->dateTextFormat(date).background().color() != Qt::red) {
            ui->calendarWidget->setDateTextFormat(date, format);
        }
    }
}



void MedicalfileWindow::on_calendarWidget_clicked(const QDate &date)
{
    // Compter le nombre de rendez-vous pour la date sélectionnée
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM DOSSIER_MEDICAL WHERE DATE_RENDEZ_VOUS = ?");
    query.addBindValue(date);
    query.exec();
    query.next();
    int nbRendezVous = query.value(0).toInt();

    // Définir le message et la couleur de la QMessageBox en fonction du nombre de rendez-vous
    QMessageBox msgBox;
    msgBox.setWindowTitle("Nombre de rendez-vous");
    QString message;
    if (nbRendezVous == 0) {
        message = "Il n'y a aucun rendez-vous.";
        msgBox.setStyleSheet("background-color: red;");
    } else {
        message = "Il existe " + QString::number(nbRendezVous) + " rendez-vous.";
        msgBox.setStyleSheet("background-color: green;");
        QTextCharFormat format;
        format.setBackground(Qt::red);
        format.setForeground(Qt::white);
        ui->calendarWidget->setDateTextFormat(date, format);
    }
    msgBox.setText(message);
    msgBox.exec();

    // Réappliquer le format de texte rouge à la date sélectionnée
    if(ui->calendarWidget->dateTextFormat(date).background() == Qt::red)
    {
        // Définir le format de texte pour chaque date de rendez-vous
        QTextCharFormat format;
        //arriere plan rouge
        format.setBackground(Qt::red);
        //text blanc
        format.setForeground(Qt::white);
        ui->calendarWidget->setDateTextFormat(date, format);
    }
}

void MedicalfileWindow::update_label()
{

    QString lesm; // déclaration de la variable lesm
    QByteArray x = A.read_from_arduino();

     if (x == "*") {
         kk = false;
         qDebug () << data ;
         QSqlQuery query;
         query.prepare("SELECT NOM FROM INDIGENTS WHERE ID_INDIGENT = ?");
         query.addBindValue(data.toInt());
         query.exec();
         query.next();
         QString lesm = query.value(0).toString();
         qDebug () << lesm ;
         if(!lesm.isNull())
         {
         A.write_to_arduino(lesm.toUtf8());
         query.prepare("INSERT INTO HISTORIQUE (ACTIVITE, HISTORIQUE_DATES , ID) VALUES (:act, :hist , :id)");
         QString a="consultation :"+lesm ;
         query.bindValue(":act", a);
         query.bindValue(":hist", QDate::currentDate());
         query.bindValue(":id", 9);
         query.exec();
         }
     }
     if (x == "#")
     {
         data="";
         kk=true;
     }
     if (kk && x!="#") {
         data += QString::fromUtf8(x);
     }

}

