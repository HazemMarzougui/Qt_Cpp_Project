#ifndef EMPLOYE_H
#define EMPLOYE_H
#include<QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

using namespace std;

class employe
{
private:
    int id;
    QString nom;
    QString prenom;
    QString service;
    QString email;
    QString mdp;
public:

    employe();
    employe (int ,QString  ,QString  ,QString  ,QString  ,QString );
    int getId();
    QString getnom();
    QString getprenom();
    QString getservice();
    QString getemail();
    QString getmdp();
    void setId(int d);
    void setnom(QString n);
    void setprenom(QString p);
    void setemail(QString e);
    void setmdp(QString m);
    void setservice(QString s);
    bool ajouter();
    bool supprimer(int id);
    employe Login(QString email,QString mdp);
    employe Loginarduino(int);
    employe findEmployerById(int id );
    bool update(int id,QString nom,QString prenom,QString service,QString email,QString mdp);
    QSqlQueryModel *trier();
    QSqlQueryModel * recherche(QString variable);
    QSqlQueryModel  *Afficher();
     QMap<QString, int> nombreDesemployeParService();
vector<employe> ListeDesEmployes(int );
};

#endif // EMPLOYE_H
