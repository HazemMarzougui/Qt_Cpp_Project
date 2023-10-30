#include "indigentswindow.h"
#include "ui_indigentswindow.h"
#include"indigent.h"
#include <QIntValidator>
#include<regex>
#include <QTextBrowser>
#include <QSqlError>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include<QLabel>
#include<QString>
#include <cstdlib>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include <QStandardItemModel>
#include <QSystemTrayIcon>
#include <QMenu>
IndigentsWindow::IndigentsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IndigentsWindow)
{
    ui->setupUi(this);
     ui->tableView->setModel(i.afficher());
       //valider que 7 chiffres
     ui->lineId->setValidator(new QIntValidator(0,9999999,this));

     ui->id->setValidator(new QIntValidator(0,9999999,this));

 //   createTrayIcon(); // appel de la fonction pour créer l'icône dans la barre système

                QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;
                QString res=QString::number(i.afficher()->rowCount());
                notifyIcon->show();
                notifyIcon->showMessage("Information ! "," le nombre totale des indigents est : "+res+"",QSystemTrayIcon::Information,15000);

}

IndigentsWindow::~IndigentsWindow()
{
    delete ui;
}


void IndigentsWindow::on_ajouter_clicked()
{
      int id_indigent=ui->lineId->text().toInt();
      QString nom=ui->nom->text();
      QString prenom=ui->prenom->text();
      QDate dob=ui->dob->date();
      QString sexe = ui->sexe->currentText();


indigent i(id_indigent,nom,prenom,dob,sexe);
bool test=i.ajouter();
if (test)
 {

 ui->tableView->setModel(i.afficher());
     QMessageBox :: information (nullptr, QObject ::tr("OK"),
                  QObject ::tr("Ajout effectué\n"
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


void IndigentsWindow::on_supprimer_clicked()
{
    int id_indigent=ui->id->text().toInt();
         bool test=i.supprimer(id_indigent);
         if (test)
         {     ui->tableView->setModel(i.afficher());
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

void IndigentsWindow::on_modifier_clicked()
{
    int id_indigent=ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt(); // get the selected row id

    QString nom=ui->nom_modif->text();
    QString prenom=ui->prenom_modif->text();
    QDate dob=ui->dob_modif->date();
    QString sexe = ui->sexe_modif->currentText();

                  indigent i(id_indigent,nom,prenom,dob,sexe);
               bool test=i.modifier(id_indigent);

               if (test)

               {

               ui->tableView->setModel(i.afficher());
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

}}

void IndigentsWindow::on_tableView_doubleClicked(const QModelIndex &index)
{ui->id->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString());

    ui->nom_modif->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),1)).toString());
  ui->prenom_modif->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),2)).toString());
   ui->dob_modif->setDate(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),3)).toDate());
   ui->sexe_modif->setCurrentText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),4)).toString());

   filePath = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),6)).toString();
       if (!filePath.isEmpty()) {
           QPixmap imagePixmap(filePath);
           if (!imagePixmap.isNull())
           {
               ui->label_image_4->setPixmap(imagePixmap);
               ui->label_image_4->setScaledContents(true); // Pour redimensionner l'image pour s'adapter au label
           }
       }
       else
            ui->label_image_4->clear();
}



void IndigentsWindow::on_chercher_nom_textChanged(const QString &arg1)
{
     ui->tableView->setModel(i.Recherche(arg1));

}

void IndigentsWindow::on_nom_textEdited(const QString &arg1)
{
    QString nom = ui->nom->text().trimmed(); // Récupération du texte en supprimant les espaces inutiles au début et à la fin

    // Vérification si le champ est vide ou non
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Le champ du prenom est vide.");
        return;
    }

    // Vérification si le prenom contient des caractères non alphabétiques ou des chiffres
    QRegularExpression regex("[^a-zA-Z]");
    if (regex.match(nom).hasMatch() || nom.contains(QRegularExpression("\\d"))) {
        QMessageBox::warning(this, "Nom invalide", "Le nom ne doit contenir que des lettres alphabétiques.");
        ui->nom->setText(nom.remove(QRegularExpression("\\d"))); // Suppression des chiffres du prenom
        return;
    }
}

