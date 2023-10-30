#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employe.h"
#include "fournisseurwindow.h"
#include "mainwindow1.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setdata();
    void remplirListeEmploye();
    void remplirMsg();

private slots:
     void on_pushButton_clicked();
     void on_listeEmploye_clicked(const QModelIndex &index);

     void on_deletebutton_clicked();

     void on_updatebutton_clicked();

     //void on_inputname_textEdited(const QString &arg1);

    // void on_inputlastname_textEdited(const QString &arg2);

     void on_pdfinput_clicked();

     //void on_searchinput_textEdited(const QString &arg1);

     void on_sortinput_clicked();


     void on_searchinput_textChanged(const QString &arg1);


     void on_loginbutton_clicked();

     void on_logout_clicked();
        void listenerslotButtonClicked(employe e);
        void on_sendMessage_clicked();

        void update_labelmemploye() ;
void clearMSG();

void on_pushButton_3_clicked();


void on_Menu_clicked();

private  :
    Ui::MainWindow *ui;
    FournisseurWindow *f;
    MainWindow1 *menu;
    int selectedId;
    QString etatformulaire="add";
    int endusavec;
};

#endif // MAINWINDOW_H
