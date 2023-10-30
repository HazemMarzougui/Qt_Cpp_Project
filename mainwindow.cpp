#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fournisseurwindow.h"
#include "ui_fournisseurwindow.h"
#include"employe.h"
#include "QMessageBox"
#include<QDebug>
#include <QPoint>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QTextDocument>
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QtCharts>
#include <QPieSeries>
#include<QPainter>
#include <random>
#include "sessionuser.h"
#include "chat.h"
#include "arduino.h"

#include <QByteArray>
#include <QDebug>

Arduino a ;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    selectedId=-1;
    int ret=a.connect_arduino(); // lancer la connexion à arduino
        switch(ret){
        case(0):qDebug()<< "arduino is available and connected to : "<< a.getarduino_port_name();
            break;
        case(1):qDebug() << "arduino is available but not connected to :" <<a.getarduino_port_name();
           break;
        case(-1):qDebug() << "arduino is not available";
        }
         QObject::connect(a.getserial(),SIGNAL(readyRead()),this,SLOT(update_labelmemploye())); // permet de lancer
         //le slot update_label suite à la reception du signal readyRead (reception des données).


}

MainWindow::~MainWindow()
{
    delete ui;
}

 void MainWindow::update_labelmemploye()
 {
     QByteArray data = a.read_from_arduino();

     qDebug() << data ;
     if (data == "3") {
        qDebug() << data.toInt();
         employe e;
         employe resultat = e.Loginarduino(64);
         qDebug() << "employe : " << resultat.getId();
         if (resultat.getId() == -1) {
             QMessageBox::warning(this, "erreur", "La connexion avec la carte Arduino a échoué");
         }
         else {
             sessionuser::getInstance()->setUser(resultat);
             setdata();

         }
     }
 }