void IndigentsWindow::on_prenom_textEdited(const QString &arg1)
{
    QString prenom = ui->prenom->text().trimmed(); // Récupération du texte en supprimant les espaces inutiles au début et à la fin

    // Vérification si le champ est vide ou non
    if (prenom.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Le champ du prenom est vide.");
        return;
    }

    // Vérification si le prenom contient des caractères non alphabétiques ou des chiffres
    QRegularExpression regex("[^a-zA-Z]");
    if (regex.match(prenom).hasMatch() || prenom.contains(QRegularExpression("\\d"))) {
        QMessageBox::warning(this, "Nom invalide", "Le prenom ne doit contenir que des lettres alphabétiques.");
        ui->prenom->setText(prenom.remove(QRegularExpression("\\d"))); // Suppression des chiffres du prenom
        return;
    }
}

void IndigentsWindow::on_nom_modif_textEdited(const QString &arg1)
{
    QString nomModif = ui->nom_modif->text().trimmed(); // Récupération du texte en supprimant les espaces inutiles au début et à la fin

    // Vérification si le champ est vide ou non
    if (nomModif.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Le champ du nom a modifier est vide.");
        return;
    }

    // Vérification si le nom contient des caractères non alphabétiques
    QRegularExpression regex("[^a-zA-Z]");
    if (regex.match(nomModif).hasMatch()) {
        QMessageBox::warning(this, "Nom invalide", "Le nom ne doit contenir que des lettres alphabétiques.");
        return;
    }
}

void IndigentsWindow::on_prenom_modif_textEdited(const QString &arg1)
{
    QString prenomModif = ui->prenom_modif->text().trimmed(); // Récupération du texte en supprimant les espaces inutiles au début et à la fin

    // Vérification si le champ est vide ou non
    if (prenomModif.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Le champ du prenom a modifier est vide.");
        return;
    }

    // Vérification si le prenom a modifier contient des caractères non alphabétiques
    QRegularExpression regex("[^a-zA-Z]");
    if (regex.match(prenomModif).hasMatch()) {
        QMessageBox::warning(this, "Nom invalide", "Le prenom a modifier ne doit contenir que des lettres alphabétiques.");
        return;
    }
}



void IndigentsWindow::on_tri_2_clicked()
{
     ui->tableView->setModel(i.triSexe());
}

void IndigentsWindow::on_tri_3_clicked()
{
    ui->tableView->setModel(i.triDob());

}

void IndigentsWindow::on_pushButton_4_clicked()
{
    i.exporterpdf(ui->textBrowser);

}






void IndigentsWindow::on_pushButton_6_clicked()
{
    s=new Dialog();

   s->setWindowTitle("statistique ");
   s->show();
}

void IndigentsWindow::on_pushButton_7_clicked()
{
    // Requête SQL pour récupérer les données
    QSqlQuery query("SELECT COUNT(*) FROM INDIGENT WHERE sexe='Femme'");
    query.next();
    int nbFemmes = query.value(0).toInt();
    query.exec("SELECT COUNT(*) FROM  INDIGENT WHERE sexe='Homme'");
    query.next();
    int nbHommes = query.value(0).toInt();

    // Calcul des pourcentages
    qreal total = nbFemmes + nbHommes;
    qreal pourcentageFemmes = nbFemmes / total * 100;
    qreal pourcentageHommes = nbHommes / total * 100;

    // Création des données pour le graphique en bâtons
    QBarSet *setFemmes = new QBarSet("Femme");
    *setFemmes << nbFemmes;

    QBarSet *setHommes = new QBarSet("Homme");
    *setHommes << nbHommes;

    QBarSeries *series = new QBarSeries();
    series->append(setFemmes);
    series->append(setHommes);

    // Création du graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des INDIGENTS selon le sexe");
    chart->createDefaultAxes();

    // Personnalisation de l'axe X
    QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
    axisX->setTitleText("Sexe");
    axisX->append("Femme");
    axisX->append("Homme");

    // Personnalisation de l'axe Y
    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    axisY->setLabelFormat("%.0f");
    axisY->setTitleText("Nombre de INDIGENT");
    axisY->setRange(0, total + 1);
    axisY->setTickCount(total + 2);

    // Création de la vue du graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Ajout de la vue dans la fenêtre principale
    ui->verticalLayout_2->addWidget(chartView);

    // Affichage des pourcentages dans un QLabel
    ui->label_stat->setText(QString("Femme : %1 (%2 %)\nHomme : %3 (%4 %)")
                               .arg(nbFemmes)
                               .arg(QString::number(pourcentageFemmes, 'f', 2))
                               .arg(nbHommes)
                               .arg(QString::number(pourcentageHommes, 'f', 2)));

}

