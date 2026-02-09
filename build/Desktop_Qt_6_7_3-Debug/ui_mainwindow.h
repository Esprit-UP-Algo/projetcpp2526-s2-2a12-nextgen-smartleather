/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *lineEdit_nom;
    QLineEdit *lineEdit_type;
    QLineEdit *lineEdit_adresse;
    QLineEdit *lineEdit_tel;
    QLineEdit *lineEdit_email;
    QLineEdit *lineEdit_recherche;
    QSpinBox *spinBox_delai;
    QSpinBox *doubleSpinBox_prix;
    QComboBox *comboBox_statut;
    QPushButton *btn_ajouter;
    QPushButton *btn_modifier;
    QPushButton *btn_supprimer;
    QPushButton *btn_clear;
    QPushButton *btn_rechercher;
    QTableWidget *tableFournisseurs;
    QLabel *status;
    QLabel *prix;
    QLabel *delai;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        lineEdit_nom = new QLineEdit(centralwidget);
        lineEdit_nom->setObjectName("lineEdit_nom");
        lineEdit_nom->setGeometry(QRect(30, 20, 191, 26));
        lineEdit_type = new QLineEdit(centralwidget);
        lineEdit_type->setObjectName("lineEdit_type");
        lineEdit_type->setGeometry(QRect(30, 70, 191, 26));
        lineEdit_adresse = new QLineEdit(centralwidget);
        lineEdit_adresse->setObjectName("lineEdit_adresse");
        lineEdit_adresse->setGeometry(QRect(30, 120, 191, 26));
        lineEdit_tel = new QLineEdit(centralwidget);
        lineEdit_tel->setObjectName("lineEdit_tel");
        lineEdit_tel->setGeometry(QRect(30, 170, 191, 26));
        lineEdit_email = new QLineEdit(centralwidget);
        lineEdit_email->setObjectName("lineEdit_email");
        lineEdit_email->setGeometry(QRect(30, 220, 191, 26));
        lineEdit_recherche = new QLineEdit(centralwidget);
        lineEdit_recherche->setObjectName("lineEdit_recherche");
        lineEdit_recherche->setGeometry(QRect(30, 270, 191, 26));
        spinBox_delai = new QSpinBox(centralwidget);
        spinBox_delai->setObjectName("spinBox_delai");
        spinBox_delai->setGeometry(QRect(250, 50, 71, 27));
        doubleSpinBox_prix = new QSpinBox(centralwidget);
        doubleSpinBox_prix->setObjectName("doubleSpinBox_prix");
        doubleSpinBox_prix->setGeometry(QRect(250, 100, 71, 27));
        comboBox_statut = new QComboBox(centralwidget);
        comboBox_statut->setObjectName("comboBox_statut");
        comboBox_statut->setGeometry(QRect(250, 160, 86, 26));
        btn_ajouter = new QPushButton(centralwidget);
        btn_ajouter->setObjectName("btn_ajouter");
        btn_ajouter->setGeometry(QRect(360, 200, 94, 26));
        btn_modifier = new QPushButton(centralwidget);
        btn_modifier->setObjectName("btn_modifier");
        btn_modifier->setGeometry(QRect(360, 240, 94, 26));
        btn_supprimer = new QPushButton(centralwidget);
        btn_supprimer->setObjectName("btn_supprimer");
        btn_supprimer->setGeometry(QRect(420, 280, 94, 26));
        btn_clear = new QPushButton(centralwidget);
        btn_clear->setObjectName("btn_clear");
        btn_clear->setGeometry(QRect(480, 200, 94, 26));
        btn_rechercher = new QPushButton(centralwidget);
        btn_rechercher->setObjectName("btn_rechercher");
        btn_rechercher->setGeometry(QRect(480, 240, 94, 26));
        tableFournisseurs = new QTableWidget(centralwidget);
        tableFournisseurs->setObjectName("tableFournisseurs");
        tableFournisseurs->setGeometry(QRect(340, 0, 256, 192));
        status = new QLabel(centralwidget);
        status->setObjectName("status");
        status->setGeometry(QRect(250, 140, 66, 18));
        prix = new QLabel(centralwidget);
        prix->setObjectName("prix");
        prix->setGeometry(QRect(250, 80, 66, 18));
        delai = new QLabel(centralwidget);
        delai->setObjectName("delai");
        delai->setGeometry(QRect(250, 20, 66, 18));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lineEdit_nom->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        lineEdit_type->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        lineEdit_adresse->setText(QCoreApplication::translate("MainWindow", "Adresse", nullptr));
        lineEdit_tel->setText(QCoreApplication::translate("MainWindow", "Num\303\251ro t\303\251lephone", nullptr));
        lineEdit_email->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        lineEdit_recherche->setText(QCoreApplication::translate("MainWindow", "Recherche", nullptr));
        btn_ajouter->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        btn_modifier->setText(QCoreApplication::translate("MainWindow", "modifier", nullptr));
        btn_supprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        btn_clear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        btn_rechercher->setText(QCoreApplication::translate("MainWindow", "Rechercher", nullptr));
        status->setText(QCoreApplication::translate("MainWindow", "Status:", nullptr));
        prix->setText(QCoreApplication::translate("MainWindow", "Prix:", nullptr));
        delai->setText(QCoreApplication::translate("MainWindow", "Delai:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
