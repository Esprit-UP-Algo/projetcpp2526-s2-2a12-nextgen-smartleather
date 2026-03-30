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
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *mainLayout_root;
    QStackedWidget *stackedWidget_main;
    QWidget *page_login_main;
    QVBoxLayout *verticalLayout_login_main;
    QSpacerItem *login_main_top;
    QFrame *cardLogin_main;
    QVBoxLayout *verticalLayout_login_main_inner;
    QLabel *lblLoginTitle_main;
    QLineEdit *editLoginUser_main;
    QLineEdit *editLoginPassword_main;
    QPushButton *btnLogin_main;
    QPushButton *btnGotoRegister_main;
    QSpacerItem *login_main_bottom;
    QWidget *page_suppliers;
    QWidget *page_register_main;
    QVBoxLayout *verticalLayout_register_main;
    QFrame *cardRegister_main;
    QVBoxLayout *verticalLayout_register_main_inner;
    QLabel *lblRegisterTitle_main;
    QLineEdit *editRegName_main;
    QLineEdit *editRegEmail_main;
    QLineEdit *editRegPassword_main;
    QLineEdit *editRegPasswordConfirm_main;
    QPushButton *btnRegister_main;
    QPushButton *btnGotoLogin_main;
    QWidget *page_main_content;
    QHBoxLayout *mainLayout;
    QFrame *sidebar;
    QVBoxLayout *sidebarLayout;
    QLabel *logo_label;
    QLabel *factory_title;
    QPushButton *btn_nav_orders;
    QPushButton *btn_nav_material;
    QPushButton *btn_nav_employees;
    QPushButton *gestion_fournisseurs;
    QPushButton *btn_nav_ai;
    QSpacerItem *vSpacer;
    QVBoxLayout *contentArea;
    QStackedWidget *stackedWidget;
    QWidget *page_suppliers_2;
    QVBoxLayout *verticalLayout_suppliers;
    QFrame *top_nav_suppliers;
    QHBoxLayout *navLayout_suppliers;
    QPushButton *btn_tab_supp_add;
    QPushButton *btn_tab_supp_update;
    QPushButton *btn_tab_supp_delete;
    QPushButton *btn_tab_supp_list;
    QSpacerItem *horizontalSpacer_supp;
    QStackedWidget *stackedWidget_suppliers;
    QWidget *page_supp_add;
    QVBoxLayout *verticalLayout_supp_add;
    QFrame *frame_add_supp;
    QVBoxLayout *layout_form_add;
    QLabel *lblFormTitle_add;
    QFrame *frame_inputs_add;
    QFormLayout *formLayout_add_supp;
    QLabel *label_supp_1;
    QLineEdit *txtNom;
    QLabel *label_supp_2;
    QLineEdit *txtType;
    QLabel *label_supp_3;
    QLineEdit *txtAdresse;
    QLabel *label_supp_4;
    QLineEdit *txtTelephone;
    QLabel *label_supp_5;
    QLineEdit *txtEmail;
    QLabel *label_supp_6;
    QSpinBox *spinDelai;
    QLabel *label_supp_7;
    QDoubleSpinBox *spinPrix;
    QLabel *label_supp_8;
    QComboBox *comboStatut;
    QLabel *label_supp_notes;
    QPlainTextEdit *txtNotes;
    QHBoxLayout *layout_button_add;
    QSpacerItem *horizontalSpacer_add;
    QPushButton *btnEnregistrer;
    QPushButton *btnReinitialiser;
    QSpacerItem *spacer_add;
    QWidget *page_supp_update;
    QVBoxLayout *verticalLayout_supp_update;
    QFrame *frame_update_supp;
    QVBoxLayout *layout_form_mod;
    QLabel *lblFormTitle_mod;
    QHBoxLayout *search_layout_mod;
    QLabel *label_search_supp;
    QLineEdit *txtRechercheModifier;
    QPushButton *btnChercherFournisseur;
    QFrame *frame_inputs_mod;
    QFormLayout *formLayout_update_supp;
    QLabel *label_mod_1;
    QLineEdit *txtNomMod;
    QLabel *label_mod_2;
    QLineEdit *txtTypeMod;
    QLabel *label_mod_3;
    QLineEdit *txtAdresseMod;
    QLabel *label_mod_4;
    QLineEdit *txtTelephoneMod;
    QLabel *label_mod_5;
    QLineEdit *txtEmailMod;
    QLabel *label_mod_6;
    QSpinBox *spinDelaiMod;
    QLabel *label_mod_7;
    QDoubleSpinBox *spinPrixMod;
    QLabel *label_mod_8;
    QComboBox *comboStatutMod;
    QLabel *label_mod_notes;
    QPlainTextEdit *txtNotesMod;
    QHBoxLayout *layout_button_update;
    QSpacerItem *horizontalSpacer_mod;
    QPushButton *btnMettreAJour;
    QSpacerItem *spacer_mod;
    QWidget *page_supp_delete;
    QVBoxLayout *verticalLayout_supp_delete;
    QFrame *frame_delete_supp;
    QVBoxLayout *layout_delete_supp;
    QLabel *lblFormTitle_del;
    QHBoxLayout *search_layout_del;
    QLabel *lblSupprimer;
    QLineEdit *txtSupprimer;
    QPushButton *btnChercherSupprimer;
    QFrame *frame_confirm_del;
    QVBoxLayout *layout_confirm_del;
    QLabel *lblValiderSupp;
    QLabel *lblDetailSupp;
    QLabel *lblWarning;
    QSpacerItem *spacer_delete;
    QHBoxLayout *layout_buttons_del;
    QSpacerItem *horizontalSpacer_del;
    QPushButton *btnAnnulerSupp;
    QPushButton *btnSupprimerDef;
    QWidget *page_supp_list;
    QVBoxLayout *verticalLayout_supp_list;
    QFrame *frame_list_supp;
    QVBoxLayout *layout_list_supp;
    QLabel *lblListeTitle;
    QHBoxLayout *filterLayout;
    QLineEdit *txtFilterNom;
    QLineEdit *txtFilterType;
    QLineEdit *txtFilterStatut;
    QPushButton *btnRechercher;
    QPushButton *btnRinitialiserFiltres;
    QTableWidget *tableFournisseurs;
    QHBoxLayout *actionsTableLayout;
    QLabel *lblSelectedSupplier;
    QSpacerItem *horizontalSpacer_actions;
    QPushButton *btnModifierDuTableau;
    QPushButton *btnSupprimerDuTableau;
    QHBoxLayout *statsLayout;
    QFrame *frame_stat_total;
    QVBoxLayout *vboxLayout;
    QLabel *lblStatTotalLabel;
    QLabel *lblStatTotal;
    QFrame *frame_stat_actifs;
    QVBoxLayout *vboxLayout1;
    QLabel *lblStatActifsLabel;
    QLabel *lblStatActifs;
    QFrame *frame_stat_prix;
    QVBoxLayout *vboxLayout2;
    QLabel *lblStatPrixLabel;
    QLabel *lblStatPrix;
    QFrame *frame_stat_delai;
    QVBoxLayout *vboxLayout3;
    QLabel *lblStatDelaiLabel;
    QLabel *lblStatDelai;
    QWidget *page_orders;
    QVBoxLayout *verticalLayout_orders;
    QFrame *top_nav;
    QHBoxLayout *navLayout;
    QPushButton *btn_tab_add;
    QPushButton *btn_tab_update;
    QPushButton *btn_tab_delete;
    QPushButton *btn_tab_list;
    QStackedWidget *stackedWidget_orders;
    QWidget *page_add;
    QVBoxLayout *pageAddLayout;
    QFrame *form_container;
    QFormLayout *addForm;
    QLabel *label;
    QLineEdit *le_id;
    QLabel *label1;
    QComboBox *cb_article_type;
    QLabel *label2;
    QSpinBox *sb_order_qty;
    QLabel *label3;
    QLineEdit *le_client_email;
    QLabel *label4;
    QComboBox *cb_client_city;
    QLabel *label5;
    QLabel *label6;
    QDateEdit *de_date;
    QLabel *label7;
    QDateEdit *de_date_delivery;
    QLabel *label8;
    QDoubleSpinBox *dsb_price;
    QLabel *label9;
    QLineEdit *le_proof;
    QPushButton *btn_valider;
    QComboBox *cb_status;
    QSpacerItem *vSpacerAdd;
    QWidget *page_update;
    QVBoxLayout *pageUpdateLayout;
    QFrame *update_container;
    QVBoxLayout *updateVLayout;
    QHBoxLayout *searchUpdateLayout;
    QLineEdit *le_id_update_search;
    QPushButton *btn_search_update;
    QFrame *line_update;
    QFormLayout *updateForm;
    QLabel *label10;
    QLineEdit *le_id_update;
    QLabel *label11;
    QComboBox *cb_type_update;
    QLabel *label12;
    QSpinBox *sb_qty_update;
    QLabel *label13;
    QLineEdit *le_email_update;
    QLabel *label14;
    QComboBox *cb_city_update;
    QLabel *label15;
    QComboBox *cb_status_update;
    QLabel *label16;
    QDateEdit *de_date_update;
    QLabel *label17;
    QDateEdit *de_date_delivery_update;
    QLabel *label18;
    QDoubleSpinBox *dsb_price_update;
    QLabel *label19;
    QLineEdit *le_proof_update;
    QPushButton *btn_update_confirm;
    QSpacerItem *vSpacerUpdate;
    QWidget *page_delete;
    QVBoxLayout *pageDeleteLayout;
    QFrame *table_container;
    QVBoxLayout *tableVLayout;
    QHBoxLayout *searchDeleteLayout;
    QLineEdit *le_id_to_delete;
    QPushButton *btn_delete_action;
    QTableWidget *table_delete;
    QWidget *page_list;
    QVBoxLayout *pageListLayout;
    QTabWidget *tabWidget_stats;
    QWidget *tab_data;
    QVBoxLayout *tabDataLayout;
    QHBoxLayout *toolbarLayout;
    QLineEdit *le_search;
    QComboBox *cb_sort;
    QPushButton *btn_pdf;
    QTableWidget *table_list;
    QWidget *tab_stats;
    QGridLayout *statsGridLayout;
    QFrame *stat_card_total;
    QVBoxLayout *vboxTotal;
    QLabel *lbl_total_orders_title;
    QLabel *lbl_total_orders_val;
    QFrame *stat_card_top;
    QVBoxLayout *vboxTop;
    QLabel *lbl_top_client_title;
    QLabel *lbl_top_client_val;
    QFrame *chart_container;
    QVBoxLayout *chartLayout;
    QWidget *page_material;
    QVBoxLayout *verticalLayout_material;
    QFrame *top_nav_materials;
    QHBoxLayout *navLayout_materials;
    QPushButton *btn_tab_mat_add;
    QPushButton *btn_tab_mat_update;
    QPushButton *btn_tab_mat_delete;
    QPushButton *btn_tab_mat_list;
    QStackedWidget *stackedWidget_materials;
    QWidget *page_mat_add;
    QVBoxLayout *verticalLayout_mat_add;
    QFrame *frame_add;
    QFormLayout *formLayout_add;
    QLabel *label_mat_1;
    QLineEdit *le_mat_name;
    QLabel *label_mat_2;
    QComboBox *cb_mat_type;
    QLabel *label_mat_3;
    QDoubleSpinBox *dsb_mat_price;
    QLabel *label_mat_4;
    QSpinBox *sb_mat_qty;
    QLabel *label_mat_5;
    QHBoxLayout *colorLayoutAdd;
    QLineEdit *le_mat_color;
    QPushButton *btn_color_add;
    QLabel *label_mat_6;
    QDoubleSpinBox *dsb_mat_yield;
    QLabel *label_mat_7;
    QDoubleSpinBox *dsb_mat_loss;
    QPushButton *btn_mat_add;
    QSpacerItem *verticalSpacer_mat_add;
    QWidget *page_mat_update;
    QVBoxLayout *verticalLayout_mat_upd;
    QFrame *frame_upd;
    QVBoxLayout *vbox_upd_inner;
    QHBoxLayout *hbox_search_upd;
    QLineEdit *le_upd_search_name;
    QPushButton *btn_upd_load;
    QFormLayout *formLayout_upd;
    QLabel *label_mat_10;
    QLineEdit *le_upd_name;
    QLabel *label_mat_11;
    QComboBox *cb_upd_type;
    QLabel *label_mat_12;
    QDoubleSpinBox *dsb_upd_price;
    QLabel *label_mat_13;
    QSpinBox *sb_upd_qty;
    QLabel *label_mat_14;
    QHBoxLayout *colorLayoutUpd;
    QLineEdit *le_upd_color;
    QPushButton *btn_color_upd;
    QLabel *label_mat_15;
    QDoubleSpinBox *dsb_upd_yield;
    QLabel *label_mat_16;
    QDoubleSpinBox *dsb_upd_loss;
    QPushButton *btn_upd_confirm;
    QSpacerItem *verticalSpacer_mat_upd;
    QWidget *page_mat_delete;
    QVBoxLayout *verticalLayout_mat_del;
    QFrame *frame_del;
    QVBoxLayout *vbox_del;
    QLabel *lbl_del_info;
    QLineEdit *le_del_name;
    QPushButton *btn_del_confirm;
    QSpacerItem *verticalSpacer_mat_del;
    QWidget *page_mat_list;
    QVBoxLayout *verticalLayout_mat_list;
    QTabWidget *tabWidget_mat_stats;
    QWidget *tab_mat_data;
    QVBoxLayout *layout_tab_mat_data;
    QHBoxLayout *hbox_mat_tools;
    QLineEdit *le_search_mat;
    QComboBox *cb_sort_mat;
    QPushButton *btn_export_mat_pdf;
    QTableWidget *table_mat;
    QWidget *tab_mat_stats;
    QGridLayout *gridLayout_mat_stats;
    QFrame *stat_card_mat_total;
    QVBoxLayout *vbox_mat_total;
    QLabel *lbl_mat_t1;
    QLabel *lbl_total_mat_val;
    QFrame *stat_card_rupture;
    QVBoxLayout *vbox_rupture;
    QLabel *lbl_r1;
    QLabel *lbl_rupture_val;
    QFrame *stat_card_waste;
    QVBoxLayout *vbox_waste;
    QLabel *lbl_w1;
    QLabel *lbl_waste_val;
    QFrame *frame_graph;
    QVBoxLayout *vbox_graph;
    QLabel *lbl_g1;
    QLabel *lbl_g2;
    QProgressBar *pb_yield;
    QLabel *lbl_g3;
    QProgressBar *pb_loss;
    QWidget *page_employees;
    QVBoxLayout *verticalLayout_employees;
    QLabel *lblTitle_employees;
    QStackedWidget *stackMain_employees;
    QWidget *pageLogin;
    QVBoxLayout *verticalLayout_login_root;
    QSpacerItem *login_top;
    QFrame *cardLogin;
    QVBoxLayout *verticalLayout_login;
    QLabel *lblLoginTitle;
    QLineEdit *editLoginUser;
    QLineEdit *editLoginPassword;
    QPushButton *btnLogin;
    QPushButton *btnGotoRegister;
    QSpacerItem *login_bottom;
    QWidget *pageRegister;
    QVBoxLayout *verticalLayout_register_root;
    QFrame *cardRegister;
    QVBoxLayout *verticalLayout_register;
    QLabel *lblRegisterTitle;
    QLineEdit *editRegName;
    QLineEdit *editRegEmail;
    QLineEdit *editRegPassword;
    QLineEdit *editRegPasswordConfirm;
    QPushButton *btnRegister;
    QPushButton *btnGotoLogin;
    QWidget *pageEmployee;
    QVBoxLayout *verticalLayout_employee_root;
    QFrame *frame_emp_top_nav;
    QHBoxLayout *horizontalLayout_emp_nav;
    QPushButton *btnTabAdd;
    QPushButton *btnTabEdit;
    QPushButton *btnTabDelete;
    QPushButton *btnTabList;
    QSpacerItem *horizontalSpacer_emp_nav;
    QStackedWidget *stackEmployee;
    QWidget *subPageAdd;
    QVBoxLayout *verticalLayout_sub_add;
    QFrame *cardEmployeeAdd;
    QVBoxLayout *verticalLayout_add_inner;
    QGridLayout *gridLayout_form;
    QLabel *lblId;
    QLineEdit *editId;
    QLabel *lblName;
    QLineEdit *editName;
    QLabel *lblAddress;
    QLineEdit *editAddress;
    QLabel *lblEmail;
    QLineEdit *editEmail;
    QLabel *lblPhone;
    QLineEdit *editPhone;
    QLabel *lblHireDate;
    QDateEdit *dateHire;
    QVBoxLayout *verticalLayout_photo;
    QLabel *lblPhotoPreview;
    QPushButton *btnChoosePhoto;
    QLabel *lblPoste;
    QComboBox *comboPoste;
    QLabel *lblSalary;
    QDoubleSpinBox *spinSalary;
    QLabel *lblSexe;
    QComboBox *comboSexe;
    QLabel *lblStatus;
    QComboBox *comboStatus;
    QLabel *lblSpeciality;
    QComboBox *comboSpeciality;
    QLabel *lblRole;
    QComboBox *comboRole;
    QHBoxLayout *hbox_btns_add;
    QSpacerItem *spacer_add_left;
    QPushButton *btnAjouter;
    QPushButton *btnVider;
    QWidget *subPageEdit;
    QVBoxLayout *verticalLayout_sub_edit;
    QFrame *cardEmployeeEdit;
    QVBoxLayout *vbox_edit;
    QHBoxLayout *hbox_search_edit;
    QLineEdit *editSearchEdit;
    QPushButton *btnSearchEdit;
    QLabel *lblEditInfo;
    QFormLayout *formLayout_Edit;
    QLabel *lblModName;
    QLineEdit *editNameMod;
    QLabel *lblModPhone;
    QLineEdit *editPhoneMod;
    QLabel *lblModSalary;
    QDoubleSpinBox *spinSalaryMod;
    QPushButton *btnModifier;
    QSpacerItem *verticalSpacer_edit;
    QWidget *subPageDelete;
    QVBoxLayout *verticalLayout_sub_del;
    QFrame *cardEmployeeDelete;
    QVBoxLayout *vbox_del1;
    QLabel *lblDelTitle;
    QLineEdit *editSearchDel;
    QPushButton *btnSupprimer;
    QSpacerItem *verticalSpacer_del;
    QWidget *subPageList;
    QVBoxLayout *verticalLayout_sub_list;
    QFrame *cardEmployeeList;
    QVBoxLayout *vbox_list;
    QHBoxLayout *hbox_list_tools;
    QLineEdit *editSearchList;
    QComboBox *comboSort;
    QPushButton *btnExportPDF;
    QTableView *tableEmployees;
    QGroupBox *groupStats;
    QHBoxLayout *hbox_stats;
    QLabel *lblStatCount;
    QLabel *lblStatAvg;
    QPushButton *btnCalculPrime;
    QWidget *page_ai;
    QVBoxLayout *verticalLayout_ai;
    QLabel *label_ai;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 970);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"    QMainWindow { background-color: #FCF9F6; }\n"
"\n"
"    #sidebar { background-color: #3D362D; border-right: 1px solid #4E2C23; }\n"
"    #sidebar QPushButton { background-color: transparent; color: #F2D2B5; text-align: left; padding: 15px 20px; border: none; font-weight: bold; text-transform: uppercase; font-size: 12px; }\n"
"    #sidebar QPushButton:hover { background-color: #4E2C23; color: #C68E65; border-left: 4px solid #C68E65; }\n"
"\n"
"    #top_nav, #top_nav_materials { background-color: white; border-bottom: 2px solid #F2D2B5; }\n"
"    #top_nav QPushButton, #top_nav_materials QPushButton { background-color: transparent; border: none; padding: 12px 25px; color: #8B4513; font-size: 13px; font-weight: 600; border-radius: 6px; margin: 5px; }\n"
"    #top_nav QPushButton:hover, #top_nav_materials QPushButton:hover { background-color: #FFF0E0; color: #4E2C23; }\n"
"    #top_nav QPushButton:checked, #top_nav_materials QPushButton:checked { background-color: #C68E65; color: white; }\n"
"\n"
"    #for"
                        "m_container, #table_container, #update_container, #frame_add, #frame_upd, #frame_del { background-color: white; border: 1px solid #F2D2B5; border-radius: 15px; margin: 20px; }\n"
