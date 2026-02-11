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
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
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
    QFrame *sidebar;
    QVBoxLayout *sidebarLayout;
    QLabel *logo_label;
    QLabel *factory_title;
    QPushButton *btn_nav_orders;
    QPushButton *btn_nav_material;
    QPushButton *btn_nav_employees;
    QPushButton *btn_nav_ai;
    QSpacerItem *vSpacer;
    QFrame *top_nav;
    QHBoxLayout *navLayout;
    QPushButton *btn_tab_add;
    QPushButton *btn_tab_update;
    QPushButton *btn_tab_delete;
    QPushButton *btn_tab_list;
    QStackedWidget *stackedWidget;
    QWidget *page_add;
    QVBoxLayout *verticalLayout_add;
    QFrame *frame_add;
    QFormLayout *formLayout_add;
    QLabel *label;
    QLineEdit *le_mat_name;
    QLabel *label1;
    QComboBox *cb_mat_type;
    QLabel *label2;
    QDoubleSpinBox *dsb_mat_price;
    QLabel *label3;
    QSpinBox *sb_mat_qty;
    QLabel *label4;
    QHBoxLayout *colorLayoutAdd;
    QLineEdit *le_mat_color;
    QPushButton *btn_color_add;
    QLabel *label5;
    QDoubleSpinBox *dsb_mat_yield;
    QLabel *label6;
    QDoubleSpinBox *dsb_mat_loss;
    QPushButton *btn_mat_add;
    QSpacerItem *verticalSpacer_add;
    QWidget *page_update;
    QVBoxLayout *verticalLayout_upd;
    QFrame *frame_upd;
    QVBoxLayout *vbox_upd_inner;
    QHBoxLayout *hbox_search_upd;
    QLineEdit *le_upd_search_name;
    QPushButton *btn_upd_load;
    QFormLayout *formLayout_upd;
    QLabel *label10;
    QLineEdit *le_upd_name;
    QLabel *label11;
    QComboBox *cb_upd_type;
    QLabel *label12;
    QDoubleSpinBox *dsb_upd_price;
    QLabel *label13;
    QSpinBox *sb_upd_qty;
    QLabel *label14;
    QHBoxLayout *colorLayoutUpd;
    QLineEdit *le_upd_color;
    QPushButton *btn_color_upd;
    QLabel *label15;
    QDoubleSpinBox *dsb_upd_yield;
    QLabel *label16;
    QDoubleSpinBox *dsb_upd_loss;
    QPushButton *btn_upd_confirm;
    QSpacerItem *verticalSpacer_upd;
    QWidget *page_delete;
    QVBoxLayout *verticalLayout_del;
    QFrame *frame_del;
    QVBoxLayout *vbox_del;
    QLabel *lbl_del_info;
    QLineEdit *le_del_name;
    QPushButton *btn_del_confirm;
    QSpacerItem *verticalSpacer_del;
    QWidget *page_list;
    QVBoxLayout *verticalLayout_list;
    QTabWidget *tabWidget_stats;
    QWidget *tab_data;
    QVBoxLayout *layout_tab_data;
    QHBoxLayout *hbox_tools;
    QLineEdit *le_search_mat;
    QComboBox *cb_sort_mat;
    QPushButton *btn_export_pdf;
    QTableWidget *table_mat;
    QWidget *tab_stats;
    QGridLayout *gridLayout_stats;
    QFrame *stat_card_total;
    QVBoxLayout *vbox_total;
    QLabel *lbl_t1;
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
        sidebar = new QFrame(centralwidget);
        sidebar->setObjectName("sidebar");
        sidebar->setGeometry(QRect(10, 10, 271, 872));
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

        top_nav = new QFrame(centralwidget);
        top_nav->setObjectName("top_nav");
        top_nav->setGeometry(QRect(283, 1, 916, 73));
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

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(283, 74, 900, 780));
        page_add = new QWidget();
        page_add->setObjectName("page_add");
        verticalLayout_add = new QVBoxLayout(page_add);
        verticalLayout_add->setObjectName("verticalLayout_add");
        frame_add = new QFrame(page_add);
        frame_add->setObjectName("frame_add");
        formLayout_add = new QFormLayout(frame_add);
        formLayout_add->setObjectName("formLayout_add");
        label = new QLabel(frame_add);
        label->setObjectName("label");

        formLayout_add->setWidget(0, QFormLayout::LabelRole, label);

        le_mat_name = new QLineEdit(frame_add);
        le_mat_name->setObjectName("le_mat_name");

        formLayout_add->setWidget(0, QFormLayout::FieldRole, le_mat_name);

        label1 = new QLabel(frame_add);
        label1->setObjectName("label1");

        formLayout_add->setWidget(1, QFormLayout::LabelRole, label1);

        cb_mat_type = new QComboBox(frame_add);
        cb_mat_type->addItem(QString());
        cb_mat_type->addItem(QString());
        cb_mat_type->addItem(QString());
        cb_mat_type->addItem(QString());
        cb_mat_type->addItem(QString());
        cb_mat_type->setObjectName("cb_mat_type");

        formLayout_add->setWidget(1, QFormLayout::FieldRole, cb_mat_type);

        label2 = new QLabel(frame_add);
        label2->setObjectName("label2");

        formLayout_add->setWidget(2, QFormLayout::LabelRole, label2);

        dsb_mat_price = new QDoubleSpinBox(frame_add);
        dsb_mat_price->setObjectName("dsb_mat_price");
        dsb_mat_price->setMaximum(1000000.000000000000000);

        formLayout_add->setWidget(2, QFormLayout::FieldRole, dsb_mat_price);

        label3 = new QLabel(frame_add);
        label3->setObjectName("label3");

        formLayout_add->setWidget(3, QFormLayout::LabelRole, label3);

        sb_mat_qty = new QSpinBox(frame_add);
        sb_mat_qty->setObjectName("sb_mat_qty");
        sb_mat_qty->setMaximum(100000);

        formLayout_add->setWidget(3, QFormLayout::FieldRole, sb_mat_qty);

        label4 = new QLabel(frame_add);
        label4->setObjectName("label4");

        formLayout_add->setWidget(4, QFormLayout::LabelRole, label4);

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

        label5 = new QLabel(frame_add);
        label5->setObjectName("label5");

        formLayout_add->setWidget(5, QFormLayout::LabelRole, label5);

        dsb_mat_yield = new QDoubleSpinBox(frame_add);
        dsb_mat_yield->setObjectName("dsb_mat_yield");
        dsb_mat_yield->setMaximum(100.000000000000000);

        formLayout_add->setWidget(5, QFormLayout::FieldRole, dsb_mat_yield);

        label6 = new QLabel(frame_add);
        label6->setObjectName("label6");

        formLayout_add->setWidget(6, QFormLayout::LabelRole, label6);

        dsb_mat_loss = new QDoubleSpinBox(frame_add);
        dsb_mat_loss->setObjectName("dsb_mat_loss");
        dsb_mat_loss->setMaximum(100.000000000000000);

        formLayout_add->setWidget(6, QFormLayout::FieldRole, dsb_mat_loss);

        btn_mat_add = new QPushButton(frame_add);
        btn_mat_add->setObjectName("btn_mat_add");

        formLayout_add->setWidget(7, QFormLayout::FieldRole, btn_mat_add);


        verticalLayout_add->addWidget(frame_add);

        verticalSpacer_add = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_add->addItem(verticalSpacer_add);

        stackedWidget->addWidget(page_add);
        page_update = new QWidget();
        page_update->setObjectName("page_update");
        verticalLayout_upd = new QVBoxLayout(page_update);
        verticalLayout_upd->setObjectName("verticalLayout_upd");
        frame_upd = new QFrame(page_update);
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
        label10 = new QLabel(frame_upd);
        label10->setObjectName("label10");

        formLayout_upd->setWidget(0, QFormLayout::LabelRole, label10);

        le_upd_name = new QLineEdit(frame_upd);
        le_upd_name->setObjectName("le_upd_name");

        formLayout_upd->setWidget(0, QFormLayout::FieldRole, le_upd_name);

        label11 = new QLabel(frame_upd);
        label11->setObjectName("label11");

        formLayout_upd->setWidget(1, QFormLayout::LabelRole, label11);

        cb_upd_type = new QComboBox(frame_upd);
        cb_upd_type->addItem(QString());
        cb_upd_type->addItem(QString());
        cb_upd_type->addItem(QString());
        cb_upd_type->addItem(QString());
        cb_upd_type->addItem(QString());
        cb_upd_type->setObjectName("cb_upd_type");

        formLayout_upd->setWidget(1, QFormLayout::FieldRole, cb_upd_type);

        label12 = new QLabel(frame_upd);
        label12->setObjectName("label12");

        formLayout_upd->setWidget(2, QFormLayout::LabelRole, label12);

        dsb_upd_price = new QDoubleSpinBox(frame_upd);
        dsb_upd_price->setObjectName("dsb_upd_price");
        dsb_upd_price->setMaximum(1000000.000000000000000);

        formLayout_upd->setWidget(2, QFormLayout::FieldRole, dsb_upd_price);

        label13 = new QLabel(frame_upd);
        label13->setObjectName("label13");

        formLayout_upd->setWidget(3, QFormLayout::LabelRole, label13);

        sb_upd_qty = new QSpinBox(frame_upd);
        sb_upd_qty->setObjectName("sb_upd_qty");
        sb_upd_qty->setMaximum(100000);

        formLayout_upd->setWidget(3, QFormLayout::FieldRole, sb_upd_qty);

        label14 = new QLabel(frame_upd);
        label14->setObjectName("label14");

        formLayout_upd->setWidget(4, QFormLayout::LabelRole, label14);

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

        label15 = new QLabel(frame_upd);
        label15->setObjectName("label15");

        formLayout_upd->setWidget(5, QFormLayout::LabelRole, label15);

        dsb_upd_yield = new QDoubleSpinBox(frame_upd);
        dsb_upd_yield->setObjectName("dsb_upd_yield");
        dsb_upd_yield->setMaximum(100.000000000000000);

        formLayout_upd->setWidget(5, QFormLayout::FieldRole, dsb_upd_yield);

        label16 = new QLabel(frame_upd);
        label16->setObjectName("label16");

        formLayout_upd->setWidget(6, QFormLayout::LabelRole, label16);

        dsb_upd_loss = new QDoubleSpinBox(frame_upd);
        dsb_upd_loss->setObjectName("dsb_upd_loss");
        dsb_upd_loss->setMaximum(100.000000000000000);

        formLayout_upd->setWidget(6, QFormLayout::FieldRole, dsb_upd_loss);


        vbox_upd_inner->addLayout(formLayout_upd);

        btn_upd_confirm = new QPushButton(frame_upd);
        btn_upd_confirm->setObjectName("btn_upd_confirm");

        vbox_upd_inner->addWidget(btn_upd_confirm);


        verticalLayout_upd->addWidget(frame_upd);

        verticalSpacer_upd = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_upd->addItem(verticalSpacer_upd);

        stackedWidget->addWidget(page_update);
        page_delete = new QWidget();
        page_delete->setObjectName("page_delete");
        verticalLayout_del = new QVBoxLayout(page_delete);
        verticalLayout_del->setObjectName("verticalLayout_del");
        frame_del = new QFrame(page_delete);
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


        verticalLayout_del->addWidget(frame_del);

        verticalSpacer_del = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_del->addItem(verticalSpacer_del);

        stackedWidget->addWidget(page_delete);
        page_list = new QWidget();
        page_list->setObjectName("page_list");
        verticalLayout_list = new QVBoxLayout(page_list);
        verticalLayout_list->setObjectName("verticalLayout_list");
        tabWidget_stats = new QTabWidget(page_list);
        tabWidget_stats->setObjectName("tabWidget_stats");
        tab_data = new QWidget();
        tab_data->setObjectName("tab_data");
        layout_tab_data = new QVBoxLayout(tab_data);
        layout_tab_data->setObjectName("layout_tab_data");
        hbox_tools = new QHBoxLayout();
        hbox_tools->setObjectName("hbox_tools");
        le_search_mat = new QLineEdit(tab_data);
        le_search_mat->setObjectName("le_search_mat");

        hbox_tools->addWidget(le_search_mat);

        cb_sort_mat = new QComboBox(tab_data);
        cb_sort_mat->addItem(QString());
        cb_sort_mat->addItem(QString());
        cb_sort_mat->addItem(QString());
        cb_sort_mat->addItem(QString());
        cb_sort_mat->setObjectName("cb_sort_mat");

        hbox_tools->addWidget(cb_sort_mat);

        btn_export_pdf = new QPushButton(tab_data);
        btn_export_pdf->setObjectName("btn_export_pdf");

        hbox_tools->addWidget(btn_export_pdf);


        layout_tab_data->addLayout(hbox_tools);

        table_mat = new QTableWidget(tab_data);
        if (table_mat->columnCount() < 7)
            table_mat->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        table_mat->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        table_mat->setObjectName("table_mat");
        table_mat->setColumnCount(7);

        layout_tab_data->addWidget(table_mat);

        tabWidget_stats->addTab(tab_data, QString());
        tab_stats = new QWidget();
        tab_stats->setObjectName("tab_stats");
        gridLayout_stats = new QGridLayout(tab_stats);
        gridLayout_stats->setObjectName("gridLayout_stats");
        stat_card_total = new QFrame(tab_stats);
        stat_card_total->setObjectName("stat_card_total");
        vbox_total = new QVBoxLayout(stat_card_total);
        vbox_total->setObjectName("vbox_total");
        lbl_t1 = new QLabel(stat_card_total);
        lbl_t1->setObjectName("lbl_t1");

        vbox_total->addWidget(lbl_t1);

        lbl_total_mat_val = new QLabel(stat_card_total);
        lbl_total_mat_val->setObjectName("lbl_total_mat_val");
        lbl_total_mat_val->setAlignment(Qt::AlignCenter);

        vbox_total->addWidget(lbl_total_mat_val);


        gridLayout_stats->addWidget(stat_card_total, 0, 0, 1, 1);

        stat_card_rupture = new QFrame(tab_stats);
        stat_card_rupture->setObjectName("stat_card_rupture");
        vbox_rupture = new QVBoxLayout(stat_card_rupture);
        vbox_rupture->setObjectName("vbox_rupture");
        lbl_r1 = new QLabel(stat_card_rupture);
        lbl_r1->setObjectName("lbl_r1");

        vbox_rupture->addWidget(lbl_r1);

        lbl_rupture_val = new QLabel(stat_card_rupture);
        lbl_rupture_val->setObjectName("lbl_rupture_val");
        lbl_rupture_val->setAlignment(Qt::AlignCenter);

        vbox_rupture->addWidget(lbl_rupture_val);


        gridLayout_stats->addWidget(stat_card_rupture, 0, 1, 1, 1);

        stat_card_waste = new QFrame(tab_stats);
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


        gridLayout_stats->addWidget(stat_card_waste, 1, 0, 1, 2);

        frame_graph = new QFrame(tab_stats);
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


        gridLayout_stats->addWidget(frame_graph, 2, 0, 1, 2);

        tabWidget_stats->addTab(tab_stats, QString());

        verticalLayout_list->addWidget(tabWidget_stats);

        stackedWidget->addWidget(page_list);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);
        tabWidget_stats->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Leather Factory Management", nullptr));
        logo_label->setText(QString());
        factory_title->setText(QCoreApplication::translate("MainWindow", "Leather Factory Management", nullptr));
        btn_nav_orders->setText(QCoreApplication::translate("MainWindow", "COMMANDES", nullptr));
        btn_nav_material->setText(QCoreApplication::translate("MainWindow", "Mati\303\250res premi\303\250res", nullptr));
        btn_nav_employees->setText(QCoreApplication::translate("MainWindow", "EMPLOY\303\211S", nullptr));
        btn_nav_ai->setText(QCoreApplication::translate("MainWindow", "ASSISTANT IA", nullptr));
        btn_tab_add->setText(QCoreApplication::translate("MainWindow", "Ajouter une Mati\303\250re premi\303\250re", nullptr));
        btn_tab_update->setText(QCoreApplication::translate("MainWindow", "Modifier une Mati\303\250re premi\303\250re", nullptr));
        btn_tab_delete->setText(QCoreApplication::translate("MainWindow", "Supprimer une Mati\303\250re premi\303\250re", nullptr));
        btn_tab_list->setText(QCoreApplication::translate("MainWindow", "Liste / Stats", nullptr));
        frame_add->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border-radius: 15px; margin: 20px;", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Nom :", nullptr));
        label1->setText(QCoreApplication::translate("MainWindow", "Mati\303\250re :", nullptr));
        cb_mat_type->setItemText(0, QCoreApplication::translate("MainWindow", "Cuir", nullptr));
        cb_mat_type->setItemText(1, QCoreApplication::translate("MainWindow", "Tissu", nullptr));
        cb_mat_type->setItemText(2, QCoreApplication::translate("MainWindow", "Fil", nullptr));
        cb_mat_type->setItemText(3, QCoreApplication::translate("MainWindow", "Accessoire", nullptr));
        cb_mat_type->setItemText(4, QCoreApplication::translate("MainWindow", "Synth\303\251tique", nullptr));

        label2->setText(QCoreApplication::translate("MainWindow", "Prix :", nullptr));
        label3->setText(QCoreApplication::translate("MainWindow", "Quantit\303\251 en stock :", nullptr));
        label4->setText(QCoreApplication::translate("MainWindow", "Couleur :", nullptr));
        btn_color_add->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border: 1px solid #ccc;", nullptr));
        btn_color_add->setText(QString());
        label5->setText(QCoreApplication::translate("MainWindow", "Rendement de mati\303\250re :", nullptr));
        dsb_mat_yield->setSuffix(QCoreApplication::translate("MainWindow", " %", nullptr));
        label6->setText(QCoreApplication::translate("MainWindow", "Perte de mati\303\250re :", nullptr));
        dsb_mat_loss->setSuffix(QCoreApplication::translate("MainWindow", " %", nullptr));
        btn_mat_add->setText(QCoreApplication::translate("MainWindow", "AJOUTER MATI\303\210RE", nullptr));
        btn_mat_add->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #8B4513; color: white; padding: 10px; border-radius: 5px; font-weight: bold;", nullptr));
        frame_upd->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border-radius: 15px; margin: 20px;", nullptr));
        le_upd_search_name->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nom de la mati\303\250re \303\240 modifier...", nullptr));
        btn_upd_load->setText(QCoreApplication::translate("MainWindow", "CHARGER", nullptr));
        label10->setText(QCoreApplication::translate("MainWindow", "Nom :", nullptr));
        label11->setText(QCoreApplication::translate("MainWindow", "Mati\303\250re :", nullptr));
        cb_upd_type->setItemText(0, QCoreApplication::translate("MainWindow", "Cuir", nullptr));
        cb_upd_type->setItemText(1, QCoreApplication::translate("MainWindow", "Tissu", nullptr));
        cb_upd_type->setItemText(2, QCoreApplication::translate("MainWindow", "Fil", nullptr));
        cb_upd_type->setItemText(3, QCoreApplication::translate("MainWindow", "Accessoire", nullptr));
        cb_upd_type->setItemText(4, QCoreApplication::translate("MainWindow", "Synth\303\251tique", nullptr));

        label12->setText(QCoreApplication::translate("MainWindow", "Prix :", nullptr));
        label13->setText(QCoreApplication::translate("MainWindow", "Quantit\303\251 :", nullptr));
        label14->setText(QCoreApplication::translate("MainWindow", "Couleur :", nullptr));
        btn_color_upd->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border: 1px solid #ccc;", nullptr));
        btn_color_upd->setText(QString());
        label15->setText(QCoreApplication::translate("MainWindow", "Rendement :", nullptr));
        dsb_upd_yield->setSuffix(QCoreApplication::translate("MainWindow", " %", nullptr));
        label16->setText(QCoreApplication::translate("MainWindow", "Perte :", nullptr));
        dsb_upd_loss->setSuffix(QCoreApplication::translate("MainWindow", " %", nullptr));
        btn_upd_confirm->setText(QCoreApplication::translate("MainWindow", "ENREGISTRER MODIFICATIONS", nullptr));
        btn_upd_confirm->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #8B4513; color: white; padding: 10px; border-radius: 5px;", nullptr));
        frame_del->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border-radius: 15px; margin: 20px;", nullptr));
        lbl_del_info->setText(QCoreApplication::translate("MainWindow", "Entrez le nom de la mati\303\250re \303\240 supprimer :", nullptr));
        btn_del_confirm->setText(QCoreApplication::translate("MainWindow", "SUPPRIMER D\303\211FINITIVEMENT", nullptr));
        btn_del_confirm->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #8B0000; color: white; padding: 10px; border-radius: 5px; font-weight: bold;", nullptr));
        le_search_mat->setPlaceholderText(QCoreApplication::translate("MainWindow", "Rechercher par Nom ou Cat\303\251gorie...", nullptr));
        cb_sort_mat->setItemText(0, QCoreApplication::translate("MainWindow", "Trier par...", nullptr));
        cb_sort_mat->setItemText(1, QCoreApplication::translate("MainWindow", "Nom", nullptr));
        cb_sort_mat->setItemText(2, QCoreApplication::translate("MainWindow", "Prix (Croissant)", nullptr));
        cb_sort_mat->setItemText(3, QCoreApplication::translate("MainWindow", "Prix (D\303\251croissant)", nullptr));

        btn_export_pdf->setText(QCoreApplication::translate("MainWindow", "Export PDF", nullptr));
        QTableWidgetItem *___qtablewidgetitem = table_mat->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = table_mat->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Mati\303\250re", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = table_mat->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Prix", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = table_mat->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Quantit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = table_mat->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Couleur", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = table_mat->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Rendement", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = table_mat->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Perte", nullptr));
        tabWidget_stats->setTabText(tabWidget_stats->indexOf(tab_data), QCoreApplication::translate("MainWindow", "Liste des Mati\303\250res", nullptr));
        stat_card_total->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #C68E65; color: white; border-radius: 8px; padding: 10px;", nullptr));
        lbl_t1->setText(QCoreApplication::translate("MainWindow", "TOTAL MATI\303\210RES", nullptr));
        lbl_total_mat_val->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lbl_total_mat_val->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 24px; font-weight: bold;", nullptr));
        stat_card_rupture->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #8B0000; color: white; border-radius: 8px; padding: 10px;", nullptr));
        lbl_r1->setText(QCoreApplication::translate("MainWindow", "ALERTE RUPTURE (< 10)", nullptr));
        lbl_rupture_val->setText(QCoreApplication::translate("MainWindow", "Aucune alerte", nullptr));
        lbl_rupture_val->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 14px; font-weight: bold;", nullptr));
        stat_card_waste->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #2E8B57; color: white; border-radius: 8px; padding: 10px;", nullptr));
        lbl_w1->setText(QCoreApplication::translate("MainWindow", "CONSEIL ANTI-GASPILLAGE", nullptr));
        lbl_waste_val->setText(QCoreApplication::translate("MainWindow", "Analyse en cours...", nullptr));
        lbl_waste_val->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; font-style: italic;", nullptr));
        frame_graph->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border-radius: 8px; padding: 10px; border: 1px solid #ccc;", nullptr));
        lbl_g1->setText(QCoreApplication::translate("MainWindow", "<b>Analyse Rendement vs Perte (Moyenne)</b>", nullptr));
        lbl_g2->setText(QCoreApplication::translate("MainWindow", "Rendement Moyen:", nullptr));
        pb_yield->setStyleSheet(QCoreApplication::translate("MainWindow", "QProgressBar::chunk { background-color: #4CAF50; }", nullptr));
        lbl_g3->setText(QCoreApplication::translate("MainWindow", "Perte Moyenne:", nullptr));
        pb_loss->setStyleSheet(QCoreApplication::translate("MainWindow", "QProgressBar::chunk { background-color: #F44336; }", nullptr));
        tabWidget_stats->setTabText(tabWidget_stats->indexOf(tab_stats), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