void IndigentsWindow::on_imprimer_clicked()
{  QPrinter printer;

    QPrintDialog dialog(&printer, this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QPainter painter(&printer);
    QRect rect = painter.viewport();
    QSize size = tableView->size();
    size.scale(rect.size(), Qt::KeepAspectRatio);
    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
    painter.setWindow(tableView->rect());
    tableView->render(&painter);}

void IndigentsWindow::on_selectImage_3_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, tr("Select Image File"), "", tr("Image Files (*.png *.jpg *.bmp)"));
        if (!filePath.isEmpty()) {
            ui->lineEdit_imagePath_3->setText(filePath);
            QPixmap imagePixmap(filePath);
            if (!imagePixmap.isNull())
            {
                ui->label_image_3->setPixmap(imagePixmap);
                ui->label_image_3->setScaledContents(true); // Pour redimensionner l'image pour s'adapter au label
            }
        }
}

void IndigentsWindow::on_insertImage_3_clicked()
{
    int id = ui->id_inserer_3->text().toInt();
    indigent indigentObject;
    QSqlQuery query;
    QString res=QString ::number(id);

    query.prepare("UPDATE INDIGENT SET ImagePath = :1 where ID_INDIGENT=:id");
    query.bindValue(":1",filePath);
    query.bindValue(":id",res);
    bool test = query.exec();
    if (test){
         ui->tableView->setModel(i.afficher());
        QMessageBox::information(nullptr, QObject::tr(" oui "),
                    QObject::tr("connection successful.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else{
        QMessageBox::critical(nullptr, QObject::tr(" non "),
                    QObject::tr("connection failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
    }
}

void IndigentsWindow::on_tabWidget_currentChanged(int index)
{
    ui->label_stat->clear();
    if (index == 2 )
    {
        QLayoutItem* child;
        while ((child = ui->verticalLayout_2->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }

        // Requête SQL pour récupérer les données
        QSqlQuery query("SELECT COUNT(*) FROM INDIGENT WHERE sexe='Femme'");
        query.next();
        int nbFemmes = query.value(0).toInt();
        query.exec("SELECT COUNT(*) FROM  INDIGENT WHERE sexe='Homme'");
        query.next();
        int nbHommes = query.value(0).toInt();

        // Calcul des pourcentages
        qreal total = nbFemmes + nbHommes;
        qreal pourcentageFemmes = nbFemmes / total * 100;
        qreal pourcentageHommes = nbHommes / total * 100;

        // Création des données pour le graphique en bâtons
        QBarSet *setFemmes = new QBarSet("Femme");
        *setFemmes << nbFemmes;

        QBarSet *setHommes = new QBarSet("Homme");
        *setHommes << nbHommes;

        QBarSeries *series = new QBarSeries();
        series->append(setFemmes);
        series->append(setHommes);

        // Création du graphique
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Répartition des INDIGENTS selon le sexe");
        chart->createDefaultAxes();

        // Personnalisation de l'axe X
        QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
        axisX->setTitleText("Sexe");
        axisX->append("Femme");
        axisX->append("Homme");

        // Personnalisation de l'axe Y
        QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
        axisY->setLabelFormat("%.0f");
        axisY->setTitleText("Nombre de INDIGENT");
        axisY->setRange(0, total + 1);
        axisY->setTickCount(total + 2);

        // Création de la vue du graphique
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        // Ajout de la vue dans la fenêtre principale
        ui->verticalLayout_2->addWidget(chartView);

        // Affichage des pourcentages dans un QLabel
        ui->label_stat->setText(QString("Femme : %1 (%2 %)\nHomme : %3 (%4 %)")
                                   .arg(nbFemmes)
                                   .arg(QString::number(pourcentageFemmes, 'f', 2))
                                   .arg(nbHommes)
                                   .arg(QString::number(pourcentageHommes, 'f', 2)));


    }

}
