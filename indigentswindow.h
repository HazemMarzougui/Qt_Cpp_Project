#ifndef INDIGENTSWINDOW_H
#define INDIGENTSWINDOW_H
#include <QMessageBox>
#include <QDebug>
#include <QMainWindow>
#include "indigent.h"
#include "dialog.h"
#include <QProgressBar>
#include <QSlider>
QT_BEGIN_NAMESPACE
namespace Ui { class IndigentsWindow; }
QT_END_NAMESPACE

class IndigentsWindow : public QMainWindow
{
    Q_OBJECT

public:
    IndigentsWindow(QWidget *parent = nullptr);
    ~IndigentsWindow();

private slots:
    void on_ajouter_clicked();

    void on_supprimer_clicked();

    void on_modifier_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_chercher_nom_textChanged(const QString &arg1);

    void on_nom_textEdited(const QString &arg1);

    void on_prenom_textEdited(const QString &arg1);

    void on_nom_modif_textEdited(const QString &arg1);

    void on_prenom_modif_textEdited(const QString &arg1);

    void on_tri_2_clicked();

    void on_tri_3_clicked();

    void on_pushButton_4_clicked();





    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_imprimer_clicked();

    void on_selectImage_3_clicked();

    void on_insertImage_3_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::IndigentsWindow *ui;
    indigent i;
       Dialog *s;
        QProgressBar* bar;
        QSlider* slider;
        QByteArray data;
         QByteArray data1;
         QTableView* tableView;
         QString filePath;


};
#endif // INDIGENTSWINDOW_H
