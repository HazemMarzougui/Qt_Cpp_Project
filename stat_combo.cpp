//#include "stat_combo.h"
//#include "ui_stat_combo.h"

//stat_combo::stat_combo(QWidget *parent) :
  //  QMainWindow(parent),
    //ui(new Ui::stat_combo)
//{
  // ui->setupUi(this);
//}




//stat_combo::~stat_combo()
//{
    //delete ui;
//}


//pie chart
//void stat_combo::on_stat_clicked()
    //{
   // QSqlQuery q1,q2,q3; //requette
   // qreal tot=0,c1=0,c2=0;

    //q1.prepare("SELECT * FROM LOGEMENT");
    //q1.exec(); //La requête récupère toutes les lignes de la table,

    //q2.prepare("SELECT * FROM GESTION_EMPLOYES WHERE ADRESSE='tunis'");
    //q2.exec();


  // q3.prepare("SELECT * FROM GESTION_EMPLOYES WHERE ADRESSE='sfax'");
    //q3.exec();
// les deux autres requêtes récupèrent uniquement les lignes où le champ "ROLE" est égal à "mecanicien" et "technicien",


   // while (q1.next()){tot++;}//compter nbr totale employe
    //while (q2.next()){c1++;}//compter nbr mecanicien
    //while (q3.next()){c2++;}//compter nbr technicien
// la fonction utilise une boucle while pour compter le nombre total d'employés et le nombre d'employés dans chaque catégorie de rôles.

    //c1=c1/tot;
    //c2=c2/tot; //calculer les pourcentages

//Définir les tranches et le pourcentage de l'ensemble qu'elles occupent
    //QPieSeries *series = new QPieSeries();//QPieSeries pour stocker les données du graphique circulaire
    //series->append("tunis",c1);
    //series->append("sfax",c2);
//La fonction utilise ensuite les classes QPieSeries et QChart de Qt pour créer le graphique circulaire.


    // Create the chart(graphique) widget
    //QChart *chart = new QChart();

    // Add data to chart with title and show legend
    //chart->addSeries(series);
    //chart->legend()->show();


    // Used to display the chart
    //chartView = new QChartView(chart,ui->graphicsView);
    //chartView->setRenderHint(QPainter::Antialiasing);//améliorer la qualité de l'affichage.
    //chartView->setMinimumSize(700,500);

        //chartView->show();
        //}
