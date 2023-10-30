#ifndef CHAT_H
#define CHAT_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
using namespace std;
class chat
{
private :
    int idSender;
    int idRece;
    QString msg;

public:
    chat();
    chat(int , int, QString);
    bool sendMessage();
vector<chat> getMessages(int idSender,int idRece);
int getidSender();
int getidRece();
QString getMg();
void setidSender(int idSender);
void setididRece(int idRece);
void setMsg(QString msg);

};

#endif // CHAT_H
