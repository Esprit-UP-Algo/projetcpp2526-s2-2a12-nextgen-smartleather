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
#include <QtWidgets/QCalendarWidget>
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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
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
    QPushButton *btn_nav_suppliers;
    QPushButton *btn_nav_ai;
    QSpacerItem *vSpacer;
    QVBoxLayout *contentArea;
    QFrame *top_nav;
    QHBoxLayout *navLayout;
    QPushButton *btn_tab_add;
    QPushButton *btn_tab_update;
    QPushButton *btn_tab_delete;
    QPushButton *btn_tab_list;
    QPushButton *btn_tab_stats;
    QPushButton *btn_tab_email_auto;
    QPushButton *btn_tab_calendar;
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
    QHBoxLayout *statsActionsLayout;
    QPushButton *btn_stat_price_asc;
    QPushButton *btn_stat_price_desc;
    QPushButton *btn_stat_type;
    QLabel *lbl_price_stats;
    QLabel *lbl_type_stats;
    QLabel *lbl_city_stats;
    QWidget *page_stats;
    QVBoxLayout *pageStatsLayout;
    QScrollArea *scrollArea_stats;
    QWidget *scrollAreaWidgetContents_stats;
    QVBoxLayout *pageStatsInnerLayout;
    QLabel *label_stats_title;
    QGridLayout *statsMainGrid;
    QFrame *stat_main_card_orders;
    QVBoxLayout *vboxMainOrders;
    QLabel *lbl_main_orders_title;
    QLabel *lbl_main_orders_value;
    QFrame *stat_main_card_revenue;
    QVBoxLayout *vboxMainRevenue;
    QLabel *lbl_main_revenue_title;
    QLabel *lbl_main_revenue_value;
    QFrame *stat_main_card_pending;
    QVBoxLayout *vboxMainPending;
    QLabel *lbl_main_pending_title;
    QLabel *lbl_main_pending_value;
    QFrame *stats_chart_frame;
    QVBoxLayout *chartMainLayout;
    QLabel *lbl_main_chart_title;
    QLabel *lbl_main_type_stats;
    QFrame *stats_main_details_container;
    QGridLayout *statsMainDetailsGrid;
    QLabel *lbl_main_top_product_title;
    QLabel *lbl_main_top_product_value;
    QLabel *lbl_main_top_city_title;
    QLabel *lbl_main_top_city_value;
    QLabel *lbl_main_avg_price_title;
    QLabel *lbl_main_avg_price_value;
    QWidget *page_email_auto;
    QVBoxLayout *pageEmailLayout;
    QLabel *label_email_title;
    QLabel *lbl_email_auto_info;
    QSpacerItem *verticalSpacer_email_top;
    QSpacerItem *verticalSpacer_email_bottom;
    QWidget *page_calendar;
    QVBoxLayout *pageCalendarLayout;
    QLabel *label_calendar_title;
    QHBoxLayout *calendarMainLayout;
    QFrame *calendar_container;
    QVBoxLayout *calendarLayout;
    QCalendarWidget *calendarWidget;
    QHBoxLayout *calendarLegendLayout;
    QLabel *lbl_legend_today;
    QLabel *lbl_legend_delivery;
    QLabel *lbl_legend_urgent;
    QFrame *deliveries_list_container;
    QVBoxLayout *deliveriesListLayout;
    QLabel *lbl_deliveries_title;
    QListWidget *list_deliveries;
    QLabel *lbl_selected_date;
    QPushButton *btn_export_calendar;
    QFrame *calendar_stats_container;
    QHBoxLayout *calendarStatsLayout;
    QFrame *stat_this_week;
    QVBoxLayout *vboxThisWeek;
    QLabel *lbl_this_week_title;
    QLabel *lbl_this_week_value;
    QFrame *stat_this_month;
    QVBoxLayout *vboxThisMonth;
    QLabel *lbl_this_month_title;
    QLabel *lbl_this_month_value;
    QFrame *stat_overdue;
    QVBoxLayout *vboxOverdue;
    QLabel *lbl_overdue_title;
    QLabel *lbl_overdue_value;
    QWidget *page_innov;
    QVBoxLayout *pageInnovLayout;
    QWidget *page_material;
    QVBoxLayout *verticalLayout_material;
    QLabel *label_material;
    QWidget *page_employees;
    QVBoxLayout *verticalLayout_employees;
    QLabel *label_employees;
    QWidget *page_suppliers;
    QVBoxLayout *verticalLayout_suppliers;
    QLabel *label_suppliers;
    QWidget *page_ai;
    QVBoxLayout *verticalLayout_ai;
    QLabel *label_ai;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 1657);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"    QMainWindow { background-color: #FCF9F6; }\n"
