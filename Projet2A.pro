QT       += core gui
QT       += sql
QT       += printsupport
QT       += network
QT       += charts
QT       += serialport
QT       += axcontainer
QT       += svg
QT       += multimedia multimediawidgets
QT       += widgets
QT       += serialport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arduino.cpp \
    chat.cpp \
    codeqr.cpp \
    connection.cpp \
    dialog.cpp \
    dossier_medical.cpp \
    employe.cpp \
    fournisseur.cpp \
    fournisseurwindow.cpp \
    indigent.cpp \
    indigentswindow.cpp \
    logement.cpp \
    logementswindow.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow1.cpp \
    medicalfilewindow.cpp \
    sessionuser.cpp \
    smtp.cpp \
    stat_combo.cpp

HEADERS += \
    arduino.h \
    chat.h \
    codeqr.h \
    connection.h \
    dialog.h \
    dossier_medical.h \
    employe.h \
    fournisseur.h \
    fournisseurwindow.h \
    indigent.h \
    indigentswindow.h \
    logement.h \
    logementswindow.h \
    mainwindow.h \
    mainwindow1.h \
    medicalfilewindow.h \
    sessionuser.h \
    smtp.h \
    stat_combo.h \
    webaxwidget.h

FORMS += \
    dialog.ui \
    fournisseurwindow.ui \
    indigentswindow.ui \
    logementswindow.ui \
    mainwindow.ui \
    mainwindow1.ui \
    medicalfilewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