"\n"
"    QLabel { color: #4E2C23; font-size: 13px; font-weight: bold; }\n"
"    QLineEdit, QSpinBox, QDoubleSpinBox, QDateEdit {\n"
"        background-color: #FFFCFA; border: 1px solid #E0D0C0; padding: 10px; border-radius: 6px; color: #3D362D; font-size: 13px;\n"
"    }\n"
"    QComboBox {\n"
"        background-color: white; border: 1px solid #D2B48C; padding: 10px; border-radius: 6px; color: #000000; font-size: 13px;\n"
"    }\n"
"    QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QDateEdit:focus {\n"
"        border: 2px solid #C68E65; background-color: white;\n"
"    }\n"
"    QComboBox QAbstractItemView { background-color: white; color: black; selection-background-color: #C68E65; selection-color: white; border: 1px solid #D2B48C; }\n"
"\n"
"    #btn_valider, #btn_update_confirm, #btn_search_update { background-color: #"
                        "8B4513; color: white; border-radius: 6px; padding: 10px 30px; font-weight: bold; border: none; }\n"
"    #btn_valider:hover, #btn_update_confirm:hover, #btn_search_update:hover { background-color: #4E2C23; }\n"
"\n"
"    #btn_delete_action { background-color: #4E2C23; color: white; border-radius: 6px; padding: 10px 30px; font-weight: bold; }\n"
"    #btn_delete_action:hover { background-color: #3D362D; }\n"
"\n"
"    #btn_pdf { background-color: #C68E65; color: white; border-radius: 6px; padding: 8px 15px; font-weight: bold; border: none; }\n"
"    #btn_pdf:hover { background-color: #8B4513; }\n"
"\n"
"    #cardLogin, #cardRegister, #cardLogin_main, #cardRegister_main, #cardEmployee, #cardEmployeeAdd, #cardEmployeeEdit, #cardEmployeeDelete, #cardEmployeeList { background-color: white; border: 1px solid #F2D2B5; border-radius: 15px; margin: 20px; padding: 20px; }\n"
"\n"
"    #btnLogin, #btnRegister, #btnLogin_main, #btnRegister_main, #btnAjouter { background-color: #8B4513; color: white; border-radius: 6px; pa"
                        "dding: 10px 30px; font-weight: bold; border: none; }\n"
"    #btnLogin:hover, #btnRegister:hover, #btnLogin_main:hover, #btnRegister_main:hover, #btnAjouter:hover { background-color: #4E2C23; }\n"
"\n"
"    #btnQuitter { background-color: #4E2C23; color: white; border-radius: 6px; padding: 10px 30px; font-weight: bold; }\n"
"    #btnQuitter:hover { background-color: #3D362D; }\n"
"\n"
"    #btnVider, #btnChoosePhoto { background-color: #C68E65; color: white; border-radius: 6px; padding: 8px 15px; font-weight: bold; border: none; }\n"
"    #btnVider:hover, #btnChoosePhoto:hover { background-color: #8B4513; }\n"
"\n"
"    QPushButton#btnGotoRegister, QPushButton#btnGotoLogin, QPushButton#btnGotoRegister_main, QPushButton#btnGotoLogin_main { background-color: transparent; color: #8B4513; border: none; font-weight: 600; text-decoration: underline; font-size: 13px; }\n"
"    QPushButton#btnGotoRegister:hover, QPushButton#btnGotoLogin:hover, QPushButton#btnGotoRegister_main:hover, QPushButton#btnGotoLogin_main:hover"
                        " { color: #4E2C23; }\n"