"    \n"
"    #sidebar { background-color: #3D362D; border-right: 1px solid #4E2C23; }\n"
"    #sidebar QPushButton { background-color: transparent; color: #F2D2B5; text-align: left; padding: 15px 20px; border: none; font-weight: bold; text-transform: uppercase; font-size: 12px; }\n"
"    #sidebar QPushButton:hover { background-color: #4E2C23; color: #C68E65; border-left: 4px solid #C68E65; }\n"
"    \n"
"    #top_nav { background-color: white; border-bottom: 2px solid #F2D2B5; }\n"
"    #top_nav QPushButton { background-color: transparent; border: none; padding: 8px 12px; color: #8B4513; font-size: 12px; font-weight: 600; border-radius: 6px; margin: 3px; }\n"
"    #top_nav QPushButton:hover { background-color: #FFF0E0; color: #4E2C23; }\n"
"    #top_nav QPushButton:checked { background-color: #C68E65; color: white; }\n"
"    \n"
"   #form_container, #table_container, #update_container, #innov_container { background-color: white; border: 1px solid #F2D2B5;"
                        " border-radius: 15px; margin: 20px; }\n"
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
"    #btn_valider:hover, #btn"
                        "_update_confirm:hover, #btn_search_update:hover { background-color: #4E2C23; }\n"
"    \n"
"    #btn_delete_action { background-color: #4E2C23; color: white; border-radius: 6px; padding: 10px 30px; font-weight: bold; }\n"
"    #btn_delete_action:hover { background-color: #3D362D; }\n"
"    \n"
"    #btn_pdf { background-color: #C68E65; color: white; border-radius: 6px; padding: 8px 15px; font-weight: bold; border: none; }\n"
"    #btn_pdf:hover { background-color: #8B4513; }\n"
"\n"
"    QTableWidget { background-color: white; color: #3D362D; gridline-color: #F2D2B5; border: none; alternate-background-color: #FFFCFA; selection-background-color: #F2D2B5; selection-color: #4E2C23; }\n"
"    QHeaderView::section { background-color: #3D362D; color: #F2D2B5; padding: 8px; border: none; font-weight: bold; }\n"
"\n"
"    QCalendarWidget QWidget#qt_calendar_navigationbar {\n"
"      background: transparent;\n"
"    }\n"
"    QCalendarWidget QToolButton {\n"
"      color: #3D362D;\n"
"      background-color: #FFF0E0;\n"
""
                        "      border: 1px solid #E0D0C0;\n"