void MainWindow::on_pushButton_clicked()
{


     QString nom=ui->inputname->text();
     QString prenom=ui->inputlastname->text();
     QString service=ui->service->text();
     QString email=ui->email->text();
     QString mdp=ui->mdp->text();
     QString name=ui->inputname->text();
     QString confirmermpd=ui->inputcmdp->text();
     QString last_name=ui->inputlastname->text();
     QRegularExpression regex("[^a-zA-Z]");
     if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || mdp.isEmpty() || confirmermpd.isEmpty() || service.isEmpty() )
     {
          a.write_to_arduino("0");
         QMessageBox::warning(this, "empty ", "check all fields ");

     }
     else if (regex.match(name).hasMatch()) {
          a.write_to_arduino("0");
           QMessageBox::warning(this, "Invalid First Name", " First Name must contain only alphabetic letters.");

         return;
     }
    else if (regex.match(last_name).hasMatch()) {
          a.write_to_arduino("0");
           QMessageBox::warning(this, "Invalid Last Name", " Last Name must contain only alphabetic letters.");
         return;
     }
    else if(mdp.compare(confirmermpd)!=0){
          a.write_to_arduino("0");
            QMessageBox::warning(this, tr("Warning"),
                                        tr("champ mot de passe et confirmer mot de passe doit etre indentique "),
                                        QMessageBox::Save);


     }




     else{

        // QString salt = QtBCrypt::generateSalt();
        // QString hashedPassword = QtBCrypt::hashPassword(mdp, salt);
     //Inscription - sing-up
     employe e(0,nom,prenom,service,email,mdp);
     e.ajouter();
       a.write_to_arduino("1");  // Success message
     QMessageBox::information(this, tr("Added"),
                                    tr("l'employe a éte bien ajouté"),
                                    QMessageBox::Save);




         // Echec message

     ui->inputname->clear();
     ui->inputlastname->clear();
     ui->service->clear();

    ui->email->clear();
     ui->mdp->clear();
     ui->inputcmdp->clear();

     setdata();





}



}
void MainWindow::remplirListeEmploye(){
    employe e;
    vector<employe>listeusers;

    /*QPushButton{
QPushButton:hover
{

}
*/

    listeusers=e.ListeDesEmployes(sessionuser::getInstance()->getUser().getId());
    for (vector<employe>::iterator it= listeusers.begin(); it!=listeusers.end();++it) {
        QPushButton *button = new QPushButton(it->getnom(), this);
        button->setStyleSheet("QPushButton {\
                                    position: relative;\
                                    background-color: #555555;\
                                    color: #ffffff;\
                                    border: none;\
                                    border-radius: 5px;\
                                    padding: 10px 20px 10px 40px;\
                                    text-align: center;\
                                    text-decoration: none;\
                                    display: inline-block;\
                                    font-size: 14px;\
                                    font-weight: bold;\
                                    transition: all 0.2s ease-in-out;\
                                }\
                                QPushButton:hover {\
                                    background-color: #888888;\
                                    cursor: pointer;\
                                }\
                                QPushButton:focus {\
                                    outline: none;\
                                }\
                                QPushButton:pressed {\
                                    background-color: #333333;\
                                    box-shadow: 0px 0px 5px rgba(0, 0, 0, 0.8);\
                                }\
                                QPushButton::before {\
                                    content: '';\
                                    position: absolute;\
                                    top: 50%;\
                                    left: 30px;\
                                    transform: translateY(-50%);\
                                    width: 10px;\
                                    height: 10px;\
                                    border-radius: 50%;\
                                    background-color: #00ff00;\
                                }");
        employe e=(*it);
        connect(button, &QPushButton::clicked, this, [this,e]() {
                   listenerslotButtonClicked(e);
               });

        QWidget* widget = new QWidget(); // Créer un widget pour contenir le bouton
               QVBoxLayout* layout = new QVBoxLayout(); // Créer un layout vertical pour le widget
               layout->addWidget(button); // Ajouter le bouton au layout vertical
               widget->setLayout(layout); // Définir le layout vertical comme layout du widget
               ui->listeUsers->addWidget(widget);

    }

}
void MainWindow::setdata(){
    ui->tabWidget->clear();

    if(sessionuser::getInstance()->getUser().getId()==0){
        ui->logout->hide();
        ui->tabWidget->insertTab(0,ui->tab_CreateEmp,"Sing up");
        ui->tabWidget->insertTab(1,ui->tab_Cnx,"Login");



    }
    else{
        ui->logout->show();
        qDebug() << sessionuser::getInstance()->getUser().getservice() ;
        if(sessionuser::getInstance()->getUser().getservice()=="administrative"){
            ui->tabWidget->insertTab(0,ui->tab_EmpManagment,"Gestion Des employes ");
            ui->tabWidget->insertTab(1,ui->tab_Statistics,"Statistiques");
           ui->tabWidget->insertTab(2,ui->tab_chat,"Discussion");
           ui->tabWidget->insertTab(3,ui->formulareemploye,"Forumlaire Employé ");
           remplirListeEmploye();

        }else if(sessionuser::getInstance()->getUser().getservice()=="medical"){
                ui->tabWidget->insertTab(0,ui->testmedicament,"test interface medicament");
                ui->tabWidget->insertTab(1,ui->tab_chat,"Discussion");
                remplirListeEmploye();

        }

    }
    employe e;
   ui->listeEmploye->setModel(e.Afficher());
       QMap<QString, int> nombreDesEmployesParService = e.nombreDesemployeParService();

       // Create a new pie series
       QPieSeries *series = new QPieSeries();

       // Add slices to the series for each service and its corresponding employee count
       for (auto it = nombreDesEmployesParService.begin(); it != nombreDesEmployesParService.end(); ++it) {
           QString service = it.key();
           int nombre = it.value();

           // Create a new pie slice for this service and add it to the series
           series->append(service + " "+QString::number(nombre), nombre);


       }

       // Create a chart view and add the series to it
       QChartView *view = new QChartView();
       view->setRenderHint(QPainter::Antialiasing);
       view->chart()->addSeries(series);
       view->chart()->setTitle("Nombre d'employés par service");
       view->chart()->legend()->setVisible(true);
       view->chart()->legend()->setAlignment(Qt::AlignBottom);
       QVBoxLayout *layout = new QVBoxLayout();
       layout->addWidget(view);
      // ui->statwidget->setLayout(layout);




}

void MainWindow::on_listeEmploye_clicked(const QModelIndex &index)
{

    QString data = index.data(Qt::DisplayRole).toString();

        // Récupérer le modèle de données associé
        QAbstractItemModel *model = ui->listeEmploye->model();

        // Récupérer l'indice de la première colonne de la ligne sélectionnée
        QModelIndexList selectedIndexes = ui->listeEmploye->selectionModel()->selectedIndexes();
        QModelIndex firstIndex = selectedIndexes.first();
        int row = firstIndex.row();
        QModelIndex firstColumnIndex = model->index(row, 0);

        QMap<int, QVariant> rowData = model->itemData(firstColumnIndex);
        int id=rowData[0].toInt();
        qInfo()<<id;

        ui->deletebutton->setDisabled(false);
        ui->updatebutton->setDisabled(false);
        selectedId=id;


;




}

void MainWindow::on_deletebutton_clicked()
{
    if(selectedId!=-1)
    {  employe e;
    e.supprimer(selectedId);
    ui->listeEmploye->setModel(e.Afficher());


    }
}

void MainWindow::on_updatebutton_clicked()
{
    employe e;
    employe find=e.findEmployerById(selectedId);
    ui->tabWidget->setCurrentIndex(3);
    ui->fe_name->setText(find.getnom());
    ui->fe_lastname->setText(find.getprenom());
    ui->fe_service->setText(find.getservice());
    ui->fe_email->setText(find.getemail());
    etatformulaire="edit";




}