"\n"
"    QTableWidget { background-color: white; color: #3D362D; gridline-color: #F2D2B5; border: none; alternate-background-color: #FFFCFA; selection-background-color: #F2D2B5; selection-color: #4E2C23; }\n"
"    QHeaderView::section { background-color: #3D362D; color: #F2D2B5; padding: 8px; border: none; font-weight: bold; }\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout_root = new QVBoxLayout(centralwidget);
        mainLayout_root->setSpacing(0);
        mainLayout_root->setObjectName("mainLayout_root");
        mainLayout_root->setContentsMargins(0, 0, 0, 0);
        stackedWidget_main = new QStackedWidget(centralwidget);
        stackedWidget_main->setObjectName("stackedWidget_main");
        page_login_main = new QWidget();
        page_login_main->setObjectName("page_login_main");
        verticalLayout_login_main = new QVBoxLayout(page_login_main);
        verticalLayout_login_main->setSpacing(12);
        verticalLayout_login_main->setObjectName("verticalLayout_login_main");
        verticalLayout_login_main->setContentsMargins(40, 20, 40, 20);
        login_main_top = new QSpacerItem(20, 30, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_login_main->addItem(login_main_top);

        cardLogin_main = new QFrame(page_login_main);
        cardLogin_main->setObjectName("cardLogin_main");
        cardLogin_main->setFrameShape(QFrame::Shape::NoFrame);
        verticalLayout_login_main_inner = new QVBoxLayout(cardLogin_main);
        verticalLayout_login_main_inner->setSpacing(16);
        verticalLayout_login_main_inner->setObjectName("verticalLayout_login_main_inner");
        verticalLayout_login_main_inner->setContentsMargins(24, 24, 24, 24);
        lblLoginTitle_main = new QLabel(cardLogin_main);
        lblLoginTitle_main->setObjectName("lblLoginTitle_main");
        lblLoginTitle_main->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_login_main_inner->addWidget(lblLoginTitle_main);

        editLoginUser_main = new QLineEdit(cardLogin_main);
        editLoginUser_main->setObjectName("editLoginUser_main");

        verticalLayout_login_main_inner->addWidget(editLoginUser_main);

        editLoginPassword_main = new QLineEdit(cardLogin_main);
        editLoginPassword_main->setObjectName("editLoginPassword_main");
        editLoginPassword_main->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout_login_main_inner->addWidget(editLoginPassword_main);

        btnLogin_main = new QPushButton(cardLogin_main);
        btnLogin_main->setObjectName("btnLogin_main");

        verticalLayout_login_main_inner->addWidget(btnLogin_main);

        btnGotoRegister_main = new QPushButton(cardLogin_main);
        btnGotoRegister_main->setObjectName("btnGotoRegister_main");

        verticalLayout_login_main_inner->addWidget(btnGotoRegister_main);


        verticalLayout_login_main->addWidget(cardLogin_main);

        login_main_bottom = new QSpacerItem(20, 30, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_login_main->addItem(login_main_bottom);

        stackedWidget_main->addWidget(page_login_main);
        page_suppliers = new QWidget();
        page_suppliers->setObjectName("page_suppliers");
        stackedWidget_main->addWidget(page_suppliers);
        page_register_main = new QWidget();
        page_register_main->setObjectName("page_register_main");
        verticalLayout_register_main = new QVBoxLayout(page_register_main);
        verticalLayout_register_main->setSpacing(12);
        verticalLayout_register_main->setObjectName("verticalLayout_register_main");
        verticalLayout_register_main->setContentsMargins(40, 20, 40, 20);
        cardRegister_main = new QFrame(page_register_main);
        cardRegister_main->setObjectName("cardRegister_main");
        cardRegister_main->setFrameShape(QFrame::Shape::NoFrame);
        verticalLayout_register_main_inner = new QVBoxLayout(cardRegister_main);
        verticalLayout_register_main_inner->setSpacing(16);
        verticalLayout_register_main_inner->setObjectName("verticalLayout_register_main_inner");
        verticalLayout_register_main_inner->setContentsMargins(24, 24, 24, 24);
        lblRegisterTitle_main = new QLabel(cardRegister_main);
        lblRegisterTitle_main->setObjectName("lblRegisterTitle_main");
        lblRegisterTitle_main->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_register_main_inner->addWidget(lblRegisterTitle_main);

        editRegName_main = new QLineEdit(cardRegister_main);
        editRegName_main->setObjectName("editRegName_main");

        verticalLayout_register_main_inner->addWidget(editRegName_main);

        editRegEmail_main = new QLineEdit(cardRegister_main);
        editRegEmail_main->setObjectName("editRegEmail_main");

        verticalLayout_register_main_inner->addWidget(editRegEmail_main);

        editRegPassword_main = new QLineEdit(cardRegister_main);
        editRegPassword_main->setObjectName("editRegPassword_main");
        editRegPassword_main->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout_register_main_inner->addWidget(editRegPassword_main);

        editRegPasswordConfirm_main = new QLineEdit(cardRegister_main);
        editRegPasswordConfirm_main->setObjectName("editRegPasswordConfirm_main");
        editRegPasswordConfirm_main->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout_register_main_inner->addWidget(editRegPasswordConfirm_main);

        btnRegister_main = new QPushButton(cardRegister_main);
        btnRegister_main->setObjectName("btnRegister_main");

        verticalLayout_register_main_inner->addWidget(btnRegister_main);

        btnGotoLogin_main = new QPushButton(cardRegister_main);
        btnGotoLogin_main->setObjectName("btnGotoLogin_main");

        verticalLayout_register_main_inner->addWidget(btnGotoLogin_main);


        verticalLayout_register_main->addWidget(cardRegister_main);

        stackedWidget_main->addWidget(page_register_main);
        page_main_content = new QWidget();
        page_main_content->setObjectName("page_main_content");
        mainLayout = new QHBoxLayout(page_main_content);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        sidebar = new QFrame(page_main_content);
        sidebar->setObjectName("sidebar");
        sidebar->setMinimumSize(QSize(220, 0));
        sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setObjectName("sidebarLayout");
        logo_label = new QLabel(sidebar);
        logo_label->setObjectName("logo_label");
        logo_label->setMinimumSize(QSize(0, 150));
        logo_label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        sidebarLayout->addWidget(logo_label);

        factory_title = new QLabel(sidebar);
        factory_title->setObjectName("factory_title");
        factory_title->setStyleSheet(QString::fromUtf8("color: #C68E65; font-size: 20px; font-weight: bold; padding: 10px 10px 30px 10px; letter-spacing: 1px;"));
        factory_title->setAlignment(Qt::AlignmentFlag::AlignCenter);

        sidebarLayout->addWidget(factory_title);

        btn_nav_orders = new QPushButton(sidebar);
        btn_nav_orders->setObjectName("btn_nav_orders");

        sidebarLayout->addWidget(btn_nav_orders);

        btn_nav_material = new QPushButton(sidebar);
        btn_nav_material->setObjectName("btn_nav_material");

        sidebarLayout->addWidget(btn_nav_material);

        btn_nav_employees = new QPushButton(sidebar);
        btn_nav_employees->setObjectName("btn_nav_employees");

        sidebarLayout->addWidget(btn_nav_employees);

        gestion_fournisseurs = new QPushButton(sidebar);
        gestion_fournisseurs->setObjectName("gestion_fournisseurs");

        sidebarLayout->addWidget(gestion_fournisseurs);

        btn_nav_ai = new QPushButton(sidebar);
        btn_nav_ai->setObjectName("btn_nav_ai");

        sidebarLayout->addWidget(btn_nav_ai);

        vSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarLayout->addItem(vSpacer);


        mainLayout->addWidget(sidebar);

        contentArea = new QVBoxLayout();
        contentArea->setSpacing(0);
        contentArea->setObjectName("contentArea");
        stackedWidget = new QStackedWidget(page_main_content);
        stackedWidget->setObjectName("stackedWidget");
        page_suppliers_2 = new QWidget();
        page_suppliers_2->setObjectName("page_suppliers_2");
        verticalLayout_suppliers = new QVBoxLayout(page_suppliers_2);
        verticalLayout_suppliers->setSpacing(0);
        verticalLayout_suppliers->setObjectName("verticalLayout_suppliers");
        verticalLayout_suppliers->setContentsMargins(0, 0, 0, 0);
        top_nav_suppliers = new QFrame(page_suppliers_2);
        top_nav_suppliers->setObjectName("top_nav_suppliers");
        navLayout_suppliers = new QHBoxLayout(top_nav_suppliers);
        navLayout_suppliers->setSpacing(5);
        navLayout_suppliers->setObjectName("navLayout_suppliers");
        navLayout_suppliers->setContentsMargins(10, -1, 10, -1);
        btn_tab_supp_add = new QPushButton(top_nav_suppliers);
        btn_tab_supp_add->setObjectName("btn_tab_supp_add");
        btn_tab_supp_add->setCheckable(true);

        navLayout_suppliers->addWidget(btn_tab_supp_add);

        btn_tab_supp_update = new QPushButton(top_nav_suppliers);
        btn_tab_supp_update->setObjectName("btn_tab_supp_update");
        btn_tab_supp_update->setCheckable(true);

        navLayout_suppliers->addWidget(btn_tab_supp_update);

        btn_tab_supp_delete = new QPushButton(top_nav_suppliers);
        btn_tab_supp_delete->setObjectName("btn_tab_supp_delete");
        btn_tab_supp_delete->setCheckable(true);

        navLayout_suppliers->addWidget(btn_tab_supp_delete);

        btn_tab_supp_list = new QPushButton(top_nav_suppliers);
        btn_tab_supp_list->setObjectName("btn_tab_supp_list");
        btn_tab_supp_list->setCheckable(true);

        navLayout_suppliers->addWidget(btn_tab_supp_list);

        horizontalSpacer_supp = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        navLayout_suppliers->addItem(horizontalSpacer_supp);


        verticalLayout_suppliers->addWidget(top_nav_suppliers);

        stackedWidget_suppliers = new QStackedWidget(page_suppliers_2);
        stackedWidget_suppliers->setObjectName("stackedWidget_suppliers");
        page_supp_add = new QWidget();
        page_supp_add->setObjectName("page_supp_add");
        verticalLayout_supp_add = new QVBoxLayout(page_supp_add);
        verticalLayout_supp_add->setSpacing(0);
        verticalLayout_supp_add->setObjectName("verticalLayout_supp_add");
        verticalLayout_supp_add->setContentsMargins(20, 20, 20, 20);
        frame_add_supp = new QFrame(page_supp_add);
        frame_add_supp->setObjectName("frame_add_supp");
        layout_form_add = new QVBoxLayout(frame_add_supp);
        layout_form_add->setObjectName("layout_form_add");
        lblFormTitle_add = new QLabel(frame_add_supp);
        lblFormTitle_add->setObjectName("lblFormTitle_add");

        layout_form_add->addWidget(lblFormTitle_add);

        frame_inputs_add = new QFrame(frame_add_supp);
        frame_inputs_add->setObjectName("frame_inputs_add");
        formLayout_add_supp = new QFormLayout(frame_inputs_add);
        formLayout_add_supp->setSpacing(10);
        formLayout_add_supp->setObjectName("formLayout_add_supp");
        label_supp_1 = new QLabel(frame_inputs_add);
        label_supp_1->setObjectName("label_supp_1");

        formLayout_add_supp->setWidget(0, QFormLayout::LabelRole, label_supp_1);

        txtNom = new QLineEdit(frame_inputs_add);
        txtNom->setObjectName("txtNom");

        formLayout_add_supp->setWidget(0, QFormLayout::FieldRole, txtNom);

        label_supp_2 = new QLabel(frame_inputs_add);
        label_supp_2->setObjectName("label_supp_2");

        formLayout_add_supp->setWidget(1, QFormLayout::LabelRole, label_supp_2);

        txtType = new QLineEdit(frame_inputs_add);
        txtType->setObjectName("txtType");

        formLayout_add_supp->setWidget(1, QFormLayout::FieldRole, txtType);

        label_supp_3 = new QLabel(frame_inputs_add);
        label_supp_3->setObjectName("label_supp_3");

        formLayout_add_supp->setWidget(2, QFormLayout::LabelRole, label_supp_3);

        txtAdresse = new QLineEdit(frame_inputs_add);
        txtAdresse->setObjectName("txtAdresse");

        formLayout_add_supp->setWidget(2, QFormLayout::FieldRole, txtAdresse);

        label_supp_4 = new QLabel(frame_inputs_add);
        label_supp_4->setObjectName("label_supp_4");

        formLayout_add_supp->setWidget(3, QFormLayout::LabelRole, label_supp_4);

        txtTelephone = new QLineEdit(frame_inputs_add);
        txtTelephone->setObjectName("txtTelephone");

        formLayout_add_supp->setWidget(3, QFormLayout::FieldRole, txtTelephone);

        label_supp_5 = new QLabel(frame_inputs_add);
        label_supp_5->setObjectName("label_supp_5");

        formLayout_add_supp->setWidget(4, QFormLayout::LabelRole, label_supp_5);

        txtEmail = new QLineEdit(frame_inputs_add);
        txtEmail->setObjectName("txtEmail");

        formLayout_add_supp->setWidget(4, QFormLayout::FieldRole, txtEmail);

        label_supp_6 = new QLabel(frame_inputs_add);
        label_supp_6->setObjectName("label_supp_6");

        formLayout_add_supp->setWidget(5, QFormLayout::LabelRole, label_supp_6);

        spinDelai = new QSpinBox(frame_inputs_add);
        spinDelai->setObjectName("spinDelai");
        spinDelai->setMinimum(1);
        spinDelai->setMaximum(365);

        formLayout_add_supp->setWidget(5, QFormLayout::FieldRole, spinDelai);

        label_supp_7 = new QLabel(frame_inputs_add);
        label_supp_7->setObjectName("label_supp_7");

        formLayout_add_supp->setWidget(6, QFormLayout::LabelRole, label_supp_7);

        spinPrix = new QDoubleSpinBox(frame_inputs_add);
        spinPrix->setObjectName("spinPrix");
        spinPrix->setMinimum(0.010000000000000);
        spinPrix->setMaximum(999999.989999999990687);

        formLayout_add_supp->setWidget(6, QFormLayout::FieldRole, spinPrix);

        label_supp_8 = new QLabel(frame_inputs_add);
        label_supp_8->setObjectName("label_supp_8");

        formLayout_add_supp->setWidget(7, QFormLayout::LabelRole, label_supp_8);

        comboStatut = new QComboBox(frame_inputs_add);
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->setObjectName("comboStatut");

        formLayout_add_supp->setWidget(7, QFormLayout::FieldRole, comboStatut);

        label_supp_notes = new QLabel(frame_inputs_add);
        label_supp_notes->setObjectName("label_supp_notes");

        formLayout_add_supp->setWidget(8, QFormLayout::LabelRole, label_supp_notes);

        txtNotes = new QPlainTextEdit(frame_inputs_add);
        txtNotes->setObjectName("txtNotes");
        txtNotes->setMaximumHeight(80);

        formLayout_add_supp->setWidget(8, QFormLayout::FieldRole, txtNotes);


        layout_form_add->addWidget(frame_inputs_add);

        layout_button_add = new QHBoxLayout();
        layout_button_add->setObjectName("layout_button_add");
        horizontalSpacer_add = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        layout_button_add->addItem(horizontalSpacer_add);

        btnEnregistrer = new QPushButton(frame_add_supp);
        btnEnregistrer->setObjectName("btnEnregistrer");

        layout_button_add->addWidget(btnEnregistrer);

        btnReinitialiser = new QPushButton(frame_add_supp);
        btnReinitialiser->setObjectName("btnReinitialiser");

        layout_button_add->addWidget(btnReinitialiser);


        layout_form_add->addLayout(layout_button_add);


        verticalLayout_supp_add->addWidget(frame_add_supp);

        spacer_add = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_supp_add->addItem(spacer_add);

        stackedWidget_suppliers->addWidget(page_supp_add);
        page_supp_update = new QWidget();
        page_supp_update->setObjectName("page_supp_update");
        verticalLayout_supp_update = new QVBoxLayout(page_supp_update);
        verticalLayout_supp_update->setSpacing(0);
        verticalLayout_supp_update->setObjectName("verticalLayout_supp_update");
        verticalLayout_supp_update->setContentsMargins(20, 20, 20, 20);
        frame_update_supp = new QFrame(page_supp_update);
        frame_update_supp->setObjectName("frame_update_supp");
        layout_form_mod = new QVBoxLayout(frame_update_supp);
        layout_form_mod->setObjectName("layout_form_mod");
        lblFormTitle_mod = new QLabel(frame_update_supp);
        lblFormTitle_mod->setObjectName("lblFormTitle_mod");

        layout_form_mod->addWidget(lblFormTitle_mod);

        search_layout_mod = new QHBoxLayout();
        search_layout_mod->setSpacing(10);
        search_layout_mod->setObjectName("search_layout_mod");
        label_search_supp = new QLabel(frame_update_supp);
        label_search_supp->setObjectName("label_search_supp");

        search_layout_mod->addWidget(label_search_supp);

        txtRechercheModifier = new QLineEdit(frame_update_supp);
        txtRechercheModifier->setObjectName("txtRechercheModifier");

        search_layout_mod->addWidget(txtRechercheModifier);

        btnChercherFournisseur = new QPushButton(frame_update_supp);
        btnChercherFournisseur->setObjectName("btnChercherFournisseur");

        search_layout_mod->addWidget(btnChercherFournisseur);


        layout_form_mod->addLayout(search_layout_mod);

        frame_inputs_mod = new QFrame(frame_update_supp);
        frame_inputs_mod->setObjectName("frame_inputs_mod");
        formLayout_update_supp = new QFormLayout(frame_inputs_mod);
        formLayout_update_supp->setSpacing(10);
        formLayout_update_supp->setObjectName("formLayout_update_supp");
        label_mod_1 = new QLabel(frame_inputs_mod);
        label_mod_1->setObjectName("label_mod_1");

        formLayout_update_supp->setWidget(0, QFormLayout::LabelRole, label_mod_1);

        txtNomMod = new QLineEdit(frame_inputs_mod);
        txtNomMod->setObjectName("txtNomMod");

        formLayout_update_supp->setWidget(0, QFormLayout::FieldRole, txtNomMod);

        label_mod_2 = new QLabel(frame_inputs_mod);
        label_mod_2->setObjectName("label_mod_2");

        formLayout_update_supp->setWidget(1, QFormLayout::LabelRole, label_mod_2);

        txtTypeMod = new QLineEdit(frame_inputs_mod);
        txtTypeMod->setObjectName("txtTypeMod");

        formLayout_update_supp->setWidget(1, QFormLayout::FieldRole, txtTypeMod);

        label_mod_3 = new QLabel(frame_inputs_mod);
        label_mod_3->setObjectName("label_mod_3");

        formLayout_update_supp->setWidget(2, QFormLayout::LabelRole, label_mod_3);

        txtAdresseMod = new QLineEdit(frame_inputs_mod);
        txtAdresseMod->setObjectName("txtAdresseMod");

        formLayout_update_supp->setWidget(2, QFormLayout::FieldRole, txtAdresseMod);

        label_mod_4 = new QLabel(frame_inputs_mod);
        label_mod_4->setObjectName("label_mod_4");

        formLayout_update_supp->setWidget(3, QFormLayout::LabelRole, label_mod_4);

        txtTelephoneMod = new QLineEdit(frame_inputs_mod);
        txtTelephoneMod->setObjectName("txtTelephoneMod");

        formLayout_update_supp->setWidget(3, QFormLayout::FieldRole, txtTelephoneMod);

        label_mod_5 = new QLabel(frame_inputs_mod);
        label_mod_5->setObjectName("label_mod_5");

        formLayout_update_supp->setWidget(4, QFormLayout::LabelRole, label_mod_5);

        txtEmailMod = new QLineEdit(frame_inputs_mod);
        txtEmailMod->setObjectName("txtEmailMod");

        formLayout_update_supp->setWidget(4, QFormLayout::FieldRole, txtEmailMod);

        label_mod_6 = new QLabel(frame_inputs_mod);
        label_mod_6->setObjectName("label_mod_6");

        formLayout_update_supp->setWidget(5, QFormLayout::LabelRole, label_mod_6);

        spinDelaiMod = new QSpinBox(frame_inputs_mod);
        spinDelaiMod->setObjectName("spinDelaiMod");
        spinDelaiMod->setMinimum(1);
        spinDelaiMod->setMaximum(365);

        formLayout_update_supp->setWidget(5, QFormLayout::FieldRole, spinDelaiMod);

        label_mod_7 = new QLabel(frame_inputs_mod);
        label_mod_7->setObjectName("label_mod_7");

        formLayout_update_supp->setWidget(6, QFormLayout::LabelRole, label_mod_7);

        spinPrixMod = new QDoubleSpinBox(frame_inputs_mod);
        spinPrixMod->setObjectName("spinPrixMod");
        spinPrixMod->setMinimum(0.010000000000000);
        spinPrixMod->setMaximum(999999.989999999990687);

        formLayout_update_supp->setWidget(6, QFormLayout::FieldRole, spinPrixMod);

        label_mod_8 = new QLabel(frame_inputs_mod);
        label_mod_8->setObjectName("label_mod_8");

        formLayout_update_supp->setWidget(7, QFormLayout::LabelRole, label_mod_8);

        comboStatutMod = new QComboBox(frame_inputs_mod);
        comboStatutMod->addItem(QString());
        comboStatutMod->addItem(QString());
        comboStatutMod->setObjectName("comboStatutMod");

        formLayout_update_supp->setWidget(7, QFormLayout::FieldRole, comboStatutMod);

        label_mod_notes = new QLabel(frame_inputs_mod);
        label_mod_notes->setObjectName("label_mod_notes");

        formLayout_update_supp->setWidget(8, QFormLayout::LabelRole, label_mod_notes);

        txtNotesMod = new QPlainTextEdit(frame_inputs_mod);
        txtNotesMod->setObjectName("txtNotesMod");
        txtNotesMod->setMaximumHeight(80);

        formLayout_update_supp->setWidget(8, QFormLayout::FieldRole, txtNotesMod);


        layout_form_mod->addWidget(frame_inputs_mod);

        layout_button_update = new QHBoxLayout();
        layout_button_update->setObjectName("layout_button_update");
        horizontalSpacer_mod = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        layout_button_update->addItem(horizontalSpacer_mod);

        btnMettreAJour = new QPushButton(frame_update_supp);
        btnMettreAJour->setObjectName("btnMettreAJour");

        layout_button_update->addWidget(btnMettreAJour);


        layout_form_mod->addLayout(layout_button_update);


        verticalLayout_supp_update->addWidget(frame_update_supp);

        spacer_mod = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_supp_update->addItem(spacer_mod);

        stackedWidget_suppliers->addWidget(page_supp_update);
        page_supp_delete = new QWidget();
        page_supp_delete->setObjectName("page_supp_delete");
        verticalLayout_supp_delete = new QVBoxLayout(page_supp_delete);
        verticalLayout_supp_delete->setSpacing(0);
        verticalLayout_supp_delete->setObjectName("verticalLayout_supp_delete");
        verticalLayout_supp_delete->setContentsMargins(20, 20, 20, 20);
        frame_delete_supp = new QFrame(page_supp_delete);
        frame_delete_supp->setObjectName("frame_delete_supp");
        layout_delete_supp = new QVBoxLayout(frame_delete_supp);
        layout_delete_supp->setObjectName("layout_delete_supp");
        lblFormTitle_del = new QLabel(frame_delete_supp);
        lblFormTitle_del->setObjectName("lblFormTitle_del");

        layout_delete_supp->addWidget(lblFormTitle_del);

        search_layout_del = new QHBoxLayout();
        search_layout_del->setSpacing(10);
        search_layout_del->setObjectName("search_layout_del");
        lblSupprimer = new QLabel(frame_delete_supp);
        lblSupprimer->setObjectName("lblSupprimer");

        search_layout_del->addWidget(lblSupprimer);

        txtSupprimer = new QLineEdit(frame_delete_supp);
        txtSupprimer->setObjectName("txtSupprimer");

        search_layout_del->addWidget(txtSupprimer);

        btnChercherSupprimer = new QPushButton(frame_delete_supp);
        btnChercherSupprimer->setObjectName("btnChercherSupprimer");

        search_layout_del->addWidget(btnChercherSupprimer);


        layout_delete_supp->addLayout(search_layout_del);

        frame_confirm_del = new QFrame(frame_delete_supp);
        frame_confirm_del->setObjectName("frame_confirm_del");
        layout_confirm_del = new QVBoxLayout(frame_confirm_del);
        layout_confirm_del->setObjectName("layout_confirm_del");
        lblValiderSupp = new QLabel(frame_confirm_del);
        lblValiderSupp->setObjectName("lblValiderSupp");

        layout_confirm_del->addWidget(lblValiderSupp);

        lblDetailSupp = new QLabel(frame_confirm_del);
        lblDetailSupp->setObjectName("lblDetailSupp");

        layout_confirm_del->addWidget(lblDetailSupp);


        layout_delete_supp->addWidget(frame_confirm_del);

        lblWarning = new QLabel(frame_delete_supp);
        lblWarning->setObjectName("lblWarning");

        layout_delete_supp->addWidget(lblWarning);

        spacer_delete = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        layout_delete_supp->addItem(spacer_delete);

        layout_buttons_del = new QHBoxLayout();
        layout_buttons_del->setObjectName("layout_buttons_del");
        horizontalSpacer_del = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        layout_buttons_del->addItem(horizontalSpacer_del);

        btnAnnulerSupp = new QPushButton(frame_delete_supp);
        btnAnnulerSupp->setObjectName("btnAnnulerSupp");

        layout_buttons_del->addWidget(btnAnnulerSupp);

        btnSupprimerDef = new QPushButton(frame_delete_supp);
        btnSupprimerDef->setObjectName("btnSupprimerDef");

        layout_buttons_del->addWidget(btnSupprimerDef);


        layout_delete_supp->addLayout(layout_buttons_del);


        verticalLayout_supp_delete->addWidget(frame_delete_supp);

        stackedWidget_suppliers->addWidget(page_supp_delete);
        page_supp_list = new QWidget();
        page_supp_list->setObjectName("page_supp_list");
        verticalLayout_supp_list = new QVBoxLayout(page_supp_list);
        verticalLayout_supp_list->setSpacing(0);
        verticalLayout_supp_list->setObjectName("verticalLayout_supp_list");
        verticalLayout_supp_list->setContentsMargins(20, 20, 20, 20);
        frame_list_supp = new QFrame(page_supp_list);
        frame_list_supp->setObjectName("frame_list_supp");
        layout_list_supp = new QVBoxLayout(frame_list_supp);
        layout_list_supp->setObjectName("layout_list_supp");
        lblListeTitle = new QLabel(frame_list_supp);
        lblListeTitle->setObjectName("lblListeTitle");

        layout_list_supp->addWidget(lblListeTitle);

        filterLayout = new QHBoxLayout();
        filterLayout->setSpacing(10);
        filterLayout->setObjectName("filterLayout");
        txtFilterNom = new QLineEdit(frame_list_supp);
        txtFilterNom->setObjectName("txtFilterNom");

        filterLayout->addWidget(txtFilterNom);

        txtFilterType = new QLineEdit(frame_list_supp);
        txtFilterType->setObjectName("txtFilterType");

        filterLayout->addWidget(txtFilterType);

        txtFilterStatut = new QLineEdit(frame_list_supp);
        txtFilterStatut->setObjectName("txtFilterStatut");

        filterLayout->addWidget(txtFilterStatut);

        btnRechercher = new QPushButton(frame_list_supp);
        btnRechercher->setObjectName("btnRechercher");

        filterLayout->addWidget(btnRechercher);

        btnRinitialiserFiltres = new QPushButton(frame_list_supp);
        btnRinitialiserFiltres->setObjectName("btnRinitialiserFiltres");

        filterLayout->addWidget(btnRinitialiserFiltres);


        layout_list_supp->addLayout(filterLayout);

        tableFournisseurs = new QTableWidget(frame_list_supp);
        tableFournisseurs->setObjectName("tableFournisseurs");
        tableFournisseurs->setColumnCount(8);

        layout_list_supp->addWidget(tableFournisseurs);

        actionsTableLayout = new QHBoxLayout();
        actionsTableLayout->setSpacing(10);
        actionsTableLayout->setObjectName("actionsTableLayout");
        lblSelectedSupplier = new QLabel(frame_list_supp);
        lblSelectedSupplier->setObjectName("lblSelectedSupplier");

        actionsTableLayout->addWidget(lblSelectedSupplier);

        horizontalSpacer_actions = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        actionsTableLayout->addItem(horizontalSpacer_actions);

        btnModifierDuTableau = new QPushButton(frame_list_supp);
        btnModifierDuTableau->setObjectName("btnModifierDuTableau");
        btnModifierDuTableau->setEnabled(false);

        actionsTableLayout->addWidget(btnModifierDuTableau);

        btnSupprimerDuTableau = new QPushButton(frame_list_supp);
        btnSupprimerDuTableau->setObjectName("btnSupprimerDuTableau");
        btnSupprimerDuTableau->setEnabled(false);

        actionsTableLayout->addWidget(btnSupprimerDuTableau);


        layout_list_supp->addLayout(actionsTableLayout);

        statsLayout = new QHBoxLayout();
        statsLayout->setSpacing(15);
        statsLayout->setObjectName("statsLayout");
        frame_stat_total = new QFrame(frame_list_supp);
        frame_stat_total->setObjectName("frame_stat_total");
        vboxLayout = new QVBoxLayout(frame_stat_total);
        vboxLayout->setObjectName("vboxLayout");
        lblStatTotalLabel = new QLabel(frame_stat_total);
        lblStatTotalLabel->setObjectName("lblStatTotalLabel");

        vboxLayout->addWidget(lblStatTotalLabel);

        lblStatTotal = new QLabel(frame_stat_total);
        lblStatTotal->setObjectName("lblStatTotal");

        vboxLayout->addWidget(lblStatTotal);


        statsLayout->addWidget(frame_stat_total);

        frame_stat_actifs = new QFrame(frame_list_supp);
        frame_stat_actifs->setObjectName("frame_stat_actifs");
        vboxLayout1 = new QVBoxLayout(frame_stat_actifs);
        vboxLayout1->setObjectName("vboxLayout1");
        lblStatActifsLabel = new QLabel(frame_stat_actifs);
        lblStatActifsLabel->setObjectName("lblStatActifsLabel");

        vboxLayout1->addWidget(lblStatActifsLabel);

        lblStatActifs = new QLabel(frame_stat_actifs);
        lblStatActifs->setObjectName("lblStatActifs");

        vboxLayout1->addWidget(lblStatActifs);


        statsLayout->addWidget(frame_stat_actifs);

        frame_stat_prix = new QFrame(frame_list_supp);
        frame_stat_prix->setObjectName("frame_stat_prix");
        vboxLayout2 = new QVBoxLayout(frame_stat_prix);
        vboxLayout2->setObjectName("vboxLayout2");
        lblStatPrixLabel = new QLabel(frame_stat_prix);
        lblStatPrixLabel->setObjectName("lblStatPrixLabel");

        vboxLayout2->addWidget(lblStatPrixLabel);

        lblStatPrix = new QLabel(frame_stat_prix);
        lblStatPrix->setObjectName("lblStatPrix");

        vboxLayout2->addWidget(lblStatPrix);


        statsLayout->addWidget(frame_stat_prix);

        frame_stat_delai = new QFrame(frame_list_supp);
        frame_stat_delai->setObjectName("frame_stat_delai");
        vboxLayout3 = new QVBoxLayout(frame_stat_delai);
        vboxLayout3->setObjectName("vboxLayout3");
        lblStatDelaiLabel = new QLabel(frame_stat_delai);
        lblStatDelaiLabel->setObjectName("lblStatDelaiLabel");

        vboxLayout3->addWidget(lblStatDelaiLabel);

        lblStatDelai = new QLabel(frame_stat_delai);
        lblStatDelai->setObjectName("lblStatDelai");

        vboxLayout3->addWidget(lblStatDelai);


        statsLayout->addWidget(frame_stat_delai);


        layout_list_supp->addLayout(statsLayout);


        verticalLayout_supp_list->addWidget(frame_list_supp);

        stackedWidget_suppliers->addWidget(page_supp_list);

        verticalLayout_suppliers->addWidget(stackedWidget_suppliers);

        stackedWidget->addWidget(page_suppliers_2);
        page_orders = new QWidget();
        page_orders->setObjectName("page_orders");
        verticalLayout_orders = new QVBoxLayout(page_orders);
        verticalLayout_orders->setSpacing(0);
        verticalLayout_orders->setObjectName("verticalLayout_orders");
        verticalLayout_orders->setContentsMargins(0, 0, 0, 0);
        top_nav = new QFrame(page_orders);
        top_nav->setObjectName("top_nav");
        navLayout = new QHBoxLayout(top_nav);
        navLayout->setObjectName("navLayout");
        btn_tab_add = new QPushButton(top_nav);
        btn_tab_add->setObjectName("btn_tab_add");
        btn_tab_add->setCheckable(true);

        navLayout->addWidget(btn_tab_add);

        btn_tab_update = new QPushButton(top_nav);
        btn_tab_update->setObjectName("btn_tab_update");
        btn_tab_update->setCheckable(true);

        navLayout->addWidget(btn_tab_update);

        btn_tab_delete = new QPushButton(top_nav);
        btn_tab_delete->setObjectName("btn_tab_delete");
        btn_tab_delete->setCheckable(true);

        navLayout->addWidget(btn_tab_delete);

        btn_tab_list = new QPushButton(top_nav);
        btn_tab_list->setObjectName("btn_tab_list");
        btn_tab_list->setCheckable(true);

        navLayout->addWidget(btn_tab_list);


        verticalLayout_orders->addWidget(top_nav);

        stackedWidget_orders = new QStackedWidget(page_orders);
        stackedWidget_orders->setObjectName("stackedWidget_orders");
        page_add = new QWidget();
        page_add->setObjectName("page_add");
        pageAddLayout = new QVBoxLayout(page_add);
        pageAddLayout->setObjectName("pageAddLayout");
        form_container = new QFrame(page_add);
        form_container->setObjectName("form_container");
        addForm = new QFormLayout(form_container);
        addForm->setObjectName("addForm");
        addForm->setHorizontalSpacing(25);
        addForm->setVerticalSpacing(18);
        label = new QLabel(form_container);
        label->setObjectName("label");

        addForm->setWidget(0, QFormLayout::LabelRole, label);

        le_id = new QLineEdit(form_container);
        le_id->setObjectName("le_id");

        addForm->setWidget(0, QFormLayout::FieldRole, le_id);

        label1 = new QLabel(form_container);
        label1->setObjectName("label1");

        addForm->setWidget(1, QFormLayout::LabelRole, label1);

        cb_article_type = new QComboBox(form_container);
        cb_article_type->addItem(QString());
        cb_article_type->addItem(QString());
        cb_article_type->addItem(QString());
        cb_article_type->addItem(QString());
        cb_article_type->addItem(QString());
        cb_article_type->addItem(QString());
        cb_article_type->setObjectName("cb_article_type");

        addForm->setWidget(1, QFormLayout::FieldRole, cb_article_type);

        label2 = new QLabel(form_container);
        label2->setObjectName("label2");

        addForm->setWidget(2, QFormLayout::LabelRole, label2);

        sb_order_qty = new QSpinBox(form_container);
        sb_order_qty->setObjectName("sb_order_qty");
        sb_order_qty->setMaximum(1000000);

        addForm->setWidget(2, QFormLayout::FieldRole, sb_order_qty);

        label3 = new QLabel(form_container);
        label3->setObjectName("label3");

        addForm->setWidget(3, QFormLayout::LabelRole, label3);

        le_client_email = new QLineEdit(form_container);
        le_client_email->setObjectName("le_client_email");

        addForm->setWidget(3, QFormLayout::FieldRole, le_client_email);

        label4 = new QLabel(form_container);
        label4->setObjectName("label4");

        addForm->setWidget(4, QFormLayout::LabelRole, label4);

        cb_client_city = new QComboBox(form_container);
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->addItem(QString());
        cb_client_city->setObjectName("cb_client_city");

        addForm->setWidget(4, QFormLayout::FieldRole, cb_client_city);

        label5 = new QLabel(form_container);
        label5->setObjectName("label5");

        addForm->setWidget(5, QFormLayout::LabelRole, label5);

        label6 = new QLabel(form_container);
        label6->setObjectName("label6");

        addForm->setWidget(6, QFormLayout::LabelRole, label6);

        de_date = new QDateEdit(form_container);
        de_date->setObjectName("de_date");
        de_date->setCalendarPopup(true);

        addForm->setWidget(6, QFormLayout::FieldRole, de_date);

        label7 = new QLabel(form_container);
        label7->setObjectName("label7");

        addForm->setWidget(7, QFormLayout::LabelRole, label7);

        de_date_delivery = new QDateEdit(form_container);
        de_date_delivery->setObjectName("de_date_delivery");
        de_date_delivery->setCalendarPopup(true);

        addForm->setWidget(7, QFormLayout::FieldRole, de_date_delivery);

        label8 = new QLabel(form_container);
        label8->setObjectName("label8");

        addForm->setWidget(9, QFormLayout::LabelRole, label8);

        dsb_price = new QDoubleSpinBox(form_container);
        dsb_price->setObjectName("dsb_price");
        dsb_price->setMaximum(100000000.000000000000000);

        addForm->setWidget(9, QFormLayout::FieldRole, dsb_price);

        label9 = new QLabel(form_container);
        label9->setObjectName("label9");

        addForm->setWidget(10, QFormLayout::LabelRole, label9);

        le_proof = new QLineEdit(form_container);
        le_proof->setObjectName("le_proof");

        addForm->setWidget(10, QFormLayout::FieldRole, le_proof);

        btn_valider = new QPushButton(form_container);
        btn_valider->setObjectName("btn_valider");

        addForm->setWidget(11, QFormLayout::FieldRole, btn_valider);

        cb_status = new QComboBox(form_container);
        cb_status->addItem(QString());
        cb_status->addItem(QString());
        cb_status->addItem(QString());
        cb_status->addItem(QString());
        cb_status->addItem(QString());
        cb_status->setObjectName("cb_status");

        addForm->setWidget(8, QFormLayout::FieldRole, cb_status);


        pageAddLayout->addWidget(form_container);

        vSpacerAdd = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        pageAddLayout->addItem(vSpacerAdd);

        stackedWidget_orders->addWidget(page_add);
        page_update = new QWidget();
        page_update->setObjectName("page_update");
        pageUpdateLayout = new QVBoxLayout(page_update);
        pageUpdateLayout->setObjectName("pageUpdateLayout");
        update_container = new QFrame(page_update);
        update_container->setObjectName("update_container");
        updateVLayout = new QVBoxLayout(update_container);
        updateVLayout->setObjectName("updateVLayout");
        searchUpdateLayout = new QHBoxLayout();
        searchUpdateLayout->setObjectName("searchUpdateLayout");
        le_id_update_search = new QLineEdit(update_container);
        le_id_update_search->setObjectName("le_id_update_search");

        searchUpdateLayout->addWidget(le_id_update_search);

        btn_search_update = new QPushButton(update_container);
        btn_search_update->setObjectName("btn_search_update");

        searchUpdateLayout->addWidget(btn_search_update);


        updateVLayout->addLayout(searchUpdateLayout);

        line_update = new QFrame(update_container);
        line_update->setObjectName("line_update");
        line_update->setFrameShape(QFrame::Shape::HLine);
        line_update->setFrameShadow(QFrame::Shadow::Sunken);

        updateVLayout->addWidget(line_update);

        updateForm = new QFormLayout();
        updateForm->setObjectName("updateForm");
        updateForm->setHorizontalSpacing(25);
        updateForm->setVerticalSpacing(18);
        updateForm->setContentsMargins(0, 0, 0, 0);
        label10 = new QLabel(update_container);
        label10->setObjectName("label10");

        updateForm->setWidget(0, QFormLayout::LabelRole, label10);

        le_id_update = new QLineEdit(update_container);
        le_id_update->setObjectName("le_id_update");
        le_id_update->setReadOnly(true);

        updateForm->setWidget(0, QFormLayout::FieldRole, le_id_update);

        label11 = new QLabel(update_container);
        label11->setObjectName("label11");

        updateForm->setWidget(1, QFormLayout::LabelRole, label11);

        cb_type_update = new QComboBox(update_container);
        cb_type_update->addItem(QString());
        cb_type_update->addItem(QString());
        cb_type_update->addItem(QString());
        cb_type_update->addItem(QString());
        cb_type_update->addItem(QString());
        cb_type_update->addItem(QString());
        cb_type_update->setObjectName("cb_type_update");

        updateForm->setWidget(1, QFormLayout::FieldRole, cb_type_update);

        label12 = new QLabel(update_container);
        label12->setObjectName("label12");

        updateForm->setWidget(2, QFormLayout::LabelRole, label12);

        sb_qty_update = new QSpinBox(update_container);
        sb_qty_update->setObjectName("sb_qty_update");
        sb_qty_update->setMaximum(1000000);

        updateForm->setWidget(2, QFormLayout::FieldRole, sb_qty_update);

        label13 = new QLabel(update_container);
        label13->setObjectName("label13");

        updateForm->setWidget(3, QFormLayout::LabelRole, label13);

        le_email_update = new QLineEdit(update_container);
        le_email_update->setObjectName("le_email_update");

        updateForm->setWidget(3, QFormLayout::FieldRole, le_email_update);

        label14 = new QLabel(update_container);
        label14->setObjectName("label14");

        updateForm->setWidget(4, QFormLayout::LabelRole, label14);

        cb_city_update = new QComboBox(update_container);
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->addItem(QString());
        cb_city_update->setObjectName("cb_city_update");

        updateForm->setWidget(4, QFormLayout::FieldRole, cb_city_update);

        label15 = new QLabel(update_container);
        label15->setObjectName("label15");

        updateForm->setWidget(5, QFormLayout::LabelRole, label15);

        cb_status_update = new QComboBox(update_container);
        cb_status_update->addItem(QString());
        cb_status_update->addItem(QString());
        cb_status_update->addItem(QString());
        cb_status_update->addItem(QString());
        cb_status_update->addItem(QString());
        cb_status_update->setObjectName("cb_status_update");

        updateForm->setWidget(5, QFormLayout::FieldRole, cb_status_update);

        label16 = new QLabel(update_container);
        label16->setObjectName("label16");

        updateForm->setWidget(6, QFormLayout::LabelRole, label16);

        de_date_update = new QDateEdit(update_container);
        de_date_update->setObjectName("de_date_update");
        de_date_update->setCalendarPopup(true);

        updateForm->setWidget(6, QFormLayout::FieldRole, de_date_update);

        label17 = new QLabel(update_container);
        label17->setObjectName("label17");

        updateForm->setWidget(7, QFormLayout::LabelRole, label17);

        de_date_delivery_update = new QDateEdit(update_container);
        de_date_delivery_update->setObjectName("de_date_delivery_update");
        de_date_delivery_update->setCalendarPopup(true);

        updateForm->setWidget(7, QFormLayout::FieldRole, de_date_delivery_update);

        label18 = new QLabel(update_container);
        label18->setObjectName("label18");

        updateForm->setWidget(8, QFormLayout::LabelRole, label18);

        dsb_price_update = new QDoubleSpinBox(update_container);
        dsb_price_update->setObjectName("dsb_price_update");
        dsb_price_update->setMaximum(100000000.000000000000000);

        updateForm->setWidget(8, QFormLayout::FieldRole, dsb_price_update);

        label19 = new QLabel(update_container);
        label19->setObjectName("label19");

        updateForm->setWidget(9, QFormLayout::LabelRole, label19);

        le_proof_update = new QLineEdit(update_container);
        le_proof_update->setObjectName("le_proof_update");

        updateForm->setWidget(9, QFormLayout::FieldRole, le_proof_update);

        btn_update_confirm = new QPushButton(update_container);
        btn_update_confirm->setObjectName("btn_update_confirm");

        updateForm->setWidget(10, QFormLayout::FieldRole, btn_update_confirm);


        updateVLayout->addLayout(updateForm);


        pageUpdateLayout->addWidget(update_container);

        vSpacerUpdate = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        pageUpdateLayout->addItem(vSpacerUpdate);

        stackedWidget_orders->addWidget(page_update);
        page_delete = new QWidget();
        page_delete->setObjectName("page_delete");
        pageDeleteLayout = new QVBoxLayout(page_delete);
        pageDeleteLayout->setObjectName("pageDeleteLayout");
        table_container = new QFrame(page_delete);
        table_container->setObjectName("table_container");
        tableVLayout = new QVBoxLayout(table_container);
        tableVLayout->setObjectName("tableVLayout");
        searchDeleteLayout = new QHBoxLayout();
        searchDeleteLayout->setObjectName("searchDeleteLayout");
        le_id_to_delete = new QLineEdit(table_container);
        le_id_to_delete->setObjectName("le_id_to_delete");

        searchDeleteLayout->addWidget(le_id_to_delete);

        btn_delete_action = new QPushButton(table_container);
        btn_delete_action->setObjectName("btn_delete_action");

        searchDeleteLayout->addWidget(btn_delete_action);


        tableVLayout->addLayout(searchDeleteLayout);

        table_delete = new QTableWidget(table_container);
        if (table_delete->columnCount() < 5)
            table_delete->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        table_delete->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        table_delete->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        table_delete->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        table_delete->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        table_delete->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        table_delete->setObjectName("table_delete");
        table_delete->setColumnCount(5);

        tableVLayout->addWidget(table_delete);


        pageDeleteLayout->addWidget(table_container);

        stackedWidget_orders->addWidget(page_delete);
        page_list = new QWidget();
        page_list->setObjectName("page_list");
        pageListLayout = new QVBoxLayout(page_list);
        pageListLayout->setObjectName("pageListLayout");
        tabWidget_stats = new QTabWidget(page_list);
        tabWidget_stats->setObjectName("tabWidget_stats");
        tab_data = new QWidget();
        tab_data->setObjectName("tab_data");
        tabDataLayout = new QVBoxLayout(tab_data);
        tabDataLayout->setObjectName("tabDataLayout");
        toolbarLayout = new QHBoxLayout();
        toolbarLayout->setObjectName("toolbarLayout");
        le_search = new QLineEdit(tab_data);
        le_search->setObjectName("le_search");

        toolbarLayout->addWidget(le_search);

        cb_sort = new QComboBox(tab_data);
        cb_sort->addItem(QString());
        cb_sort->addItem(QString());
        cb_sort->addItem(QString());
        cb_sort->setObjectName("cb_sort");

        toolbarLayout->addWidget(cb_sort);

        btn_pdf = new QPushButton(tab_data);
        btn_pdf->setObjectName("btn_pdf");

        toolbarLayout->addWidget(btn_pdf);


        tabDataLayout->addLayout(toolbarLayout);

        table_list = new QTableWidget(tab_data);
        if (table_list->columnCount() < 10)
            table_list->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        table_list->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        table_list->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        table_list->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        table_list->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        table_list->setHorizontalHeaderItem(4, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        table_list->setHorizontalHeaderItem(5, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        table_list->setHorizontalHeaderItem(6, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        table_list->setHorizontalHeaderItem(7, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        table_list->setHorizontalHeaderItem(8, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        table_list->setHorizontalHeaderItem(9, __qtablewidgetitem14);
        table_list->setObjectName("table_list");
        table_list->setColumnCount(10);

        tabDataLayout->addWidget(table_list);

        tabWidget_stats->addTab(tab_data, QString());
        tab_stats = new QWidget();
        tab_stats->setObjectName("tab_stats");
        statsGridLayout = new QGridLayout(tab_stats);
        statsGridLayout->setObjectName("statsGridLayout");
        stat_card_total = new QFrame(tab_stats);
        stat_card_total->setObjectName("stat_card_total");
        vboxTotal = new QVBoxLayout(stat_card_total);
        vboxTotal->setObjectName("vboxTotal");
        lbl_total_orders_title = new QLabel(stat_card_total);
        lbl_total_orders_title->setObjectName("lbl_total_orders_title");

        vboxTotal->addWidget(lbl_total_orders_title);

        lbl_total_orders_val = new QLabel(stat_card_total);
        lbl_total_orders_val->setObjectName("lbl_total_orders_val");
        lbl_total_orders_val->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxTotal->addWidget(lbl_total_orders_val);


        statsGridLayout->addWidget(stat_card_total, 0, 0, 1, 1);

        stat_card_top = new QFrame(tab_stats);
        stat_card_top->setObjectName("stat_card_top");
        vboxTop = new QVBoxLayout(stat_card_top);
        vboxTop->setObjectName("vboxTop");
        lbl_top_client_title = new QLabel(stat_card_top);
        lbl_top_client_title->setObjectName("lbl_top_client_title");

        vboxTop->addWidget(lbl_top_client_title);

        lbl_top_client_val = new QLabel(stat_card_top);
        lbl_top_client_val->setObjectName("lbl_top_client_val");
        lbl_top_client_val->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxTop->addWidget(lbl_top_client_val);


        statsGridLayout->addWidget(stat_card_top, 0, 1, 1, 1);

        chart_container = new QFrame(tab_stats);
        chart_container->setObjectName("chart_container");
        chartLayout = new QVBoxLayout(chart_container);
        chartLayout->setObjectName("chartLayout");

        statsGridLayout->addWidget(chart_container, 1, 0, 1, 2);

        tabWidget_stats->addTab(tab_stats, QString());

        pageListLayout->addWidget(tabWidget_stats);

        stackedWidget_orders->addWidget(page_list);

        verticalLayout_orders->addWidget(stackedWidget_orders);

        stackedWidget->addWidget(page_orders);
        page_material = new QWidget();
        page_material->setObjectName("page_material");
        verticalLayout_material = new QVBoxLayout(page_material);
        verticalLayout_material->setSpacing(0);
        verticalLayout_material->setObjectName("verticalLayout_material");
        verticalLayout_material->setContentsMargins(0, 0, 0, 0);
        top_nav_materials = new QFrame(page_material);
        top_nav_materials->setObjectName("top_nav_materials");
        navLayout_materials = new QHBoxLayout(top_nav_materials);
        navLayout_materials->setObjectName("navLayout_materials");
        btn_tab_mat_add = new QPushButton(top_nav_materials);
        btn_tab_mat_add->setObjectName("btn_tab_mat_add");
        btn_tab_mat_add->setCheckable(true);

        navLayout_materials->addWidget(btn_tab_mat_add);

        btn_tab_mat_update = new QPushButton(top_nav_materials);
        btn_tab_mat_update->setObjectName("btn_tab_mat_update");
        btn_tab_mat_update->setCheckable(true);

        navLayout_materials->addWidget(btn_tab_mat_update);

        btn_tab_mat_delete = new QPushButton(top_nav_materials);
        btn_tab_mat_delete->setObjectName("btn_tab_mat_delete");
        btn_tab_mat_delete->setCheckable(true);

        navLayout_materials->addWidget(btn_tab_mat_delete);

        btn_tab_mat_list = new QPushButton(top_nav_materials);
        btn_tab_mat_list->setObjectName("btn_tab_mat_list");
        btn_tab_mat_list->setCheckable(true);

        navLayout_materials->addWidget(btn_tab_mat_list);


        verticalLayout_material->addWidget(top_nav_materials);

        stackedWidget_materials = new QStackedWidget(page_material);
        stackedWidget_materials->setObjectName("stackedWidget_materials");
        page_mat_add = new QWidget();
        page_mat_add->setObjectName("page_mat_add");
        verticalLayout_mat_add = new QVBoxLayout(page_mat_add);
        verticalLayout_mat_add->setObjectName("verticalLayout_mat_add");
        frame_add = new QFrame(page_mat_add);
        frame_add->setObjectName("frame_add");
        formLayout_add = new QFormLayout(frame_add);
        formLayout_add->setObjectName("formLayout_add");
        label_mat_1 = new QLabel(frame_add);
        label_mat_1->setObjectName("label_mat_1");

        formLayout_add->setWidget(0, QFormLayout::LabelRole, label_mat_1);

        le_mat_name = new QLineEdit(frame_add);
        le_mat_name->setObjectName("le_mat_name");

        formLayout_add->setWidget(0, QFormLayout::FieldRole, le_mat_name);

        label_mat_2 = new QLabel(frame_add);
        label_mat_2->setObjectName("label_mat_2");

        formLayout_add->setWidget(1, QFormLayout::LabelRole, label_mat_2);

        cb_mat_type = new QComboBox(frame_add);
        cb_mat_type->addItem(QString());
        cb_mat_type->addItem(QString());
        cb_mat_type->addItem(QString());
        cb_mat_type->addItem(QString());
        cb_mat_type->addItem(QString());
        cb_mat_type->setObjectName("cb_mat_type");

        formLayout_add->setWidget(1, QFormLayout::FieldRole, cb_mat_type);

        label_mat_3 = new QLabel(frame_add);
        label_mat_3->setObjectName("label_mat_3");

        formLayout_add->setWidget(2, QFormLayout::LabelRole, label_mat_3);

        dsb_mat_price = new QDoubleSpinBox(frame_add);
        dsb_mat_price->setObjectName("dsb_mat_price");
        dsb_mat_price->setMaximum(1000000.000000000000000);

        formLayout_add->setWidget(2, QFormLayout::FieldRole, dsb_mat_price);

        label_mat_4 = new QLabel(frame_add);
        label_mat_4->setObjectName("label_mat_4");

        formLayout_add->setWidget(3, QFormLayout::LabelRole, label_mat_4);

        sb_mat_qty = new QSpinBox(frame_add);
        sb_mat_qty->setObjectName("sb_mat_qty");
        sb_mat_qty->setMaximum(100000);

        formLayout_add->setWidget(3, QFormLayout::FieldRole, sb_mat_qty);

        label_mat_5 = new QLabel(frame_add);
        label_mat_5->setObjectName("label_mat_5");

        formLayout_add->setWidget(4, QFormLayout::LabelRole, label_mat_5);

        colorLayoutAdd = new QHBoxLayout();
        colorLayoutAdd->setObjectName("colorLayoutAdd");
        le_mat_color = new QLineEdit(frame_add);
        le_mat_color->setObjectName("le_mat_color");

        colorLayoutAdd->addWidget(le_mat_color);

        btn_color_add = new QPushButton(frame_add);
        btn_color_add->setObjectName("btn_color_add");
        btn_color_add->setMaximumSize(QSize(30, 30));

        colorLayoutAdd->addWidget(btn_color_add);


        formLayout_add->setLayout(4, QFormLayout::FieldRole, colorLayoutAdd);

        label_mat_6 = new QLabel(frame_add);
        label_mat_6->setObjectName("label_mat_6");

        formLayout_add->setWidget(5, QFormLayout::LabelRole, label_mat_6);

        dsb_mat_yield = new QDoubleSpinBox(frame_add);
        dsb_mat_yield->setObjectName("dsb_mat_yield");
        dsb_mat_yield->setMaximum(100.000000000000000);

        formLayout_add->setWidget(5, QFormLayout::FieldRole, dsb_mat_yield);

        label_mat_7 = new QLabel(frame_add);
        label_mat_7->setObjectName("label_mat_7");

        formLayout_add->setWidget(6, QFormLayout::LabelRole, label_mat_7);

        dsb_mat_loss = new QDoubleSpinBox(frame_add);
        dsb_mat_loss->setObjectName("dsb_mat_loss");
        dsb_mat_loss->setMaximum(100.000000000000000);

        formLayout_add->setWidget(6, QFormLayout::FieldRole, dsb_mat_loss);

        btn_mat_add = new QPushButton(frame_add);
        btn_mat_add->setObjectName("btn_mat_add");

        formLayout_add->setWidget(7, QFormLayout::FieldRole, btn_mat_add);


        verticalLayout_mat_add->addWidget(frame_add);

        verticalSpacer_mat_add = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_mat_add->addItem(verticalSpacer_mat_add);

        stackedWidget_materials->addWidget(page_mat_add);
        page_mat_update = new QWidget();
        page_mat_update->setObjectName("page_mat_update");
        verticalLayout_mat_upd = new QVBoxLayout(page_mat_update);
        verticalLayout_mat_upd->setObjectName("verticalLayout_mat_upd");
        frame_upd = new QFrame(page_mat_update);
        frame_upd->setObjectName("frame_upd");
        vbox_upd_inner = new QVBoxLayout(frame_upd);
        vbox_upd_inner->setObjectName("vbox_upd_inner");
        hbox_search_upd = new QHBoxLayout();
        hbox_search_upd->setObjectName("hbox_search_upd");
        le_upd_search_name = new QLineEdit(frame_upd);
        le_upd_search_name->setObjectName("le_upd_search_name");

        hbox_search_upd->addWidget(le_upd_search_name);

        btn_upd_load = new QPushButton(frame_upd);
        btn_upd_load->setObjectName("btn_upd_load");

        hbox_search_upd->addWidget(btn_upd_load);


        vbox_upd_inner->addLayout(hbox_search_upd);

        formLayout_upd = new QFormLayout();
        formLayout_upd->setObjectName("formLayout_upd");
        label_mat_10 = new QLabel(frame_upd);
        label_mat_10->setObjectName("label_mat_10");

        formLayout_upd->setWidget(0, QFormLayout::LabelRole, label_mat_10);

        le_upd_name = new QLineEdit(frame_upd);
        le_upd_name->setObjectName("le_upd_name");

        formLayout_upd->setWidget(0, QFormLayout::FieldRole, le_upd_name);

        label_mat_11 = new QLabel(frame_upd);
        label_mat_11->setObjectName("label_mat_11");

        formLayout_upd->setWidget(1, QFormLayout::LabelRole, label_mat_11);

        cb_upd_type = new QComboBox(frame_upd);
        cb_upd_type->addItem(QString());
        cb_upd_type->addItem(QString());
        cb_upd_type->addItem(QString());
        cb_upd_type->addItem(QString());
        cb_upd_type->addItem(QString());
        cb_upd_type->setObjectName("cb_upd_type");

        formLayout_upd->setWidget(1, QFormLayout::FieldRole, cb_upd_type);

        label_mat_12 = new QLabel(frame_upd);
        label_mat_12->setObjectName("label_mat_12");

        formLayout_upd->setWidget(2, QFormLayout::LabelRole, label_mat_12);

        dsb_upd_price = new QDoubleSpinBox(frame_upd);
        dsb_upd_price->setObjectName("dsb_upd_price");
        dsb_upd_price->setMaximum(1000000.000000000000000);

        formLayout_upd->setWidget(2, QFormLayout::FieldRole, dsb_upd_price);

        label_mat_13 = new QLabel(frame_upd);
        label_mat_13->setObjectName("label_mat_13");

        formLayout_upd->setWidget(3, QFormLayout::LabelRole, label_mat_13);

        sb_upd_qty = new QSpinBox(frame_upd);
        sb_upd_qty->setObjectName("sb_upd_qty");
        sb_upd_qty->setMaximum(100000);

        formLayout_upd->setWidget(3, QFormLayout::FieldRole, sb_upd_qty);

        label_mat_14 = new QLabel(frame_upd);
        label_mat_14->setObjectName("label_mat_14");

        formLayout_upd->setWidget(4, QFormLayout::LabelRole, label_mat_14);

        colorLayoutUpd = new QHBoxLayout();
        colorLayoutUpd->setObjectName("colorLayoutUpd");
        le_upd_color = new QLineEdit(frame_upd);
        le_upd_color->setObjectName("le_upd_color");

        colorLayoutUpd->addWidget(le_upd_color);

        btn_color_upd = new QPushButton(frame_upd);
        btn_color_upd->setObjectName("btn_color_upd");
        btn_color_upd->setMaximumSize(QSize(30, 30));

        colorLayoutUpd->addWidget(btn_color_upd);


        formLayout_upd->setLayout(4, QFormLayout::FieldRole, colorLayoutUpd);

        label_mat_15 = new QLabel(frame_upd);
        label_mat_15->setObjectName("label_mat_15");

        formLayout_upd->setWidget(5, QFormLayout::LabelRole, label_mat_15);

        dsb_upd_yield = new QDoubleSpinBox(frame_upd);
        dsb_upd_yield->setObjectName("dsb_upd_yield");
        dsb_upd_yield->setMaximum(100.000000000000000);

        formLayout_upd->setWidget(5, QFormLayout::FieldRole, dsb_upd_yield);

        label_mat_16 = new QLabel(frame_upd);
        label_mat_16->setObjectName("label_mat_16");

        formLayout_upd->setWidget(6, QFormLayout::LabelRole, label_mat_16);

        dsb_upd_loss = new QDoubleSpinBox(frame_upd);
        dsb_upd_loss->setObjectName("dsb_upd_loss");
        dsb_upd_loss->setMaximum(100.000000000000000);

        formLayout_upd->setWidget(6, QFormLayout::FieldRole, dsb_upd_loss);


        vbox_upd_inner->addLayout(formLayout_upd);

        btn_upd_confirm = new QPushButton(frame_upd);
        btn_upd_confirm->setObjectName("btn_upd_confirm");

        vbox_upd_inner->addWidget(btn_upd_confirm);


        verticalLayout_mat_upd->addWidget(frame_upd);

        verticalSpacer_mat_upd = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_mat_upd->addItem(verticalSpacer_mat_upd);

        stackedWidget_materials->addWidget(page_mat_update);
        page_mat_delete = new QWidget();
        page_mat_delete->setObjectName("page_mat_delete");
        verticalLayout_mat_del = new QVBoxLayout(page_mat_delete);
        verticalLayout_mat_del->setObjectName("verticalLayout_mat_del");
        frame_del = new QFrame(page_mat_delete);
        frame_del->setObjectName("frame_del");
        vbox_del = new QVBoxLayout(frame_del);
        vbox_del->setObjectName("vbox_del");
        lbl_del_info = new QLabel(frame_del);
        lbl_del_info->setObjectName("lbl_del_info");

        vbox_del->addWidget(lbl_del_info);

        le_del_name = new QLineEdit(frame_del);
        le_del_name->setObjectName("le_del_name");

        vbox_del->addWidget(le_del_name);

        btn_del_confirm = new QPushButton(frame_del);
        btn_del_confirm->setObjectName("btn_del_confirm");

        vbox_del->addWidget(btn_del_confirm);


        verticalLayout_mat_del->addWidget(frame_del);

        verticalSpacer_mat_del = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_mat_del->addItem(verticalSpacer_mat_del);

        stackedWidget_materials->addWidget(page_mat_delete);
        page_mat_list = new QWidget();
        page_mat_list->setObjectName("page_mat_list");
        verticalLayout_mat_list = new QVBoxLayout(page_mat_list);
        verticalLayout_mat_list->setObjectName("verticalLayout_mat_list");
        tabWidget_mat_stats = new QTabWidget(page_mat_list);
        tabWidget_mat_stats->setObjectName("tabWidget_mat_stats");
        tab_mat_data = new QWidget();
        tab_mat_data->setObjectName("tab_mat_data");
        layout_tab_mat_data = new QVBoxLayout(tab_mat_data);
        layout_tab_mat_data->setObjectName("layout_tab_mat_data");
        hbox_mat_tools = new QHBoxLayout();
        hbox_mat_tools->setObjectName("hbox_mat_tools");
        le_search_mat = new QLineEdit(tab_mat_data);
        le_search_mat->setObjectName("le_search_mat");

        hbox_mat_tools->addWidget(le_search_mat);

        cb_sort_mat = new QComboBox(tab_mat_data);
        cb_sort_mat->addItem(QString());
        cb_sort_mat->addItem(QString());
        cb_sort_mat->addItem(QString());
        cb_sort_mat->addItem(QString());
        cb_sort_mat->setObjectName("cb_sort_mat");

        hbox_mat_tools->addWidget(cb_sort_mat);

        btn_export_mat_pdf = new QPushButton(tab_mat_data);
        btn_export_mat_pdf->setObjectName("btn_export_mat_pdf");

        hbox_mat_tools->addWidget(btn_export_mat_pdf);


        layout_tab_mat_data->addLayout(hbox_mat_tools);

        table_mat = new QTableWidget(tab_mat_data);
        if (table_mat->columnCount() < 7)
            table_mat->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(0, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(1, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(2, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(3, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(4, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(5, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(6, __qtablewidgetitem21);
        table_mat->setObjectName("table_mat");
        table_mat->setColumnCount(7);

        layout_tab_mat_data->addWidget(table_mat);

        tabWidget_mat_stats->addTab(tab_mat_data, QString());
        tab_mat_stats = new QWidget();
        tab_mat_stats->setObjectName("tab_mat_stats");
        gridLayout_mat_stats = new QGridLayout(tab_mat_stats);
        gridLayout_mat_stats->setObjectName("gridLayout_mat_stats");
        stat_card_mat_total = new QFrame(tab_mat_stats);
        stat_card_mat_total->setObjectName("stat_card_mat_total");
        vbox_mat_total = new QVBoxLayout(stat_card_mat_total);
        vbox_mat_total->setObjectName("vbox_mat_total");
        lbl_mat_t1 = new QLabel(stat_card_mat_total);
        lbl_mat_t1->setObjectName("lbl_mat_t1");

        vbox_mat_total->addWidget(lbl_mat_t1);

        lbl_total_mat_val = new QLabel(stat_card_mat_total);
        lbl_total_mat_val->setObjectName("lbl_total_mat_val");
        lbl_total_mat_val->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vbox_mat_total->addWidget(lbl_total_mat_val);


        gridLayout_mat_stats->addWidget(stat_card_mat_total, 0, 0, 1, 1);

        stat_card_rupture = new QFrame(tab_mat_stats);
        stat_card_rupture->setObjectName("stat_card_rupture");
        vbox_rupture = new QVBoxLayout(stat_card_rupture);
        vbox_rupture->setObjectName("vbox_rupture");
        lbl_r1 = new QLabel(stat_card_rupture);
        lbl_r1->setObjectName("lbl_r1");

        vbox_rupture->addWidget(lbl_r1);

        lbl_rupture_val = new QLabel(stat_card_rupture);
        lbl_rupture_val->setObjectName("lbl_rupture_val");
        lbl_rupture_val->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vbox_rupture->addWidget(lbl_rupture_val);


        gridLayout_mat_stats->addWidget(stat_card_rupture, 0, 1, 1, 1);

        stat_card_waste = new QFrame(tab_mat_stats);
        stat_card_waste->setObjectName("stat_card_waste");
        vbox_waste = new QVBoxLayout(stat_card_waste);
        vbox_waste->setObjectName("vbox_waste");
        lbl_w1 = new QLabel(stat_card_waste);
        lbl_w1->setObjectName("lbl_w1");

        vbox_waste->addWidget(lbl_w1);

        lbl_waste_val = new QLabel(stat_card_waste);
        lbl_waste_val->setObjectName("lbl_waste_val");
        lbl_waste_val->setWordWrap(true);

        vbox_waste->addWidget(lbl_waste_val);


        gridLayout_mat_stats->addWidget(stat_card_waste, 1, 0, 1, 2);

        frame_graph = new QFrame(tab_mat_stats);
        frame_graph->setObjectName("frame_graph");
        vbox_graph = new QVBoxLayout(frame_graph);
        vbox_graph->setObjectName("vbox_graph");
        lbl_g1 = new QLabel(frame_graph);
        lbl_g1->setObjectName("lbl_g1");

        vbox_graph->addWidget(lbl_g1);

        lbl_g2 = new QLabel(frame_graph);
        lbl_g2->setObjectName("lbl_g2");

        vbox_graph->addWidget(lbl_g2);

        pb_yield = new QProgressBar(frame_graph);
        pb_yield->setObjectName("pb_yield");
        pb_yield->setValue(0);

        vbox_graph->addWidget(pb_yield);

        lbl_g3 = new QLabel(frame_graph);
        lbl_g3->setObjectName("lbl_g3");

        vbox_graph->addWidget(lbl_g3);

        pb_loss = new QProgressBar(frame_graph);
        pb_loss->setObjectName("pb_loss");
        pb_loss->setValue(0);

        vbox_graph->addWidget(pb_loss);


        gridLayout_mat_stats->addWidget(frame_graph, 2, 0, 1, 2);

        tabWidget_mat_stats->addTab(tab_mat_stats, QString());

        verticalLayout_mat_list->addWidget(tabWidget_mat_stats);

        stackedWidget_materials->addWidget(page_mat_list);

        verticalLayout_material->addWidget(stackedWidget_materials);

        stackedWidget->addWidget(page_material);
        page_employees = new QWidget();
        page_employees->setObjectName("page_employees");
        verticalLayout_employees = new QVBoxLayout(page_employees);
        verticalLayout_employees->setSpacing(0);
        verticalLayout_employees->setObjectName("verticalLayout_employees");
        verticalLayout_employees->setContentsMargins(0, 0, 0, 0);
        lblTitle_employees = new QLabel(page_employees);
        lblTitle_employees->setObjectName("lblTitle_employees");
        lblTitle_employees->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_employees->addWidget(lblTitle_employees);

        stackMain_employees = new QStackedWidget(page_employees);
        stackMain_employees->setObjectName("stackMain_employees");
        pageLogin = new QWidget();
        pageLogin->setObjectName("pageLogin");
        verticalLayout_login_root = new QVBoxLayout(pageLogin);
        verticalLayout_login_root->setSpacing(12);
        verticalLayout_login_root->setObjectName("verticalLayout_login_root");
        verticalLayout_login_root->setContentsMargins(40, 20, 40, 20);
        login_top = new QSpacerItem(20, 30, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_login_root->addItem(login_top);

        cardLogin = new QFrame(pageLogin);
        cardLogin->setObjectName("cardLogin");
        cardLogin->setFrameShape(QFrame::Shape::NoFrame);
        verticalLayout_login = new QVBoxLayout(cardLogin);
        verticalLayout_login->setSpacing(16);
        verticalLayout_login->setObjectName("verticalLayout_login");
        verticalLayout_login->setContentsMargins(24, 24, 24, 24);
        lblLoginTitle = new QLabel(cardLogin);
        lblLoginTitle->setObjectName("lblLoginTitle");
        lblLoginTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_login->addWidget(lblLoginTitle);

        editLoginUser = new QLineEdit(cardLogin);
        editLoginUser->setObjectName("editLoginUser");

        verticalLayout_login->addWidget(editLoginUser);

        editLoginPassword = new QLineEdit(cardLogin);
        editLoginPassword->setObjectName("editLoginPassword");
        editLoginPassword->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout_login->addWidget(editLoginPassword);

        btnLogin = new QPushButton(cardLogin);
        btnLogin->setObjectName("btnLogin");

        verticalLayout_login->addWidget(btnLogin);

        btnGotoRegister = new QPushButton(cardLogin);
        btnGotoRegister->setObjectName("btnGotoRegister");

        verticalLayout_login->addWidget(btnGotoRegister);


        verticalLayout_login_root->addWidget(cardLogin);

        login_bottom = new QSpacerItem(20, 30, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_login_root->addItem(login_bottom);

        stackMain_employees->addWidget(pageLogin);
        pageRegister = new QWidget();
        pageRegister->setObjectName("pageRegister");
        verticalLayout_register_root = new QVBoxLayout(pageRegister);
        verticalLayout_register_root->setSpacing(12);
        verticalLayout_register_root->setObjectName("verticalLayout_register_root");
        verticalLayout_register_root->setContentsMargins(40, 20, 40, 20);
        cardRegister = new QFrame(pageRegister);
        cardRegister->setObjectName("cardRegister");
        cardRegister->setFrameShape(QFrame::Shape::NoFrame);
        verticalLayout_register = new QVBoxLayout(cardRegister);
        verticalLayout_register->setSpacing(16);
        verticalLayout_register->setObjectName("verticalLayout_register");
        verticalLayout_register->setContentsMargins(24, 24, 24, 24);
        lblRegisterTitle = new QLabel(cardRegister);
        lblRegisterTitle->setObjectName("lblRegisterTitle");
        lblRegisterTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_register->addWidget(lblRegisterTitle);

        editRegName = new QLineEdit(cardRegister);
        editRegName->setObjectName("editRegName");

        verticalLayout_register->addWidget(editRegName);

        editRegEmail = new QLineEdit(cardRegister);
        editRegEmail->setObjectName("editRegEmail");

        verticalLayout_register->addWidget(editRegEmail);

        editRegPassword = new QLineEdit(cardRegister);
        editRegPassword->setObjectName("editRegPassword");
        editRegPassword->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout_register->addWidget(editRegPassword);

        editRegPasswordConfirm = new QLineEdit(cardRegister);
        editRegPasswordConfirm->setObjectName("editRegPasswordConfirm");
        editRegPasswordConfirm->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout_register->addWidget(editRegPasswordConfirm);

        btnRegister = new QPushButton(cardRegister);
        btnRegister->setObjectName("btnRegister");

        verticalLayout_register->addWidget(btnRegister);

        btnGotoLogin = new QPushButton(cardRegister);
        btnGotoLogin->setObjectName("btnGotoLogin");

        verticalLayout_register->addWidget(btnGotoLogin);


        verticalLayout_register_root->addWidget(cardRegister);

        stackMain_employees->addWidget(pageRegister);
        pageEmployee = new QWidget();
        pageEmployee->setObjectName("pageEmployee");
        verticalLayout_employee_root = new QVBoxLayout(pageEmployee);
        verticalLayout_employee_root->setSpacing(0);
        verticalLayout_employee_root->setObjectName("verticalLayout_employee_root");
        verticalLayout_employee_root->setContentsMargins(0, 0, 0, 0);
        frame_emp_top_nav = new QFrame(pageEmployee);
        frame_emp_top_nav->setObjectName("frame_emp_top_nav");
        frame_emp_top_nav->setStyleSheet(QString::fromUtf8("\n"
"#frame_emp_top_nav { background-color: #F7F1E6; border-bottom: 2px solid #D2B48C; }\n"
"QPushButton {\n"
"background-color: transparent; border: 2px solid #D2B48C; border-radius: 15px;\n"
"padding: 8px 16px; color: #8B4513; font-weight: bold; margin: 4px;\n"
"}\n"
"QPushButton:hover { background-color: #FFF0E0; border-color: #8B4513; }\n"
"QPushButton:checked { background-color: #C68E65; color: white; border-color: #C68E65; }\n"
"                 "));
        horizontalLayout_emp_nav = new QHBoxLayout(frame_emp_top_nav);
        horizontalLayout_emp_nav->setObjectName("horizontalLayout_emp_nav");
        btnTabAdd = new QPushButton(frame_emp_top_nav);
        btnTabAdd->setObjectName("btnTabAdd");
        btnTabAdd->setCheckable(true);
        btnTabAdd->setChecked(true);
        btnTabAdd->setAutoExclusive(true);

        horizontalLayout_emp_nav->addWidget(btnTabAdd);

        btnTabEdit = new QPushButton(frame_emp_top_nav);
        btnTabEdit->setObjectName("btnTabEdit");
        btnTabEdit->setCheckable(true);
        btnTabEdit->setAutoExclusive(true);

        horizontalLayout_emp_nav->addWidget(btnTabEdit);

        btnTabDelete = new QPushButton(frame_emp_top_nav);
        btnTabDelete->setObjectName("btnTabDelete");
        btnTabDelete->setCheckable(true);
        btnTabDelete->setAutoExclusive(true);

        horizontalLayout_emp_nav->addWidget(btnTabDelete);

        btnTabList = new QPushButton(frame_emp_top_nav);
        btnTabList->setObjectName("btnTabList");
        btnTabList->setCheckable(true);
        btnTabList->setAutoExclusive(true);

        horizontalLayout_emp_nav->addWidget(btnTabList);

        horizontalSpacer_emp_nav = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_emp_nav->addItem(horizontalSpacer_emp_nav);


        verticalLayout_employee_root->addWidget(frame_emp_top_nav);

        stackEmployee = new QStackedWidget(pageEmployee);
        stackEmployee->setObjectName("stackEmployee");
        subPageAdd = new QWidget();
        subPageAdd->setObjectName("subPageAdd");
        verticalLayout_sub_add = new QVBoxLayout(subPageAdd);
        verticalLayout_sub_add->setObjectName("verticalLayout_sub_add");
        cardEmployeeAdd = new QFrame(subPageAdd);
        cardEmployeeAdd->setObjectName("cardEmployeeAdd");
        cardEmployeeAdd->setFrameShape(QFrame::Shape::NoFrame);
        verticalLayout_add_inner = new QVBoxLayout(cardEmployeeAdd);
        verticalLayout_add_inner->setObjectName("verticalLayout_add_inner");
        gridLayout_form = new QGridLayout();
        gridLayout_form->setObjectName("gridLayout_form");
        lblId = new QLabel(cardEmployeeAdd);
        lblId->setObjectName("lblId");

        gridLayout_form->addWidget(lblId, 0, 0, 1, 1);

        editId = new QLineEdit(cardEmployeeAdd);
        editId->setObjectName("editId");

        gridLayout_form->addWidget(editId, 0, 1, 1, 1);

        lblName = new QLabel(cardEmployeeAdd);
        lblName->setObjectName("lblName");

        gridLayout_form->addWidget(lblName, 1, 0, 1, 1);

        editName = new QLineEdit(cardEmployeeAdd);
        editName->setObjectName("editName");

        gridLayout_form->addWidget(editName, 1, 1, 1, 1);

        lblAddress = new QLabel(cardEmployeeAdd);
        lblAddress->setObjectName("lblAddress");

        gridLayout_form->addWidget(lblAddress, 2, 0, 1, 1);

        editAddress = new QLineEdit(cardEmployeeAdd);
        editAddress->setObjectName("editAddress");

        gridLayout_form->addWidget(editAddress, 2, 1, 1, 1);

        lblEmail = new QLabel(cardEmployeeAdd);
        lblEmail->setObjectName("lblEmail");

        gridLayout_form->addWidget(lblEmail, 3, 0, 1, 1);

        editEmail = new QLineEdit(cardEmployeeAdd);
        editEmail->setObjectName("editEmail");

        gridLayout_form->addWidget(editEmail, 3, 1, 1, 1);

        lblPhone = new QLabel(cardEmployeeAdd);
        lblPhone->setObjectName("lblPhone");

        gridLayout_form->addWidget(lblPhone, 4, 0, 1, 1);

        editPhone = new QLineEdit(cardEmployeeAdd);
        editPhone->setObjectName("editPhone");

        gridLayout_form->addWidget(editPhone, 4, 1, 1, 1);

        lblHireDate = new QLabel(cardEmployeeAdd);
        lblHireDate->setObjectName("lblHireDate");

        gridLayout_form->addWidget(lblHireDate, 5, 0, 1, 1);

        dateHire = new QDateEdit(cardEmployeeAdd);
        dateHire->setObjectName("dateHire");
        dateHire->setCalendarPopup(true);

        gridLayout_form->addWidget(dateHire, 5, 1, 1, 1);

        verticalLayout_photo = new QVBoxLayout();
        verticalLayout_photo->setObjectName("verticalLayout_photo");
        lblPhotoPreview = new QLabel(cardEmployeeAdd);
        lblPhotoPreview->setObjectName("lblPhotoPreview");
        lblPhotoPreview->setMinimumSize(QSize(150, 150));
        lblPhotoPreview->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_photo->addWidget(lblPhotoPreview);

        btnChoosePhoto = new QPushButton(cardEmployeeAdd);
        btnChoosePhoto->setObjectName("btnChoosePhoto");

        verticalLayout_photo->addWidget(btnChoosePhoto);


        gridLayout_form->addLayout(verticalLayout_photo, 0, 2, 3, 1);

        lblPoste = new QLabel(cardEmployeeAdd);
        lblPoste->setObjectName("lblPoste");

        gridLayout_form->addWidget(lblPoste, 3, 2, 1, 1);

        comboPoste = new QComboBox(cardEmployeeAdd);
        comboPoste->setObjectName("comboPoste");

        gridLayout_form->addWidget(comboPoste, 3, 3, 1, 1);

        lblSalary = new QLabel(cardEmployeeAdd);
        lblSalary->setObjectName("lblSalary");

        gridLayout_form->addWidget(lblSalary, 4, 2, 1, 1);

        spinSalary = new QDoubleSpinBox(cardEmployeeAdd);
        spinSalary->setObjectName("spinSalary");
        spinSalary->setMaximum(9999999.000000000000000);

        gridLayout_form->addWidget(spinSalary, 4, 3, 1, 1);

        lblSexe = new QLabel(cardEmployeeAdd);
        lblSexe->setObjectName("lblSexe");

        gridLayout_form->addWidget(lblSexe, 5, 2, 1, 1);

        comboSexe = new QComboBox(cardEmployeeAdd);
        comboSexe->addItem(QString());
        comboSexe->addItem(QString());
        comboSexe->setObjectName("comboSexe");

        gridLayout_form->addWidget(comboSexe, 5, 3, 1, 1);

        lblStatus = new QLabel(cardEmployeeAdd);
        lblStatus->setObjectName("lblStatus");

        gridLayout_form->addWidget(lblStatus, 6, 2, 1, 1);

        comboStatus = new QComboBox(cardEmployeeAdd);
        comboStatus->setObjectName("comboStatus");

        gridLayout_form->addWidget(comboStatus, 6, 3, 1, 1);

        lblSpeciality = new QLabel(cardEmployeeAdd);
        lblSpeciality->setObjectName("lblSpeciality");

        gridLayout_form->addWidget(lblSpeciality, 7, 2, 1, 1);

        comboSpeciality = new QComboBox(cardEmployeeAdd);
        comboSpeciality->setObjectName("comboSpeciality");

        gridLayout_form->addWidget(comboSpeciality, 7, 3, 1, 1);

        lblRole = new QLabel(cardEmployeeAdd);
        lblRole->setObjectName("lblRole");

        gridLayout_form->addWidget(lblRole, 7, 0, 1, 1);

        comboRole = new QComboBox(cardEmployeeAdd);
        comboRole->setObjectName("comboRole");

        gridLayout_form->addWidget(comboRole, 7, 1, 1, 1);


        verticalLayout_add_inner->addLayout(gridLayout_form);

        hbox_btns_add = new QHBoxLayout();
        hbox_btns_add->setObjectName("hbox_btns_add");
        spacer_add_left = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hbox_btns_add->addItem(spacer_add_left);

        btnAjouter = new QPushButton(cardEmployeeAdd);
        btnAjouter->setObjectName("btnAjouter");

        hbox_btns_add->addWidget(btnAjouter);

        btnVider = new QPushButton(cardEmployeeAdd);
        btnVider->setObjectName("btnVider");

        hbox_btns_add->addWidget(btnVider);


        verticalLayout_add_inner->addLayout(hbox_btns_add);


        verticalLayout_sub_add->addWidget(cardEmployeeAdd);

        stackEmployee->addWidget(subPageAdd);
        subPageEdit = new QWidget();
        subPageEdit->setObjectName("subPageEdit");
        verticalLayout_sub_edit = new QVBoxLayout(subPageEdit);
        verticalLayout_sub_edit->setObjectName("verticalLayout_sub_edit");
        cardEmployeeEdit = new QFrame(subPageEdit);
        cardEmployeeEdit->setObjectName("cardEmployeeEdit");
        cardEmployeeEdit->setFrameShape(QFrame::Shape::NoFrame);
        vbox_edit = new QVBoxLayout(cardEmployeeEdit);
        vbox_edit->setObjectName("vbox_edit");
        hbox_search_edit = new QHBoxLayout();
        hbox_search_edit->setObjectName("hbox_search_edit");
        editSearchEdit = new QLineEdit(cardEmployeeEdit);
        editSearchEdit->setObjectName("editSearchEdit");

        hbox_search_edit->addWidget(editSearchEdit);

        btnSearchEdit = new QPushButton(cardEmployeeEdit);
        btnSearchEdit->setObjectName("btnSearchEdit");

        hbox_search_edit->addWidget(btnSearchEdit);


        vbox_edit->addLayout(hbox_search_edit);

        lblEditInfo = new QLabel(cardEmployeeEdit);
        lblEditInfo->setObjectName("lblEditInfo");

        vbox_edit->addWidget(lblEditInfo);

        formLayout_Edit = new QFormLayout();
        formLayout_Edit->setObjectName("formLayout_Edit");
        lblModName = new QLabel(cardEmployeeEdit);
        lblModName->setObjectName("lblModName");

        formLayout_Edit->setWidget(0, QFormLayout::LabelRole, lblModName);

        editNameMod = new QLineEdit(cardEmployeeEdit);
        editNameMod->setObjectName("editNameMod");

        formLayout_Edit->setWidget(0, QFormLayout::FieldRole, editNameMod);

        lblModPhone = new QLabel(cardEmployeeEdit);
        lblModPhone->setObjectName("lblModPhone");

        formLayout_Edit->setWidget(1, QFormLayout::LabelRole, lblModPhone);

        editPhoneMod = new QLineEdit(cardEmployeeEdit);
        editPhoneMod->setObjectName("editPhoneMod");

        formLayout_Edit->setWidget(1, QFormLayout::FieldRole, editPhoneMod);

        lblModSalary = new QLabel(cardEmployeeEdit);
        lblModSalary->setObjectName("lblModSalary");

        formLayout_Edit->setWidget(2, QFormLayout::LabelRole, lblModSalary);

        spinSalaryMod = new QDoubleSpinBox(cardEmployeeEdit);
        spinSalaryMod->setObjectName("spinSalaryMod");
        spinSalaryMod->setMaximum(9999999.000000000000000);

        formLayout_Edit->setWidget(2, QFormLayout::FieldRole, spinSalaryMod);


        vbox_edit->addLayout(formLayout_Edit);

        btnModifier = new QPushButton(cardEmployeeEdit);
        btnModifier->setObjectName("btnModifier");

        vbox_edit->addWidget(btnModifier);

        verticalSpacer_edit = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vbox_edit->addItem(verticalSpacer_edit);


        verticalLayout_sub_edit->addWidget(cardEmployeeEdit);

        stackEmployee->addWidget(subPageEdit);
        subPageDelete = new QWidget();
        subPageDelete->setObjectName("subPageDelete");
        verticalLayout_sub_del = new QVBoxLayout(subPageDelete);
        verticalLayout_sub_del->setObjectName("verticalLayout_sub_del");
        cardEmployeeDelete = new QFrame(subPageDelete);
        cardEmployeeDelete->setObjectName("cardEmployeeDelete");
        cardEmployeeDelete->setFrameShape(QFrame::Shape::NoFrame);
        vbox_del1 = new QVBoxLayout(cardEmployeeDelete);
        vbox_del1->setObjectName("vbox_del1");
        lblDelTitle = new QLabel(cardEmployeeDelete);
        lblDelTitle->setObjectName("lblDelTitle");

        vbox_del1->addWidget(lblDelTitle);

        editSearchDel = new QLineEdit(cardEmployeeDelete);
        editSearchDel->setObjectName("editSearchDel");

        vbox_del1->addWidget(editSearchDel);

        btnSupprimer = new QPushButton(cardEmployeeDelete);
        btnSupprimer->setObjectName("btnSupprimer");

        vbox_del1->addWidget(btnSupprimer);

        verticalSpacer_del = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        vbox_del1->addItem(verticalSpacer_del);


        verticalLayout_sub_del->addWidget(cardEmployeeDelete);

        stackEmployee->addWidget(subPageDelete);
        subPageList = new QWidget();
        subPageList->setObjectName("subPageList");
        verticalLayout_sub_list = new QVBoxLayout(subPageList);
        verticalLayout_sub_list->setObjectName("verticalLayout_sub_list");
        cardEmployeeList = new QFrame(subPageList);
        cardEmployeeList->setObjectName("cardEmployeeList");
        cardEmployeeList->setFrameShape(QFrame::Shape::NoFrame);
        vbox_list = new QVBoxLayout(cardEmployeeList);
        vbox_list->setObjectName("vbox_list");
        hbox_list_tools = new QHBoxLayout();
        hbox_list_tools->setObjectName("hbox_list_tools");
        editSearchList = new QLineEdit(cardEmployeeList);
        editSearchList->setObjectName("editSearchList");

        hbox_list_tools->addWidget(editSearchList);

        comboSort = new QComboBox(cardEmployeeList);
        comboSort->addItem(QString());
        comboSort->addItem(QString());
        comboSort->addItem(QString());
        comboSort->setObjectName("comboSort");

        hbox_list_tools->addWidget(comboSort);

        btnExportPDF = new QPushButton(cardEmployeeList);
        btnExportPDF->setObjectName("btnExportPDF");

        hbox_list_tools->addWidget(btnExportPDF);


        vbox_list->addLayout(hbox_list_tools);

        tableEmployees = new QTableView(cardEmployeeList);
        tableEmployees->setObjectName("tableEmployees");
        tableEmployees->setAlternatingRowColors(true);
        tableEmployees->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableEmployees->setSortingEnabled(true);

        vbox_list->addWidget(tableEmployees);

        groupStats = new QGroupBox(cardEmployeeList);
        groupStats->setObjectName("groupStats");
        hbox_stats = new QHBoxLayout(groupStats);
        hbox_stats->setObjectName("hbox_stats");
        lblStatCount = new QLabel(groupStats);
        lblStatCount->setObjectName("lblStatCount");

        hbox_stats->addWidget(lblStatCount);

        lblStatAvg = new QLabel(groupStats);
        lblStatAvg->setObjectName("lblStatAvg");

        hbox_stats->addWidget(lblStatAvg);

        btnCalculPrime = new QPushButton(groupStats);
        btnCalculPrime->setObjectName("btnCalculPrime");

        hbox_stats->addWidget(btnCalculPrime);


        vbox_list->addWidget(groupStats);


        verticalLayout_sub_list->addWidget(cardEmployeeList);

        stackEmployee->addWidget(subPageList);

        verticalLayout_employee_root->addWidget(stackEmployee);

        stackMain_employees->addWidget(pageEmployee);

        verticalLayout_employees->addWidget(stackMain_employees);

        stackedWidget->addWidget(page_employees);
        page_ai = new QWidget();
        page_ai->setObjectName("page_ai");
        verticalLayout_ai = new QVBoxLayout(page_ai);
        verticalLayout_ai->setObjectName("verticalLayout_ai");
        label_ai = new QLabel(page_ai);
        label_ai->setObjectName("label_ai");
        label_ai->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_ai->addWidget(label_ai);

        stackedWidget->addWidget(page_ai);

        contentArea->addWidget(stackedWidget, 0, Qt::AlignmentFlag::AlignLeft);


        mainLayout->addLayout(contentArea);

        stackedWidget_main->addWidget(page_main_content);

        mainLayout_root->addWidget(stackedWidget_main);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget_main->setCurrentIndex(3);
        stackedWidget->setCurrentIndex(0);
        stackedWidget_suppliers->setCurrentIndex(0);
        stackedWidget_orders->setCurrentIndex(3);
        tabWidget_stats->setCurrentIndex(0);
        stackedWidget_materials->setCurrentIndex(2);
        tabWidget_mat_stats->setCurrentIndex(0);
        stackMain_employees->setCurrentIndex(2);
        stackEmployee->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "NextGen - Leader Factory Management", nullptr));
        lblLoginTitle_main->setText(QCoreApplication::translate("MainWindow", "Connexion", nullptr));
        editLoginUser_main->setPlaceholderText(QCoreApplication::translate("MainWindow", "Adresse e-mail ou nom d'utilisateur", nullptr));
        editLoginPassword_main->setPlaceholderText(QCoreApplication::translate("MainWindow", "Mot de passe", nullptr));
        btnLogin_main->setText(QCoreApplication::translate("MainWindow", "Se connecter", nullptr));
        btnGotoRegister_main->setText(QCoreApplication::translate("MainWindow", "Cr\303\251er un compte", nullptr));
        lblRegisterTitle_main->setText(QCoreApplication::translate("MainWindow", "Cr\303\251er un compte", nullptr));
        editRegName_main->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nom complet", nullptr));
        editRegEmail_main->setPlaceholderText(QCoreApplication::translate("MainWindow", "Adresse e-mail", nullptr));
        editRegPassword_main->setPlaceholderText(QCoreApplication::translate("MainWindow", "Mot de passe", nullptr));
        editRegPasswordConfirm_main->setPlaceholderText(QCoreApplication::translate("MainWindow", "Confirmer le mot de passe", nullptr));
        btnRegister_main->setText(QCoreApplication::translate("MainWindow", "S'inscrire", nullptr));
        btnGotoLogin_main->setText(QCoreApplication::translate("MainWindow", "D\303\251j\303\240 un compte ? Se connecter", nullptr));
        logo_label->setText(QString());
        factory_title->setText(QCoreApplication::translate("MainWindow", "Leader Factory Management", nullptr));
        btn_nav_orders->setText(QCoreApplication::translate("MainWindow", "COMMANDES", nullptr));
        btn_nav_material->setText(QCoreApplication::translate("MainWindow", "Mati\303\250res Premi\303\250res", nullptr));
        btn_nav_employees->setText(QCoreApplication::translate("MainWindow", "EMPLOY\303\211S", nullptr));
        gestion_fournisseurs->setText(QCoreApplication::translate("MainWindow", "GESTION FOURNISSEURS", nullptr));
        btn_nav_ai->setText(QCoreApplication::translate("MainWindow", "ASSISTANT IA", nullptr));
        btn_tab_supp_add->setText(QCoreApplication::translate("MainWindow", "Ajouter un Fournisseur", nullptr));
        btn_tab_supp_update->setText(QCoreApplication::translate("MainWindow", "Modifier un Fournisseur", nullptr));
        btn_tab_supp_delete->setText(QCoreApplication::translate("MainWindow", "Supprimer un Fournisseur", nullptr));
        btn_tab_supp_list->setText(QCoreApplication::translate("MainWindow", "Liste / Stats", nullptr));
        frame_add_supp->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border-radius: 10px; border: 1px solid #F2D2B5;", nullptr));
        lblFormTitle_add->setText(QCoreApplication::translate("MainWindow", "Ajouter un Nouveau Fournisseur", nullptr));
        lblFormTitle_add->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D362D; margin-bottom: 15px;", nullptr));
        label_supp_1->setText(QCoreApplication::translate("MainWindow", "Nom *", nullptr));
        label_supp_1->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtNom->setPlaceholderText(QCoreApplication::translate("MainWindow", "Ex: Fournisseur ABC Sarl", nullptr));
        txtNom->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_supp_2->setText(QCoreApplication::translate("MainWindow", "Type *", nullptr));
        label_supp_2->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtType->setPlaceholderText(QCoreApplication::translate("MainWindow", "Ex: Cuir, Textile, Colle...", nullptr));
        txtType->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_supp_3->setText(QCoreApplication::translate("MainWindow", "Adresse *", nullptr));
        label_supp_3->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtAdresse->setPlaceholderText(QCoreApplication::translate("MainWindow", "Adresse compl\303\250te", nullptr));
        txtAdresse->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_supp_4->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone *", nullptr));
        label_supp_4->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtTelephone->setPlaceholderText(QCoreApplication::translate("MainWindow", "Ex: +212 5XX-XXXXXX", nullptr));
        txtTelephone->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_supp_5->setText(QCoreApplication::translate("MainWindow", "Email *", nullptr));
        label_supp_5->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtEmail->setPlaceholderText(QCoreApplication::translate("MainWindow", "contact@fournisseur.com", nullptr));
        txtEmail->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_supp_6->setText(QCoreApplication::translate("MainWindow", "D\303\251lai de livraison (jours) *", nullptr));
        label_supp_6->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        spinDelai->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_supp_7->setText(QCoreApplication::translate("MainWindow", "Prix moyen (DH) *", nullptr));
        label_supp_7->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        spinPrix->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_supp_8->setText(QCoreApplication::translate("MainWindow", "Statut *", nullptr));
        label_supp_8->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        comboStatut->setItemText(0, QCoreApplication::translate("MainWindow", "Actif", nullptr));
        comboStatut->setItemText(1, QCoreApplication::translate("MainWindow", "Suspendu", nullptr));

        comboStatut->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_supp_notes->setText(QCoreApplication::translate("MainWindow", "Notes", nullptr));
        label_supp_notes->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtNotes->setPlaceholderText(QCoreApplication::translate("MainWindow", "Informations suppl\303\251mentaires...", nullptr));
        txtNotes->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        btnEnregistrer->setText(QCoreApplication::translate("MainWindow", "ENREGISTRER", nullptr));
        btnEnregistrer->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #C68E65; color: white; border: none; padding: 10px 25px; border-radius: 5px; font-weight: bold;", nullptr));
        btnReinitialiser->setText(QCoreApplication::translate("MainWindow", "R\303\211INITIALISER", nullptr));
        btnReinitialiser->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #F2D2B5; color: #3D362D; border: none; padding: 10px 25px; border-radius: 5px; font-weight: bold;", nullptr));
        frame_update_supp->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border-radius: 10px; border: 1px solid #F2D2B5;", nullptr));
        lblFormTitle_mod->setText(QCoreApplication::translate("MainWindow", "Modifier un Fournisseur", nullptr));
        lblFormTitle_mod->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D362D; margin-bottom: 15px;", nullptr));
        label_search_supp->setText(QCoreApplication::translate("MainWindow", "Rechercher :", nullptr));
        label_search_supp->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtRechercheModifier->setPlaceholderText(QCoreApplication::translate("MainWindow", "Par nom ou ID...", nullptr));
        txtRechercheModifier->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        btnChercherFournisseur->setText(QCoreApplication::translate("MainWindow", "CHARGER", nullptr));
        btnChercherFournisseur->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #C68E65; color: white; border: none; padding: 8px 15px; border-radius: 5px;", nullptr));
        label_mod_1->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_mod_1->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtNomMod->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nom du fournisseur", nullptr));
        txtNomMod->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_mod_2->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        label_mod_2->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtTypeMod->setPlaceholderText(QCoreApplication::translate("MainWindow", "Type de mati\303\250re", nullptr));
        txtTypeMod->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_mod_3->setText(QCoreApplication::translate("MainWindow", "Adresse", nullptr));
        label_mod_3->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtAdresseMod->setPlaceholderText(QCoreApplication::translate("MainWindow", "Adresse", nullptr));
        txtAdresseMod->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_mod_4->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone", nullptr));
        label_mod_4->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtTelephoneMod->setPlaceholderText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone", nullptr));
        txtTelephoneMod->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_mod_5->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_mod_5->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtEmailMod->setPlaceholderText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        txtEmailMod->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_mod_6->setText(QCoreApplication::translate("MainWindow", "D\303\251lai de livraison (jours)", nullptr));
        label_mod_6->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        spinDelaiMod->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_mod_7->setText(QCoreApplication::translate("MainWindow", "Prix moyen (DH)", nullptr));
        label_mod_7->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        spinPrixMod->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_mod_8->setText(QCoreApplication::translate("MainWindow", "Statut", nullptr));
        label_mod_8->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        comboStatutMod->setItemText(0, QCoreApplication::translate("MainWindow", "Actif", nullptr));
        comboStatutMod->setItemText(1, QCoreApplication::translate("MainWindow", "Suspendu", nullptr));

        comboStatutMod->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        label_mod_notes->setText(QCoreApplication::translate("MainWindow", "Notes", nullptr));
        label_mod_notes->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtNotesMod->setPlaceholderText(QCoreApplication::translate("MainWindow", "Informations suppl\303\251mentaires...", nullptr));
        txtNotesMod->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        btnMettreAJour->setText(QCoreApplication::translate("MainWindow", "METTRE \303\200 JOUR", nullptr));
        btnMettreAJour->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #C68E65; color: white; border: none; padding: 10px 25px; border-radius: 5px; font-weight: bold;", nullptr));
        frame_delete_supp->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border-radius: 10px; border: 1px solid #F2D2B5;", nullptr));
        lblFormTitle_del->setText(QCoreApplication::translate("MainWindow", "Supprimer un Fournisseur", nullptr));
        lblFormTitle_del->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D362D; margin-bottom: 15px;", nullptr));
        lblSupprimer->setText(QCoreApplication::translate("MainWindow", "Rechercher :", nullptr));
        lblSupprimer->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        txtSupprimer->setPlaceholderText(QCoreApplication::translate("MainWindow", "Par nom ou ID...", nullptr));
        txtSupprimer->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        btnChercherSupprimer->setText(QCoreApplication::translate("MainWindow", "CHARGER", nullptr));
        btnChercherSupprimer->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #C68E65; color: white; border: none; padding: 8px 15px; border-radius: 5px;", nullptr));
        frame_confirm_del->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FCF9F6; border: 1px solid #C68E65; border-radius: 5px; padding: 15px;", nullptr));
        lblValiderSupp->setText(QCoreApplication::translate("MainWindow", "Fournisseur trouv\303\251 :", nullptr));
        lblValiderSupp->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-weight: bold;", nullptr));
        lblDetailSupp->setText(QCoreApplication::translate("MainWindow", "(Aucun fournisseur s\303\251lectionn\303\251)", nullptr));
        lblDetailSupp->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #666; font-size: 12px;", nullptr));
        lblWarning->setText(QCoreApplication::translate("MainWindow", "Attention : Cette action est irr\303\251versible", nullptr));
        lblWarning->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #d9534f; font-weight: bold; background-color: #fff8f8; padding: 10px; border-radius: 5px;", nullptr));
        btnAnnulerSupp->setText(QCoreApplication::translate("MainWindow", "ANNULER", nullptr));
        btnAnnulerSupp->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #F2D2B5; color: #3D362D; border: none; padding: 10px 25px; border-radius: 5px; font-weight: bold;", nullptr));
        btnSupprimerDef->setText(QCoreApplication::translate("MainWindow", "SUPPRIMER D\303\211FINITIVEMENT", nullptr));
        btnSupprimerDef->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #d9534f; color: white; border: none; padding: 10px 25px; border-radius: 5px; font-weight: bold;", nullptr));
        frame_list_supp->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border-radius: 10px; border: 1px solid #F2D2B5;", nullptr));
        lblListeTitle->setText(QCoreApplication::translate("MainWindow", "Liste et Statistiques des Fournisseurs", nullptr));
        lblListeTitle->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D362D; margin-bottom: 15px;", nullptr));
        txtFilterNom->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nom...", nullptr));
        txtFilterNom->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        txtFilterType->setPlaceholderText(QCoreApplication::translate("MainWindow", "Type...", nullptr));
        txtFilterType->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        txtFilterStatut->setPlaceholderText(QCoreApplication::translate("MainWindow", "Statut...", nullptr));
        txtFilterStatut->setStyleSheet(QCoreApplication::translate("MainWindow", "border: 1px solid #F2D2B5; border-radius: 5px; padding: 8px; background-color: #FCF9F6;", nullptr));
        btnRechercher->setText(QCoreApplication::translate("MainWindow", "FILTRER", nullptr));
        btnRechercher->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #C68E65; color: white; border: none; padding: 8px 15px; border-radius: 5px; font-weight: bold;", nullptr));
        btnRinitialiserFiltres->setText(QCoreApplication::translate("MainWindow", "R\303\211INITIALISER", nullptr));
        btnRinitialiserFiltres->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #F2D2B5; color: #3D362D; border: none; padding: 8px 15px; border-radius: 5px;", nullptr));
        tableFournisseurs->setProperty("horizontalHeaderLabels", QVariant(QStringList{
            QCoreApplication::translate("MainWindow", "ID", nullptr),
            QCoreApplication::translate("MainWindow", "Nom", nullptr),
            QCoreApplication::translate("MainWindow", "Type", nullptr),
            QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone", nullptr),
            QCoreApplication::translate("MainWindow", "D\303\251lai", nullptr),
            QCoreApplication::translate("MainWindow", "Prix", nullptr),
            QCoreApplication::translate("MainWindow", "Statut", nullptr),
            QCoreApplication::translate("MainWindow", "Actions", nullptr)}));
        tableFournisseurs->setStyleSheet(QCoreApplication::translate("MainWindow", "QTableWidget { border: 1px solid #F2D2B5; } QHeaderView::section { background-color: #3D362D; color: white; padding: 8px; } QTableWidget::item { padding: 8px; }", nullptr));
        lblSelectedSupplier->setText(QCoreApplication::translate("MainWindow", "S\303\251lectionnez un fournisseur pour effectuer une action", nullptr));
        lblSelectedSupplier->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #666; font-size: 12px;", nullptr));
        btnModifierDuTableau->setText(QCoreApplication::translate("MainWindow", "MODIFIER", nullptr));
        btnModifierDuTableau->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #C68E65; color: white; border: none; padding: 8px 20px; border-radius: 5px; font-weight: bold;", nullptr));
        btnSupprimerDuTableau->setText(QCoreApplication::translate("MainWindow", "SUPPRIMER", nullptr));
        btnSupprimerDuTableau->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #d9534f; color: white; border: none; padding: 8px 20px; border-radius: 5px; font-weight: bold;", nullptr));
        frame_stat_total->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FCF9F6; border: 1px solid #F2D2B5; border-radius: 5px; padding: 10px;", nullptr));
        lblStatTotalLabel->setText(QCoreApplication::translate("MainWindow", "Total Fournisseurs", nullptr));
        lblStatTotalLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #666; font-weight: bold;", nullptr));
        lblStatTotal->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblStatTotal->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #C68E65; font-size: 18px; font-weight: bold;", nullptr));
        frame_stat_actifs->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FCF9F6; border: 1px solid #F2D2B5; border-radius: 5px; padding: 10px;", nullptr));
        lblStatActifsLabel->setText(QCoreApplication::translate("MainWindow", "Actifs", nullptr));
        lblStatActifsLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #666; font-weight: bold;", nullptr));
        lblStatActifs->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lblStatActifs->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #5cb85c; font-size: 18px; font-weight: bold;", nullptr));
        frame_stat_prix->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FCF9F6; border: 1px solid #F2D2B5; border-radius: 5px; padding: 10px;", nullptr));
        lblStatPrixLabel->setText(QCoreApplication::translate("MainWindow", "Prix Moyen", nullptr));
        lblStatPrixLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #666; font-weight: bold;", nullptr));
        lblStatPrix->setText(QCoreApplication::translate("MainWindow", "0.00 DH", nullptr));
        lblStatPrix->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #0275d8; font-size: 18px; font-weight: bold;", nullptr));
        frame_stat_delai->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #FCF9F6; border: 1px solid #F2D2B5; border-radius: 5px; padding: 10px;", nullptr));
        lblStatDelaiLabel->setText(QCoreApplication::translate("MainWindow", "D\303\251lai Moyen", nullptr));
        lblStatDelaiLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #666; font-weight: bold;", nullptr));
        lblStatDelai->setText(QCoreApplication::translate("MainWindow", "0 jours", nullptr));
        lblStatDelai->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #f0ad4e; font-size: 18px; font-weight: bold;", nullptr));
        btn_tab_add->setText(QCoreApplication::translate("MainWindow", "Ajouter une commande", nullptr));
        btn_tab_update->setText(QCoreApplication::translate("MainWindow", "Modifier une commande", nullptr));
        btn_tab_delete->setText(QCoreApplication::translate("MainWindow", "Supprimer une commande", nullptr));
        btn_tab_list->setText(QCoreApplication::translate("MainWindow", "Liste / Stats", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "ID Commande :", nullptr));
        label1->setText(QCoreApplication::translate("MainWindow", "Type d'article :", nullptr));
        cb_article_type->setItemText(0, QCoreApplication::translate("MainWindow", "--S\303\251lectionner--", nullptr));
        cb_article_type->setItemText(1, QCoreApplication::translate("MainWindow", "Sac \303\240 main", nullptr));
        cb_article_type->setItemText(2, QCoreApplication::translate("MainWindow", "Veste en cuir", nullptr));
        cb_article_type->setItemText(3, QCoreApplication::translate("MainWindow", "Chaussures", nullptr));
        cb_article_type->setItemText(4, QCoreApplication::translate("MainWindow", "Ceinture", nullptr));
        cb_article_type->setItemText(5, QCoreApplication::translate("MainWindow", "Portefeuille", nullptr));

        label2->setText(QCoreApplication::translate("MainWindow", "Quantit\303\251 :", nullptr));
        label3->setText(QCoreApplication::translate("MainWindow", "Email Client :", nullptr));
        label4->setText(QCoreApplication::translate("MainWindow", "Ville Client :", nullptr));
        cb_client_city->setItemText(0, QCoreApplication::translate("MainWindow", "--S\303\251lectionner--", nullptr));
        cb_client_city->setItemText(1, QCoreApplication::translate("MainWindow", "Tunis", nullptr));
        cb_client_city->setItemText(2, QCoreApplication::translate("MainWindow", "Ariana", nullptr));
        cb_client_city->setItemText(3, QCoreApplication::translate("MainWindow", "Ben Arous", nullptr));
        cb_client_city->setItemText(4, QCoreApplication::translate("MainWindow", "Manouba", nullptr));
        cb_client_city->setItemText(5, QCoreApplication::translate("MainWindow", "Nabeul", nullptr));
        cb_client_city->setItemText(6, QCoreApplication::translate("MainWindow", "Zaghouan", nullptr));
        cb_client_city->setItemText(7, QCoreApplication::translate("MainWindow", "Bizerte", nullptr));
        cb_client_city->setItemText(8, QCoreApplication::translate("MainWindow", "B\303\251ja", nullptr));
        cb_client_city->setItemText(9, QCoreApplication::translate("MainWindow", "Jendouba", nullptr));
        cb_client_city->setItemText(10, QCoreApplication::translate("MainWindow", "Le Kef", nullptr));
        cb_client_city->setItemText(11, QCoreApplication::translate("MainWindow", "Siliana", nullptr));
        cb_client_city->setItemText(12, QCoreApplication::translate("MainWindow", "Kairouan", nullptr));
        cb_client_city->setItemText(13, QCoreApplication::translate("MainWindow", "Kasserine", nullptr));
        cb_client_city->setItemText(14, QCoreApplication::translate("MainWindow", "Sidi Bouzid", nullptr));
        cb_client_city->setItemText(15, QCoreApplication::translate("MainWindow", "Sousse", nullptr));
        cb_client_city->setItemText(16, QCoreApplication::translate("MainWindow", "Monastir", nullptr));
        cb_client_city->setItemText(17, QCoreApplication::translate("MainWindow", "Mahdia", nullptr));
        cb_client_city->setItemText(18, QCoreApplication::translate("MainWindow", "Sfax", nullptr));
        cb_client_city->setItemText(19, QCoreApplication::translate("MainWindow", "Gab\303\250s", nullptr));
        cb_client_city->setItemText(20, QCoreApplication::translate("MainWindow", "M\303\251denine", nullptr));
        cb_client_city->setItemText(21, QCoreApplication::translate("MainWindow", "Tataouine", nullptr));
        cb_client_city->setItemText(22, QCoreApplication::translate("MainWindow", "Gafsa", nullptr));
        cb_client_city->setItemText(23, QCoreApplication::translate("MainWindow", "Tozeur", nullptr));
        cb_client_city->setItemText(24, QCoreApplication::translate("MainWindow", "K\303\251bili", nullptr));

        label5->setText(QCoreApplication::translate("MainWindow", "Statut :", nullptr));
        label6->setText(QCoreApplication::translate("MainWindow", "Date Commande :", nullptr));
        de_date->setDisplayFormat(QCoreApplication::translate("MainWindow", "dd/MM/yyyy", nullptr));
        label7->setText(QCoreApplication::translate("MainWindow", "Date Livraison :", nullptr));
        de_date_delivery->setDisplayFormat(QCoreApplication::translate("MainWindow", "dd/MM/yyyy", nullptr));
        label8->setText(QCoreApplication::translate("MainWindow", "Prix (TND) :", nullptr));
        label9->setText(QCoreApplication::translate("MainWindow", "Preuve :", nullptr));
        le_proof->setPlaceholderText(QCoreApplication::translate("MainWindow", "Lien ou nom du fichier...", nullptr));
        btn_valider->setText(QCoreApplication::translate("MainWindow", "ENREGISTRER", nullptr));
        cb_status->setItemText(0, QCoreApplication::translate("MainWindow", "--S\303\251lectionner--", nullptr));
        cb_status->setItemText(1, QCoreApplication::translate("MainWindow", "En attente", nullptr));
        cb_status->setItemText(2, QCoreApplication::translate("MainWindow", "Valid\303\251", nullptr));
        cb_status->setItemText(3, QCoreApplication::translate("MainWindow", "Exp\303\251di\303\251", nullptr));
        cb_status->setItemText(4, QCoreApplication::translate("MainWindow", "Livr\303\251", nullptr));

        le_id_update_search->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez l'ID \303\240 modifier...", nullptr));
        btn_search_update->setText(QCoreApplication::translate("MainWindow", "CHARGER", nullptr));
        label10->setText(QCoreApplication::translate("MainWindow", "ID Commande :", nullptr));
        le_id_update->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #E0E0E0;", nullptr));
        label11->setText(QCoreApplication::translate("MainWindow", "Type d'article :", nullptr));
        cb_type_update->setItemText(0, QCoreApplication::translate("MainWindow", "-- S\303\251lectionner --", nullptr));
        cb_type_update->setItemText(1, QCoreApplication::translate("MainWindow", "Sac \303\240 main", nullptr));
        cb_type_update->setItemText(2, QCoreApplication::translate("MainWindow", "Veste en cuir", nullptr));
        cb_type_update->setItemText(3, QCoreApplication::translate("MainWindow", "Chaussures", nullptr));
        cb_type_update->setItemText(4, QCoreApplication::translate("MainWindow", "Ceinture", nullptr));
        cb_type_update->setItemText(5, QCoreApplication::translate("MainWindow", "Portefeuille", nullptr));

        label12->setText(QCoreApplication::translate("MainWindow", "Quantit\303\251 :", nullptr));
        label13->setText(QCoreApplication::translate("MainWindow", "Email Client :", nullptr));
        label14->setText(QCoreApplication::translate("MainWindow", "Ville Client :", nullptr));
        cb_city_update->setItemText(0, QCoreApplication::translate("MainWindow", "--S\303\251lectionner--", nullptr));
        cb_city_update->setItemText(1, QCoreApplication::translate("MainWindow", "Tunis", nullptr));
        cb_city_update->setItemText(2, QCoreApplication::translate("MainWindow", "Ariana", nullptr));
        cb_city_update->setItemText(3, QCoreApplication::translate("MainWindow", "Ben Arous", nullptr));
        cb_city_update->setItemText(4, QCoreApplication::translate("MainWindow", "Manouba", nullptr));
        cb_city_update->setItemText(5, QCoreApplication::translate("MainWindow", "Nabeul", nullptr));
        cb_city_update->setItemText(6, QCoreApplication::translate("MainWindow", "Zaghouan", nullptr));
        cb_city_update->setItemText(7, QCoreApplication::translate("MainWindow", "Bizerte", nullptr));
        cb_city_update->setItemText(8, QCoreApplication::translate("MainWindow", "B\303\251ja", nullptr));
        cb_city_update->setItemText(9, QCoreApplication::translate("MainWindow", "Jendouba", nullptr));
        cb_city_update->setItemText(10, QCoreApplication::translate("MainWindow", "Le Kef", nullptr));
        cb_city_update->setItemText(11, QCoreApplication::translate("MainWindow", "Siliana", nullptr));
        cb_city_update->setItemText(12, QCoreApplication::translate("MainWindow", "Kairouan", nullptr));
        cb_city_update->setItemText(13, QCoreApplication::translate("MainWindow", "Kasserine", nullptr));
        cb_city_update->setItemText(14, QCoreApplication::translate("MainWindow", "Sidi Bouzid", nullptr));
        cb_city_update->setItemText(15, QCoreApplication::translate("MainWindow", "Sousse", nullptr));
        cb_city_update->setItemText(16, QCoreApplication::translate("MainWindow", "Monastir", nullptr));
        cb_city_update->setItemText(17, QCoreApplication::translate("MainWindow", "Mahdia", nullptr));
        cb_city_update->setItemText(18, QCoreApplication::translate("MainWindow", "Sfax", nullptr));
        cb_city_update->setItemText(19, QCoreApplication::translate("MainWindow", "Gab\303\250s", nullptr));
        cb_city_update->setItemText(20, QCoreApplication::translate("MainWindow", "M\303\251denine", nullptr));
        cb_city_update->setItemText(21, QCoreApplication::translate("MainWindow", "Tataouine", nullptr));
        cb_city_update->setItemText(22, QCoreApplication::translate("MainWindow", "Gafsa", nullptr));
        cb_city_update->setItemText(23, QCoreApplication::translate("MainWindow", "Tozeur", nullptr));
        cb_city_update->setItemText(24, QCoreApplication::translate("MainWindow", "K\303\251bili", nullptr));

        label15->setText(QCoreApplication::translate("MainWindow", "Statut :", nullptr));
        cb_status_update->setItemText(0, QCoreApplication::translate("MainWindow", "--S\303\251lectionner--", nullptr));
        cb_status_update->setItemText(1, QCoreApplication::translate("MainWindow", "En attente", nullptr));
        cb_status_update->setItemText(2, QCoreApplication::translate("MainWindow", "Valid\303\251", nullptr));
        cb_status_update->setItemText(3, QCoreApplication::translate("MainWindow", "Exp\303\251di\303\251", nullptr));
        cb_status_update->setItemText(4, QCoreApplication::translate("MainWindow", "Livr\303\251", nullptr));

        label16->setText(QCoreApplication::translate("MainWindow", "Date Commande :", nullptr));
        de_date_update->setDisplayFormat(QCoreApplication::translate("MainWindow", "dd/MM/yyyy", nullptr));
        label17->setText(QCoreApplication::translate("MainWindow", "Date Livraison :", nullptr));
        de_date_delivery_update->setDisplayFormat(QCoreApplication::translate("MainWindow", "dd/MM/yyyy", nullptr));
        label18->setText(QCoreApplication::translate("MainWindow", "Prix (TND) :", nullptr));
        label19->setText(QCoreApplication::translate("MainWindow", "Preuve :", nullptr));
        le_proof_update->setPlaceholderText(QCoreApplication::translate("MainWindow", "Lien ou nom du fichier...", nullptr));
        btn_update_confirm->setText(QCoreApplication::translate("MainWindow", "MODIFIER", nullptr));
        le_id_to_delete->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez l'ID \303\240 supprimer...", nullptr));
        btn_delete_action->setText(QCoreApplication::translate("MainWindow", "SUPPRIMER", nullptr));
        QTableWidgetItem *___qtablewidgetitem = table_delete->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = table_delete->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = table_delete->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Quantit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = table_delete->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Statut", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = table_delete->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        le_search->setPlaceholderText(QCoreApplication::translate("MainWindow", "Rechercher (ID, Statut, Date)...", nullptr));
        cb_sort->setItemText(0, QCoreApplication::translate("MainWindow", "Trier par Date", nullptr));
        cb_sort->setItemText(1, QCoreApplication::translate("MainWindow", "Trier par Quantit\303\251", nullptr));
        cb_sort->setItemText(2, QCoreApplication::translate("MainWindow", "Trier par Statut", nullptr));

        btn_pdf->setText(QCoreApplication::translate("MainWindow", "Export PDF", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = table_list->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = table_list->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = table_list->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Quantit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = table_list->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = table_list->horizontalHeaderItem(4);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Ville", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = table_list->horizontalHeaderItem(5);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "Date Com.", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = table_list->horizontalHeaderItem(6);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "Date Livr.", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = table_list->horizontalHeaderItem(7);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "Prix", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = table_list->horizontalHeaderItem(8);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "Preuve", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = table_list->horizontalHeaderItem(9);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "Statut", nullptr));
        tabWidget_stats->setTabText(tabWidget_stats->indexOf(tab_data), QCoreApplication::translate("MainWindow", "Donn\303\251es & Actions", nullptr));
        stat_card_total->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #C68E65; color: white; border-radius: 8px; padding: 10px;", nullptr));
        lbl_total_orders_title->setText(QCoreApplication::translate("MainWindow", "TOTAL COMMANDES", nullptr));
        lbl_total_orders_val->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 24px; font-weight: bold;", nullptr));
        lbl_total_orders_val->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        stat_card_top->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #3D362D; color: white; border-radius: 8px; padding: 10px;", nullptr));
        lbl_top_client_title->setText(QCoreApplication::translate("MainWindow", "TOP CLIENT (Ville)", nullptr));
        lbl_top_client_val->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold;", nullptr));
        lbl_top_client_val->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        chart_container->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border-radius: 8px;", nullptr));
        tabWidget_stats->setTabText(tabWidget_stats->indexOf(tab_stats), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        btn_tab_mat_add->setText(QCoreApplication::translate("MainWindow", "Ajouter une Mati\303\250re premi\303\250re", nullptr));
        btn_tab_mat_update->setText(QCoreApplication::translate("MainWindow", "Modifier une Mati\303\250re premi\303\250re", nullptr));
        btn_tab_mat_delete->setText(QCoreApplication::translate("MainWindow", "Supprimer une Mati\303\250re premi\303\250re", nullptr));
        btn_tab_mat_list->setText(QCoreApplication::translate("MainWindow", "Liste / Stats", nullptr));
        frame_add->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border-radius: 15px; margin: 20px;", nullptr));
        label_mat_1->setText(QCoreApplication::translate("MainWindow", "Nom :", nullptr));
        label_mat_2->setText(QCoreApplication::translate("MainWindow", "Mati\303\250re :", nullptr));
        cb_mat_type->setItemText(0, QCoreApplication::translate("MainWindow", "Cuir", nullptr));
        cb_mat_type->setItemText(1, QCoreApplication::translate("MainWindow", "Tissu", nullptr));
        cb_mat_type->setItemText(2, QCoreApplication::translate("MainWindow", "Fil", nullptr));
        cb_mat_type->setItemText(3, QCoreApplication::translate("MainWindow", "Accessoire", nullptr));
        cb_mat_type->setItemText(4, QCoreApplication::translate("MainWindow", "Synth\303\251tique", nullptr));

        label_mat_3->setText(QCoreApplication::translate("MainWindow", "Prix :", nullptr));
        label_mat_4->setText(QCoreApplication::translate("MainWindow", "Quantit\303\251 en stock :", nullptr));
        label_mat_5->setText(QCoreApplication::translate("MainWindow", "Couleur :", nullptr));
        btn_color_add->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border: 1px solid #ccc;", nullptr));
        btn_color_add->setText(QString());
        label_mat_6->setText(QCoreApplication::translate("MainWindow", "Rendement de mati\303\250re :", nullptr));
        dsb_mat_yield->setSuffix(QCoreApplication::translate("MainWindow", " %", nullptr));
        label_mat_7->setText(QCoreApplication::translate("MainWindow", "Perte de mati\303\250re :", nullptr));
        dsb_mat_loss->setSuffix(QCoreApplication::translate("MainWindow", " %", nullptr));
        btn_mat_add->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #8B4513; color: white; padding: 10px; border-radius: 5px; font-weight: bold;", nullptr));
        btn_mat_add->setText(QCoreApplication::translate("MainWindow", "AJOUTER MATI\303\210RE", nullptr));
        frame_upd->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border-radius: 15px; margin: 20px;", nullptr));
        le_upd_search_name->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nom de la mati\303\250re \303\240 modifier...", nullptr));
        btn_upd_load->setText(QCoreApplication::translate("MainWindow", "CHARGER", nullptr));
        label_mat_10->setText(QCoreApplication::translate("MainWindow", "Nom :", nullptr));
        label_mat_11->setText(QCoreApplication::translate("MainWindow", "Mati\303\250re :", nullptr));
        cb_upd_type->setItemText(0, QCoreApplication::translate("MainWindow", "Cuir", nullptr));
        cb_upd_type->setItemText(1, QCoreApplication::translate("MainWindow", "Tissu", nullptr));
        cb_upd_type->setItemText(2, QCoreApplication::translate("MainWindow", "Fil", nullptr));
        cb_upd_type->setItemText(3, QCoreApplication::translate("MainWindow", "Accessoire", nullptr));
        cb_upd_type->setItemText(4, QCoreApplication::translate("MainWindow", "Synth\303\251tique", nullptr));

        label_mat_12->setText(QCoreApplication::translate("MainWindow", "Prix :", nullptr));
        label_mat_13->setText(QCoreApplication::translate("MainWindow", "Quantit\303\251 :", nullptr));
        label_mat_14->setText(QCoreApplication::translate("MainWindow", "Couleur :", nullptr));
        btn_color_upd->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border: 1px solid #ccc;", nullptr));
        btn_color_upd->setText(QString());
        label_mat_15->setText(QCoreApplication::translate("MainWindow", "Rendement :", nullptr));
        dsb_upd_yield->setSuffix(QCoreApplication::translate("MainWindow", " %", nullptr));
        label_mat_16->setText(QCoreApplication::translate("MainWindow", "Perte :", nullptr));
        dsb_upd_loss->setSuffix(QCoreApplication::translate("MainWindow", " %", nullptr));
        btn_upd_confirm->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #8B4513; color: white; padding: 10px; border-radius: 5px;", nullptr));
        btn_upd_confirm->setText(QCoreApplication::translate("MainWindow", "ENREGISTRER MODIFICATIONS", nullptr));
        frame_del->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border-radius: 15px; margin: 20px;", nullptr));
        lbl_del_info->setText(QCoreApplication::translate("MainWindow", "Entrez le nom de la mati\303\250re \303\240 supprimer :", nullptr));
        btn_del_confirm->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #8B0000; color: white; padding: 10px; border-radius: 5px; font-weight: bold;", nullptr));
        btn_del_confirm->setText(QCoreApplication::translate("MainWindow", "SUPPRIMER D\303\211FINITIVEMENT", nullptr));
        le_search_mat->setPlaceholderText(QCoreApplication::translate("MainWindow", "Rechercher par Nom ou Cat\303\251gorie...", nullptr));
        cb_sort_mat->setItemText(0, QCoreApplication::translate("MainWindow", "Trier par...", nullptr));
        cb_sort_mat->setItemText(1, QCoreApplication::translate("MainWindow", "Nom", nullptr));
        cb_sort_mat->setItemText(2, QCoreApplication::translate("MainWindow", "Prix (Croissant)", nullptr));
        cb_sort_mat->setItemText(3, QCoreApplication::translate("MainWindow", "Prix (D\303\251croissant)", nullptr));

        btn_export_mat_pdf->setText(QCoreApplication::translate("MainWindow", "Export PDF", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = table_mat->horizontalHeaderItem(0);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = table_mat->horizontalHeaderItem(1);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("MainWindow", "Mati\303\250re", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = table_mat->horizontalHeaderItem(2);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("MainWindow", "Prix", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = table_mat->horizontalHeaderItem(3);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("MainWindow", "Quantit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = table_mat->horizontalHeaderItem(4);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("MainWindow", "Couleur", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = table_mat->horizontalHeaderItem(5);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("MainWindow", "Rendement", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = table_mat->horizontalHeaderItem(6);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("MainWindow", "Perte", nullptr));
        tabWidget_mat_stats->setTabText(tabWidget_mat_stats->indexOf(tab_mat_data), QCoreApplication::translate("MainWindow", "Liste des Mati\303\250res", nullptr));
        stat_card_mat_total->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #C68E65; color: white; border-radius: 8px; padding: 10px;", nullptr));
        lbl_mat_t1->setText(QCoreApplication::translate("MainWindow", "TOTAL MATI\303\210RES", nullptr));
        lbl_total_mat_val->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 24px; font-weight: bold;", nullptr));
        lbl_total_mat_val->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        stat_card_rupture->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #8B0000; color: white; border-radius: 8px; padding: 10px;", nullptr));
        lbl_r1->setText(QCoreApplication::translate("MainWindow", "ALERTE RUPTURE (< 10)", nullptr));
        lbl_rupture_val->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 14px; font-weight: bold;", nullptr));
        lbl_rupture_val->setText(QCoreApplication::translate("MainWindow", "Aucune alerte", nullptr));
        stat_card_waste->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #2E8B57; color: white; border-radius: 8px; padding: 10px;", nullptr));
        lbl_w1->setText(QCoreApplication::translate("MainWindow", "CONSEIL ANTI-GASPILLAGE", nullptr));
        lbl_waste_val->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; font-style: italic;", nullptr));
        lbl_waste_val->setText(QCoreApplication::translate("MainWindow", "Analyse en cours...", nullptr));
        frame_graph->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border-radius: 8px; padding: 10px; border: 1px solid #ccc;", nullptr));
        lbl_g1->setText(QCoreApplication::translate("MainWindow", "<b>Analyse Rendement vs Perte (Moyenne)</b>", nullptr));
        lbl_g2->setText(QCoreApplication::translate("MainWindow", "Rendement Moyen:", nullptr));
        pb_yield->setStyleSheet(QCoreApplication::translate("MainWindow", "QProgressBar::chunk { background-color: #4CAF50; }", nullptr));
        lbl_g3->setText(QCoreApplication::translate("MainWindow", "Perte Moyenne:", nullptr));
        pb_loss->setStyleSheet(QCoreApplication::translate("MainWindow", "QProgressBar::chunk { background-color: #F44336; }", nullptr));
        tabWidget_mat_stats->setTabText(tabWidget_mat_stats->indexOf(tab_mat_stats), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        lblTitle_employees->setText(QCoreApplication::translate("MainWindow", "Gestion des employ\303\251s", nullptr));
        lblLoginTitle->setText(QCoreApplication::translate("MainWindow", "Connexion", nullptr));
        editLoginUser->setPlaceholderText(QCoreApplication::translate("MainWindow", "Adresse e-mail ou nom d'utilisateur", nullptr));
        editLoginPassword->setPlaceholderText(QCoreApplication::translate("MainWindow", "Mot de passe", nullptr));
        btnLogin->setText(QCoreApplication::translate("MainWindow", "Se connecter", nullptr));
        btnGotoRegister->setText(QCoreApplication::translate("MainWindow", "Cr\303\251er un compte", nullptr));
        lblRegisterTitle->setText(QCoreApplication::translate("MainWindow", "Cr\303\251er un compte", nullptr));
        editRegName->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nom complet", nullptr));
        editRegEmail->setPlaceholderText(QCoreApplication::translate("MainWindow", "Adresse e-mail", nullptr));
        editRegPassword->setPlaceholderText(QCoreApplication::translate("MainWindow", "Mot de passe", nullptr));
        editRegPasswordConfirm->setPlaceholderText(QCoreApplication::translate("MainWindow", "Confirmer le mot de passe", nullptr));
        btnRegister->setText(QCoreApplication::translate("MainWindow", "S'inscrire", nullptr));
        btnGotoLogin->setText(QCoreApplication::translate("MainWindow", "D\303\251j\303\240 un compte ? Se connecter", nullptr));
        btnTabAdd->setText(QCoreApplication::translate("MainWindow", "Ajouter un employ\303\251", nullptr));
        btnTabEdit->setText(QCoreApplication::translate("MainWindow", "Modifier un employ\303\251", nullptr));
        btnTabDelete->setText(QCoreApplication::translate("MainWindow", "Supprimer un employ\303\251", nullptr));
        btnTabList->setText(QCoreApplication::translate("MainWindow", "Liste / Stats", nullptr));
        lblId->setText(QCoreApplication::translate("MainWindow", "CIN / ID", nullptr));
        lblName->setText(QCoreApplication::translate("MainWindow", "Nom complet", nullptr));
        lblAddress->setText(QCoreApplication::translate("MainWindow", "Adresse", nullptr));
        lblEmail->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        lblPhone->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone", nullptr));
        lblHireDate->setText(QCoreApplication::translate("MainWindow", "Date d'embauche", nullptr));
        lblPhotoPreview->setText(QCoreApplication::translate("MainWindow", "Photo", nullptr));
        btnChoosePhoto->setText(QCoreApplication::translate("MainWindow", "Choisir Photo", nullptr));
        lblPoste->setText(QCoreApplication::translate("MainWindow", "Poste", nullptr));
        lblSalary->setText(QCoreApplication::translate("MainWindow", "Salaire", nullptr));
        lblSexe->setText(QCoreApplication::translate("MainWindow", "Sexe", nullptr));
        comboSexe->setItemText(0, QCoreApplication::translate("MainWindow", "Homme", nullptr));
        comboSexe->setItemText(1, QCoreApplication::translate("MainWindow", "Femme", nullptr));

        lblStatus->setText(QCoreApplication::translate("MainWindow", "Statut", nullptr));
        lblSpeciality->setText(QCoreApplication::translate("MainWindow", "Sp\303\251cialit\303\251", nullptr));
        lblRole->setText(QCoreApplication::translate("MainWindow", "R\303\264le", nullptr));
        btnAjouter->setText(QCoreApplication::translate("MainWindow", "Confirmer Ajout", nullptr));
        btnVider->setText(QCoreApplication::translate("MainWindow", "Vider Champs", nullptr));
        editSearchEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez CIN pour modifier...", nullptr));
        btnSearchEdit->setText(QCoreApplication::translate("MainWindow", "Rechercher", nullptr));
        lblEditInfo->setText(QCoreApplication::translate("MainWindow", "Modifiez les informations ci-dessous apr\303\250s recherche :", nullptr));
        lblModName->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        lblModPhone->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone", nullptr));
        lblModSalary->setText(QCoreApplication::translate("MainWindow", "Salaire", nullptr));
        btnModifier->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #8B4513; color: white;", nullptr));
        btnModifier->setText(QCoreApplication::translate("MainWindow", "Enregistrer Modifications", nullptr));
        lblDelTitle->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; color: #8B0000;", nullptr));
        lblDelTitle->setText(QCoreApplication::translate("MainWindow", "Suppression d'un employ\303\251", nullptr));
        editSearchDel->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez CIN \303\240 supprimer...", nullptr));
        btnSupprimer->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #8B0000; color: white; padding: 12px;", nullptr));
        btnSupprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer D\303\251finitivement", nullptr));
        editSearchList->setPlaceholderText(QCoreApplication::translate("MainWindow", "Rechercher...", nullptr));
        comboSort->setItemText(0, QCoreApplication::translate("MainWindow", "Trier par Nom", nullptr));
        comboSort->setItemText(1, QCoreApplication::translate("MainWindow", "Trier par Salaire", nullptr));
        comboSort->setItemText(2, QCoreApplication::translate("MainWindow", "Trier par Date", nullptr));

        btnExportPDF->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        groupStats->setTitle(QCoreApplication::translate("MainWindow", "Statistiques Rapides", nullptr));
        lblStatCount->setText(QCoreApplication::translate("MainWindow", "Total: 0", nullptr));
        lblStatAvg->setText(QCoreApplication::translate("MainWindow", "Moyenne Salaire: 0.0", nullptr));
        btnCalculPrime->setText(QCoreApplication::translate("MainWindow", "Calculer Prime", nullptr));
        label_ai->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 24px; font-weight: bold; color: #8B4513;", nullptr));
        label_ai->setText(QCoreApplication::translate("MainWindow", "ASSISTANT IA", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