"      border-radius: 10px;\n"
"      padding: 6px 10px;\n"
"      min-width: 34px;\n"
"      font-weight: 600;\n"
"    }\n"
"    QCalendarWidget QToolButton:hover {\n"
"      background-color: #F2D2B5;\n"
"    }\n"
"    QCalendarWidget QSpinBox,\n"
"    QCalendarWidget QComboBox {\n"
"      color: #3D362D;\n"
"      background-color: #FFFCFA;\n"
"      border: 1px solid #E0D0C0;\n"
"      border-radius: 10px;\n"
"      padding: 4px 8px;\n"
"      font-weight: 600;\n"
"    }\n"
"    QCalendarWidget QAbstractItemView:enabled {\n"
"      color: #3D362D;\n"
"      background-color: white;\n"
"      selection-background-color: #C68E65;\n"
"      selection-color: white;\n"
"    }\n"
"    QCalendarWidget QTableView {\n"
"      outline: 0;\n"
"    }\n"
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

        btn_nav_suppliers = new QPushButton(sidebar);
        btn_nav_suppliers->setObjectName("btn_nav_suppliers");

        sidebarLayout->addWidget(btn_nav_suppliers);

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

        btn_tab_stats = new QPushButton(top_nav);
        btn_tab_stats->setObjectName("btn_tab_stats");
        btn_tab_stats->setCheckable(true);

        navLayout->addWidget(btn_tab_stats);

        btn_tab_email_auto = new QPushButton(top_nav);
        btn_tab_email_auto->setObjectName("btn_tab_email_auto");
        btn_tab_email_auto->setCheckable(true);

        navLayout->addWidget(btn_tab_email_auto);

        btn_tab_calendar = new QPushButton(top_nav);
        btn_tab_calendar->setObjectName("btn_tab_calendar");
        btn_tab_calendar->setCheckable(true);

        navLayout->addWidget(btn_tab_calendar);


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
        dsb_price->setMinimumSize(QSize(0, 36));
        dsb_price->setMaximumSize(QSize(16777215, 36));
        dsb_price->setButtonSymbols(QAbstractSpinBox::NoButtons);
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
        le_email_update->setMaximumSize(QSize(16777215, 36));

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
        de_date_update->setMaximumSize(QSize(16777215, 36));
        de_date_update->setCalendarPopup(true);

        updateForm->setWidget(6, QFormLayout::FieldRole, de_date_update);

        label17 = new QLabel(update_container);
        label17->setObjectName("label17");

        updateForm->setWidget(7, QFormLayout::LabelRole, label17);

        de_date_delivery_update = new QDateEdit(update_container);
        de_date_delivery_update->setObjectName("de_date_delivery_update");
        de_date_delivery_update->setMaximumSize(QSize(16777215, 36));
        de_date_delivery_update->setCalendarPopup(true);

        updateForm->setWidget(7, QFormLayout::FieldRole, de_date_delivery_update);

        label18 = new QLabel(update_container);
        label18->setObjectName("label18");

        updateForm->setWidget(8, QFormLayout::LabelRole, label18);

        dsb_price_update = new QDoubleSpinBox(update_container);
        dsb_price_update->setObjectName("dsb_price_update");
        dsb_price_update->setMinimumSize(QSize(0, 36));
        dsb_price_update->setMaximumSize(QSize(16777215, 36));
        dsb_price_update->setButtonSymbols(QAbstractSpinBox::NoButtons);
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
        statsActionsLayout = new QHBoxLayout();
        statsActionsLayout->setObjectName("statsActionsLayout");
        btn_stat_price_asc = new QPushButton(chart_container);
        btn_stat_price_asc->setObjectName("btn_stat_price_asc");

        statsActionsLayout->addWidget(btn_stat_price_asc);

        btn_stat_price_desc = new QPushButton(chart_container);
        btn_stat_price_desc->setObjectName("btn_stat_price_desc");

        statsActionsLayout->addWidget(btn_stat_price_desc);

        btn_stat_type = new QPushButton(chart_container);
        btn_stat_type->setObjectName("btn_stat_type");

        statsActionsLayout->addWidget(btn_stat_type);


        chartLayout->addLayout(statsActionsLayout);

        lbl_price_stats = new QLabel(chart_container);
        lbl_price_stats->setObjectName("lbl_price_stats");
        lbl_price_stats->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        lbl_price_stats->setWordWrap(true);

        chartLayout->addWidget(lbl_price_stats);

        lbl_type_stats = new QLabel(chart_container);
        lbl_type_stats->setObjectName("lbl_type_stats");
        lbl_type_stats->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        lbl_type_stats->setWordWrap(true);

        chartLayout->addWidget(lbl_type_stats);

        lbl_city_stats = new QLabel(chart_container);
        lbl_city_stats->setObjectName("lbl_city_stats");
        lbl_city_stats->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        lbl_city_stats->setWordWrap(true);

        chartLayout->addWidget(lbl_city_stats);


        statsGridLayout->addWidget(chart_container, 1, 0, 1, 2);

        tabWidget_stats->addTab(tab_stats, QString());

        pageListLayout->addWidget(tabWidget_stats);

        stackedWidget->addWidget(page_list);
        page_stats = new QWidget();
        page_stats->setObjectName("page_stats");
        pageStatsLayout = new QVBoxLayout(page_stats);
        pageStatsLayout->setSpacing(0);
        pageStatsLayout->setObjectName("pageStatsLayout");
        pageStatsLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea_stats = new QScrollArea(page_stats);
        scrollArea_stats->setObjectName("scrollArea_stats");
        scrollArea_stats->setFrameShape(QFrame::Shape::NoFrame);
        scrollArea_stats->setWidgetResizable(true);
        scrollAreaWidgetContents_stats = new QWidget();
        scrollAreaWidgetContents_stats->setObjectName("scrollAreaWidgetContents_stats");
        scrollAreaWidgetContents_stats->setGeometry(QRect(0, 0, 920, 1595));
        pageStatsInnerLayout = new QVBoxLayout(scrollAreaWidgetContents_stats);
        pageStatsInnerLayout->setSpacing(15);
        pageStatsInnerLayout->setObjectName("pageStatsInnerLayout");
        pageStatsInnerLayout->setContentsMargins(15, 10, 15, 20);
        label_stats_title = new QLabel(scrollAreaWidgetContents_stats);
        label_stats_title->setObjectName("label_stats_title");
        label_stats_title->setAlignment(Qt::AlignmentFlag::AlignCenter);

        pageStatsInnerLayout->addWidget(label_stats_title);

        statsMainGrid = new QGridLayout();
        statsMainGrid->setObjectName("statsMainGrid");
        stat_main_card_orders = new QFrame(scrollAreaWidgetContents_stats);
        stat_main_card_orders->setObjectName("stat_main_card_orders");
        vboxMainOrders = new QVBoxLayout(stat_main_card_orders);
        vboxMainOrders->setObjectName("vboxMainOrders");
        lbl_main_orders_title = new QLabel(stat_main_card_orders);
        lbl_main_orders_title->setObjectName("lbl_main_orders_title");

        vboxMainOrders->addWidget(lbl_main_orders_title);

        lbl_main_orders_value = new QLabel(stat_main_card_orders);
        lbl_main_orders_value->setObjectName("lbl_main_orders_value");
        lbl_main_orders_value->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxMainOrders->addWidget(lbl_main_orders_value);


        statsMainGrid->addWidget(stat_main_card_orders, 0, 0, 1, 1);

        stat_main_card_revenue = new QFrame(scrollAreaWidgetContents_stats);
        stat_main_card_revenue->setObjectName("stat_main_card_revenue");
        vboxMainRevenue = new QVBoxLayout(stat_main_card_revenue);
        vboxMainRevenue->setObjectName("vboxMainRevenue");
        lbl_main_revenue_title = new QLabel(stat_main_card_revenue);
        lbl_main_revenue_title->setObjectName("lbl_main_revenue_title");

        vboxMainRevenue->addWidget(lbl_main_revenue_title);

        lbl_main_revenue_value = new QLabel(stat_main_card_revenue);
        lbl_main_revenue_value->setObjectName("lbl_main_revenue_value");
        lbl_main_revenue_value->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxMainRevenue->addWidget(lbl_main_revenue_value);


        statsMainGrid->addWidget(stat_main_card_revenue, 0, 1, 1, 1);

        stat_main_card_pending = new QFrame(scrollAreaWidgetContents_stats);
        stat_main_card_pending->setObjectName("stat_main_card_pending");
        vboxMainPending = new QVBoxLayout(stat_main_card_pending);
        vboxMainPending->setObjectName("vboxMainPending");
        lbl_main_pending_title = new QLabel(stat_main_card_pending);
        lbl_main_pending_title->setObjectName("lbl_main_pending_title");

        vboxMainPending->addWidget(lbl_main_pending_title);

        lbl_main_pending_value = new QLabel(stat_main_card_pending);
        lbl_main_pending_value->setObjectName("lbl_main_pending_value");
        lbl_main_pending_value->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxMainPending->addWidget(lbl_main_pending_value);


        statsMainGrid->addWidget(stat_main_card_pending, 0, 2, 1, 1);

        stats_chart_frame = new QFrame(scrollAreaWidgetContents_stats);
        stats_chart_frame->setObjectName("stats_chart_frame");
        chartMainLayout = new QVBoxLayout(stats_chart_frame);
        chartMainLayout->setObjectName("chartMainLayout");
        lbl_main_chart_title = new QLabel(stats_chart_frame);
        lbl_main_chart_title->setObjectName("lbl_main_chart_title");
        lbl_main_chart_title->setAlignment(Qt::AlignmentFlag::AlignCenter);

        chartMainLayout->addWidget(lbl_main_chart_title);

        lbl_main_type_stats = new QLabel(stats_chart_frame);
        lbl_main_type_stats->setObjectName("lbl_main_type_stats");
        lbl_main_type_stats->setMinimumSize(QSize(0, 150));
        lbl_main_type_stats->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        lbl_main_type_stats->setWordWrap(true);

        chartMainLayout->addWidget(lbl_main_type_stats);


        statsMainGrid->addWidget(stats_chart_frame, 1, 0, 1, 3);

        stats_main_details_container = new QFrame(scrollAreaWidgetContents_stats);
        stats_main_details_container->setObjectName("stats_main_details_container");
        statsMainDetailsGrid = new QGridLayout(stats_main_details_container);
        statsMainDetailsGrid->setObjectName("statsMainDetailsGrid");
        lbl_main_top_product_title = new QLabel(stats_main_details_container);
        lbl_main_top_product_title->setObjectName("lbl_main_top_product_title");

        statsMainDetailsGrid->addWidget(lbl_main_top_product_title, 0, 0, 1, 1);

        lbl_main_top_product_value = new QLabel(stats_main_details_container);
        lbl_main_top_product_value->setObjectName("lbl_main_top_product_value");

        statsMainDetailsGrid->addWidget(lbl_main_top_product_value, 0, 1, 1, 1);

        lbl_main_top_city_title = new QLabel(stats_main_details_container);
        lbl_main_top_city_title->setObjectName("lbl_main_top_city_title");

        statsMainDetailsGrid->addWidget(lbl_main_top_city_title, 1, 0, 1, 1);

        lbl_main_top_city_value = new QLabel(stats_main_details_container);
        lbl_main_top_city_value->setObjectName("lbl_main_top_city_value");

        statsMainDetailsGrid->addWidget(lbl_main_top_city_value, 1, 1, 1, 1);

        lbl_main_avg_price_title = new QLabel(stats_main_details_container);
        lbl_main_avg_price_title->setObjectName("lbl_main_avg_price_title");

        statsMainDetailsGrid->addWidget(lbl_main_avg_price_title, 2, 0, 1, 1);

        lbl_main_avg_price_value = new QLabel(stats_main_details_container);
        lbl_main_avg_price_value->setObjectName("lbl_main_avg_price_value");

        statsMainDetailsGrid->addWidget(lbl_main_avg_price_value, 2, 1, 1, 1);


        statsMainGrid->addWidget(stats_main_details_container, 2, 0, 1, 3);


        pageStatsInnerLayout->addLayout(statsMainGrid);

        scrollArea_stats->setWidget(scrollAreaWidgetContents_stats);

        pageStatsLayout->addWidget(scrollArea_stats);

        stackedWidget->addWidget(page_stats);
        page_email_auto = new QWidget();
        page_email_auto->setObjectName("page_email_auto");
        pageEmailLayout = new QVBoxLayout(page_email_auto);
        pageEmailLayout->setSpacing(20);
        pageEmailLayout->setObjectName("pageEmailLayout");
        pageEmailLayout->setContentsMargins(20, 30, 20, 40);
        label_email_title = new QLabel(page_email_auto);
        label_email_title->setObjectName("label_email_title");
        label_email_title->setAlignment(Qt::AlignmentFlag::AlignCenter);

        pageEmailLayout->addWidget(label_email_title);

        lbl_email_auto_info = new QLabel(page_email_auto);
        lbl_email_auto_info->setObjectName("lbl_email_auto_info");
        lbl_email_auto_info->setAlignment(Qt::AlignmentFlag::AlignCenter);
        lbl_email_auto_info->setWordWrap(true);

        pageEmailLayout->addWidget(lbl_email_auto_info);

        verticalSpacer_email_top = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        pageEmailLayout->addItem(verticalSpacer_email_top);

        verticalSpacer_email_bottom = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        pageEmailLayout->addItem(verticalSpacer_email_bottom);

        stackedWidget->addWidget(page_email_auto);
        page_calendar = new QWidget();
        page_calendar->setObjectName("page_calendar");
        pageCalendarLayout = new QVBoxLayout(page_calendar);
        pageCalendarLayout->setSpacing(15);
        pageCalendarLayout->setObjectName("pageCalendarLayout");
        pageCalendarLayout->setContentsMargins(15, 10, 15, 20);
        label_calendar_title = new QLabel(page_calendar);
        label_calendar_title->setObjectName("label_calendar_title");
        label_calendar_title->setAlignment(Qt::AlignmentFlag::AlignCenter);

        pageCalendarLayout->addWidget(label_calendar_title);

        calendarMainLayout = new QHBoxLayout();
        calendarMainLayout->setObjectName("calendarMainLayout");
        calendar_container = new QFrame(page_calendar);
        calendar_container->setObjectName("calendar_container");
        calendarLayout = new QVBoxLayout(calendar_container);
        calendarLayout->setObjectName("calendarLayout");
        calendarWidget = new QCalendarWidget(calendar_container);
        calendarWidget->setObjectName("calendarWidget");
        calendarWidget->setMinimumSize(QSize(450, 350));
        calendarWidget->setGridVisible(true);
        calendarWidget->setVerticalHeaderFormat(QCalendarWidget::VerticalHeaderFormat::NoVerticalHeader);

        calendarLayout->addWidget(calendarWidget);

        calendarLegendLayout = new QHBoxLayout();
        calendarLegendLayout->setObjectName("calendarLegendLayout");
        lbl_legend_today = new QLabel(calendar_container);
        lbl_legend_today->setObjectName("lbl_legend_today");

        calendarLegendLayout->addWidget(lbl_legend_today);

        lbl_legend_delivery = new QLabel(calendar_container);
        lbl_legend_delivery->setObjectName("lbl_legend_delivery");

        calendarLegendLayout->addWidget(lbl_legend_delivery);

        lbl_legend_urgent = new QLabel(calendar_container);
        lbl_legend_urgent->setObjectName("lbl_legend_urgent");

        calendarLegendLayout->addWidget(lbl_legend_urgent);


        calendarLayout->addLayout(calendarLegendLayout);


        calendarMainLayout->addWidget(calendar_container);

        deliveries_list_container = new QFrame(page_calendar);
        deliveries_list_container->setObjectName("deliveries_list_container");
        deliveriesListLayout = new QVBoxLayout(deliveries_list_container);
        deliveriesListLayout->setObjectName("deliveriesListLayout");
        lbl_deliveries_title = new QLabel(deliveries_list_container);
        lbl_deliveries_title->setObjectName("lbl_deliveries_title");

        deliveriesListLayout->addWidget(lbl_deliveries_title);

        list_deliveries = new QListWidget(deliveries_list_container);
        list_deliveries->setObjectName("list_deliveries");
        list_deliveries->setMinimumSize(QSize(350, 0));

        deliveriesListLayout->addWidget(list_deliveries);

        lbl_selected_date = new QLabel(deliveries_list_container);
        lbl_selected_date->setObjectName("lbl_selected_date");

        deliveriesListLayout->addWidget(lbl_selected_date);

        btn_export_calendar = new QPushButton(deliveries_list_container);
        btn_export_calendar->setObjectName("btn_export_calendar");

        deliveriesListLayout->addWidget(btn_export_calendar);


        calendarMainLayout->addWidget(deliveries_list_container);


        pageCalendarLayout->addLayout(calendarMainLayout);

        calendar_stats_container = new QFrame(page_calendar);
        calendar_stats_container->setObjectName("calendar_stats_container");
        calendarStatsLayout = new QHBoxLayout(calendar_stats_container);
        calendarStatsLayout->setObjectName("calendarStatsLayout");
        stat_this_week = new QFrame(calendar_stats_container);
        stat_this_week->setObjectName("stat_this_week");
        vboxThisWeek = new QVBoxLayout(stat_this_week);
        vboxThisWeek->setObjectName("vboxThisWeek");
        lbl_this_week_title = new QLabel(stat_this_week);
        lbl_this_week_title->setObjectName("lbl_this_week_title");
        lbl_this_week_title->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxThisWeek->addWidget(lbl_this_week_title);

        lbl_this_week_value = new QLabel(stat_this_week);
        lbl_this_week_value->setObjectName("lbl_this_week_value");
        lbl_this_week_value->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxThisWeek->addWidget(lbl_this_week_value);


        calendarStatsLayout->addWidget(stat_this_week);

        stat_this_month = new QFrame(calendar_stats_container);
        stat_this_month->setObjectName("stat_this_month");
        vboxThisMonth = new QVBoxLayout(stat_this_month);
        vboxThisMonth->setObjectName("vboxThisMonth");
        lbl_this_month_title = new QLabel(stat_this_month);
        lbl_this_month_title->setObjectName("lbl_this_month_title");
        lbl_this_month_title->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxThisMonth->addWidget(lbl_this_month_title);

        lbl_this_month_value = new QLabel(stat_this_month);
        lbl_this_month_value->setObjectName("lbl_this_month_value");
        lbl_this_month_value->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxThisMonth->addWidget(lbl_this_month_value);


        calendarStatsLayout->addWidget(stat_this_month);

        stat_overdue = new QFrame(calendar_stats_container);
        stat_overdue->setObjectName("stat_overdue");
        vboxOverdue = new QVBoxLayout(stat_overdue);
        vboxOverdue->setObjectName("vboxOverdue");
        lbl_overdue_title = new QLabel(stat_overdue);
        lbl_overdue_title->setObjectName("lbl_overdue_title");
        lbl_overdue_title->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxOverdue->addWidget(lbl_overdue_title);

        lbl_overdue_value = new QLabel(stat_overdue);
        lbl_overdue_value->setObjectName("lbl_overdue_value");
        lbl_overdue_value->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxOverdue->addWidget(lbl_overdue_value);


        calendarStatsLayout->addWidget(stat_overdue);


        pageCalendarLayout->addWidget(calendar_stats_container);

        stackedWidget->addWidget(page_calendar);
        page_innov = new QWidget();
        page_innov->setObjectName("page_innov");
        pageInnovLayout = new QVBoxLayout(page_innov);
        pageInnovLayout->setSpacing(0);
        pageInnovLayout->setObjectName("pageInnovLayout");
        pageInnovLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget->addWidget(page_innov);
        page_material = new QWidget();
        page_material->setObjectName("page_material");
        verticalLayout_material = new QVBoxLayout(page_material);
        verticalLayout_material->setObjectName("verticalLayout_material");
        label_material = new QLabel(page_material);
        label_material->setObjectName("label_material");
        label_material->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_material->addWidget(label_material);

        stackedWidget->addWidget(page_material);
        page_employees = new QWidget();
        page_employees->setObjectName("page_employees");
        verticalLayout_employees = new QVBoxLayout(page_employees);
        verticalLayout_employees->setObjectName("verticalLayout_employees");
        label_employees = new QLabel(page_employees);
        label_employees->setObjectName("label_employees");
        label_employees->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_employees->addWidget(label_employees);

        stackedWidget->addWidget(page_employees);
        page_suppliers = new QWidget();
        page_suppliers->setObjectName("page_suppliers");
        verticalLayout_suppliers = new QVBoxLayout(page_suppliers);
        verticalLayout_suppliers->setObjectName("verticalLayout_suppliers");
        label_suppliers = new QLabel(page_suppliers);
        label_suppliers->setObjectName("label_suppliers");
        label_suppliers->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_suppliers->addWidget(label_suppliers);

        stackedWidget->addWidget(page_suppliers);
        page_ai = new QWidget();
        page_ai->setObjectName("page_ai");
        verticalLayout_ai = new QVBoxLayout(page_ai);
        verticalLayout_ai->setObjectName("verticalLayout_ai");
        label_ai = new QLabel(page_ai);
        label_ai->setObjectName("label_ai");
        label_ai->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_ai->addWidget(label_ai);

        stackedWidget->addWidget(page_ai);

        contentArea->addWidget(stackedWidget);


        mainLayout->addLayout(contentArea);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);
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
        btn_nav_suppliers->setText(QCoreApplication::translate("MainWindow", "FOURNISSEUR", nullptr));
        btn_nav_ai->setText(QCoreApplication::translate("MainWindow", "ASSISTANT IA", nullptr));
        btn_tab_add->setText(QCoreApplication::translate("MainWindow", "Ajouter une commande", nullptr));
        btn_tab_update->setText(QCoreApplication::translate("MainWindow", "Modifier une commande", nullptr));
        btn_tab_delete->setText(QCoreApplication::translate("MainWindow", "Supprimer une commande", nullptr));
        btn_tab_list->setText(QCoreApplication::translate("MainWindow", "Liste", nullptr));
        btn_tab_stats->setText(QCoreApplication::translate("MainWindow", "Stats", nullptr));
        btn_tab_email_auto->setText(QCoreApplication::translate("MainWindow", "Historique", nullptr));
        btn_tab_calendar->setText(QCoreApplication::translate("MainWindow", "Calendrier", nullptr));
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
        label9->setText(QCoreApplication::translate("MainWindow", "CIN Employ\303\251 :", nullptr));
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
        label19->setText(QCoreApplication::translate("MainWindow", "CIN Employ\303\251 :", nullptr));
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
        cb_sort->setItemText(2, QCoreApplication::translate("MainWindow", "Trier par Prix (Croissant)", nullptr));
        cb_sort->setItemText(3, QCoreApplication::translate("MainWindow", "Trier par Prix (Decroissant)", nullptr));
        cb_sort->setItemText(4, QCoreApplication::translate("MainWindow", "Trier par Type", nullptr));
        cb_sort->setItemText(5, QCoreApplication::translate("MainWindow", "Trier par Statut", nullptr));

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
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "CIN Employ\303\251", nullptr));
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
        btn_stat_price_asc->setText(QCoreApplication::translate("MainWindow", "Prix Croissant", nullptr));
        btn_stat_price_desc->setText(QCoreApplication::translate("MainWindow", "Prix Decroissant", nullptr));
        btn_stat_type->setText(QCoreApplication::translate("MainWindow", "Par Type", nullptr));
        lbl_price_stats->setText(QCoreApplication::translate("MainWindow", "Prix: -", nullptr));
        lbl_type_stats->setText(QCoreApplication::translate("MainWindow", "Aucune donn\303\251e.", nullptr));
        lbl_city_stats->setText(QCoreApplication::translate("MainWindow", "Aucune donn\303\251e.", nullptr));
        tabWidget_stats->setTabText(tabWidget_stats->indexOf(tab_stats), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        label_stats_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 26px; font-weight: bold; color: #8B4513; padding: 20px;", nullptr));
        label_stats_title->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 TABLEAU DE BORD - STATISTIQUES", nullptr));
        stat_main_card_orders->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #C68E65; color: white; border-radius: 12px; padding: 20px;", nullptr));
        lbl_main_orders_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 14px; font-weight: bold;", nullptr));
        lbl_main_orders_title->setText(QCoreApplication::translate("MainWindow", "TOTAL COMMANDES", nullptr));
        lbl_main_orders_value->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 36px; font-weight: bold;", nullptr));
        lbl_main_orders_value->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        stat_main_card_revenue->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #3D362D; color: white; border-radius: 12px; padding: 20px;", nullptr));
        lbl_main_revenue_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 14px; font-weight: bold;", nullptr));
        lbl_main_revenue_title->setText(QCoreApplication::translate("MainWindow", "REVENU TOTAL", nullptr));
        lbl_main_revenue_value->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 36px; font-weight: bold;", nullptr));
        lbl_main_revenue_value->setText(QCoreApplication::translate("MainWindow", "0 DT", nullptr));
        stat_main_card_pending->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #8B4513; color: white; border-radius: 12px; padding: 20px;", nullptr));
        lbl_main_pending_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 14px; font-weight: bold;", nullptr));
        lbl_main_pending_title->setText(QCoreApplication::translate("MainWindow", "EN ATTENTE", nullptr));
        lbl_main_pending_value->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 36px; font-weight: bold;", nullptr));
        lbl_main_pending_value->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        stats_chart_frame->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border: 2px solid #F2D2B5; border-radius: 12px; padding: 20px;", nullptr));
        lbl_main_chart_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold; color: #3D362D;", nullptr));
        lbl_main_chart_title->setText(QCoreApplication::translate("MainWindow", "R\303\211PARTITION PAR TYPE DE CUIR", nullptr));
        lbl_main_type_stats->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #3D362D; font-size: 14px; padding: 10px;", nullptr));
        lbl_main_type_stats->setText(QCoreApplication::translate("MainWindow", "Aucune donn\303\251e disponible.", nullptr));
        stats_main_details_container->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border: 2px solid #F2D2B5; border-radius: 12px; padding: 20px;", nullptr));
        lbl_main_top_product_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 14px; font-weight: bold; color: #3D362D;", nullptr));
        lbl_main_top_product_title->setText(QCoreApplication::translate("MainWindow", "PRODUIT LE PLUS VENDU:", nullptr));
        lbl_main_top_product_value->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 14px; color: #C68E65; font-weight: bold;", nullptr));
        lbl_main_top_product_value->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        lbl_main_top_city_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 14px; font-weight: bold; color: #3D362D;", nullptr));
        lbl_main_top_city_title->setText(QCoreApplication::translate("MainWindow", "VILLE LA PLUS ACTIVE:", nullptr));
        lbl_main_top_city_value->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 14px; color: #C68E65; font-weight: bold;", nullptr));
        lbl_main_top_city_value->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        lbl_main_avg_price_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 14px; font-weight: bold; color: #3D362D;", nullptr));
        lbl_main_avg_price_title->setText(QCoreApplication::translate("MainWindow", "PRIX MOYEN PAR COMMANDE:", nullptr));
        lbl_main_avg_price_value->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 14px; color: #C68E65; font-weight: bold;", nullptr));
        lbl_main_avg_price_value->setText(QCoreApplication::translate("MainWindow", "0 DT", nullptr));
        label_email_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 28px; font-weight: bold; color: #8B4513; padding: 20px;", nullptr));
        label_email_title->setText(QCoreApplication::translate("MainWindow", "\357\277\275 HISTORIQUE DES EMAILS", nullptr));
        lbl_email_auto_info->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; color: #666; padding: 10px; background-color: #e8f5e9; border-radius: 8px; margin: 0px 40px;", nullptr));
        lbl_email_auto_info->setText(QCoreApplication::translate("MainWindow", "\342\204\271\357\270\217 Les emails sont envoy\303\251s automatiquement lors de la modification du statut des commandes.", nullptr));
        label_calendar_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 26px; font-weight: bold; color: #8B4513; padding: 20px;", nullptr));
        label_calendar_title->setText(QCoreApplication::translate("MainWindow", "\360\237\223\205 CALENDRIER DES LIVRAISONS", nullptr));
        calendar_container->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border: 2px solid #F2D2B5; border-radius: 12px; padding: 15px;", nullptr));
        lbl_legend_today->setText(QCoreApplication::translate("MainWindow", "\360\237\237\242 Aujourd'hui", nullptr));
        lbl_legend_delivery->setText(QCoreApplication::translate("MainWindow", "\360\237\224\265 Livraison pr\303\251vue", nullptr));
        lbl_legend_urgent->setText(QCoreApplication::translate("MainWindow", "\360\237\224\264 Urgent", nullptr));
        deliveries_list_container->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border: 2px solid #F2D2B5; border-radius: 12px; padding: 15px;", nullptr));
        lbl_deliveries_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 16px; font-weight: bold; color: #3D362D; padding-bottom: 10px;", nullptr));
        lbl_deliveries_title->setText(QCoreApplication::translate("MainWindow", "Livraisons du jour s\303\251lectionn\303\251", nullptr));
        list_deliveries->setStyleSheet(QCoreApplication::translate("MainWindow", "QListWidget { background-color: white; color: #3D362D; border: 1px solid #F2D2B5; }\n"
"QListWidget::item { color: #3D362D; padding: 10px; border-bottom: 1px solid #F2D2B5; }\n"
"QListWidget::item:selected { background-color: #C68E65; color: white; }", nullptr));
        lbl_selected_date->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #8B4513; font-weight: bold; padding-top: 10px;", nullptr));
        lbl_selected_date->setText(QCoreApplication::translate("MainWindow", "Date s\303\251lectionn\303\251e: -", nullptr));
        btn_export_calendar->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #8B4513; color: white; border-radius: 6px; padding: 10px 20px; font-weight: bold;", nullptr));
        btn_export_calendar->setText(QCoreApplication::translate("MainWindow", "Exporter le calendrier (PDF)", nullptr));
        calendar_stats_container->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: white; border: 2px solid #F2D2B5; border-radius: 12px; padding: 20px;", nullptr));
        stat_this_week->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #C68E65; color: white; border-radius: 8px; padding: 15px;", nullptr));
        lbl_this_week_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; font-weight: bold;", nullptr));
        lbl_this_week_title->setText(QCoreApplication::translate("MainWindow", "CETTE SEMAINE", nullptr));
        lbl_this_week_value->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 28px; font-weight: bold;", nullptr));
        lbl_this_week_value->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        stat_this_month->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #3D362D; color: white; border-radius: 8px; padding: 15px;", nullptr));
        lbl_this_month_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; font-weight: bold;", nullptr));
        lbl_this_month_title->setText(QCoreApplication::translate("MainWindow", "CE MOIS-CI", nullptr));
        lbl_this_month_value->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 28px; font-weight: bold;", nullptr));
        lbl_this_month_value->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        stat_overdue->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #8B4513; color: white; border-radius: 8px; padding: 15px;", nullptr));
        lbl_overdue_title->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 12px; font-weight: bold;", nullptr));
        lbl_overdue_title->setText(QCoreApplication::translate("MainWindow", "EN RETARD", nullptr));
        lbl_overdue_value->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 28px; font-weight: bold;", nullptr));
        lbl_overdue_value->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_material->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 24px; font-weight: bold; color: #8B4513;", nullptr));
        label_material->setText(QString());
        label_employees->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 24px; font-weight: bold; color: #8B4513;", nullptr));
        label_employees->setText(QCoreApplication::translate("MainWindow", "GESTION DES EMPLOY\303\211S", nullptr));
        label_suppliers->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 24px; font-weight: bold; color: #8B4513;", nullptr));
        label_suppliers->setText(QCoreApplication::translate("MainWindow", "GESTION DES FOURNISSEURS", nullptr));
        label_ai->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 24px; font-weight: bold; color: #8B4513;", nullptr));
        label_ai->setText(QCoreApplication::translate("MainWindow", "ASSISTANT IA", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
