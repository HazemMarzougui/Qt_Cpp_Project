#include "logementswindow.h"
#include "ui_logementswindow.h"
#include <QPixmap>
#include <QtWidgets>
#include"logement.h"
#include <QMessageBox>
#include"QtPrintSupport/QPrinter"
#include"QPdfWriter"
#include <QTextBrowser>
#include <QPainter>
#include <QPrinter>
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


LogementsWindow::LogementsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LogementsWindow)
{
    ui->setupUi(this);
    //controle de saisie id_logement
    ui->lineEdit_2->setValidator(new QIntValidator(0,1000,this));
     ui->lineEdit_id_2->setValidator(new QIntValidator(0,1000,this));

    //controle de saisie adresse
    QLineEdit* lineEdit = new QLineEdit;
    lineEdit->setMaxLength(8);
    ui->lineEdit->setMaxLength(8);
 ui->lineEdit_3->setMaxLength(5);
 //controle de saisie apres modif
 ui->lineEdit_adresse_2->setMaxLength(8);
ui->lineEdit_condition_2->setMaxLength(5);
//controle de saisie pour etat
QRegExpValidator *validator = new QRegExpValidator(QRegExp("^(occup|dispo)$"), this);
ui->lineEdit_3->setValidator(validator);
QRegExpValidator *validator1 = new QRegExpValidator(QRegExp("^(occup|dispo)$"), this);
ui->lineEdit_condition_2->setValidator(validator1);





    //appel affichage
    ui->tableView_list->setModel(Etmp.afficher());
    //appel tri
     ui->tableView_list->setModel(Etmp.triadresse());

}

LogementsWindow::~LogementsWindow()
{
    delete ui;
}
void LogementsWindow::on_pushButton_3_clicked()//save
{
int ID_LOGEMENT=ui->lineEdit_2->text().toInt();
QString ETAT=ui->lineEdit_3->text();//recupération des données
QString ADRESSE=ui->lineEdit->text();
// Vérification que les champs "etat" et "adresse" ne contiennent que des alphabets
QRegExp regExp("^[A-Za-z ]+$");
if (!ETAT.contains(regExp) || !ADRESSE.contains(regExp)) {
    // Afficher un message d'erreur à l'utilisateur
    QMessageBox::warning(this, "Erreur", "Veuillez saisir des alphabets seulement dans les champs adresse et etat.");
    return;
}

logement L(ID_LOGEMENT,ADRESSE,ETAT);
bool test=L.ajouter();
if(test)
{
    ui->tableView_list->setModel(Etmp.afficher());
    ui->tableView_list->setModel(Etmp.triadresse());
    QMessageBox::information(nullptr,QObject::tr("ok"),
                             QObject::tr("ajout effectue\n"
                                         "click cancel to exit."),QMessageBox::Cancel);
}
else
    QMessageBox::critical(nullptr,QObject::tr("not ok"),
                          QObject::tr("ajout non effectue.\n "
                                      "click cancel to exit."), QMessageBox::Cancel);
ui->lineEdit_2->clear();
    ui->lineEdit->clear();
    ui->lineEdit_3->clear();
}

