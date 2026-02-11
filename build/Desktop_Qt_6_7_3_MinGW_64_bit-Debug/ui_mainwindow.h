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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *mainLayout;
    QFrame *sidebar;
    QVBoxLayout *sidebarLayout;
    QLabel *logo_label;
    QLabel *factory_title;
    QPushButton *btn_nav_orders;
    QPushButton *btn_nav_material;
    QPushButton *btn_nav_employees;
    QPushButton *btn_nav_ai;
    QSpacerItem *vSpacer;
    QVBoxLayout *contentArea;
    QFrame *top_nav;
    QHBoxLayout *navLayout;
    QPushButton *btn_tab_add;
    QPushButton *btn_tab_update;
    QPushButton *btn_tab_delete;
    QPushButton *btn_tab_list;
    QStackedWidget *stackedWidget;
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
    QLabel *label_material;
    QWidget *page_employees;
    QVBoxLayout *verticalLayout_employees;
    QLabel *label_employees;
    QWidget *page_ai;
    QVBoxLayout *verticalLayout_ai;
    QLabel *label_ai;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 872);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"    QMainWindow { background-color: #FCF9F6; }\n"
"    \n"
"    #sidebar { background-color: #3D362D; border-right: 1px solid #4E2C23; }\n"
"    #sidebar QPushButton { background-color: transparent; color: #F2D2B5; text-align: left; padding: 15px 20px; border: none; font-weight: bold; text-transform: uppercase; font-size: 12px; }\n"
"    #sidebar QPushButton:hover { background-color: #4E2C23; color: #C68E65; border-left: 4px solid #C68E65; }\n"
"    \n"
"    #top_nav { background-color: white; border-bottom: 2px solid #F2D2B5; }\n"
"    #top_nav QPushButton { background-color: transparent; border: none; padding: 12px 25px; color: #8B4513; font-size: 13px; font-weight: 600; border-radius: 6px; margin: 5px; }\n"
"    #top_nav QPushButton:hover { background-color: #FFF0E0; color: #4E2C23; }\n"
"    #top_nav QPushButton:checked { background-color: #C68E65; color: white; }\n"
"    \n"
"    #form_container, #table_container, #update_container { background-color: white; border: 1px solid #F2D2B5; border-radius: "
                        "15px; margin: 20px; }\n"
"    \n"
"    QLabel { color: #4E2C23; font-size: 13px; font-weight: bold; }\n"
"    QLineEdit, QSpinBox, QDoubleSpinBox, QDateEdit { \n"
"        background-color: #FFFCFA; border: 1px solid #E0D0C0; padding: 10px; border-radius: 6px; color: #3D362D; font-size: 13px;\n"
"    }\n"
"    QComboBox {\n"
"        background-color: white; border: 1px solid #D2B48C; padding: 10px; border-radius: 6px; color: #000000; font-size: 13px;\n"
"    }\n"
"    QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QDateEdit:focus {\n"
"        border: 2px solid #C68E65; background-color: white;\n"
"    }\n"
"    QComboBox QAbstractItemView { background-color: white; color: black; selection-background-color: #C68E65; selection-color: white; border: 1px solid #D2B48C; }\n"
"    \n"
"    #btn_valider, #btn_update_confirm, #btn_search_update { background-color: #8B4513; color: white; border-radius: 6px; padding: 10px 30px; font-weight: bold; border: none; }\n"
"    #btn_valider:hover, #btn_update_confirm:"
                        "hover, #btn_search_update:hover { background-color: #4E2C23; }\n"
