#include "chat.h"

chat::chat()
{

}

chat::chat(int idSender,int idRece,QString msg)
{
this->idSender=idSender;
    this->idRece=idRece;
    this->msg=msg;

}
bool chat::sendMessage(){
    QSqlQuery query;
    query.prepare("INSERT INTO CHAT ( IDSENDER, IDRECEIVER, MSG) VALUES ( :IDSENDER, :IDRECEIVER, :MSG)");

    query.bindValue(":IDSENDER",this->idSender);
     query.bindValue(":IDRECEIVER",this->idRece);
      query.bindValue(":MSG",this->msg);
 return query.exec();
}
int chat::getidSender(){
    return this->idSender;

}
int chat::getidRece(){
    return this->idRece;

}
QString chat::getMg(){
    return this->msg;
}
void chat::setidSender(int idSender){
    this->idSender=idSender;

}
void chat::setididRece(int idRece){
    this->idRece=idRece;
}
void chat::setMsg(QString msg){
      this->msg=msg;
}

vector<chat> chat::getMessages(int idSender,int idRece){
    vector<chat> vecteurChat;
    chat c;
        QSqlQuery query;
        query.prepare("SELECT * from CHAT where (IDSENDER=:IDSENDER and IDRECEIVER=:IDRECEIVER)or (IDSENDER=:IDRECEIVER and IDRECEIVER=:IDSENDER)  ");
        query.bindValue(":IDSENDER",idSender);
         query.bindValue(":IDRECEIVER",idRece);
        if (query.exec()) {
            while(query.next()){
           c.setidSender(query.value("IDSENDER").toInt());
            c.setididRece(query.value("IDRECEIVER").toInt());
            c.setMsg(query.value("MSG").toString());
            vecteurChat.push_back(c);

            }

        }
        return vecteurChat;

}