void LogementsWindow:: on_Button_Delete_clicked()
{
    logement l;
        // Obtenez les données de cette cellule dans le modèle de données
        QVariant rowData = ui->tableView_list->model()->data(ui->tableView_list->currentIndex());

        // Affichez les données de la ligne sélectionnée dans la console
        qDebug() << "La ligne sélectionnée est : " << rowData.toString();

        bool test=l.supprimer(rowData.toInt());

        if(test)
        {

            QMessageBox::information(nullptr, QObject::tr("ok"),
                        QObject::tr("Suppression effectuee.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);
            ui->tableView_list->setModel(l.afficher());
            ui->tableView_list->setModel(Etmp.triadresse());

        }
        else
            QMessageBox::critical(nullptr, QObject::tr("not ok"),
                        QObject::tr("Suppression non effectuee.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);
}

void LogementsWindow::on_Button_update_clicked()
{


        QModelIndexList selectedList=ui->tableView_list->selectionModel()->selectedRows();
        if(!(selectedList.isEmpty())){
            ui->lineEdit_id_2->setText(ui->tableView_list->model()->data(ui->tableView_list->model()->index(selectedList.at(0).row(),0)).toString());
                    ui->lineEdit_adresse_2->setText(ui->tableView_list->model()->data(ui->tableView_list->model()->index(selectedList.at(0).row(),1)).toString());
                    ui->lineEdit_condition_2->setText(ui->tableView_list->model()->data(ui->tableView_list->model()->index(selectedList.at(0).row(),2)).toString());


    }
        else {
            QMessageBox::information(nullptr, QObject::tr("ok"),
                        QObject::tr("Select row to Update please!!.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);
    }
    }
void LogementsWindow::on_pushButton_4_clicked()//save apres modif
{ int ID_LOGEMENT=ui->lineEdit_id_2->text().toInt();
    QString ADRESSE=ui->lineEdit_adresse_2->text();
    QString ETAT=ui->lineEdit_condition_2->text();
    // Vérification que les champs "etat" et "adresse" ne contiennent que des alphabets
    QRegExp regExp("^[A-Za-z ]+$");

    if (!ETAT.contains(regExp) || !ADRESSE.contains(regExp)) {
        // Afficher un message d'erreur à l'utilisateur
        QMessageBox::warning(this, "Erreur", "Veuillez saisir des alphabets seulement.");
        return;
    }

    logement L(ID_LOGEMENT,ADRESSE,ETAT);
    bool test=L.modifier_logement(ID_LOGEMENT);
    if(test)
    {//actualiser
        ui->tableView_list->setModel(Etmp.afficher());

        //appel tri
        ui->tableView_list->setModel(Etmp.triadresse());
        QMessageBox::information(nullptr,QObject::tr("ok"),
                                 QObject::tr("modification effectue\n"
                                             "click cancel to exit."),QMessageBox::Cancel);
    }
    else
        QMessageBox::critical(nullptr,QObject::tr("not ok"),
                              QObject::tr("modification non effectue.\n "
                                          "click cancel to exit."), QMessageBox::Cancel);
    ui->lineEdit_id_2->clear();
        ui->lineEdit_adresse_2->clear();
        ui->lineEdit_condition_2->clear();
}
//metiers
void LogementsWindow::on_lineEdit_Search_textChanged(const QString &arg1)
{
        ui->tableView_list->setModel(Etmp.recherche_logement(arg1));


}
void LogementsWindow::on_pushButton_pdf_clicked()
{
   Etmp.exporterpdf(ui->textBrowser1);
}
void LogementsWindow::on_pushButton_somme_clicked()
{
    QString etatDISPO = "dispo";
    int countDISPO = Etmp.calculerSommeEtat(etatDISPO);
    QString messageDISPO = QString("Nombre de logements ayant l'état %1 : %2").arg(etatDISPO).arg(countDISPO);

    QString etatOCCUP = "occup";
    int countOCCUP = Etmp.calculerSommeEtat(etatOCCUP);
    QString messageOCCUP = QString("Nombre de logements ayant l'état %1 : %2").arg(etatOCCUP).arg(countOCCUP);

    // Ajouter les résultats au QTextEdit
    ui->textEdit0->append(messageDISPO);
    ui->textEdit0->append(messageOCCUP);
}
void LogementsWindow::on_stat_clicked()
{
    QSqlQuery q1,q2,q3; //requette
    qreal tot=0,c1=0,c2=0;

    q1.prepare("SELECT * FROM LOGEMENT");
    q1.exec(); //La requête récupère toutes les lignes de la table,

    q2.prepare("SELECT * FROM LOGEMENT WHERE ADRESSE='tunis'");
    q2.exec();

    q3.prepare("SELECT * FROM LOGEMENT WHERE ADRESSE='sfax'");
    q3.exec();
    // les deux autres requêtes récupèrent uniquement les lignes où le champ "adresse" est égal à "tunis" et "sfax",


    while (q1.next()){tot++;}//compter nbr totale logement
    while (q2.next()){c1++;}//compter nbr tunis
    while (q3.next()){c2++;}//compter nbr sfax
    // la fonction utilise une boucle while pour compter le nombre total logement et le nombre logements dans chaque catégorie d'adresse.

    c1=c1/tot;
    c2=c2/tot; //calculer les pourcentages

    QPieSeries *series = new QPieSeries();//QPieSeries pour stocker les données du graphique circulaire

       QString tunisStr = QString("tunis (%1%)").arg(QString::number(c1*100, 'f', 2));
       QPieSlice *slice1 = series->append(tunisStr,c1);

       slice1->setColor(QColor(255,165,0)); // set orange color for the "bien" slice

       QString sfaxStr = QString("sfax (%1%)").arg(QString::number(c2*100, 'f', 2));
       QPieSlice *slice2 = series->append(sfaxStr,c2);

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
QSqlQueryModel* LogementsWindow::afficherHistorique(QString query)
{

        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery(query);
        ui->listView_historique->setModel(model);
        return model;
    }
void LogementsWindow::on_historique_clicked()
{
    QString query = "SELECT ACTIVITE FROM HISTORIQUE";
    QSqlQueryModel *model = afficherHistorique(query);
    ui->listView_historique->setModel(model);
}







