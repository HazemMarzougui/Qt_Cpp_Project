#ifndef INDIGENT_H
#define INDIGENT_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QMessageBox>
#include <QDate>
#include <QTextBrowser>
#include <QFileDialog>
#include <QtPrintSupport/QPrinter>
#include <QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QTableView>
class indigent
{
private:
    int id_indigent;
    QString nom;
    QString prenom;
    QDate dob;
    QString sexe;

    public:
    indigent();
    indigent(int,QString,QString,QDate,QString);
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer (int );
    bool modifier(int );
    QSqlQueryModel *Recherche(QString );
    QSqlQueryModel *trisexe();
    int  getid();
    QString getnom();
    QString getprenom();
    QDate  getdate();
    QString  getsexe();
    void setid(int);
    void setnom(QString);
    void setprenom(QString );
    void setdate(QDate);
    void setsexe(QString);
    QSqlQueryModel* triSexe();
    QSqlQueryModel* triDob();
    void exporterpdf(QTextBrowser *text);
    bool insertImageToDatabase(const QString& imagePath, const QString& imageName, const int& id);
     int alerte();
     void statistiqueSegment();
     void imprimer(QTextEdit* widget);




};
#endif // INDIGENT_H