void MainWindow::on_pdfinput_clicked()
{



        QSqlDatabase db;
                    QTableView listemploye;
                    QSqlQueryModel * Modal=new  QSqlQueryModel();

                    QSqlQuery qry;
                     qry.prepare("SELECT * FROM EMPLOYE ");
                     qry.exec();
                     Modal->setQuery(qry);
                     listemploye.setModel(Modal);

                     db.close();

                     QString strStream;
                     QTextStream out(&strStream);

                     const int rowCount = listemploye.model()->rowCount();
                     const int columnCount =  listemploye.model()->columnCount();

                     const QString strTitle ="Document";


                     out <<  "<html>\n"
                             "<img src='C:/Users/ASUS/Documents/kiosqueK/logocin.png' height='120' width='120'/>"
                         "<head>\n"
                             "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                         <<  QString("<title>%1</title>\n").arg(strTitle)
                         <<  "</head>\n"
                         "<body bgcolor=#ffffff link=#5000A0>\n"
                        << QString("<h3 style=\" font-size: 50px; font-family: Arial, Helvetica, sans-serif; color: #e80e32; font-weight: lighter; text-align: center;\">%1</h3>\n").arg("Liste des employes")
                        <<"<br>"

                        <<"<table border=1 cellspacing=0 cellpadding=2 width=\"100%\">\n";
                     out << "<thead><tr bgcolor=#f0f0f0>";
                     for (int column = 0; column < columnCount; column++)
                         if (!listemploye.isColumnHidden(column))
                             out << QString("<th>%1</th>").arg(listemploye.model()->headerData(column, Qt::Horizontal).toString());
                     out << "</tr></thead>\n";

                     for (int row = 0; row < rowCount; row++) {
                         out << "<tr>";
                         for (int column = 0; column < columnCount; column++) {
                             if (!listemploye.isColumnHidden(column)) {
                                 QString data = listemploye.model()->data(listemploye.model()->index(row, column)).toString().simplified();
                                 out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                             }
                         }
                         out << "</tr>\n";
                     }
                     out <<  "</table>\n"
                             "<br><br>"
                             <<"<br>"
                             <<"<table border=1 cellspacing=0 cellpadding=2>\n";


                         out << "<thead><tr bgcolor=#f0f0f0>";

                             out <<  "</table>\n"

                         "</body>\n"
                         "</html>\n";

                     QTextDocument *document = new QTextDocument();
                     document->setHtml(strStream);

                     QPrinter printer;
                     QPrintDialog *dialog = new QPrintDialog(&printer, NULL);
                     if (dialog->exec() == QDialog::Accepted) {

                         document->print(&printer);
                     }

                     printer.setOutputFormat(QPrinter::PdfFormat);
                     printer.setPaperSize(QPrinter::A4);
                     printer.setOutputFileName("/tmp/listemploye.pdf");
                     printer.setPageMargins(QMarginsF(15, 15, 15, 15));
                     // Ajout de l'image à droite de la page
                     QPainter painter(&printer);
                     QString htmlText = "<span style='color: orange; font-size: 20px; font-weight: bold;'>employees FILES</span><br><br>";
                           QPixmap img("C:\\Users\\Molka\Desktop\\Interface Qt\Employees\\Icons\\ApplicationHomeShare.png");
                           int imgWidth = printer.pageRect().width() / 4;
                           int imgHeight = imgWidth * img.height() / img.width();
                           painter.drawPixmap(QRect(printer.pageRect().width() - imgWidth, 0, imgWidth, imgHeight), img);
                                 QMessageBox::information(nullptr, QObject::tr("Export PDF"),
                                                           QObject::tr("Export avec succes .\n"
                                                                       "Click OK to exit."), QMessageBox::Ok);

                     delete document;


}


void MainWindow::on_sortinput_clicked()
{
    employe e;
        ui->listeEmploye->setModel(e.trier());
}





void MainWindow::on_searchinput_textChanged(const QString &arg1)
{
       employe e;
            ui->listeEmploye->setModel(e.recherche(ui->searchinput->text()));


}



void MainWindow::on_loginbutton_clicked()
{

        if(ui->emailcnx->text() == "" || ui->mdp_cnx->text() == "") {
            QMessageBox::warning(this, "erreur", "Champ email et champ mot de passe est obligatoire");
        }
        else {
            employe e;
            employe resultat = e.Login(ui->emailcnx->text(), ui->mdp_cnx->text());
            if (resultat.getId() == -1) {
                QMessageBox::warning(this, "erreur", "Email ou mot de passe incorrect");
            }
            else {
                sessionuser::getInstance()->setUser(resultat);
                setdata();

            }
        }

}

