#ifndef CONNEXION_H
#define CONNEXION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>




class Connection
{
public:
    Connection();
    bool createconnect();
};


#endif // CONNECTION_H
