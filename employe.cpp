#include "employe.h"
#include<QString>
#include<QDebug>
employe::employe()
{

}
int employe::getId()
{
return id;}
QString employe::getnom()
{
    return nom;
}
QString employe::getprenom()
{
return prenom;}
QString employe::getservice()
{
 return service;}
QString employe::getemail()
{return email;}
QString employe::getmdp(){
    return mdp;
}
void employe::setId(int d)
{ id=d;
}
void employe::setnom(QString n)
{nom=n;
}
void employe::setprenom(QString p)
{
    prenom=p;
}
void employe::setemail(QString e)
{
email=e;}
void employe::setmdp(QString m)
{
mdp=m;}
void employe::setservice(QString s)
{
    service=s;
}

bool employe::ajouter(){
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYE ( NOM, PRENOM, SERVICE, EMAIL,MDP) VALUES ( :nom, :prenom, :service, :email,:mdp)");

    query.bindValue(":nom",this->nom);
     query.bindValue(":prenom",this->prenom);
      query.bindValue(":service",this->service);
 query.bindValue(":email",this->email);
 query.bindValue(":mdp",this->mdp);
 return query.exec();


}
bool employe::supprimer(int id){
    QSqlQuery query;
    QString res=QString::number(id);
    query.prepare("Delete from EMPLOYE where ID=:id");
    query.bindValue(":id",res);
    return  query.exec();

}
 bool employe::update(int id, QString nom, QString prenom, QString service, QString email, QString mdp)
 {
     QSqlQuery query;
         query.prepare("UPDATE EMPLOYE SET NOM=:nom, PRENOM=:prenom, EMAIL=:email , SERVICE=:service, MDP=:mdp WHERE ID=:id");
         query.bindValue(":nom", nom);
         query.bindValue(":prenom", prenom);
         query.bindValue(":service", service);
         query.bindValue(":email", email);
         query.bindValue(":mdp", mdp);
         query.bindValue(":id", id);
         return query.exec();


 }
employe employe::findEmployerById(int id){
    employe emp;
        QSqlQuery query;
        query.prepare("SELECT * from EMPLOYE WHERE id = ?");
        query.addBindValue(id);
        if (query.exec() && query.first()) {
            emp.setId(query.value("ID").toInt());
            emp.setnom(query.value("NOM").toString());
            emp.setprenom(query.value("PRENOM").toString());
            emp.setemail(query.value("EMAIL").toString());
            emp.setmdp(query.value("MDP").toString());
            emp.setservice(query.value("SERVICE").toString());

        }
        return emp;
}
QSqlQueryModel * employe::Afficher(){
    QSqlQueryModel * m=new QSqlQueryModel();
    m->setQuery("SELECT * from EMPLOYE");
    m->setHeaderData(0,Qt::Horizontal,QObject::tr("ID"));
    m->setHeaderData(1,Qt::Horizontal,QObject::tr("NOM"));

    m->setHeaderData(2,Qt::Horizontal,QObject::tr("PRENOM"));
    m->setHeaderData(3,Qt::Horizontal,QObject::tr("SERVICE"));
    m->setHeaderData(4,Qt::Horizontal,QObject::tr("EMAIL"));
    m->setHeaderData(5,Qt::Horizontal,QObject::tr("MDP"));


    return m;

}
employe:: employe (int id ,QString n  ,QString  p,QString s ,QString e ,QString f)
{
   id=0;
   nom=n;
   prenom=p;
   service=s;
   email=e;
   mdp=f;
}
QSqlQueryModel * employe::trier()
{
    QSqlQueryModel * model= new QSqlQueryModel();
    model->setQuery("SELECT * FROM EMPLOYE order by ID  asc");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("SERVICE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("MDP"));

        return model;
}

QSqlQueryModel * employe::recherche(QString variable)

{

    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;



    query.prepare("SELECT * FROM EMPLOYE WHERE NOM like :variable or PRENOM like :variable or SERVICE like :variable or EMAIL like :variable ");

    query.bindValue(":variable", "%"+variable+"%");

    query.exec();



    model->setQuery(query);

    return model;

}
QMap<QString, int> employe::nombreDesemployeParService()

{


        QMap<QString, int> result;

        QSqlQuery query;
        query.exec("SELECT SERVICE, COUNT(*) as employe_count FROM EMPLOYE GROUP BY SERVICE");

        while (query.next()) {
            QString service = query.value(0).toString();
            int employe_count = query.value(1).toInt();
            result[service] = employe_count;

        }

        return result;


}
employe employe::Login(QString email,QString mdp){
    employe emp;
    emp.setId(-1);
        QSqlQuery query;
        query.prepare("SELECT * from EMPLOYE WHERE MDP =:MDP and EMAIL=:EMAIL");
        query.bindValue(":MDP", mdp);
        query.bindValue(":EMAIL", email);

        if (query.exec() && query.first()) {
            emp.setId(query.value("ID").toInt());
            emp.setnom(query.value("NOM").toString());
            emp.setprenom(query.value("PRENOM").toString());
            emp.setemail(query.value("EMAIL").toString());
            emp.setmdp(query.value("MDP").toString());
            emp.setservice(query.value("SERVICE").toString());

        }
        return emp;
}
employe employe::Loginarduino(int id){
    employe emp;
    qDebug() << "id rech " << id ;
    emp.setId(-1);
        QSqlQuery query;
        query.prepare("SELECT * from EMPLOYE WHERE ID=:ID ");
        query.bindValue(":ID", id);


         if (query.exec() && query.first()) {
             qDebug() << "value 0 " << query.value(0).toInt() ;
            emp.setId(query.value(0).toInt());
            emp.setnom(query.value(1).toString());
            emp.setprenom(query.value(2).toString());
            emp.setemail(query.value(3).toString());
            emp.setmdp(query.value(4).toString());
            emp.setservice(query.value(3).toString());

        }
         else
         {
              qDebug() << "error" << emp.getId() ;
         }



        return emp;
}
vector<employe> employe::ListeDesEmployes(int id){
    employe emp;
    vector<employe> vecteurEmployes;

        QSqlQuery query;
        query.prepare("SELECT * from EMPLOYE where ID!=:ID");
        query.bindValue(":ID",id);
        if (query.exec()) {
            while(query.next()){
            emp.setId(query.value("ID").toInt());
            emp.setnom(query.value("NOM").toString());
            emp.setprenom(query.value("PRENOM").toString());
            emp.setemail(query.value("EMAIL").toString());
            emp.setmdp(query.value("MDP").toString());
            emp.setservice(query.value("SERVICE").toString());
            vecteurEmployes.push_back(emp);
            }

        }
        return vecteurEmployes;

}