"    \n"
"    #btn_delete_action { background-color: #4E2C23; color: white; border-radius: 6px; padding: 10px 30px; font-weight: bold; }\n"
"    #btn_delete_action:hover { background-color: #3D362D; }\n"
"    \n"
"    #btn_pdf { background-color: #C68E65; color: white; border-radius: 6px; padding: 8px 15px; font-weight: bold; border: none; }\n"
"    #btn_pdf:hover { background-color: #8B4513; }\n"
"\n"
"    QTableWidget { background-color: white; color: #3D362D; gridline-color: #F2D2B5; border: none; alternate-background-color: #FFFCFA; selection-background-color: #F2D2B5; selection-color: #4E2C23; }\n"
"    QHeaderView::section { background-color: #3D362D; color: #F2D2B5; padding: 8px; border: none; font-weight: bold; }\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QHBoxLayout(centralwidget);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        sidebar = new QFrame(centralwidget);
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

        btn_nav_ai = new QPushButton(sidebar);
        btn_nav_ai->setObjectName("btn_nav_ai");

        sidebarLayout->addWidget(btn_nav_ai);

        vSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarLayout->addItem(vSpacer);


        mainLayout->addWidget(sidebar);

        contentArea = new QVBoxLayout();
        contentArea->setSpacing(0);
        contentArea->setObjectName("contentArea");
        top_nav = new QFrame(centralwidget);
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


        contentArea->addWidget(top_nav);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
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

        stackedWidget->addWidget(page_add);
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

        stackedWidget->addWidget(page_update);
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

        stackedWidget->addWidget(page_delete);
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

        stackedWidget->addWidget(page_list);
        page_material = new QWidget();
        page_material->setObjectName("page_material");
        verticalLayout_material = new QVBoxLayout(page_material);
        verticalLayout_material->setObjectName("verticalLayout_material");
        label_material = new QLabel(page_material);
        label_material->setObjectName("label_material");
        label_material->setAlignment(Qt::AlignCenter);

        verticalLayout_material->addWidget(label_material);

        stackedWidget->addWidget(page_material);
        page_employees = new QWidget();
        page_employees->setObjectName("page_employees");
        verticalLayout_employees = new QVBoxLayout(page_employees);
        verticalLayout_employees->setObjectName("verticalLayout_employees");
        label_employees = new QLabel(page_employees);
        label_employees->setObjectName("label_employees");
        label_employees->setAlignment(Qt::AlignCenter);

        verticalLayout_employees->addWidget(label_employees);

        stackedWidget->addWidget(page_employees);
        page_ai = new QWidget();
        page_ai->setObjectName("page_ai");
        verticalLayout_ai = new QVBoxLayout(page_ai);
        verticalLayout_ai->setObjectName("verticalLayout_ai");
        label_ai = new QLabel(page_ai);
        label_ai->setObjectName("label_ai");
        label_ai->setAlignment(Qt::AlignCenter);

        verticalLayout_ai->addWidget(label_ai);

        stackedWidget->addWidget(page_ai);

        contentArea->addWidget(stackedWidget);


        mainLayout->addLayout(contentArea);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);
        tabWidget_stats->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "NextGen - Leader Factory Management", nullptr));
        logo_label->setText(QString());
        factory_title->setText(QCoreApplication::translate("MainWindow", "Leader Factory Management", nullptr));
        btn_nav_orders->setText(QCoreApplication::translate("MainWindow", "COMMANDES", nullptr));
        btn_nav_material->setText(QCoreApplication::translate("MainWindow", "Mati\303\250res Premi\303\250res", nullptr));
        btn_nav_employees->setText(QCoreApplication::translate("MainWindow", "EMPLOY\303\211S", nullptr));
        btn_nav_ai->setText(QCoreApplication::translate("MainWindow", "ASSISTANT IA", nullptr));
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
        label_material->setText(QCoreApplication::translate("MainWindow", "GESTION DES MATI\303\210RES PREMI\303\210RES", nullptr));
        label_material->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 24px; font-weight: bold; color: #8B4513;", nullptr));
        label_employees->setText(QCoreApplication::translate("MainWindow", "GESTION DES EMPLOY\303\211S", nullptr));
        label_employees->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 24px; font-weight: bold; color: #8B4513;", nullptr));
        label_ai->setText(QCoreApplication::translate("MainWindow", "ASSISTANT IA", nullptr));
        label_ai->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 24px; font-weight: bold; color: #8B4513;", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