void MainWindow::on_logout_clicked()
{
    employe e;
    e.setId(0);
    sessionuser::getInstance()->setUser(e);
setdata();

}
void MainWindow::listenerslotButtonClicked(employe e) {
    ui->nomre->setText(e.getnom() + " " + e.getprenom());
    endusavec=e.getId();
    remplirMsg();

    ui->sendMessage->setEnabled(true);

}
void MainWindow::clearMSG()
{
    // Remove all widgets from the layout
    QLayoutItem *child;
    while ((child = ui->listeMsg->takeAt(0)) != nullptr)
    {
        QWidget *widget = child->widget();
        if (widget)
        {
            ui->listeMsg->removeWidget(widget);
            delete widget;
        }
    }
}
void MainWindow::remplirMsg(){


    clearMSG();

    chat c;
    int connectedId=sessionuser::getInstance()->getUser().getId();

    vector<chat> listedesmsg = c.getMessages(connectedId,endusavec);
    for (auto it= listedesmsg.begin(); it!=listedesmsg.end();++it) {
        if(it->getidSender()==connectedId){
            qDebug()<<"enter as sender";

            QLabel *l = new QLabel();
            l->setText(it->getMg());
            l->setStyleSheet("font-family: Arial; font-size: 16px; background-color: blue; color: white; border: 1px solid black;");


            QWidget* widget = new QWidget(); // Créer un widget pour contenir le bouton
                   QVBoxLayout* layout = new QVBoxLayout(); // Créer un layout vertical pour le widget
                   layout->addWidget(l); // Ajouter le bouton au layout vertical
                   widget->setLayout(layout); // Définir le layout vertical comme layout du widget
                   ui->listeMsg->addWidget(widget);

        }else{
            QLabel *l = new QLabel();
            l->setText(it->getMg());
            l->setStyleSheet("color: black;");

            QWidget* widget = new QWidget(); // Créer un widget pour contenir le bouton
                   QVBoxLayout* layout = new QVBoxLayout(); // Créer un layout vertical pour le widget
                   layout->addWidget(l); // Ajouter le bouton au layout vertical
                   widget->setLayout(layout); // Définir le layout vertical comme layout du widget
                   ui->listeMsg->addWidget(widget);
                   qDebug()<<"enter as rec";


        }


}


}
void MainWindow::on_sendMessage_clicked()
{
    if(ui->textmsg->text()==""){
        QMessageBox::warning(this, "erreur", "champ message ");

    }else{
    chat c(sessionuser::getInstance()->getUser().getId(),endusavec,ui->textmsg->text());
    c.sendMessage();
        ui->textmsg->clear();
    remplirMsg();



    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QString nom=ui->fe_name->text();
    QString prenom=ui->fe_lastname->text();
    QString service=ui->fe_service->text();
    QString email=ui->fe_email->text();
    QString mdp=ui->fe_password->text();
    QString confirmermpd=ui->fe_confirm_password->text();
    QRegularExpression regex("[^a-zA-Z]");
    //formulaire employe

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || mdp.isEmpty() || confirmermpd.isEmpty() || service.isEmpty() )
    {
         a.write_to_arduino("0");
        QMessageBox::warning(this, "empty ", "check all fields ");

    }
    else if (regex.match(nom).hasMatch()) {
         a.write_to_arduino("0");
          QMessageBox::warning(this, "Invalid First Name", " First Name must contain only alphabetic letters.");

        return;
    }
   else if (regex.match(prenom).hasMatch()) {
         a.write_to_arduino("0");
          QMessageBox::warning(this, "Invalid Last Name", " Last Name must contain only alphabetic letters.");
        return;
    }
   else if(mdp.compare(confirmermpd)!=0){
         a.write_to_arduino("0");
           QMessageBox::warning(this, tr("Warning"),
                                       tr("champ mot de passe et confirmer mot de passe doit etre indentique "),
                                       QMessageBox::Save);


    }




    else{


    if(etatformulaire=="add"){
    employe e(0,nom,prenom,service,email,mdp);
    e.ajouter();
      a.write_to_arduino("1");  // Success message
    QMessageBox::information(this, tr("Added"),
                                   tr("l'employe a éte bien ajouté"),
                                   QMessageBox::Save);
    }else{
        employe e2;
        if( e2.update(selectedId,nom,prenom,service,email,mdp)==true){
                QMessageBox::information(this, tr("updated"),
                                               tr("l'employe a éte bien modifié"),
                                         QMessageBox::Save);



                    }


     etatformulaire="add";
    }
    ui->inputname->clear();
    ui->inputlastname->clear();
    ui->service->clear();

   ui->email->clear();
    ui->mdp->clear();
    ui->inputcmdp->clear();

    setdata();


    }
}

void MainWindow::on_Menu_clicked()
{
    menu=new MainWindow1(this);
    menu->show();
}
