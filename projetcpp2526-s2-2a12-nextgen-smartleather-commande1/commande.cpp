#include "commande.h"
#include "ui_commande.h"
#include "connection.h"
#include "materialswindow.h"
#include "logindialog.h"
#include <QMessageBox>
#include <QClipboard>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QDateTime>
#include <QCalendarWidget>
#include <QHeaderView>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QPdfWriter>
#include <QInputDialog>
#include <QTimer>
#include <QRegularExpression>
#include <QIntValidator>
#include <QVariant>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <utility>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QFrame>
#include <QFormLayout>
#include <QGridLayout>
#include <QStackedWidget>
#include <QDateEdit>
#include <QTextBrowser>
#include <QDir>
#include <QFileInfo>
#include <QSizePolicy>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <algorithm>
#include <QDesktopServices>
#include <QUrl>
#include <QUrlQuery>
#include <QTextStream>
#include <QCoreApplication>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QPieSeries>

namespace {
constexpr bool kSmtpEnabled = true; // toggle SMTP sending
constexpr const char* kDefaultEmployeeCin = "12345678"; // CIN temporaire en attendant un champ UI dédié

QString detectOrdersTableName(QSqlDatabase &db)
{
    static QString cached;
    if (!cached.isEmpty()) return cached;

    const QStringList candidates = {"COMMANDES", "COMMANDE", "ORDRES", "ORDERES", "ORDERS", "ORDER", "ORDRE"};
    const QStringList tables = db.tables(QSql::Tables);

    for (const QString &candidate : candidates) {
        for (const QString &tableName : tables) {
            if (tableName.compare(candidate, Qt::CaseInsensitive) == 0) {
                cached = tableName;
                return cached;
            }
        }
    }

    for (const QString &candidate : candidates) {
        QSqlQuery q(db);
        if (q.exec(QString("SELECT COUNT(*) FROM %1").arg(candidate))) {
            cached = candidate;
            return cached;
        }
    }
    return {};
}

// Cache des colonnes par table pour éviter les requêtes répétées
static QMap<QString, QStringList> s_colCache;

QStringList getTableColumns(QSqlDatabase &db, const QString &tableName)
{
    const QString key = tableName.toUpper();
    if (s_colCache.contains(key)) {
        return s_colCache.value(key);
    }

    QStringList cols;
    QSqlRecord record = db.record(tableName);
    if (record.count() > 0) {
        for (int i = 0; i < record.count(); ++i) {
            cols << record.fieldName(i).toUpper();
        }
    }

    if (cols.isEmpty()) {
        QSqlQuery q(db);
        const QString sql = QString("SELECT * FROM %1 WHERE 1=0").arg(tableName);
        if (q.exec(sql)) {
            record = q.record();
            for (int i = 0; i < record.count(); ++i) {
                cols << record.fieldName(i).toUpper();
            }
        } else {
            qDebug() << "getTableColumns: SELECT * FROM 1=0 failed for" << tableName << q.lastError().text();
        }
    }

    if (cols.isEmpty()) {
        QSqlQuery q(db);
        q.prepare("SELECT column_name FROM user_tab_columns WHERE table_name = :t");
        q.bindValue(":t", key);
        if (q.exec()) {
            while (q.next()) {
                cols << q.value(0).toString().toUpper();
            }
        } else {
            qDebug() << "getTableColumns: user_tab_columns query failed for" << tableName << q.lastError().text();
        }
    }

    s_colCache.insert(key, cols);
    return cols;
}

static QString findFirstExistingColumn(const QStringList &availableCols, const QStringList &candidates)
{
    for (const QString &candidate : candidates) {
        const QString can = candidate.toUpper();
        for (const QString &available : availableCols) {
            const QString avail = available.toUpper();
            if (avail == can || avail.contains(can)) {
                return avail;
            }
        }
    }
    return {};
}

static QString selectExpr(const QString &columnName, const QString &alias, const QString &fallback = "NULL")
{
    if (!columnName.isEmpty()) {
        return columnName + " AS " + alias;
    }
    return fallback + " AS " + alias;
}

struct OrderColumns {
    QString id;
    QString type;
    QString qty;
    QString email;
    QString status;
    QString dateCmd;
    QString dateLivr;
    QString price;
    QString cin;
    bool valid = true;
};

OrderColumns detectOrderColumns(QSqlDatabase &db, const QString &tableName)
{
    static QMap<QString, OrderColumns> cache;
    const QString key = tableName.toUpper();
    if (cache.contains(key)) return cache.value(key);

    OrderColumns oc;
    const QStringList cols = getTableColumns(db, key);

    if (!cols.isEmpty()) {
        oc.id      = findFirstExistingColumn(cols, {"ID_COMMANDE","ID","ID_CMD","COMMANDE_ID","ORDER_ID"});
        oc.type    = findFirstExistingColumn(cols, {"TYPE_PRODUIT","TYPE","TYPE_ARTICLE","ITEM_TYPE","PRODUCT_TYPE"});
        oc.qty     = findFirstExistingColumn(cols, {"QUANTITE","QTE","QUANTITY","QTY","AMOUNT"});
        oc.email   = findFirstExistingColumn(cols, {"EMAIL_CLIENT","EMAIL","MAIL_CLIENT","CLIENT_EMAIL","CUSTOMER_EMAIL"});
        oc.status  = findFirstExistingColumn(cols, {"STATUT","STATUS","ETAT","ORDER_STATUS","STATE"});
        oc.dateCmd = findFirstExistingColumn(cols, {"DATE_COMMANDE","DATE_CMD","ORDER_DATE","DATE_ORDER"});
        oc.dateLivr= findFirstExistingColumn(cols, {"DATE_LIVRAISON_PREVUE","DATE_LIVRAISON","DELIVERY_DATE","DATE_DELIVERY","DELIVERY_DATE_PREVUE"});
        oc.price   = findFirstExistingColumn(cols, {"PRIX_TOTAL","PRIX","MONTANT","TOTAL_PRICE","PRICE"});
        oc.cin     = findFirstExistingColumn(cols, {"CIN_EMPLOYE","CIN","ID_EMPLOYE","ID","EMPLOYEE_ID","EMP_ID"});

        if (oc.price.isEmpty()) {
            qDebug() << "detectOrderColumns: no price column found for" << key << "available:" << cols;
        }
    }

    oc.valid = !oc.id.isEmpty() && !oc.type.isEmpty() && !oc.qty.isEmpty();
    cache.insert(key, oc);
    return oc;
}

struct EmpColumns {
    QString id           = "CIN_EMPLOYE";
    QString nom          = "NOM";
    QString poste        = "POSTE";
    QString adresse      = "ADRESSE";
    QString telephone    = "TELEPHONE";
    QString dateEmbauche = "DATE_EMBAUCHE";
    QString salaire      = "SALAIRE";
    QString statut       = "STATUT";
    QString sexe         = "SEXE";
    bool valid = true;
};

EmpColumns detectEmpColumns(QSqlDatabase &db, const QString &tableName)
{
    static QMap<QString, EmpColumns> cache;
    const QString key = tableName.toUpper();
    if (cache.contains(key)) return cache.value(key);

    EmpColumns ec;
    const QStringList cols = getTableColumns(db, key);
    if (!cols.isEmpty()) {
        ec.id           = findFirstExistingColumn(cols, {"CIN_EMPLOYE","CIN","ID_EMPLOYE","ID","EMPLOYEE_ID","EMP_ID"});
        ec.nom          = findFirstExistingColumn(cols, {"NOM","NOM_EMPLOYE","NAME","EMPLOYEE_NAME","FULL_NAME","NOM_COMPLET"});
        ec.poste        = findFirstExistingColumn(cols, {"POSTE","ROLE","POSITION","JOB","JOB_TITLE","FUNCTION","TITLE"});
        ec.adresse      = findFirstExistingColumn(cols, {"ADRESSE","ADDRESS","ADDR","EMP_ADDRESS","EMP_ADRESSE","ADDRESS_LINE"});
        ec.telephone    = findFirstExistingColumn(cols, {"TELEPHONE","TEL","PHONE","PHONE_NUMBER","MOBILE","MOBILE_PHONE"});
        ec.dateEmbauche = findFirstExistingColumn(cols, {"DATE_EMBAUCHE","HIRE_DATE","DATE_HIRED","START_DATE","EMPLOYED_DATE"});
        ec.salaire      = findFirstExistingColumn(cols, {"SALAIRE","SALARY","WAGE","PAY","SALARY_AMOUNT"});
        ec.statut       = findFirstExistingColumn(cols, {"STATUT","STATUS","EMP_STATUS","STATE"});
        ec.sexe         = findFirstExistingColumn(cols, {"SEXE","GENDER","SEX"});
    }
    cache.insert(key, ec);
    return ec;
}

QString detectEmployeeTableName(QSqlDatabase &db)
{
    static QString cached;
    if (!cached.isEmpty()) return cached;

    // 1. Via db.tables() — pas de requête SQL directe
    const QStringList tables = db.tables(QSql::Tables);
    for (const QString &candidate : {"EMPLOYE","EMPLOYES","EMPLOYEE","EMPLOYEES"}) {
        for (const QString &t : tables) {
            if (t.compare(candidate, Qt::CaseInsensitive) == 0) {
                cached = t.toUpper();
                return cached;
            }
        }
    }

    // 2. Fallback : tester directement avec SELECT COUNT(*)
    for (const QString &candidate : {"EMPLOYE","EMPLOYES","EMPLOYEE","EMPLOYEES"}) {
        QSqlQuery q(db);
        if (q.exec(QString("SELECT COUNT(*) FROM %1").arg(candidate))) {
            cached = candidate;
            return cached;
        }
    }

    return {};
}

bool deleteOrderDependencies(QSqlDatabase &db, const QString &parentTableName, const QString &orderId, QString *errorOut = nullptr)
{
    QSqlQuery fkQuery(db);
    fkQuery.prepare(
        "SELECT fkc.table_name AS child_table, fkc.column_name AS child_column "
        "FROM user_constraints fk "
        "JOIN user_constraints pk ON fk.r_constraint_name = pk.constraint_name "
        "JOIN user_cons_columns pkc ON pk.constraint_name = pkc.constraint_name "
        "JOIN user_cons_columns fkc ON fk.constraint_name = fkc.constraint_name AND fkc.position = pkc.position "
        "WHERE fk.constraint_type = 'R' "
        "AND pk.table_name = :parentTable "
        "AND pkc.column_name = 'ID_COMMANDE'"
        );
    fkQuery.bindValue(":parentTable", parentTableName.toUpper());

    if (!fkQuery.exec()) {
        if (errorOut) {
            *errorOut = "Impossible de détecter les dépendances: " + fkQuery.lastError().text();
        }
        return false;
    }

    while (fkQuery.next()) {
        const QString childTable = fkQuery.value("child_table").toString();
        const QString childColumn = fkQuery.value("child_column").toString();

        if (childTable.isEmpty() || childColumn.isEmpty()) {
            continue;
        }

        QSqlQuery childDelete(db);
        childDelete.prepare(QString("DELETE FROM %1 WHERE %2 = :id").arg(childTable, childColumn));
        childDelete.bindValue(":id", orderId);

        if (!childDelete.exec()) {
            if (errorOut) {
                *errorOut = QString("Suppression des enregistrements liés échouée (%1.%2): %3")
                                .arg(childTable, childColumn, childDelete.lastError().text());
            }
            return false;
        }

        if (childDelete.numRowsAffected() > 0) {
            qDebug() << "Suppression dépendances:" << childDelete.numRowsAffected()
                     << "ligne(s) dans" << childTable << "pour ID_COMMANDE =" << orderId;
        }
    }

    return true;
}

QString normalizeDateText(const QVariant &v)
{
    if (!v.isValid() || v.isNull()) {
        return {};
    }

    // Oracle via ODBC can return DATE/TIMESTAMP as QDate/QDateTime or as QString.
    if (v.canConvert<QDateTime>()) {
        QDateTime dt = v.toDateTime();
        if (dt.isValid()) {
            return dt.date().toString("dd/MM/yyyy");
        }
    }
    if (v.canConvert<QDate>()) {
        QDate d = v.toDate();
        if (d.isValid()) {
            return d.toString("dd/MM/yyyy");
        }
    }

    return v.toString();
}

QDate parseDateLoose(const QString &s)
{
    const QString input = s.trimmed();
    if (input.isEmpty()) {
        return {};
    }

    // 1) Formats date simples
    QDate d = QDate::fromString(input, "dd/MM/yyyy");
    if (!d.isValid()) d = QDate::fromString(input, "d/M/yyyy");
    if (!d.isValid()) d = QDate::fromString(input, "yyyy-MM-dd");
    if (d.isValid()) {
        return d;
    }

    // 2) Formats date+heure courants (Oracle/ODBC)
    const QStringList dateTimeFormats = {
        "dd/MM/yyyy HH:mm:ss",
        "d/M/yyyy HH:mm:ss",
        "yyyy-MM-dd HH:mm:ss",
        "yyyy-MM-ddTHH:mm:ss",
        "yyyy-MM-ddTHH:mm:ss.zzz",
        "dd-MMM-yy HH:mm:ss",
        "dd-MMM-yyyy HH:mm:ss"
    };
    for (const QString &fmt : dateTimeFormats) {
        const QDateTime dt = QDateTime::fromString(input, fmt);
        if (dt.isValid()) {
            return dt.date();
        }
    }

    // 3) Fallback: si une heure est présente, garder la partie avant l'espace
    const QString datePart = input.section(' ', 0, 0).trimmed();
    if (datePart != input) {
        d = QDate::fromString(datePart, "dd/MM/yyyy");
        if (!d.isValid()) d = QDate::fromString(datePart, "d/M/yyyy");
        if (!d.isValid()) d = QDate::fromString(datePart, "yyyy-MM-dd");
        if (d.isValid()) {
            return d;
        }
    }

    return {};
}

QString loadKey(const QString &keyName)
{
    // 1) Env variable
    const QByteArray envVal = qgetenv(keyName.toLocal8Bit().constData());
    if (!envVal.isEmpty()) {
        return QString::fromLocal8Bit(envVal).trimmed();
    }

    // 2) Fichier local .env.local (non versionné) dans cwd ou dossier de l'exe
    const QStringList candidates = {
        QDir::current().filePath(".env.local"),
        QCoreApplication::applicationDirPath() + "/.env.local",
        QDir(QCoreApplication::applicationDirPath() + "/..").absoluteFilePath(".env.local"),
        "C:/Users/ASUS/Documents/rayenproject1/.env.local"
    };

    for (const QString &path : candidates) {
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) continue;
        QTextStream ts(&f);
        while (!ts.atEnd()) {
            const QString line = ts.readLine().trimmed();
            if (line.startsWith('#') || line.isEmpty()) continue;
            const QString key = keyName + "=";
            if (line.startsWith(key)) {
                const QString value = line.mid(key.length()).trimmed();
                if (!value.isEmpty()) return value;
            }
        }
    }

    return {};
}

}

MainWindow::MainWindow(const QString &userEmail, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_userEmail(userEmail)
{
    qDebug() << "MainWindow constructor start";
    ui->setupUi(this);
    qDebug() << "UI setup done";

    // Style commun pour les barres de boutons (toutes les tâches)
    buttonBarStyle = "QPushButton{padding:10px 16px;border:1px solid #C68E65;border-radius:8px;background:#f7ede4;color:#3b2a20;font-weight:600;}"
                     "QPushButton:checked{background:#C68E65;color:white;}"
                     "QPushButton:hover{background:#e7d6c8;}";

    if (!Connection::instance()->createConnect()) {
        QMessageBox::warning(this, "Base de données", "Connexion à la base impossible (vérifiez la source ODBC).");
    }

    // Intégrer l'UI matières premières dans la page dédiée du stackedWidget
    materialsWindow = new MaterialsWindow(this);
    materialsWindow->setEmbeddedMode(true);
    materialsWindow->setParent(ui->page_material);
    materialsWindow->setWindowFlag(Qt::Widget);
    materialsWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    if (ui->verticalLayout_material) {
        ui->verticalLayout_material->addWidget(materialsWindow);
    }
    qDebug() << "MaterialsWindow created";

    // Supprimer le titre "GESTION DES MATIÈRES PREMIÈRES" pour gagner de l'espace
    if (ui->label_material) {
        ui->label_material->hide();
        ui->label_material->setMaximumHeight(0);
    }
    if (ui->verticalLayout_material) {
        ui->verticalLayout_material->setContentsMargins(0, 0, 0, 0);
        ui->verticalLayout_material->setSpacing(0);
    }

    if (ui->dsb_price_update) {
        ui->dsb_price_update->setButtonSymbols(QAbstractSpinBox::NoButtons);
        ui->dsb_price_update->setFixedHeight(44);
        ui->dsb_price_update->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }

    if (ui->le_email_update) {
        ui->le_email_update->setFixedHeight(44);
        ui->le_email_update->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }

    // Masquer les champs UI non presents dans le schema (ville, cinEmploye)
    if (ui->cb_client_city) ui->cb_client_city->setVisible(false);
    if (ui->label_order_city) ui->label_order_city->setVisible(false);
    if (ui->le_proof) ui->le_proof->setVisible(false);
    if (ui->label_order_employee_cin) ui->label_order_employee_cin->setVisible(false);

    if (ui->cb_city_update) ui->cb_city_update->setVisible(false);
    if (ui->label_update_city) ui->label_update_city->setVisible(false);
    if (ui->le_proof_update) ui->le_proof_update->setVisible(false);
    if (ui->label_update_employee_cin) ui->label_update_employee_cin->setVisible(false);

    if (ui->lbl_main_top_city_title) ui->lbl_main_top_city_title->setVisible(false);
    if (ui->lbl_main_top_city_value) ui->lbl_main_top_city_value->setVisible(false);
    if (ui->lbl_city_stats) ui->lbl_city_stats->setVisible(false);

    // ===== CONFIGURATION DES VALIDATEURS EN TEMPS RÉEL =====
    auto setFieldStyle = [](QWidget *field, bool isValid) {
        if (!field) return;
        if (isValid) {
            field->setStyleSheet("background-color: white; border: 1px solid #ccc;");
        } else {
            field->setStyleSheet("background-color: #ffe6e6; border: 2px solid red;");
        }
    };

    // Champs de recherche / tri : bordure et texte visibles
    auto styleSearchField = [](QWidget *w) {
        if (!w) return;
        w->setStyleSheet(
            "QLineEdit{background:#fff;border:1px solid #C68E65;border-radius:6px;color:#3b2a20;padding:6px;font-weight:600;}"
            "QLineEdit:placeholder{color:#8c715f;}"
            "QComboBox{background:#fff;border:1px solid #C68E65;border-radius:6px;color:#3b2a20;padding:6px;font-weight:600;}"
            "QComboBox QAbstractItemView{background:#fff;border:1px solid #C68E65;color:#3b2a20;}"
            );
    };
    styleSearchField(ui->le_search);
    styleSearchField(ui->cb_sort);
    styleSearchField(ui->le_id_update_search);

    // Validation pour ID : accepter UNIQUEMENT des nombres positifs
    if (ui->le_id) {
        // Validateur Qt : force les nombres entiers positifs (1 à 2147483647)
        QIntValidator *idValidator = new QIntValidator(1, 2147483647, ui->le_id);
        ui->le_id->setValidator(idValidator);

        // Coloration visuelle pendant la saisie
        connect(ui->le_id, &QLineEdit::textChanged, this, [=](const QString &text) {
            bool valid = !text.isEmpty();
            if (!text.isEmpty()) {
                bool ok = false;
                int num = text.toInt(&ok);
                valid = ok && num > 0;
            }
            setFieldStyle(ui->le_id, valid || text.isEmpty());
        });
    }

    if (ui->le_client_email) {
        connect(ui->le_client_email, &QLineEdit::textChanged, this, [=](const QString &text) {
            QString email = text.trimmed();
            bool valid = email.isEmpty(); // Vide = pas encore saisi, donc neutre
            if (!email.isEmpty()) {
                QRegularExpression emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
                valid = emailRegex.match(email).hasMatch();
            }
            setFieldStyle(ui->le_client_email, valid);
        });
    }

    if (ui->dsb_price) {
        connect(ui->dsb_price, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double value) {
            bool valid = value > 0 && value <= 1000000;
            setFieldStyle(ui->dsb_price, valid);
        });
    }

    if (ui->sb_order_qty) {
        connect(ui->sb_order_qty, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
            bool valid = value > 0 && value <= 10000;
            setFieldStyle(ui->sb_order_qty, valid);
        });
    }

    if (ui->le_proof) {
        connect(ui->le_proof, &QLineEdit::textChanged, this, [=](const QString &text) {
            bool valid = !text.trimmed().isEmpty();
            setFieldStyle(ui->le_proof, valid);
        });
    }

    // Validation en temps réel pour formulaire de MODIFICATION
    if (ui->le_email_update) {
        connect(ui->le_email_update, &QLineEdit::textChanged, this, [=](const QString &text) {
            QString email = text.trimmed();
            bool valid = email.isEmpty();
            if (!email.isEmpty()) {
                QRegularExpression emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
                valid = emailRegex.match(email).hasMatch();
            }
            setFieldStyle(ui->le_email_update, valid);
        });
    }

    if (ui->dsb_price_update) {
        connect(ui->dsb_price_update, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double value) {
            bool valid = value > 0 && value <= 1000000;
            setFieldStyle(ui->dsb_price_update, valid);
        });
    }

    if (ui->sb_qty_update) {
        connect(ui->sb_qty_update, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
            bool valid = value > 0 && value <= 10000;
            setFieldStyle(ui->sb_qty_update, valid);
        });
    }

    if (ui->le_proof_update) {
        connect(ui->le_proof_update, &QLineEdit::textChanged, this, [=](const QString &text) {
            bool valid = !text.trimmed().isEmpty();
            setFieldStyle(ui->le_proof_update, valid);
        });
    }

    /* Désactivé - Envoi manuel supprimé
    // Validation Email simple
    if (ui->le_email_simple) {
        connect(ui->le_email_simple, &QLineEdit::textChanged, this, [=](const QString &text) {
            QString email = text.trimmed();
            bool valid = email.isEmpty();
            if (!email.isEmpty()) {
                QRegularExpression emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
                valid = emailRegex.match(email).hasMatch();
            }
            setFieldStyle(ui->le_email_simple, valid);
        });
    }
    */

    // Validation pour ID de recherche/modification : numérique uniquement
    if (ui->le_id_update_search) {
        QIntValidator *updateSearchValidator = new QIntValidator(1, 2147483647, ui->le_id_update_search);
        ui->le_id_update_search->setValidator(updateSearchValidator);
    }

    // Validation pour ID à supprimer : numérique uniquement
    if (ui->le_id_to_delete) {
        QIntValidator *deleteValidator = new QIntValidator(1, 2147483647, ui->le_id_to_delete);
        ui->le_id_to_delete->setValidator(deleteValidator);
    }

    // ===== FIN VALIDATEURS =====

    // Tables: fill available width and keep a readable size
    auto setupTable = [&](QTableWidget *table, int minHeight = -1) {
        if (!table) return;
        if (minHeight > 0) table->setMinimumHeight(minHeight);
        if (table->horizontalHeader()) {
            table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        }
        table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
    };

    setupTable(ui->table_list);
    setupTable(ui->table_delete);

    auto stylePrimaryBtn = [](QPushButton *btn){
        if (!btn) return;
        btn->setMinimumHeight(44);
        btn->setStyleSheet(
            "QPushButton{background:#8B4513;color:white;padding:10px;border-radius:8px;font-weight:600;}"
            "QPushButton:hover{background:#a05a22;}"
            "QPushButton:pressed{background:#723a0f;}"
            );
    };

    for (QPushButton *btn : {
             ui->btn_valider,
             ui->btn_update_confirm,
             ui->btn_delete_action,
             ui->btn_search_update,
             ui->btn_pdf,
             ui->btn_stat_price_asc,
             ui->btn_stat_price_desc,
             ui->btn_stat_type
         }) {
        stylePrimaryBtn(btn);
    }

    // Supprimer l'onglet "Statistiques" de la page Liste
    if (ui->tabWidget_stats) {
        for (int i = ui->tabWidget_stats->count() - 1; i >= 0; --i) {
            const QString tabText = ui->tabWidget_stats->tabText(i).toLower();
            if (tabText.contains("stat")) {
                ui->tabWidget_stats->removeTab(i);
            }
        }
    }

    // Stats page: keep cards aligned and avoid horizontal clipping
    if (ui->statsMainGrid) {
        ui->statsMainGrid->setColumnStretch(0, 1);
        ui->statsMainGrid->setColumnStretch(1, 1);
        ui->statsMainGrid->setColumnStretch(2, 1);
    }
    if (ui->scrollArea_stats) {
        ui->scrollArea_stats->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->scrollArea_stats->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }

    // Stats chart: pie chart by order type
    if (ui->chartMainLayout) {
        auto chartView = new QChartView(ui->stats_chart_frame);
        chartView->setObjectName("typePieChartView");
        chartView->setMinimumHeight(240);
        chartView->setRenderHint(QPainter::Antialiasing, true);
        chartView->setStyleSheet("background: transparent;");
        ui->chartMainLayout->insertWidget(1, chartView);
    }
    if (ui->lbl_main_type_stats) {
        ui->lbl_main_type_stats->setVisible(false);
    }

    // Chargement du logo "logoo" (essaie .png puis .jpg)
    QPixmap logo("C:/Users/ASUS/Documents/rayenproject1/logoo.png");
    if (logo.isNull()) {
        logo.load("C:/Users/ASUS/Documents/rayenproject1/logoo.jpg");
    }
    if (!logo.isNull()) {
        ui->logo_label->setPixmap(logo.scaled(200, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // Harmoniser le fond et les cadres des pages Commandes
    const QString ordersBg = "background-color: #fdf7f2;";
    auto setPageBg = [&](QWidget *w){ if (w) w->setStyleSheet(ordersBg); };
    setPageBg(ui->page_add);
    setPageBg(ui->page_update);
    setPageBg(ui->page_delete);
    setPageBg(ui->page_list);
    setPageBg(ui->page_stats);
    setPageBg(ui->page_email_auto);
    setPageBg(ui->page_calendar);

    const QString ordersPanel = "background:#ffffff;border:1px solid #C68E65;border-radius:10px;";
    const QString scrollBarStyle =
        "QScrollBar:vertical{background:#fdf7f2;width:14px;margin:4px 0;border:1px solid #C68E65;border-radius:7px;}"
        "QScrollBar::handle:vertical{background:#8B4513;min-height:24px;border-radius:6px;}"
        "QScrollBar::handle:vertical:hover{background:#a05a22;}"
        "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0px;}"
        "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:transparent;}";

    auto applyScrollStyle = [&](QWidget *w){ if (w) w->setStyleSheet(w->styleSheet() + scrollBarStyle); };
    auto applyOrdersPanel = [&](QWidget *page){
        if (!page) return;
        page->setStyleSheet(ordersBg + ordersPanel + scrollBarStyle);
        if (auto *lay = page->layout()) {
            lay->setContentsMargins(20, 20, 20, 20);
            lay->setSpacing(16);
        }
    };
    applyOrdersPanel(ui->page_add);
    applyOrdersPanel(ui->page_update);
    applyOrdersPanel(ui->page_delete);
    applyOrdersPanel(ui->page_list);
    applyOrdersPanel(ui->page_stats);

    // Scrollbars brun sur les widgets listés des pages commandes
    applyScrollStyle(ui->table_list);
    applyScrollStyle(ui->table_delete);
    applyScrollStyle(ui->scrollArea_stats);
    applyScrollStyle(ui->tabWidget_stats);

    // Sidebar: teinte cuir plus chaleureuse
    if (ui->sidebar) {
        ui->sidebar->setStyleSheet(
            "QFrame#sidebar{"
            "background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #5e4334, stop:1 #3b2b21);"
            "border-right: 2px solid #C68E65;"
            "}"
            );
    }

    aiNetwork = new QNetworkAccessManager(this);
    geminiApiKey = loadKey("GEMINI_API_KEY");

    // Connexion des boutons du menu supérieur pour changer de page
    applyBarStyle({ui->btn_tab_add, ui->btn_tab_update, ui->btn_tab_delete, ui->btn_tab_list, ui->btn_tab_stats, ui->btn_tab_email_auto, ui->btn_tab_calendar}, false);
    connect(ui->btn_tab_add, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_add);
    });
    connect(ui->btn_tab_update, &QPushButton::clicked, this, [=]() {
        reloadOrdersFromDb();
        displayOrdersForUpdate();
        ui->stackedWidget->setCurrentWidget(ui->page_update);
    });
    connect(ui->btn_tab_delete, &QPushButton::clicked, this, [=]() {
        reloadOrdersFromDb();
        ui->stackedWidget->setCurrentWidget(ui->page_delete);
    });
    connect(ui->btn_tab_list, &QPushButton::clicked, this, [=]() {
        reloadOrdersFromDb();
        ui->stackedWidget->setCurrentWidget(ui->page_list);
    });
    connect(ui->btn_tab_stats, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_stats);
        updateStatistics(); // Mettre à jour les stats lors de l'affichage
    });
    connect(ui->btn_tab_email_auto, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_email_auto);
        displayEmailHistory(); // Afficher automatiquement l'historique
    });
    connect(ui->btn_tab_calendar, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_calendar);
    });

    // --- NAVIGATION DU MENU LATÉRAL (MÉTIERS) ---

    // 1. COMMANDES : Affiche la page des commandes et le menu du haut
    connect(ui->btn_nav_orders, &QPushButton::clicked, this, [=]() {
        reloadOrdersFromDb();
        ui->stackedWidget->setCurrentWidget(ui->page_list);
        ui->top_nav->setVisible(true); // On affiche le menu CRUD (Ajouter, Modifier...)
    });

    // 2. MATÉRIEL
    connect(ui->btn_nav_material, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_material);
        ui->top_nav->setVisible(false); // On masque le menu CRUD des commandes
    });

    // 3. EMPLOYÉS
    connect(ui->btn_nav_employees, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_employees);
        ui->top_nav->setVisible(false);
        // Afficher directement l'onglet "Liste / Stats" et charger les données
        if (employeeTabButtons.size() > 3 && employeeTabButtons[3])
            employeeTabButtons[3]->click();
    });

    // 4. FOURNISSEURS
    connect(ui->btn_nav_suppliers, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_suppliers);
        ui->top_nav->setVisible(false);
    });

    // 5. ASSISTANT IA
    connect(ui->btn_nav_ai, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_ai);
        ui->top_nav->setVisible(false);
    });

    auto handleAiSend = [=]() {
        if (!ui->le_ai_input) return;
        const QString userText = ui->le_ai_input->text().trimmed();
        if (userText.isEmpty()) return;
        ui->le_ai_input->clear();
        sendAiMessage(userText);
    };

    if (ui->tb_ai_log) {
        ui->tb_ai_log->setOpenExternalLinks(true);
        const bool envSet = qEnvironmentVariableIsSet("GEMINI_API_KEY");
        const int envLen = envSet ? qgetenv("GEMINI_API_KEY").size() : 0;
        QString intro;
        if (geminiApiKey.isEmpty()) {
            intro = envSet
                        ? QString("Clé Gemini non lue (env présente, taille %1). Relancez depuis ce terminal ou placez .env.local.").arg(envLen)
                        : "Mode connecté prêt. Configurez GEMINI_API_KEY ou .env.local pour activer les réponses IA.";
        } else {
            intro = "Connexion Gemini prête. Posez vos questions sur les matières et les stocks.";
        }
        appendAiMessage("Assistant", intro);
    }
    if (ui->btn_ai_send) {
        connect(ui->btn_ai_send, &QPushButton::clicked, this, handleAiSend);
    }
    if (ui->le_ai_input) {
        connect(ui->le_ai_input, &QLineEdit::returnPressed, this, handleAiSend);
    }

    // Style unifié pour le menu latéral
    applyBarStyle({ui->btn_nav_orders, ui->btn_nav_material, ui->btn_nav_employees, ui->btn_nav_suppliers, ui->btn_nav_ai}, false);

    // Intégration des interfaces employé/fournisseur depuis l'ancien projet
    setupSupplierUI();
    setupEmployeeUI();
    refreshSupplierTable();

    // ── Restrictions d'accès selon le rôle ──────────────────────────────
    // admin@ → accès complet
    // manager@ → Commandes uniquement
    // employe@ → Employés uniquement
    // contact@ → Fournisseurs uniquement
    bool isAdmin    = m_userEmail.startsWith("admin@");
    bool isManager  = m_userEmail.startsWith("manager@");
    bool isEmploye  = m_userEmail.startsWith("employe@");
    bool isContact  = m_userEmail.startsWith("contact@");

    auto hideNav = [&](QPushButton *btn) {
        if (btn) { btn->setVisible(false); btn->setEnabled(false); }
    };

    if (!isAdmin) {
        // Masquer les sections non autorisées
        if (!isManager) hideNav(ui->btn_nav_orders);
        if (!isEmploye) hideNav(ui->btn_nav_employees);
        if (!isContact) hideNav(ui->btn_nav_suppliers);
        // Matières et IA : admin seulement
        hideNav(ui->btn_nav_material);
        hideNav(ui->btn_nav_ai);
        // Masquer le menu top (commandes) si pas manager
        if (!isManager) ui->top_nav->setVisible(false);
    }

    // Naviguer directement vers la page autorisée
    if (isManager) {
        reloadOrdersFromDb();
        ui->stackedWidget->setCurrentWidget(ui->page_list);
        ui->top_nav->setVisible(true);
    } else if (isEmploye) {
        ui->stackedWidget->setCurrentWidget(ui->page_employees);
        if (employeeTabButtons.size() > 3 && employeeTabButtons[3])
            employeeTabButtons[3]->click();
    } else if (isContact) {
        ui->stackedWidget->setCurrentWidget(ui->page_suppliers);
    }
    // ────────────────────────────────────────────────────────────────────

    // --- FONCTIONNALITÉ ENREGISTRER (Ajout dans le tableau) ---
    connect(ui->btn_valider, &QPushButton::clicked, this, [=]() {
        // 1. Récupération des valeurs du formulaire
        QString id = ui->le_id->text().trimmed();
        QString type = ui->cb_article_type->currentText();
        int qty = ui->sb_order_qty->value();
        QString email = ui->le_client_email->text().trimmed();
        QString status = ui->cb_status->currentText();
        QDate dateOrder = ui->de_date->date();
        QDate dateDel = ui->de_date_delivery->date();
        double price = ui->dsb_price->value();

        // 2. Validation complète des champs
        // Validation ID
        if (id.isEmpty()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Veuillez entrer un numéro de commande.");
            ui->le_id->setFocus();
            return;
        }
        bool isValidNumber = false;
        int idNumber = id.toInt(&isValidNumber);
        if (!isValidNumber || idNumber <= 0) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ L'ID doit être un nombre entier positif.");
            ui->le_id->setFocus();
            return;
        }

        // Validation Type
        if (type.isEmpty() || type == "Sélectionner un type") {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Veuillez sélectionner un type d'article.");
            ui->cb_article_type->setFocus();
            return;
        }

        // Validation Quantité
        if (qty <= 0) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ La quantité doit être supérieure à 0.");
            ui->sb_order_qty->setFocus();
            return;
        }
        if (qty > 10000) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ La quantité semble anormalement grande (>10000).");
            ui->sb_order_qty->setFocus();
            return;
        }

        // Validation Email
        if (email.isEmpty()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Veuillez entrer l'email du client.");
            ui->le_client_email->setFocus();
            return;
        }
        QRegularExpression emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
        if (!emailRegex.match(email).hasMatch()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Format email invalide (ex: client@example.com).");
            ui->le_client_email->setFocus();
            return;
        }


        // Validation Dates
        if (!dateOrder.isValid()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ La date de commande est invalide.");
            ui->de_date->setFocus();
            return;
        }
        if (!dateDel.isValid()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ La date de livraison est invalide.");
            ui->de_date_delivery->setFocus();
            return;
        }
        if (dateDel < dateOrder) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ La date de livraison doit être >= la date de commande.");
            ui->de_date_delivery->setFocus();
            return;
        }

        // Validation Prix
        if (price <= 0) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Le prix doit être supérieur à 0.");
            ui->dsb_price->setFocus();
            return;
        }
        if (price > 1000000) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Le prix semble anormalement élevé (>1M).");
            ui->dsb_price->setFocus();
            return;
        }

        // Validation Statut
        if (status.isEmpty() || status == "Sélectionner un statut") {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Veuillez sélectionner un statut.");
            ui->cb_status->setFocus();
            return;
        }


        // 3. Insertion dans la base de données
        QSqlDatabase db = Connection::instance()->getDatabase();
        if (!db.isOpen()) {
            QMessageBox::critical(this, "Erreur Base de données", "Connexion BD fermée. Relancez l'application.");
            return;
        }

        // Info debug: utilisateur Oracle connecté
        {
            QSqlQuery who(db);
            if (who.exec("SELECT USER FROM DUAL") && who.next()) {
                qDebug() << "Oracle USER =" << who.value(0).toString();
            }
        }

        // Détecter le vrai nom de table (certains scripts utilisent COMMANDES)
        QString tableName = detectOrdersTableName(db);
        if (tableName.isEmpty()) {
            QMessageBox::critical(this, "Erreur Base de données",
                                  "Aucune table COMMANDE/COMMANDES trouvée dans ce schéma Oracle.");
            return;
        }
        qDebug() << "Insertion dans la table" << tableName;

        const OrderColumns cols = detectOrderColumns(db, tableName);
        if (!cols.valid) {
            const QStringList realCols = getTableColumns(db, tableName);
            QMessageBox::critical(this, "Erreur Base de données",
                                  QString("Colonnes non reconnues dans '%1'.\nColonnes disponibles: %2\n\n"
                                          "Ajoutez vos noms de colonnes dans detectOrderColumns().")
                                      .arg(tableName, realCols.join(", ")));
            return;
        }

        // Construire INSERT dynamiquement
        QStringList insCols = {cols.id};
        QStringList insVals = {":id"};
        if (!cols.type.isEmpty())    { insCols << cols.type;    insVals << ":type"; }
        if (!cols.qty.isEmpty())     { insCols << cols.qty;     insVals << ":qty"; }
        if (!cols.email.isEmpty())   { insCols << cols.email;   insVals << ":email"; }
        if (!cols.status.isEmpty())  { insCols << cols.status;  insVals << ":status"; }
        if (!cols.dateCmd.isEmpty()) { insCols << cols.dateCmd; insVals << ":date"; }
        if (!cols.dateLivr.isEmpty()){ insCols << cols.dateLivr;insVals << ":dateDel"; }
        if (!cols.price.isEmpty())   { insCols << cols.price;   insVals << ":price"; }
        if (!cols.cin.isEmpty())     { insCols << cols.cin;     insVals << ":cin"; }

        QSqlQuery query(db);
        query.prepare(QString("INSERT INTO %1 (%2) VALUES (%3)")
                          .arg(tableName, insCols.join(", "), insVals.join(", ")));
        query.bindValue(":id", id);
        if (!cols.type.isEmpty())    query.bindValue(":type",    type);
        if (!cols.qty.isEmpty())     query.bindValue(":qty",     qty);
        if (!cols.email.isEmpty())   query.bindValue(":email",   email);
        if (!cols.status.isEmpty())  query.bindValue(":status",  status);
        if (!cols.dateCmd.isEmpty()) query.bindValue(":date",    dateOrder);
        if (!cols.dateLivr.isEmpty())query.bindValue(":dateDel", dateDel);
        if (!cols.price.isEmpty())   query.bindValue(":price",   price);
        if (!cols.cin.isEmpty())     query.bindValue(":cin",     QString::fromLatin1(kDefaultEmployeeCin));

        if (!query.exec()) {
            QMessageBox::critical(this, "Erreur Base de données",
                                  "Erreur lors de l'insertion dans la base de données:\n" + query.lastError().text());
            qDebug() << "Erreur SQL:" << query.lastError().text();
            return;
        }

        if (query.numRowsAffected() != 1) {
            qDebug() << "INSERT exécuté mais lignes affectées =" << query.numRowsAffected();
        }

        // Valider la transaction avec COMMIT
        QSqlQuery commitQuery(db);
        if (!commitQuery.exec("COMMIT")) {
            qDebug() << "Erreur COMMIT:" << commitQuery.lastError().text();
            QMessageBox::warning(this, "Erreur", "La commande a pu ne pas être enregistrée correctement.");
            return;
        }

        // Vérifier que la ligne existe bien dans la même session
        {
            QSqlQuery verify(db);
            verify.prepare(QString("SELECT COUNT(*) FROM %1 WHERE ID_COMMANDE = :id").arg(tableName));
            verify.bindValue(":id", id);
            if (verify.exec() && verify.next()) {
                int count = verify.value(0).toInt();
                qDebug() << "Vérification insertion (count) =" << count;
                if (count == 0) {
                    QMessageBox::warning(this, "Attention",
                                         "INSERT OK mais la ligne n'apparaît pas dans la table vérifiée.\n"
                                         "Ca arrive si vous n'êtes pas sur le même schéma/utilisateur dans SQL Developer.");
                }
            }
        }

        qDebug() << "✓ Commande enregistrée avec succès dans la base de données";

        // Rafraîchir depuis la BD pour que la Liste/Supprimer/Modifier affichent tout
        reloadOrdersFromDb();

        // Envoi automatique d'un email de confirmation de réception de commande
        {
            QMap<QString, QString> orderData;
            orderData["id"] = id;
            orderData["type"] = type;
            orderData["qty"] = QString::number(qty);
            orderData["email"] = email;
            orderData["orderDate"] = dateOrder.toString("dd/MM/yyyy");
            orderData["deliveryDate"] = dateDel.toString("dd/MM/yyyy");
            orderData["price"] = QString::number(price, 'f', 2);
            orderData["status"] = status;

            QString subject = QString("Confirmation de réception de votre commande #%1").arg(id);
            QString body = getEmailTemplate("confirmation", orderData);

            bool sent = sendEmail(email, subject, body);
            if (sent) {
                addToEmailHistory(email, subject, body, "Confirmation de commande", id);
                qDebug() << "Email automatique de confirmation envoyé pour la commande" << id;
            } else {
                qDebug() << "Échec de l'envoi de l'email de confirmation pour la commande" << id;
            }
        }

        // Notification simple si la commande est "Prête"
        if (status == "Prête" || status == "Prete") {
            QMessageBox::information(this, "Commande prête",
                                     QString("La commande %1 est prête!\nVous pouvez envoyer un email au client %2").arg(id, email));
        }

        QMessageBox::information(this, "Succès", "La commande a été enregistrée !");

        // Réinitialiser le formulaire
        ui->le_id->clear();
        ui->cb_article_type->setCurrentIndex(0);
        ui->sb_order_qty->setValue(1);
        ui->le_client_email->clear();
        ui->cb_client_city->setCurrentIndex(0);
        ui->de_date->setDate(QDate::currentDate());
        ui->de_date_delivery->setDate(QDate::currentDate());
        ui->dsb_price->setValue(0);
        ui->cb_status->setCurrentIndex(0);
        ui->le_proof->clear();
    });

    // --- RECHERCHE D'UNE COMMANDE (PAGE MODIFIER) ---
    connect(ui->btn_search_update, &QPushButton::clicked, this, [=]() {
        QString searchId = ui->le_id_update_search->text().trimmed();
        if (searchId.isEmpty()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Veuillez entrer l'ID à rechercher.");
            ui->le_id_update_search->setFocus();
            return;
        }

        QSqlDatabase db = Connection::instance()->getDatabase();
        if (!db.isOpen()) {
            QMessageBox::critical(this, "Erreur Base de données", "Connexion BD fermée. Relancez l'application.");
            return;
        }

        QString tableName = detectOrdersTableName(db);
        if (tableName.isEmpty()) {
            QMessageBox::critical(this, "Erreur Base de données",
                                  "Aucune table COMMANDE/COMMANDES trouvée dans ce schéma Oracle.");
            return;
        }

        QSqlQuery q(db);
        const OrderColumns cols = detectOrderColumns(db, tableName);
        if (!cols.valid) {
            const QStringList realCols = getTableColumns(db, tableName);
            QMessageBox::critical(this, "Erreur Base de données",
                                  QString("Colonnes non reconnues dans '%1'.\nColonnes disponibles: %2")
                                      .arg(tableName, realCols.join(", ")));
            return;
        }
        QStringList selCols;
        selCols << selectExpr(cols.id, "ORDER_ID")
                << selectExpr(cols.type, "ORDER_TYPE")
                << selectExpr(cols.qty, "ORDER_QTY")
                << selectExpr(cols.email, "ORDER_EMAIL")
                << selectExpr(cols.status, "ORDER_STATUS")
                << selectExpr(cols.dateCmd, "ORDER_DATE")
                << selectExpr(cols.dateLivr, "DELIVERY_DATE")
                << selectExpr(cols.price, "ORDER_PRICE", "0");
        q.prepare(QString("SELECT %1 FROM %2 WHERE %3 = :id")
                      .arg(selCols.join(", "), tableName, cols.id));
        q.bindValue(":id", searchId);

        if (!q.exec()) {
            QMessageBox::critical(this, "Erreur Base de données",
                                  "Erreur lors de la recherche:\n" + q.lastError().text());
            qDebug() << "Erreur SQL (search update):" << q.lastError().text();
            return;
        }
        if (!q.next()) {
            QMessageBox::warning(this, "Introuvable", "❌ Aucune commande trouvée avec l'ID: " + searchId);
            return;
        }

        ui->le_id_update->setText(q.value(0).toString());
        {
            QString val = q.value(1).toString();
            int idx = ui->cb_type_update->findText(val);
            if (idx >= 0) ui->cb_type_update->setCurrentIndex(idx);
        }
        ui->sb_qty_update->setValue(q.value(2).toInt());
        ui->le_email_update->setText(q.value(3).toString());
        {
            QString val = q.value(4).toString();
            int idx = ui->cb_status_update->findText(val);
            if (idx >= 0) ui->cb_status_update->setCurrentIndex(idx);
        }
        {
            QDate d = parseDateLoose(q.value(5).toString());
            if (d.isValid()) ui->de_date_update->setDate(d);
        }
        {
            QDate d = parseDateLoose(q.value(6).toString());
            if (d.isValid()) ui->de_date_delivery_update->setDate(d);
        }
        ui->dsb_price_update->setValue(q.value(7).toDouble());
        QMessageBox::information(this, "Succès", "✓ Commande chargée ! Vous pouvez maintenant la modifier.");

        QMessageBox::information(this, "OK", "Commande chargée. Vous pouvez modifier puis confirmer.");
    });

    // --- CONFIRMATION MODIFICATION (PAGE MODIFIER) ---
    connect(ui->btn_update_confirm, &QPushButton::clicked, this, [=]() {
        QString id = ui->le_id_update->text().trimmed();
        if (id.isEmpty()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Veuillez d'abord rechercher une commande.");
            return;
        }

        // Validation des champs de modification
        QString type = ui->cb_type_update->currentText();
        int qty = ui->sb_qty_update->value();
        QString email = ui->le_email_update->text().trimmed();
        QString status = ui->cb_status_update->currentText();
        QDate dateOrder = ui->de_date_update->date();
        QDate dateDel = ui->de_date_delivery_update->date();
        double price = ui->dsb_price_update->value();

        // Validation Type
        if (type.isEmpty() || type == "Sélectionner un type") {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Veuillez sélectionner un type d'article.");
            ui->cb_type_update->setFocus();
            return;
        }

        // Validation Quantité
        if (qty <= 0) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ La quantité doit être supérieure à 0.");
            ui->sb_qty_update->setFocus();
            return;
        }

        // Validation Email
        if (email.isEmpty()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Veuillez entrer l'email du client.");
            ui->le_email_update->setFocus();
            return;
        }
        QRegularExpression emailRegex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
        if (!emailRegex.match(email).hasMatch()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Format email invalide.");
            ui->le_email_update->setFocus();
            return;
        }


        // Validation Dates
        if (!dateOrder.isValid()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ La date de commande est invalide.");
            ui->de_date_update->setFocus();
            return;
        }
        if (!dateDel.isValid()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ La date de livraison est invalide.");
            ui->de_date_delivery_update->setFocus();
            return;
        }
        if (dateDel < dateOrder) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ La date de livraison doit être >= la date de commande.");
            ui->de_date_delivery_update->setFocus();
            return;
        }

        // Validation Prix
        if (price <= 0) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Le prix doit être supérieur à 0.");
            ui->dsb_price_update->setFocus();
            return;
        }

        // Validation Statut
        if (status.isEmpty() || status == "Sélectionner un statut") {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Veuillez sélectionner un statut.");
            ui->cb_status_update->setFocus();
            return;
        }


        QSqlDatabase db = Connection::instance()->getDatabase();
        if (!db.isOpen()) {
            QMessageBox::critical(this, "Erreur Base de données", "Connexion BD fermée. Relancez l'application.");
            return;
        }

        QString tableName = detectOrdersTableName(db);
        if (tableName.isEmpty()) {
            QMessageBox::critical(this, "Erreur Base de données",
                                  "Aucune table COMMANDE/COMMANDES trouvée dans ce schéma Oracle.");
            return;
        }

        // Récupérer TOUTES les anciennes valeurs pour détecter les changements
        QString oldType, oldEmail, oldStatus;
        int oldQty = 0;
        double oldPrice = 0.0;
        QDate oldOrderDate, oldDeliveryDate;

        const OrderColumns cols = detectOrderColumns(db, tableName);

        QSqlQuery qOld(db);
        if (cols.valid) {
            QStringList selOld;
            selOld << selectExpr(cols.type, "ORDER_TYPE")
                   << selectExpr(cols.qty, "ORDER_QTY")
                   << selectExpr(cols.email, "ORDER_EMAIL")
                   << selectExpr(cols.status, "ORDER_STATUS")
                   << selectExpr(cols.dateCmd, "ORDER_DATE")
                   << selectExpr(cols.dateLivr, "DELIVERY_DATE")
                   << selectExpr(cols.price, "ORDER_PRICE", "0");
            qOld.prepare(QString("SELECT %1 FROM %2 WHERE %3 = :id")
                             .arg(selOld.join(", "), tableName, cols.id));
            qOld.bindValue(":id", id);
            if (qOld.exec() && qOld.next()) {
                oldType         = qOld.value(0).toString().trimmed();
                oldQty          = qOld.value(1).toInt();
                oldEmail        = qOld.value(2).toString().trimmed();
                oldStatus       = qOld.value(3).toString().trimmed();
                oldOrderDate    = parseDateLoose(qOld.value(4).toString());
                oldDeliveryDate = parseDateLoose(qOld.value(5).toString());
                oldPrice        = qOld.value(6).toDouble();
            }
        }

        QSqlQuery q(db);
        // Construire l'UPDATE dynamiquement
        QStringList setParts;
        if (!cols.type.isEmpty())    setParts << cols.type    + " = :type";
        if (!cols.qty.isEmpty())     setParts << cols.qty     + " = :qty";
        if (!cols.email.isEmpty())   setParts << cols.email   + " = :email";
        if (!cols.status.isEmpty())  setParts << cols.status  + " = :status";
        if (!cols.dateCmd.isEmpty()) setParts << cols.dateCmd + " = :date";
        if (!cols.dateLivr.isEmpty())setParts << cols.dateLivr+ " = :dateDel";
        if (!cols.price.isEmpty())   setParts << cols.price   + " = :price";
        if (setParts.isEmpty()) {
            QMessageBox::critical(this, "Erreur", "Aucune colonne modifiable détectée.");
            return;
        }
        q.prepare(QString("UPDATE %1 SET %2 WHERE %3 = :id")
                      .arg(tableName, setParts.join(", "), cols.id));
        if (!cols.type.isEmpty())    q.bindValue(":type",    type);
        if (!cols.qty.isEmpty())     q.bindValue(":qty",     qty);
        if (!cols.email.isEmpty())   q.bindValue(":email",   email);
        if (!cols.status.isEmpty())  q.bindValue(":status",  status);
        if (!cols.dateCmd.isEmpty()) q.bindValue(":date",    dateOrder);
        if (!cols.dateLivr.isEmpty())q.bindValue(":dateDel", dateDel);
        if (!cols.price.isEmpty())   q.bindValue(":price",   price);
        q.bindValue(":id", id);

        if (!q.exec()) {
            QMessageBox::critical(this, "Erreur Base de données",
                                  "Erreur lors de la mise à jour:\n" + q.lastError().text());
            qDebug() << "Erreur SQL (update):" << q.lastError().text();
            return;
        }
        if (q.numRowsAffected() == 0) {
            QMessageBox::warning(this, "Avertissement", "Aucune ligne mise à jour (ID introuvable ?).");
            return;
        }

        QSqlQuery commitQuery(db);
        if (!commitQuery.exec("COMMIT")) {
            qDebug() << "Erreur COMMIT (update):" << commitQuery.lastError().text();
            QMessageBox::warning(this, "Erreur", "La mise à jour a pu ne pas être enregistrée correctement.");
            return;
        }

        reloadOrdersFromDb();

        // Envoi automatique d'email si N'IMPORTE QUEL champ a été modifié
        bool hasChanges = false;
        QStringList changes; // Liste des modifications pour le message

        if (oldType != type) {
            hasChanges = true;
            changes << QString("Type de produit: %1 → %2").arg(oldType, type);
        }
        if (oldQty != qty) {
            hasChanges = true;
            changes << QString("Quantité: %1 → %2").arg(oldQty).arg(qty);
        }
        if (oldEmail != email) {
            hasChanges = true;
            changes << QString("Email: %1 → %2").arg(oldEmail, email);
        }
        if (oldStatus.compare(status, Qt::CaseInsensitive) != 0) {
            hasChanges = true;
            changes << QString("Statut: %1 → %2").arg(oldStatus, status);
        }
        if (oldOrderDate != dateOrder) {
            hasChanges = true;
            changes << QString("Date de commande: %1 → %2")
                           .arg(oldOrderDate.toString("dd/MM/yyyy"), dateOrder.toString("dd/MM/yyyy"));
        }
        if (oldDeliveryDate != dateDel) {
            hasChanges = true;
            changes << QString("Date de livraison: %1 → %2")
                           .arg(oldDeliveryDate.toString("dd/MM/yyyy"), dateDel.toString("dd/MM/yyyy"));
        }
        if (qAbs(oldPrice - price) > 0.01) { // Comparaison de doubles avec tolérance
            hasChanges = true;
            changes << QString("Prix: %1 DT → %2 DT")
                           .arg(QString::number(oldPrice, 'f', 2), QString::number(price, 'f', 2));
        }

        if (hasChanges) {
            QMap<QString, QString> orderData;
            orderData["id"] = id;
            orderData["type"] = type;
            orderData["qty"] = QString::number(qty);
            orderData["email"] = email;
            orderData["orderDate"] = dateOrder.toString("dd/MM/yyyy");
            orderData["deliveryDate"] = dateDel.toString("dd/MM/yyyy");
            orderData["price"] = QString::number(price, 'f', 2);
            orderData["status"] = status;
            orderData["changes"] = changes.join("\n• ");

            QString subject = QString("Mise à jour de votre commande #%1").arg(id);
            QString templateName = "modification"; // Nouveau template
            QString emailType = "Mise à jour de commande";

            // Si le statut a changé, adapter le template selon le nouveau statut
            if (oldStatus.compare(status, Qt::CaseInsensitive) != 0) {
                if (status.contains("prête", Qt::CaseInsensitive) || status.contains("prete", Qt::CaseInsensitive) ||
                    status.contains("livraison", Qt::CaseInsensitive)) {
                    templateName = "livraison";
                    subject = QString("Votre commande #%1 est prête !").arg(id);
                    emailType = "Notification de livraison";
                } else if (status.contains("retard", Qt::CaseInsensitive)) {
                    templateName = "retard";
                    subject = QString("Retard sur votre commande #%1").arg(id);
                    emailType = "Alerte de retard";
                } else if (status.contains("attente", Qt::CaseInsensitive)) {
                    templateName = "attente";
                    subject = QString("Votre commande #%1 est en attente").arg(id);
                    emailType = "Notification d'attente";
                } else {
                    templateName = "modification";
                    emailType = "Modification de statut";
                }
            }

            QString body = getEmailTemplate(templateName, orderData);
            bool sent = sendEmail(email, subject, body);
            if (sent) {
                addToEmailHistory(email, subject, body, emailType, id);
            }

            qDebug() << "Email automatique envoyé - Modifications détectées:";
            for (const QString &change : std::as_const(changes)) {
                qDebug() << "  •" << change;
            }
        } else {
            qDebug() << "Aucune modification détectée, pas d'email envoyé.";
        }

        QMessageBox::information(this, "Succès", "Commande mise à jour avec succès !");
    });

    // --- SUPPRIMER UNE COMMANDE (PAGE SUPPRIMER) ---
    connect(ui->btn_delete_action, &QPushButton::clicked, this, [=]() {
        QString id = ui->le_id_to_delete->text().trimmed();
        if (id.isEmpty()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Veuillez entrer l'ID de commande à supprimer.");
            ui->le_id_to_delete->setFocus();
            return;
        }

        // Confirmation avant suppression
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                                  QString("⚠️  Êtes-vous sûr de vouloir supprimer la commande '%1' ?\nCette action est irréversible !").arg(id),
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            return;
        }

        QSqlDatabase db = Connection::instance()->getDatabase();
        if (!db.isOpen()) {
            QMessageBox::critical(this, "Erreur Base de données", "Connexion BD fermée. Relancez l'application.");
            return;
        }

        QString tableName = detectOrdersTableName(db);
        if (tableName.isEmpty()) {
            QMessageBox::critical(this, "Erreur Base de données",
                                  "Aucune table COMMANDE/COMMANDES trouvée dans ce schéma Oracle.");
            return;
        }

        // Supprimer d'abord les enregistrements enfants liés (évite ORA-02292)
        QString depError;
        if (!deleteOrderDependencies(db, tableName, id, &depError)) {
            QMessageBox::critical(this, "Erreur Base de données",
                                  "Impossible de supprimer les enregistrements liés à cette commande.\n" + depError);
            qDebug() << "Erreur dépendances (delete):" << depError;
            return;
        }

        QSqlQuery del(db);
        del.prepare(QString("DELETE FROM %1 WHERE ID_COMMANDE = :id").arg(tableName));
        del.bindValue(":id", id);

        if (!del.exec()) {
            const QString sqlErr = del.lastError().text();
            if (sqlErr.contains("ORA-02292", Qt::CaseInsensitive)) {
                QMessageBox::critical(this, "Erreur Base de données",
                                      "Suppression impossible: cette commande est encore référencée par d'autres données.\n"
                                      "Supprimez d'abord les éléments liés, puis réessayez.\n\n"
                                      "Détail SQL: " + sqlErr);
            } else {
                QMessageBox::critical(this, "Erreur Base de données",
                                      "Erreur lors de la suppression:\n" + sqlErr);
            }
            qDebug() << "Erreur SQL (delete):" << sqlErr;
            return;
        }
        if (del.numRowsAffected() == 0) {
            QMessageBox::warning(this, "Introuvable", "❌ Aucune commande supprimée (ID introuvable)." );
            return;
        }

        QSqlQuery commitQuery(db);
        if (!commitQuery.exec("COMMIT")) {
            qDebug() << "Erreur COMMIT (delete):" << commitQuery.lastError().text();
            QMessageBox::warning(this, "Erreur", "La suppression a pu ne pas être enregistrée correctement.");
            return;
        }

        reloadOrdersFromDb();

        ui->le_id_to_delete->clear();
        QMessageBox::information(this, "Succès", "✓ Commande supprimée avec succès !");
    });

    // --- CONNEXIONS POUR LA PAGE LISTE ---
    connect(ui->le_search, &QLineEdit::textChanged, this, &MainWindow::searchOrdersList);
    connect(ui->cb_sort, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::sortOrdersList);
    connect(ui->btn_pdf, &QPushButton::clicked, this, &MainWindow::exportListToPDF);

    // --- CONNEXIONS POUR LE CALENDRIER ---
    connect(ui->calendarWidget, &QCalendarWidget::selectionChanged, this, &MainWindow::onCalendarDateChanged);
    connect(ui->btn_export_calendar, &QPushButton::clicked, this, &MainWindow::onExportCalendar);

    // --- NOTIFICATIONS ET RAPPELS ---
    // Vérification immédiate + timer horaire pour les rappels de livraison
    QTimer *notificationTimer = new QTimer(this);
    connect(notificationTimer, &QTimer::timeout, this, &MainWindow::checkForNotifications);
    notificationTimer->start(3600000); // toutes les heures

    // Initialisation
    reloadOrdersFromDb();
    qDebug() << "reloadOrdersFromDb done";
    onCalendarDateChanged();
    updateStatistics();
    checkForNotifications(); // Vérification initiale
    loadEmailHistory(); // Charger l'historique des emails

    // Configuration SMTP Gmail pré-configurée
    smtpUsername = "rayenbenabdallah21@gmail.com";
    smtpPassword = "imghwonvblnjenfv"; // Mot de passe d'application (sans espaces)
    qDebug() << "✅ Configuration SMTP: Email configuré pour" << smtpUsername;
    // Initialisation Arduino
    if (arduinoSensor ? arduinoSensor->connectArduino() : false == 0) {
        qDebug() << "✅ Arduino connecté avec succès";
        connect(arduinoSensor ? arduinoSensor->getSerialPort() : nullptr, &QSerialPort::readyRead, this, &MainWindow::update_label);
    } else {
        qDebug() << "⚠️ Échec de connexion Arduino (vérifiez les câbles/drivers)";
    }

    qDebug() << "MainWindow constructor end";

    // Création dynamique du label de statut de porte dans la barre latérale
    lbl_door_status = new QLabel(this);
    lbl_door_status->setAlignment(Qt::AlignCenter);
    lbl_door_status->setStyleSheet("color: #2ea043; font-size: 16px; font-weight: bold; background: #e8f5e9; border-radius: 8px; padding: 10px; margin: 10px;");
    lbl_door_status->setText("🚪 PORTE FERMÉE");
    lbl_door_status->setVisible(false);
    if (ui->sidebarLayout) {
        // Insérer avant le spacer à la fin
        ui->sidebarLayout->insertWidget(ui->sidebarLayout->count() - 1, lbl_door_status);
    }
}

void MainWindow::reloadOrdersFromDb()
{
    static bool warnedDbClosed = false;
    static bool warnedNoTable = false;
    static bool warnedSelectFailed = false;

    QSqlDatabase db = Connection::instance()->getDatabase();
    if (!db.isOpen()) {
        qDebug() << "reloadOrdersFromDb: DB not open";
        if (!warnedDbClosed) {
            warnedDbClosed = true;
            QMessageBox::critical(this, "Erreur Base de données",
                                  "Connexion BD fermée. Relancez l'application.");
        }
        return;
    }

    QString tableName = detectOrdersTableName(db);
    if (tableName.isEmpty()) {
        qDebug() << "reloadOrdersFromDb: no COMMANDE/COMMANDES table found";
        if (!warnedNoTable) {
            warnedNoTable = true;
            QMessageBox::critical(this, "Erreur Base de données",
                                  "Aucune table COMMANDE/COMMANDES trouvée dans ce schéma Oracle.\n"
                                  "Vérifiez que vous êtes connecté au bon utilisateur/schéma.");
        }
        return;
    }

    const OrderColumns cols = detectOrderColumns(db, tableName);
    if (!cols.valid) {
        const QStringList realCols = getTableColumns(db, tableName);
        QMessageBox::critical(this, "Erreur Base de données",
                              QString("Colonnes non reconnues dans '%1'.\nColonnes disponibles: %2\n\n"
                                      "Ajoutez vos noms de colonnes dans detectOrderColumns().")
                                  .arg(tableName, realCols.join(", ")));
        return;
    }

    const QStringList realCols = getTableColumns(db, tableName);
    bool hasEmail = !cols.email.isEmpty() && realCols.contains(cols.email.toUpper());
    bool hasStatus = !cols.status.isEmpty() && realCols.contains(cols.status.toUpper());
    bool hasDateCmd = !cols.dateCmd.isEmpty() && realCols.contains(cols.dateCmd.toUpper());
    bool hasDateLivr = !cols.dateLivr.isEmpty() && realCols.contains(cols.dateLivr.toUpper());
    bool hasPrice = !cols.price.isEmpty() && realCols.contains(cols.price.toUpper());
    bool hasCin = !cols.cin.isEmpty() && realCols.contains(cols.cin.toUpper());

    QStringList selCols;
    selCols << cols.id << cols.type << cols.qty;
    if (hasEmail)   selCols << cols.email;
    if (hasStatus)  selCols << cols.status;
    if (hasDateCmd) selCols << cols.dateCmd;
    if (hasDateLivr)selCols << cols.dateLivr;
    if (hasPrice)   selCols << cols.price;
    else            selCols << "0 AS ORDER_PRICE";
    if (hasCin)     selCols << cols.cin + " AS ORDER_EMPLOYEE_CIN";

    const QString sql = QString("SELECT %1 FROM %2 ORDER BY %3")
                            .arg(selCols.join(", "), tableName, cols.id);
    qDebug() << "reloadOrdersFromDb SQL:" << sql;

    QSqlQuery q(db);
    q.prepare(sql);

    if (!q.exec()) {
        qDebug() << "reloadOrdersFromDb: SELECT failed:" << q.lastError().text();
        if (!warnedSelectFailed) {
            warnedSelectFailed = true;
            QMessageBox::critical(this, "Erreur Base de données",
                                  "Erreur lors du chargement des commandes:\n" + q.lastError().text());
        }
        return;
    }

    ui->table_list->setRowCount(0);
    ui->table_delete->setRowCount(0);

    deliveryDates.clear();
    deliveryStatuses.clear();

    while (q.next()) {
        int i = 0;
        QString id       = q.value(i++).toString();
        QString type     = q.value(i++).toString();
        int     qty      = q.value(i++).toInt();
        QString email    = hasEmail ? q.value(i++).toString() : QString();
        QString status   = hasStatus ? q.value(i++).toString() : QString();
        QString date     = normalizeDateText(hasDateCmd ? q.value(i++).toString() : QString());
        QString dateDel  = normalizeDateText(hasDateLivr ? q.value(i++).toString() : QString());
        double  price    = q.value(i++).toDouble();
        QString cinEmploye = hasCin ? q.value(i++).toString() : QString();

        // Liste (10 colonnes)
        {
            int row = ui->table_list->rowCount();
            ui->table_list->insertRow(row);
            ui->table_list->setItem(row, 0, new QTableWidgetItem(id));
            ui->table_list->setItem(row, 1, new QTableWidgetItem(type));
            ui->table_list->setItem(row, 2, new QTableWidgetItem(QString::number(qty)));
            ui->table_list->setItem(row, 3, new QTableWidgetItem(email));
            ui->table_list->setItem(row, 4, new QTableWidgetItem(""));
            ui->table_list->setItem(row, 5, new QTableWidgetItem(date));
            ui->table_list->setItem(row, 6, new QTableWidgetItem(dateDel));
            ui->table_list->setItem(row, 7, new QTableWidgetItem(QString::number(price)));
            ui->table_list->setItem(row, 8, new QTableWidgetItem(cinEmploye));
            ui->table_list->setItem(row, 9, new QTableWidgetItem(status));
        }

        // Supprimer (5 colonnes)
        {
            int row = ui->table_delete->rowCount();
            ui->table_delete->insertRow(row);
            ui->table_delete->setItem(row, 0, new QTableWidgetItem(id));
            ui->table_delete->setItem(row, 1, new QTableWidgetItem(type));
            ui->table_delete->setItem(row, 2, new QTableWidgetItem(QString::number(qty)));
            ui->table_delete->setItem(row, 3, new QTableWidgetItem(status));
            ui->table_delete->setItem(row, 4, new QTableWidgetItem(email));
        }

        // Calendrier
        QDate deliveryDate = parseDateLoose(dateDel);
        if (deliveryDate.isValid()) {
            deliveryDates[deliveryDate].append(id);
            deliveryStatuses[deliveryDate][id] = status;
        }
    }

    updateCalendarHighlights();
    updateStatistics();
    updateCalendarStats();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- IMPLÉMENTATION DES NOTIFICATIONS ---


// --- IMPLÉMENTATION DU CALENDRIER ---

void MainWindow::onCalendarDateChanged()
{
    QDate selectedDate = ui->calendarWidget->selectedDate();
    qDebug() << "Calendrier: Date sélectionnée:" << selectedDate.toString("dd/MM/yyyy");

    ui->lbl_selected_date->setText("📅 Date sélectionnée: " + selectedDate.toString("dd/MM/yyyy"));
    updateDeliveryList(selectedDate);

    // Met à jour les statistiques du calendrier pour refléter la sélection
    updateCalendarStats();
}

void MainWindow::updateDeliveryList(const QDate &date)
{
    ui->list_deliveries->clear();

    qDebug() << "Mise à jour liste livraisons pour:" << date.toString("dd/MM/yyyy");
    qDebug() << "Nombre total de dates avec livraisons:" << deliveryDates.size();

    if (deliveryDates.contains(date)) {
        QStringList orderIds = deliveryDates[date];
        qDebug() << "Livraisons trouvées pour cette date:" << orderIds.size();

        // Ajouter un en-tête avec le nombre de livraisons
        ui->list_deliveries->addItem(QString("═══ %1 livraison(s) prévue(s) ═══").arg(orderIds.size()));

        for (const QString &orderId : orderIds) {
            bool foundInList = false;
            // Trouver les détails de la commande
            for (int row = 0; row < ui->table_list->rowCount(); ++row) {
                if (ui->table_list->item(row, 0) && ui->table_list->item(row, 0)->text() == orderId) {
                    QString type = ui->table_list->item(row, 1)->text();
                    QString quantity = ui->table_list->item(row, 2)->text();
                    QString email = ui->table_list->item(row, 3)->text();
                    QString orderDate = ui->table_list->item(row, 5)->text();
                    QString deliveryDate = ui->table_list->item(row, 6)->text();
                    QString price = ui->table_list->item(row, 7)->text();
                    QString status = ui->table_list->item(row, 9)->text();

                    // Indicateur coloré basé sur le statut
                    QString statusIcon = "🔵";
                    if (status.contains("Prête", Qt::CaseInsensitive)) {
                        statusIcon = "🟢";
                    } else if (status.contains("En cours", Qt::CaseInsensitive)) {
                        statusIcon = "🟠";
                    } else if (status.contains("En attente", Qt::CaseInsensitive)) {
                        statusIcon = "🔴";
                    } else if (status.contains("Complétée", Qt::CaseInsensitive)) {
                        statusIcon = "🔵";
                    }

                    QString itemText = QString(
                                           "%1 Commande #%2\n"
                                           "   Type: %3\n"
                                           "   Client: %4\n"
                                           "   Quantité: %5\n"
                                           "   Prix: %6 DT\n"
                                           "   Statut: %7\n"
                                           "   Commandée: %8\n"
                                           "   À livrer: %9"
                                           ).arg(statusIcon, orderId, type, email, quantity, price, status, orderDate, deliveryDate);

                    ui->list_deliveries->addItem(itemText);
                    foundInList = true;
                    break;
                }
            }

            // Fallback: afficher au moins l'ID si les détails ne sont pas trouvés dans table_list
            if (!foundInList) {
                ui->list_deliveries->addItem(QString("🔵 Commande #%1 (détails non disponibles)").arg(orderId));
            }
        }
    } else {
        qDebug() << "Aucune livraison pour cette date";
        ui->list_deliveries->addItem("═══════════════════════");
        ui->list_deliveries->addItem("❌ Aucune livraison prévue");
        ui->list_deliveries->addItem("pour cette date.");
        ui->list_deliveries->addItem("═══════════════════════");

        // Afficher un aperçu des prochaines livraisons
        if (!deliveryDates.isEmpty()) {
            ui->list_deliveries->addItem("");
            ui->list_deliveries->addItem("📅 Prochaines livraisons:");

            QList<QDate> allDates = deliveryDates.keys();
            std::sort(allDates.begin(), allDates.end());

            int count = 0;
            for (const QDate &nextDate : std::as_const(allDates)) {
                if (nextDate >= date && count < 5) {
                    int nbOrders = deliveryDates[nextDate].size();
                    ui->list_deliveries->addItem(
                        QString("  • %1: %2 commande(s)")
                            .arg(nextDate.toString("dd/MM/yyyy"))
                            .arg(nbOrders)
                        );
                    count++;
                }
            }
        }
    }
}

void MainWindow::updateCalendarHighlights()
{
    // Réinitialiser le format du calendrier
    QTextCharFormat defaultFormat;
    defaultFormat.setForeground(Qt::black);

    QTextCharFormat weekendFormat;
    weekendFormat.setForeground(QColor(220, 20, 60)); // Rouge écarlate
    weekendFormat.setFontWeight(QFont::Bold);

    QTextCharFormat todayFormat;
    todayFormat.setBackground(QColor(255, 215, 0)); // Jaune or
    todayFormat.setForeground(Qt::black);
    todayFormat.setFontWeight(QFont::Bold);

    // Couleurs par statut
    QTextCharFormat readyFormat;
    readyFormat.setBackground(QColor(144, 238, 144)); // Vert clair
    readyFormat.setForeground(Qt::darkGreen);
    readyFormat.setFontWeight(QFont::Bold);

    QTextCharFormat inProgressFormat;
    inProgressFormat.setBackground(QColor(255, 179, 71)); // Orange
    inProgressFormat.setForeground(Qt::darkRed);
    inProgressFormat.setFontWeight(QFont::Bold);

    QTextCharFormat pendingFormat;
    pendingFormat.setBackground(QColor(255, 107, 107)); // Rouge
    pendingFormat.setForeground(Qt::white);
    pendingFormat.setFontWeight(QFont::Bold);

    QTextCharFormat completedFormat;
    completedFormat.setBackground(QColor(135, 206, 235)); // Bleu ciel
    completedFormat.setForeground(Qt::darkBlue);
    completedFormat.setFontWeight(QFont::Bold);

    QTextCharFormat deliveryFormat;
    deliveryFormat.setBackground(QColor(211, 211, 211)); // Gris clair pour statut inconnu
    deliveryFormat.setForeground(Qt::black);
    deliveryFormat.setFontWeight(QFont::Bold);

    QDate today = QDate::currentDate();
    QDate monthStart = QDate(today.year(), today.month(), 1);
    QDate monthEnd = monthStart.addMonths(1).addDays(-1);

    // Parcourir tout le mois et appliquer les mises en forme
    for (QDate date = monthStart; date <= monthEnd; date = date.addDays(1)) {
        if (date == today) {
            // Aujourd'hui : priorité la plus haute
            ui->calendarWidget->setDateTextFormat(date, todayFormat);
        } else if (deliveryDates.contains(date)) {
            // Déterminer le pire statut pour la date (utiliser celui-ci pour la couleur)
            QString worstStatus = "En attente"; // défaut
            bool hasReady = false;
            bool hasInProgress = false;
            bool hasPending = false;
            bool hasCompleted = false;

            QStringList orderIds = deliveryDates[date];
            for (const QString& orderId : orderIds) {
                QString status = deliveryStatuses[date].value(orderId, "En attente");

                if (status.contains("Prête", Qt::CaseInsensitive)) {
                    hasReady = true;
                } else if (status.contains("En cours", Qt::CaseInsensitive)) {
                    hasInProgress = true;
                } else if (status.contains("En attente", Qt::CaseInsensitive)) {
                    hasPending = true;
                } else if (status.contains("Complétée", Qt::CaseInsensitive)) {
                    hasCompleted = true;
                }
            }

            // Hiérarchie : Attente > En cours > Prête > Complétée
            if (hasPending) {
                ui->calendarWidget->setDateTextFormat(date, pendingFormat);
            } else if (hasInProgress) {
                ui->calendarWidget->setDateTextFormat(date, inProgressFormat);
            } else if (hasReady) {
                ui->calendarWidget->setDateTextFormat(date, readyFormat);
            } else if (hasCompleted) {
                ui->calendarWidget->setDateTextFormat(date, completedFormat);
            } else {
                ui->calendarWidget->setDateTextFormat(date, deliveryFormat);
            }
        } else if (date.dayOfWeek() == 6 || date.dayOfWeek() == 7) {
            // Samedis et dimanches : texte en rouge
            ui->calendarWidget->setDateTextFormat(date, weekendFormat);
        } else {
            // Autres jours : format par défaut (noir)
            ui->calendarWidget->setDateTextFormat(date, defaultFormat);
        }
    }
}

void MainWindow::updateCalendarStats()
{
    QDate today = QDate::currentDate();
    QDate weekEnd = today.addDays(7);

    int thisWeekCount = 0;
    int thisMonthCount = 0;
    int overdueCount = 0;
    int readyCount = 0;
    int inProgressCount = 0;
    int pendingCount = 0;
    int completedCount = 0;
    double totalRevenue = 0.0;

    // Statistiques par date de livraison
    for (auto it = deliveryDates.begin(); it != deliveryDates.end(); ++it) {
        QDate deliveryDate = it.key();
        QStringList orderIds = it.value();
        int orderCount = orderIds.count();

        if (deliveryDate < today) {
            overdueCount += orderCount;
        }

        if (deliveryDate >= today && deliveryDate <= weekEnd) {
            thisWeekCount += orderCount;
        }

        if (deliveryDate.year() == today.year() && deliveryDate.month() == today.month()) {
            thisMonthCount += orderCount;
        }

        // Comptage par statut
        for (const QString& orderId : orderIds) {
            QString status = deliveryStatuses[deliveryDate].value(orderId, "En attente");
            if (status.contains("Prête", Qt::CaseInsensitive)) {
                readyCount++;
            } else if (status.contains("En cours", Qt::CaseInsensitive)) {
                inProgressCount++;
            } else if (status.contains("En attente", Qt::CaseInsensitive)) {
                pendingCount++;
            } else if (status.contains("Complétée", Qt::CaseInsensitive)) {
                completedCount++;
            }
        }
    }

    // Calculer le revenu total
    for (int row = 0; row < ui->table_list->rowCount(); ++row) {
        QString priceStr = ui->table_list->item(row, 7) ? ui->table_list->item(row, 7)->text() : "0";
        totalRevenue += priceStr.toDouble();
    }

    ui->lbl_this_week_value->setText(QString::number(thisWeekCount));
    ui->lbl_this_month_value->setText(QString::number(thisMonthCount));
    ui->lbl_overdue_value->setText(QString::number(overdueCount));
}

void MainWindow::onExportCalendar()
{
    // Créer une boîte de dialogue pour demander le format d'export
    QStringList formats;
    formats << "PDF" << "CSV";

    bool ok;
    QString format = QInputDialog::getItem(this, "Format d'export",
                                           "Choisir le format d'export:", formats, 0, false, &ok);

    if (!ok) return;

    // Demander le chemin de sauvegarde
    QString fileName;
    if (format == "PDF") {
        fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF",
                                                "commandes.pdf", "PDF Files (*.pdf)");
    } else {
        fileName = QFileDialog::getSaveFileName(this, "Exporter en CSV",
                                                "commandes.csv", "CSV Files (*.csv)");
    }

    if (fileName.isEmpty()) return;

    if (format == "PDF") {
        exportToPDF(fileName);
    } else {
        exportToCSV(fileName);
    }
}

void MainWindow::exportToPDF(const QString &fileName)
{
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setTitle("Rapport des Commandes");

    QPainter painter;
    if (!painter.begin(&pdfWriter)) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier PDF.");
        return;
    }

    // En-tête
    QFont headerFont("Arial", 16, QFont::Bold);
    painter.setFont(headerFont);
    painter.drawText(50, 50, "Rapport des Commandes");
    painter.drawText(50, 100, QString("Généré le %1").arg(QDate::currentDate().toString("dd/MM/yyyy")));

    // Tableau
    QFont tableFont("Arial", 10);
    painter.setFont(tableFont);

    int y = 150;
    int lineHeight = 20;

    // En-têtes du tableau
    painter.drawText(50, y, "ID");
    painter.drawText(120, y, "Type");
    painter.drawText(220, y, "Qty");
    painter.drawText(280, y, "Email");
    painter.drawText(420, y, "Statut");

    y += lineHeight;

    // Données du tableau
    for (int row = 0; row < ui->table_list->rowCount(); ++row) {
        if (y > pdfWriter.height() - 50) {
            pdfWriter.newPage();
            y = 50;
        }

        QString id = ui->table_list->item(row, 0) ? ui->table_list->item(row, 0)->text() : "";
        QString type = ui->table_list->item(row, 1) ? ui->table_list->item(row, 1)->text() : "";
        QString qty = ui->table_list->item(row, 2) ? ui->table_list->item(row, 2)->text() : "";
        QString email = ui->table_list->item(row, 3) ? ui->table_list->item(row, 3)->text() : "";
        QString status = ui->table_list->item(row, 9) ? ui->table_list->item(row, 9)->text() : "";

        painter.drawText(50, y, id);
        painter.drawText(120, y, type);
        painter.drawText(220, y, qty);
        painter.drawText(280, y, email.left(20));
        painter.drawText(420, y, status);

        y += lineHeight;
    }

    painter.end();
    QMessageBox::information(this, "Succès", "Le fichier PDF a été créé avec succès:\n" + fileName);
}

void MainWindow::exportToCSV(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier CSV.");
        return;
    }

    QTextStream out(&file);

    // En-têtes
    out << "ID,Type,Quantité,Email,Date Commande,Date Livraison,Prix,Statut\n";

    // Données
    for (int row = 0; row < ui->table_list->rowCount(); ++row) {
        QString id = ui->table_list->item(row, 0) ? ui->table_list->item(row, 0)->text() : "";
        QString type = ui->table_list->item(row, 1) ? ui->table_list->item(row, 1)->text() : "";
        QString qty = ui->table_list->item(row, 2) ? ui->table_list->item(row, 2)->text() : "";
        QString email = ui->table_list->item(row, 3) ? ui->table_list->item(row, 3)->text() : "";
        QString dateOrder = ui->table_list->item(row, 5) ? ui->table_list->item(row, 5)->text() : "";
        QString dateDeliv = ui->table_list->item(row, 6) ? ui->table_list->item(row, 6)->text() : "";
        QString price = ui->table_list->item(row, 7) ? ui->table_list->item(row, 7)->text() : "";
        QString status = ui->table_list->item(row, 9) ? ui->table_list->item(row, 9)->text() : "";

        out << QString("\"%1\",\"%2\",\"%3\",\"%4\",\"%5\",\"%6\",\"%7\",\"%8\"\n")
                   .arg(id, type, qty, email, dateOrder, dateDeliv, price, status);
    }

    file.close();
    QMessageBox::information(this, "Succès", "Le fichier CSV a été créé avec succès:\n" + fileName);
}

// --- IMPLÉMENTATION DES STATISTIQUES ---

void MainWindow::updateStatistics()
{
    int totalOrders = ui->table_list->rowCount();
    double totalRevenue = 0.0;
    int pendingOrders = 0;

    QMap<QString, int> typeCount;

    for (int row = 0; row < totalOrders; ++row) {
        auto *priceItem = ui->table_list->item(row, 7);
        auto *statusItem = ui->table_list->item(row, 9);
        auto *typeItem = ui->table_list->item(row, 1);
        if (!priceItem || !statusItem || !typeItem) {
            continue;
        }

        // Calcul du revenu total
        QString priceStr = priceItem->text();
        totalRevenue += priceStr.toDouble();

        // Comptage des commandes en attente
        QString status = statusItem->text();
        if (status == "En attente" || status == "En cours") {
            pendingOrders++;
        }

        // Comptage par type
        QString type = typeItem->text();
        typeCount[type]++;

    }

    // ===== MISE À JOUR DE LA PAGE STATS PRINCIPALE (SIDEBAR) =====
    ui->lbl_main_orders_value->setText(QString::number(totalOrders));
    ui->lbl_main_revenue_value->setText(QString::number(totalRevenue, 'f', 2) + " DT");
    ui->lbl_main_pending_value->setText(QString::number(pendingOrders));

    // Trouver le produit le plus vendu
    QString topProduct = "-";
    int maxTypeCount = 0;
    for (auto it = typeCount.begin(); it != typeCount.end(); ++it) {
        if (it.value() > maxTypeCount) {
            maxTypeCount = it.value();
            topProduct = it.key() + " (" + QString::number(it.value()) + " commandes)";
        }
    }
    ui->lbl_main_top_product_value->setText(topProduct);

    ui->lbl_main_top_city_value->setText("N/A");

    // Prix moyen
    double avgPrice = totalOrders > 0 ? totalRevenue / totalOrders : 0.0;
    ui->lbl_main_avg_price_value->setText(QString::number(avgPrice, 'f', 2) + " DT");

    // Répartition par type
    QString typeStats = "📊 Répartition par type de cuir:\n\n";
    for (auto it = typeCount.begin(); it != typeCount.end(); ++it) {
        double percentage = totalOrders > 0 ? (it.value() * 100.0 / totalOrders) : 0.0;
        typeStats += QString("• %1: %2 commandes (%3%)\n").arg(it.key()).arg(it.value()).arg(QString::number(percentage, 'f', 1));
    }
    ui->lbl_main_type_stats->setText(typeStats.isEmpty() || totalOrders == 0 ? "Aucune donnée disponible." : typeStats);

    // Pie chart rendering for type distribution
    if (ui->stats_chart_frame) {
        auto chartView = ui->stats_chart_frame->findChild<QChartView*>("typePieChartView");
        if (chartView) {
            auto *series = new QPieSeries();
            if (totalOrders > 0) {
                for (auto it = typeCount.begin(); it != typeCount.end(); ++it) {
                    series->append(it.key(), it.value());
                }
            } else {
                series->append("Aucune donnée", 1);
            }

            const auto slices = series->slices();
            for (QPieSlice *slice : std::as_const(slices)) {
                double pct = slice->percentage() * 100.0;
                if (pct > 0.0) {
                    slice->setLabel(QString("%1 (%2%)").arg(slice->label(), QString::number(pct, 'f', 1)));
                }
                slice->setLabelVisible(true);
            }

            auto *chart = new QChart();
            chart->addSeries(series);
            chart->setTitle(QString());
            chart->legend()->setVisible(true);
            chart->legend()->setAlignment(Qt::AlignBottom);
            chart->setBackgroundVisible(false);
            chart->setMargins(QMargins(0, 0, 0, 0));

            chartView->setChart(chart);
        }
    }

    // ===== MISE À JOUR DE L'ONGLET STATS DANS LA PAGE LISTE =====
    ui->lbl_total_orders_val->setText(QString::number(totalOrders));
    ui->lbl_top_client_val->setText("N/A");

    // Mise à jour des statistiques détaillées de l'onglet
    QString typeStatsTab = "📊 Répartition par type:\n";
    for (auto it = typeCount.begin(); it != typeCount.end(); ++it) {
        typeStatsTab += QString("• %1: %2 commandes\n").arg(it.key()).arg(it.value());
    }
    ui->lbl_type_stats->setText(typeStatsTab.isEmpty() ? "Aucune donnée." : typeStatsTab);

    ui->lbl_city_stats->setText("Répartition par ville: N/A");

    QString priceStatsTab = QString("💰 Revenu total: %1 DT\n💵 Prix moyen: %2 DT\n📦 En attente: %3")
                                .arg(QString::number(totalRevenue, 'f', 2),
                                     QString::number(avgPrice, 'f', 2),
                                     QString::number(pendingOrders));
    ui->lbl_price_stats->setText(priceStatsTab);
}

void MainWindow::displayOrdersForUpdate()
{
    // Créer une table pour afficher les commandes si elle n'existe pas
    if (!table_update) {
        // Chercher le parent layout de la page_update pour insérer la table
        if (ui->page_update) {
            // Créer le widget table
            table_update = new QTableWidget(ui->page_update);
            table_update->setColumnCount(5);
            table_update->setHorizontalHeaderLabels({"ID", "Type", "Quantité", "Email", "Status"});
            table_update->setMaximumHeight(200);
            table_update->setMinimumHeight(150);

            // Ajouter à la première position du layout si c'est un VBox
            QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->page_update->layout());
            if (layout) {
                layout->insertWidget(0, table_update);
            }

            // Connexion pour charger une commande au double-clic
            connect(table_update, &QTableWidget::cellDoubleClicked, this, [=](int row, int) {
                QString orderId = table_update->item(row, 0)->text();
                ui->le_id_update_search->setText(orderId);

                // Simuler un clic sur le bouton CHARGER
                if (ui->btn_search_update) {
                    ui->btn_search_update->click();
                }
            });
        }
    }

    // Remplir la table avec les commandes actuelles
    if (table_update) {
        table_update->setRowCount(0);

        for (int row = 0; row < ui->table_list->rowCount(); ++row) {
            QTableWidgetItem *idItem = ui->table_list->item(row, 0);
            QTableWidgetItem *typeItem = ui->table_list->item(row, 1);
            QTableWidgetItem *qtyItem = ui->table_list->item(row, 2);
            QTableWidgetItem *emailItem = ui->table_list->item(row, 3);
            QTableWidgetItem *statusItem = ui->table_list->item(row, 9);

            if (idItem && typeItem && qtyItem && emailItem && statusItem) {
                int newRow = table_update->rowCount();
                table_update->insertRow(newRow);

                table_update->setItem(newRow, 0, new QTableWidgetItem(idItem->text()));
                table_update->setItem(newRow, 1, new QTableWidgetItem(typeItem->text()));
                table_update->setItem(newRow, 2, new QTableWidgetItem(qtyItem->text()));
                table_update->setItem(newRow, 3, new QTableWidgetItem(emailItem->text()));
                table_update->setItem(newRow, 4, new QTableWidgetItem(statusItem->text()));
            }
        }

        // Ajuster la largeur des colonnes
        table_update->resizeColumnsToContents();
    }
}

// --- FILTRES AVANCÉS ---

void MainWindow::applyAdvancedFilters()
{
    // Appliquer les filtres et mettre à jour le calendrier
    updateCalendarHighlights();

    QDate selectedDate = ui->calendarWidget->selectedDate();
    updateDeliveryList(selectedDate);

    QMessageBox::information(this, "Filtres appliqués",
                             "Les filtres avancés ont été appliqués\nau calendrier et aux listes.");
}

void MainWindow::clearAdvancedFilters()
{
    // Réinitialiser tous les filtres
    updateCalendarHighlights();

    QDate selectedDate = ui->calendarWidget->selectedDate();
    updateDeliveryList(selectedDate);
    updateStatistics();

    QMessageBox::information(this, "Filtres réinitialisés",
                             "Tous les filtres ont été réinitialisés.");
}

// --- NOTIFICATIONS ET RAPPELS ---

void MainWindow::checkForNotifications()
{
    QDate today = QDate::currentDate();
    QDate tomorrow = today.addDays(1);
    QDate nextWeek = today.addDays(7);

    int tomorrowCount = 0;
    int nextWeekCount = 0;
    int overdueCount = 0;
    int readyCount = 0;

    // Parcourir les dates de livraison et vérifier les notifications
    for (auto it = deliveryDates.begin(); it != deliveryDates.end(); ++it) {
        QDate deliveryDate = it.key();
        QStringList orderIds = it.value();

        if (deliveryDate == tomorrow) {
            tomorrowCount += orderIds.count();
        }

        if (deliveryDate > tomorrow && deliveryDate <= nextWeek) {
            nextWeekCount += orderIds.count();
        }

        if (deliveryDate < today) {
            overdueCount += orderIds.count();
        }

        // Compter les commandes prêtes
        for (const QString& orderId : orderIds) {
            QString status = deliveryStatuses[deliveryDate].value(orderId, "En attente");
            if (status.contains("Prête", Qt::CaseInsensitive)) {
                readyCount++;
            }
        }
    }

    // Construire le message de notification
    QString notificationText;

    if (overdueCount > 0) {
        notificationText += QString("⚠️ %1 commande(s) retardée(s)!\n\n").arg(overdueCount);
    }

    if (tomorrowCount > 0) {
        notificationText += QString("📅 %1 livraison(s) demain!\n").arg(tomorrowCount);
    }

    if (nextWeekCount > 0) {
        notificationText += QString("📆 %1 livraison(s) cette semaine!\n").arg(nextWeekCount);
    }

    if (readyCount > 0) {
        notificationText += QString("\n✅ %1 commande(s) prête(s) à être livrée(s)!").arg(readyCount);
    }

    // Afficher une notification si nécessaire
    if (!notificationText.isEmpty() && (overdueCount > 0 || tomorrowCount > 0 || readyCount > 0)) {
        QMessageBox::warning(this, "🔔 Notifications de Livraison", notificationText);
    }
}

// --- FONCTIONNALITÉ D'ENVOI D'EMAIL ---

QString MainWindow::getEmailTemplate(const QString &templateName, const QMap<QString, QString> &data)
{
    QString template_html;

    if (templateName == "confirmation") {
        template_html = QString(
                            "<html><body style='font-family: Arial, sans-serif;'>"
                            "<div style='background-color: #f8f5f2; padding: 20px; border-radius: 10px;'>"
                            "<h2 style='color: #8B4513;'>✅ Confirmation de Commande</h2>"
                            "<p>Bonjour,</p>"
                            "<p>Votre commande <strong>#%1</strong> a été confirmée avec succès.</p>"
                            "<div style='background-color: white; padding: 15px; margin: 20px 0; border-left: 4px solid #8B4513;'>"
                            "<h3 style='margin-top: 0;'>Détails de la commande:</h3>"
                            "<ul>"
                            "<li><strong>Type de produit:</strong> %2</li>"
                            "<li><strong>Quantité:</strong> %3</li>"
                            "<li><strong>Prix total:</strong> %4 DT</li>"
                            "<li><strong>Date de commande:</strong> %5</li>"
                            "<li><strong>Date de livraison prévue:</strong> %6</li>"
                            "<li><strong>Statut:</strong> <span style='color: #4CAF50;'>%7</span></li>"
                            "</ul>"
                            "</div>"
                            "<p>Nous vous remercions pour votre confiance.</p>"
                            "<p style='color: #666; font-size: 12px; margin-top: 30px;'>"
                            "Ceci est un email automatique, merci de ne pas y répondre."
                            "</p>"
                            "</div>"
                            "</body></html>"
                            ).arg(data.value("id"), data.value("type"), data.value("qty"),
                                 data.value("price"), data.value("orderDate"), data.value("deliveryDate"),
                                 data.value("status"));
    }
    else if (templateName == "livraison") {
        template_html = QString(
                            "<html><body style='font-family: Arial, sans-serif;'>"
                            "<div style='background-color: #e8f5e9; padding: 20px; border-radius: 10px;'>"
                            "<h2 style='color: #2E7D32;'>🚚 Notification de Livraison</h2>"
                            "<p>Bonjour,</p>"
                            "<p>Votre commande <strong>#%1</strong> est prête pour la livraison!</p>"
                            "<div style='background-color: white; padding: 15px; margin: 20px 0; border-left: 4px solid #4CAF50;'>"
                            "<h3 style='margin-top: 0;'>Informations de livraison:</h3>"
                            "<ul>"
                            "<li><strong>Produit:</strong> %2</li>"
                            "<li><strong>Quantité:</strong> %3 unité(s)</li>"
                            "<li><strong>Date de livraison:</strong> %4</li>"
                            "<li><strong>Statut:</strong> <span style='color: #4CAF50; font-weight: bold;'>PRÊTE</span></li>"
                            "</ul>"
                            "</div>"
                            "<p style='background-color: #FFF9C4; padding: 10px; border-radius: 5px;'>"
                            "⏰ Veuillez vous assurer d'être disponible à la date prévue."
                            "</p>"
                            "<p>Cordialement,<br>L'équipe de gestion</p>"
                            "</div>"
                            "</body></html>"
                            ).arg(data.value("id"), data.value("type"), data.value("qty"), data.value("deliveryDate"));
    }
    else if (templateName == "retard") {
        template_html = QString(
                            "<html><body style='font-family: Arial, sans-serif;'>"
                            "<div style='background-color: #ffebee; padding: 20px; border-radius: 10px;'>"
                            "<h2 style='color: #c62828;'>⚠️ Notification de Retard</h2>"
                            "<p>Bonjour,</p>"
                            "<p>Nous vous informons que la commande <strong>#%1</strong> a subi un retard.</p>"
                            "<div style='background-color: white; padding: 15px; margin: 20px 0; border-left: 4px solid #f44336;'>"
                            "<h3 style='margin-top: 0;'>Détails:</h3>"
                            "<ul>"
                            "<li><strong>Produit:</strong> %2</li>"
                            "<li><strong>Date initialement prévue:</strong> %3</li>"
                            "<li><strong>Nouvelle date estimée:</strong> En cours de détermination</li>"
                            "</ul>"
                            "</div>"
                            "<p>Nous nous excusons pour ce désagrément et mettons tout en œuvre pour accélérer la livraison.</p>"
                            "<p>Cordialement,<br>L'équipe de gestion</p>"
                            "</div>"
                            "</body></html>"
                            ).arg(data.value("id"), data.value("type"), data.value("deliveryDate"));
    }
    else if (templateName == "attente") {
        template_html = QString(
                            "<html><body style='font-family: Arial, sans-serif;'>"
                            "<div style='background-color: #fff8e1; padding: 20px; border-radius: 10px;'>"
                            "<h2 style='color: #f57c00;'>⏳ Commande en Attente</h2>"
                            "<p>Bonjour,</p>"
                            "<p>Votre commande <strong>#%1</strong> est actuellement en attente de traitement.</p>"
                            "<div style='background-color: white; padding: 15px; margin: 20px 0; border-left: 4px solid #ff9800;'>"
                            "<h3 style='margin-top: 0;'>Détails de la commande:</h3>"
                            "<ul>"
                            "<li><strong>Produit:</strong> %2</li>"
                            "<li><strong>Quantité:</strong> %3 unité(s)</li>"
                            "<li><strong>Date de commande:</strong> %4</li>"
                            "<li><strong>Date de livraison prévue:</strong> %5</li>"
                            "<li><strong>Statut:</strong> <span style='color: #ff9800; font-weight: bold;'>EN ATTENTE</span></li>"
                            "</ul>"
                            "</div>"
                            "<p style='background-color: #e3f2fd; padding: 10px; border-radius: 5px;'>"
                            "ℹ️ Nous vous tiendrons informé(e) dès que votre commande sera traitée."
                            "</p>"
                            "<p>Merci pour votre patience.<br>Cordialement,<br>L'équipe de gestion</p>"
                            "</div>"
                            "</body></html>"
                            ).arg(data.value("id"), data.value("type"), data.value("qty"),
                                 data.value("orderDate"), data.value("deliveryDate"));
    }
    else if (templateName == "modification") {
        template_html = QString(
                            "<html><body style='font-family: Arial, sans-serif;'>"
                            "<div style='background-color: #e3f2fd; padding: 20px; border-radius: 10px;'>"
                            "<h2 style='color: #1976D2;'>🔄 Mise à Jour de Commande</h2>"
                            "<p>Bonjour,</p>"
                            "<p>Votre commande <strong>#%1</strong> a été modifiée.</p>"
                            "<div style='background-color: white; padding: 15px; margin: 20px 0; border-left: 4px solid #2196F3;'>"
                            "<h3 style='margin-top: 0;'>Détails actuels de la commande:</h3>"
                            "<ul>"
                            "<li><strong>Type de produit:</strong> %2</li>"
                            "<li><strong>Quantité:</strong> %3</li>"
                            "<li><strong>Prix total:</strong> %4 DT</li>"
                            "<li><strong>Date de commande:</strong> %5</li>"
                            "<li><strong>Date de livraison prévue:</strong> %6</li>"
                            "<li><strong>Statut:</strong> <span style='color: #1976D2;'>%7</span></li>"
                            "</ul>"
                            "</div>"
                            "%8"
                            "<p>Nous vous remercions pour votre confiance.</p>"
                            "<p style='color: #666; font-size: 12px; margin-top: 30px;'>"
                            "Ceci est un email automatique, merci de ne pas y répondre."
                            "</p>"
                            "</div>"
                            "</body></html>"
                            ).arg(data.value("id"), data.value("type"), data.value("qty"),
                                 data.value("price"), data.value("orderDate"), data.value("deliveryDate"),
                                 data.value("status"),
                                 data.contains("changes") && !data.value("changes").isEmpty()
                                     ? QString("<div style='background-color: #fff3e0; padding: 15px; margin: 20px 0; border-left: 4px solid #FF9800; border-radius: 5px;'>"
                                               "<h3 style='margin-top: 0; color: #E65100;'>📝 Modifications effectuées:</h3>"
                                               "<p style='font-size: 14px; line-height: 1.8;'>• %1</p>"
                                               "</div>").arg(data.value("changes"))
                                     : QString());
    }
    else {
        // Template par défaut
        template_html = QString(
                            "<html><body style='font-family: Arial, sans-serif;'>"
                            "<div style='background-color: #f5f5f5; padding: 20px; border-radius: 10px;'>"
                            "<h2 style='color: #8B4513;'>📧 Notification</h2>"
                            "<p>Bonjour,</p>"
                            "<p>Ceci est une notification concernant votre commande <strong>#%1</strong>.</p>"
                            "<div style='background-color: white; padding: 15px; margin: 20px 0;'>"
                            "<p><strong>Type:</strong> %2</p>"
                            "<p><strong>Quantité:</strong> %3</p>"
                            "<p><strong>Statut:</strong> %4</p>"
                            "</div>"
                            "<p>Cordialement,<br>L'équipe de gestion</p>"
                            "</div>"
                            "</body></html>"
                            ).arg(data.value("id"), data.value("type"), data.value("qty"), data.value("status"));
    }

    return template_html;
}

// Fonction désactivée - Envoi automatique uniquement depuis la modification de statut
/*
void MainWindow::saveEmailHistory()
{
    QFile file(QDir::homePath() + "/emails_history.csv");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Date,To,Subject,Body,Type,OrderID\n";
        for (const auto &email : emailHistory) {
            out << email.dateTime.toString("yyyy-MM-dd HH:mm:ss") << ","
                << email.to << ","
                << email.subject << ","
                << email.body.replace("\n", " ") << ","
                << email.type << ","
                << email.orderID << "\n";
        }
        file.close();
    }
}

        qDebug() << "Ligne" << row << "- Email:" << emailClient << "| Match:" << (emailClient.toLower() == email.toLower());

        if (emailClient.toLower() == email.toLower()) {
            QString id = ui->table_list->item(row, 0)->text();
            commandesClient.append(id);

            QMap<QString, QString> details;
            details["id"] = id;
            details["type"] = ui->table_list->item(row, 1)->text();
            details["qty"] = ui->table_list->item(row, 2)->text();
            details["email"] = emailClient;
            details["orderDate"] = ui->table_list->item(row, 5)->text();
            details["deliveryDate"] = ui->table_list->item(row, 6)->text();
            details["price"] = ui->table_list->item(row, 7)->text();
            details["status"] = ui->table_list->item(row, 9)->text();

            detailsCommandes.append(details);
        }
    }

    qDebug() << "Emails disponibles dans la base:" << allEmails;
    qDebug() << "Commandes trouvées:" << commandesClient.size();

    if (commandesClient.isEmpty()) {
        QString errorMsg = QString(
            "Aucune commande trouvée pour l'email:\n%1\n\n"
            "📊 Statistiques:\n"
            "• Total de commandes: %2\n"
            "• Clients différents: %3\n\n"
        ).arg(email).arg(ui->table_list->rowCount()).arg(allEmails.size());

        if (!allEmails.isEmpty()) {
            errorMsg += "💡 Emails disponibles:\n";
            int count = 0;
            for (const QString &availEmail : allEmails) {
                if (count < 5) {  // Afficher max 5 emails
                    errorMsg += QString("   • %1\n").arg(availEmail);
                    count++;
                }
            }
            if (allEmails.size() > 5) {
                errorMsg += QString("   ... et %1 autre(s)\n").arg(allEmails.size() - 5);
            }
        } else {
            errorMsg += "⚠️ Aucune commande avec email dans la base.\n"
                       "Vérifiez que les commandes sont bien chargées.";
        }

        QMessageBox::warning(this, "❌ Aucune commande trouvée", errorMsg);
        return;
    }

    // Proposer le type d'email à envoyer
    QStringList emailTypes;
    emailTypes << "Confirmation de commande" << "Notification de livraison" << "Alerte de retard" << "Email personnalisé";

    bool ok;
    QString typeEmail = QInputDialog::getItem(this, "Type d'email",
        QString("Client: %1\n%2 commande(s) trouvée(s)\n\nChoisir le type d'email:")
            .arg(email).arg(commandesClient.size()),
        emailTypes, 0, false, &ok);

    if (!ok) return;

    // Sélectionner la commande si plusieurs
    QString selectedOrder;
    QMap<QString, QString> orderData;

    if (commandesClient.size() == 1) {
        selectedOrder = commandesClient.first();
        orderData = detailsCommandes.first();
    } else {
        selectedOrder = QInputDialog::getItem(this, "Sélectionner une commande",
            "Choisir la commande:", commandesClient, 0, false, &ok);
        if (!ok) return;

        for (const auto &details : detailsCommandes) {
            if (details.value("id") == selectedOrder) {
                orderData = details;
                break;
            }
        }
    }

    // Générer le contenu de l'email
    QString subject;
    QString body;

    if (typeEmail == "Confirmation de commande") {
        subject = QString("Confirmation de votre commande #%1").arg(selectedOrder);
        body = getEmailTemplate("confirmation", orderData);
    }
    else if (typeEmail == "Notification de livraison") {
        subject = QString("Votre commande #%1 est prête!").arg(selectedOrder);
        body = getEmailTemplate("livraison", orderData);
    }
    else if (typeEmail == "Alerte de retard") {
        subject = QString("Retard sur votre commande #%1").arg(selectedOrder);
        body = getEmailTemplate("retard", orderData);
    }
    else {
        subject = QInputDialog::getText(this, "Sujet de l'email", "Entrez le sujet:");
        if (subject.isEmpty()) return;

        QInputDialog dialog(this);
        dialog.setWindowTitle("Corps de l'email");
        dialog.setLabelText("Entrez le message:");
        dialog.setTextValue(QString(
            "Bonjour,\n\n"
            "Concernant votre commande #%1:\n"
            "Type: %2\n"
            "Quantité: %3\n"
            "Statut: %4\n\n"
            "Cordialement,\n"
            "L'équipe de gestion"
        ).arg(orderData["id"], orderData["type"], orderData["qty"], orderData["status"]));
        dialog.setOption(QInputDialog::UseListViewForComboBoxItems);

        if (dialog.exec() == QDialog::Accepted) {
            body = dialog.textValue();
        } else {
            return;
        }
    }

    // Envoyer l'email
    bool sent = sendEmail(email, subject, body);

    if (sent) {
        // Ajouter à l'historique
        addToEmailHistory(email, subject, body, typeEmail, selectedOrder);

        QMessageBox::information(this, "✅ Email envoyé",
            QString("Email envoyé avec succès à: %1\n\n"
                    "Sujet: %2\n"
                    "Commande: #%3\n"
                    "Type: %4").arg(email, subject, selectedOrder, typeEmail));
        ui->le_email_simple->clear();

        // Log dans la console
        qDebug() << "EMAIL ENVOYÉ:";
        qDebug() << "  Destinataire:" << email;
        qDebug() << "  Sujet:" << subject;
        qDebug() << "  Commande:" << selectedOrder;

        // Rafraîchir l'affichage de l'historique si visible
        if (table_email_history) {
            displayEmailHistory();
        }
    }
}
*/

bool MainWindow::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    if (!kSmtpEnabled) {
        qDebug() << "SMTP désactivé temporairement: email non envoyé à" << to;
        return true;
    }

    qDebug() << "=== ENVOI AUTOMATIQUE D'EMAIL ===";
    qDebug() << "À:" << to;
    qDebug() << "Sujet:" << subject;

    // Envoi automatique via SMTP
    return sendEmailSMTP(to, subject, body);
}

bool MainWindow::sendEmailSMTP(const QString &to, const QString &subject, const QString &body)
{
    qDebug() << "========================================";
    qDebug() << "=== ENVOI SMTP AUTOMATIQUE ===";
    qDebug() << "========================================";
    qDebug() << "Destinataire:" << to;
    qDebug() << "Sujet:" << subject;
    qDebug() << "Serveur SMTP:" << smtpServer << ":" << smtpPort;
    qDebug() << "Email expéditeur:" << smtpUsername;
    qDebug() << "Mot de passe configuré:" << (!smtpPassword.isEmpty() ? "OUI" : "NON");
    qDebug() << "========================================";

    // Vérifier si les identifiants SMTP sont configurés
    if (smtpUsername.isEmpty() || smtpPassword.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("⚙️ Configuration SMTP Gmail");
        msgBox.setText("Pour envoyer des emails automatiquement, vous devez configurer votre compte Gmail.\n\n"
                       "📌 IMPORTANT:\n"
                       "• Utilisez un 'Mot de passe d'application' Gmail\n"
                       "• Allez sur: myaccount.google.com → Sécurité → Validation en 2 étapes → Mots de passe d'application\n"
                       "• Créez un mot de passe pour 'Mail' et copiez-le\n\n"
                       "Voulez-vous configurer maintenant?");
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        if (msgBox.exec() == QMessageBox::No) {
            return false;
        }

        // Demander l'email
        bool ok;
        smtpUsername = QInputDialog::getText(this, "📧 Email Gmail",
                                             "Entrez votre adresse Gmail:\n(ex: votre.nom@gmail.com)",
                                             QLineEdit::Normal, "", &ok);
        if (!ok || smtpUsername.isEmpty()) return false;
        smtpUsername = smtpUsername.trimmed();

        // Demander le mot de passe d'application
        smtpPassword = QInputDialog::getText(this, "🔑 Mot de passe d'application",
                                             "Entrez votre mot de passe d'application Gmail:\n"
                                             "(16 caractères sans espaces)",
                                             QLineEdit::Password, "", &ok);
        if (!ok || smtpPassword.isEmpty()) return false;
        smtpPassword = smtpPassword.trimmed();
        smtpPassword.remove(' ');
    }

    // Sécuriser les identifiants (éviter les erreurs liées aux espaces)
    smtpUsername = smtpUsername.trimmed();
    smtpPassword = smtpPassword.trimmed();
    smtpPassword.remove(' ');

    // Créer le client SMTP
    SmtpClient *smtp = new SmtpClient(this);

    qDebug() << "========================================";
    qDebug() << "🚀 Création du client SMTP...";
    qDebug() << "Serveur:" << smtpServer;
    qDebug() << "Port:" << smtpPort;
    qDebug() << "De:" << smtpUsername;
    qDebug() << "À:" << to;
    qDebug() << "========================================";

    // Créer la boîte d'attente AVANT de connecter le signal
    QMessageBox *waitBox = new QMessageBox(this);
    waitBox->setWindowTitle("📤 Envoi en cours...");
    waitBox->setText(QString("Envoi de l'email à %1...\n\n"
                             "⏳ Connexion au serveur SMTP...\n"
                             "Veuillez patienter...").arg(to));
    waitBox->setStandardButtons(QMessageBox::NoButton);
    waitBox->setIcon(QMessageBox::Information);

    // Connecter le signal de résultat
    connect(smtp, &SmtpClient::emailSent, this, [this, smtp, waitBox](bool success, const QString &message) {
        qDebug() << "========================================";
        qDebug() << "📨 CALLBACK emailSent reçu!";
        qDebug() << "Succès:" << success;
        qDebug() << "Message:" << message;
        qDebug() << "========================================";

        // Fermer la boîte d'attente
        if (waitBox) {
            waitBox->close();
            waitBox->deleteLater();
        }

        // Afficher le résultat
        if (success) {
            QMessageBox::information(this, "✅ Email envoyé!",
                                     QString("Email envoyé avec succès!\n\n"
                                             "Destinataire: %1\n\n"
                                             "Vérifiez votre boîte Gmail dans quelques instants.").arg(
                                             waitBox ? waitBox->text().split("à ").last().split("...").first() : ""));
        } else {
            QMessageBox::critical(this, "❌ Erreur d'envoi",
                                  QString("Impossible d'envoyer l'email.\n\n"
                                          "Erreur: %1\n\n"
                                          "Vérifiez:\n"
                                          "• Votre connexion Internet\n"
                                          "• Le mot de passe d'application Gmail\n"
                                          "• Les logs dans la console").arg(message));
        }

        smtp->deleteLater();
    });

    // Envoyer l'email
    qDebug() << "========================================";
    qDebug() << "🚀 LANCEMENT DE L'ENVOI SMTP...";
    qDebug() << "========================================";
    smtp->sendEmail(smtpUsername, to, subject, body, smtpServer, smtpPort, smtpUsername, smtpPassword);

    // Afficher la boîte d'attente (elle sera fermée par le callback)
    waitBox->show();

    // Timeout de sécurité - fermer après 10 secondes si pas de réponse
    QTimer::singleShot(10000, this, [waitBox, smtp]() {
        if (waitBox && waitBox->isVisible()) {
            qDebug() << "⚠️ TIMEOUT: Aucune réponse après 10 secondes";
            waitBox->close();

            QString logPath = QDir::currentPath() + "/smtp_debug.log";
            QMessageBox::critical(nullptr, "❌ Échec d'envoi",
                                  QString("La connexion SMTP a échoué.\n\n"
                                          "Vérifications nécessaires:\n"
                                          "• Connexion Internet active\n"
                                          "• Port 587 non bloqué par le firewall\n"
                                          "• Mot de passe d'application Gmail valide\n\n"
                                          "Fichier de log: %1").arg(logPath));

            smtp->deleteLater();
        }
    });

    return true;
}

// --- HISTORIQUE DES EMAILS ---

void MainWindow::addToEmailHistory(const QString &to, const QString &subject, const QString &body, const QString &type, const QString &orderID)
{
    EmailHistory entry;
    entry.dateTime = QDateTime::currentDateTime();
    entry.to = to;
    entry.subject = subject;
    entry.body = body;
    entry.type = type;
    entry.orderID = orderID;

    emailHistory.prepend(entry); // Ajouter au début (plus récent en premier)

    // Limiter à 100 emails maximum
    if (emailHistory.size() > 100) {
        emailHistory.removeLast();
    }

    saveEmailHistory();
    qDebug() << "Email ajouté à l'historique:" << to << subject;
}

void MainWindow::loadEmailHistory()
{
    QString historyFile = "emails_history.csv";
    QFile file(historyFile);

    emailHistory.clear();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Aucun historique d'emails trouvé, création d'un nouveau.";
        return;
    }

    QTextStream in(&file);
    bool firstLine = true;

    while (!in.atEnd()) {
        QString line = in.readLine();

        // Ignorer la ligne d'en-tête
        if (firstLine) {
            firstLine = false;
            continue;
        }

        // Parser le CSV (format: DateTime,To,Subject,Type,OrderID,Body)
        QStringList parts = line.split("|||"); // Séparateur spécial pour éviter problèmes avec virgules

        if (parts.size() >= 6) {
            EmailHistory entry;
            entry.dateTime = QDateTime::fromString(parts[0], "yyyy-MM-dd hh:mm:ss");
            entry.to = parts[1];
            entry.subject = parts[2];
            entry.type = parts[3];
            entry.orderID = parts[4];
            entry.body = parts[5];

            emailHistory.append(entry);
        }
    }

    file.close();
    qDebug() << "Historique chargé:" << emailHistory.size() << "emails";
}

void MainWindow::saveEmailHistory()
{
    QString historyFile = "emails_history.csv";
    QFile file(historyFile);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Impossible de sauvegarder l'historique";
        return;
    }

    QTextStream out(&file);

    // En-tête
    out << "DateTime|||To|||Subject|||Type|||OrderID|||Body\n";

    // Données
    for (const EmailHistory &entry : std::as_const(emailHistory)) {
        out << entry.dateTime.toString("yyyy-MM-dd hh:mm:ss") << "|||"
            << entry.to << "|||"
            << entry.subject << "|||"
            << entry.type << "|||"
            << entry.orderID << "|||"
            << entry.body << "\n";
    }

    file.close();
    qDebug() << "Historique sauvegardé:" << emailHistory.size() << "emails";
}

void MainWindow::displayEmailHistory()
{
    // Créer ou réinitialiser la table
    if (!table_email_history) {
        table_email_history = new QTableWidget(this);

        // Chercher le conteneur de l'historique dans l'UI
        // (On va l'ajouter dynamiquement à la page email)
        QWidget *emailPage = ui->page_email_auto;

        // Créer un groupe pour l'historique
        QGroupBox *historyGroup = new QGroupBox("📜 Historique des Emails", emailPage);
        historyGroup->setObjectName("email_history_group");
        historyGroup->setStyleSheet(
            "QGroupBox { "
            "   font-size: 18px; "
            "   font-weight: bold; "
            "   color: #8B4513; "
            "   padding: 20px; "
            "   margin-top: 20px; "
            "   border: 3px solid #8B4513; "
            "   border-radius: 10px; "
            "   background-color: #FFF8DC; "
            "}"
            "QGroupBox::title { "
            "   subcontrol-origin: margin; "
            "   subcontrol-position: top left; "
            "   padding: 5px 15px; "
            "   background-color: #8B4513; "
            "   color: white; "
            "   border-radius: 5px; "
            "}"
            );

        QVBoxLayout *historyLayout = new QVBoxLayout(historyGroup);

        // === BARRE DE RECHERCHE ET FILTRAGE ===
        QHBoxLayout *searchLayout = new QHBoxLayout();

        // Champ de recherche
        QLabel *searchLabel = new QLabel("🔍 Rechercher:", historyGroup);
        searchLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #8B4513;");
        searchLayout->addWidget(searchLabel);

        search_email_history = new QLineEdit(historyGroup);
        search_email_history->setPlaceholderText("Chercher par destinataire, sujet, commande...");
        search_email_history->setStyleSheet(
            "QLineEdit { "
            "   padding: 10px; "
            "   border: 2px solid #C68E65; "
            "   border-radius: 5px; "
            "   font-size: 14px; "
            "   background-color: white; "
            "}"
            "QLineEdit:focus { border-color: #8B4513; }"
            );
        connect(search_email_history, &QLineEdit::textChanged, this, &MainWindow::filterEmailHistory);
        searchLayout->addWidget(search_email_history, 2);

        // Filtre par type
        QLabel *filterLabel = new QLabel("📂 Type:", historyGroup);
        filterLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #8B4513; margin-left: 15px;");
        searchLayout->addWidget(filterLabel);

        filter_email_type = new QComboBox(historyGroup);
        filter_email_type->addItem("Tous les types");
        filter_email_type->addItem("Confirmation de commande");
        filter_email_type->addItem("Notification de livraison");
        filter_email_type->addItem("Alerte de retard");
        filter_email_type->addItem("Notification d'attente");
        filter_email_type->addItem("Mise à jour de commande");
        filter_email_type->addItem("Modification de statut");
        filter_email_type->setStyleSheet(
            "QComboBox { "
            "   padding: 8px; "
            "   border: 2px solid #C68E65; "
            "   border-radius: 5px; "
            "   font-size: 14px; "
            "   background-color: white; "
            "   min-width: 200px; "
            "}"
            "QComboBox:hover { border-color: #8B4513; }"
            "QComboBox::drop-down { "
            "   border: none; "
            "   width: 30px; "
            "}"
            );
        connect(filter_email_type, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &MainWindow::filterEmailHistory);
        searchLayout->addWidget(filter_email_type, 1);

        // Bouton pour effacer l'historique (placé dans la barre d'actions)
        QPushButton *btnClearHistory = new QPushButton("🗑️ Effacer l'historique", historyGroup);
        btnClearHistory->setMinimumHeight(42);
        btnClearHistory->setMinimumWidth(220);
        btnClearHistory->setStyleSheet(
            "QPushButton { "
            "   background-color: #dc3545; "
            "   color: white; "
            "   padding: 10px 14px; "
            "   border-radius: 8px; "
            "   font-weight: bold; "
            "   font-size: 14px; "
            "}"
            "QPushButton:hover { "
            "   background-color: #c82333; "
            "}"
            );
        connect(btnClearHistory, &QPushButton::clicked, this, &MainWindow::clearEmailHistory);
        searchLayout->addWidget(btnClearHistory, 0, Qt::AlignRight);

        historyLayout->addLayout(searchLayout);

        // Séparateur
        QFrame *separator = new QFrame(historyGroup);
        separator->setFrameShape(QFrame::HLine);
        separator->setStyleSheet("background-color: #8B4513; margin: 10px 0;");
        historyLayout->addWidget(separator);

        // Configuration de la table
        table_email_history->setColumnCount(6);
        table_email_history->setHorizontalHeaderLabels({"📅 Date", "📧 Destinataire", "📝 Sujet", "📂 Type", "🔖 Commande", "Actions"});
        table_email_history->setSelectionBehavior(QAbstractItemView::SelectRows);
        table_email_history->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table_email_history->setAlternatingRowColors(true);
        table_email_history->setMinimumHeight(260);
        table_email_history->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        table_email_history->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        table_email_history->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        table_email_history->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        table_email_history->setStyleSheet(
            "QTableWidget { background-color: white; border: 2px solid #C68E65; border-radius: 8px; font-size: 13px; }"
            "QHeaderView::section { background-color: #8B4513; color: white; font-weight: bold; padding: 10px; font-size: 14px; }"
            "QTableWidget::item { padding: 8px; }"
            "QTableWidget::item:selected { background-color: #FFE4B5; }"
            );

        // Définir les largeurs de colonnes optimales
        table_email_history->setColumnWidth(0, 140);  // Date
        table_email_history->setColumnWidth(1, 250);  // Destinataire
        table_email_history->setColumnWidth(2, 300);  // Sujet
        table_email_history->setColumnWidth(3, 180);  // Type
        table_email_history->setColumnWidth(4, 100);  // Commande
        table_email_history->setColumnWidth(5, 120);  // Actions

        // Permettre le redimensionnement manuel
        table_email_history->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        table_email_history->horizontalHeader()->setStretchLastSection(false);

        // Activer le tri par clic sur les en-têtes
        table_email_history->setSortingEnabled(false); // On gère le tri manuellement
        table_email_history->horizontalHeader()->setSectionsClickable(true);
        connect(table_email_history->horizontalHeader(), &QHeaderView::sectionClicked,
                this, &MainWindow::sortEmailHistory);

        historyLayout->setSpacing(12);
        historyLayout->addWidget(table_email_history, 1);

        // Ajouter le groupe à la page email
        QVBoxLayout *pageLayout = qobject_cast<QVBoxLayout*>(emailPage->layout());
        if (pageLayout) {
            // L'insérer avant les spacers existants pour qu'il reste visible
            pageLayout->insertWidget(2, historyGroup, 1);
        }
    }

    // Remplir la table avec l'historique (utiliser la fonction de tri)
    // Cela appliquera automatiquement le tri actuel (par défaut: date décroissante)
    sortEmailHistory(currentSortColumn);
}

void MainWindow::resendEmailFromHistory()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int index = btn->property("historyIndex").toInt();
    if (index < 0 || index >= emailHistory.size()) return;

    const EmailHistory &entry = emailHistory[index];

    // Demander confirmation
    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "🔄 Renvoyer l'email?",
                                                              QString("Êtes-vous sûr de vouloir renvoyer cet email?\n\n"
                                                                      "📧 À: %1\n"
                                                                      "📝 Sujet: %2\n"
                                                                      "📅 Envoyé: %3")
                                                                  .arg(entry.to, entry.subject, entry.dateTime.toString("dd/MM/yyyy hh:mm")),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Renvoyer l'email
        bool sent = sendEmail(entry.to, entry.subject, entry.body);

        if (sent) {
            // Ajouter à l'historique avec marqueur de renvoi
            addToEmailHistory(entry.to, entry.subject + " [Renvoi]", entry.body, entry.type + " (Renvoi)", entry.orderID);
            displayEmailHistory(); // Rafraîchir l'affichage
        }
    }
}

void MainWindow::clearEmailHistory()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this,
                                                             "⚠️ Confirmation",
                                                             "Êtes-vous sûr de vouloir effacer tout l'historique des emails?\n\n"
                                                             "Cette action est irréversible!",
                                                             QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        emailHistory.clear();
        saveEmailHistory();
        displayEmailHistory();

        QMessageBox::information(this, "✅ Historique effacé",
                                 "L'historique des emails a été complètement effacé.");
    }
}

void MainWindow::filterEmailHistory()
{
    if (!table_email_history || !search_email_history || !filter_email_type) return;

    QString searchText = search_email_history->text().toLower();
    QString selectedType = filter_email_type->currentText();

    // Parcourir toutes les lignes et afficher/masquer selon les critères
    for (int row = 0; row < table_email_history->rowCount(); ++row) {
        bool matchSearch = true;
        bool matchType = true;

        // Vérifier la recherche textuelle
        if (!searchText.isEmpty()) {
            matchSearch = false;

            // Chercher dans toutes les colonnes textuelles
            for (int col = 0; col < 5; ++col) { // Colonnes 0-4 (pas la colonne Actions)
                QTableWidgetItem *item = table_email_history->item(row, col);
                if (item && item->text().toLower().contains(searchText)) {
                    matchSearch = true;
                    break;
                }
            }
        }

        // Vérifier le filtre de type
        if (selectedType != "Tous les types") {
            QTableWidgetItem *typeItem = table_email_history->item(row, 3); // Colonne Type
            if (typeItem) {
                QString rowType = typeItem->text();
                // Vérifier si le type correspond (peut contenir "(Renvoi)" donc on utilise contains)
                matchType = rowType.contains(selectedType, Qt::CaseInsensitive);
            }
        }

        // Afficher/masquer la ligne selon les critères
        table_email_history->setRowHidden(row, !(matchSearch && matchType));
    }

    // Afficher le nombre de résultats
    int visibleCount = 0;
    for (int row = 0; row < table_email_history->rowCount(); ++row) {
        if (!table_email_history->isRowHidden(row)) {
            visibleCount++;
        }
    }

    qDebug() << "Filtrage: " << visibleCount << "/" << table_email_history->rowCount()
             << "emails affichés (recherche:" << searchText << ", type:" << selectedType << ")";
}


void MainWindow::sortEmailHistory(int column)
{
    if (!table_email_history || column == 5) return; // Pas de tri sur la colonne Actions

    // Inverser l'ordre de tri si on clique sur la même colonne
    if (column == currentSortColumn) {
        currentSortOrder = (currentSortOrder == Qt::AscendingOrder)
        ? Qt::DescendingOrder
        : Qt::AscendingOrder;
    } else {
        currentSortColumn = column;
        currentSortOrder = Qt::AscendingOrder;
    }

    // Créer une liste temporaire avec les données et indices
    struct RowData {
        int originalIndex;
        QString sortKey;
        QDateTime dateTime;
    };

    QList<RowData> rows;
    rows.reserve(emailHistory.size());

    for (int i = 0; i < emailHistory.size(); ++i) {
        const EmailHistory &entry = emailHistory[i];
        RowData rowData;
        rowData.originalIndex = i;
        rowData.dateTime = entry.dateTime;

        // Déterminer la clé de tri selon la colonne
        switch (column) {
        case 0: // Date
            rowData.sortKey = entry.dateTime.toString("yyyyMMddhhmmss");
            break;
        case 1: // Destinataire
            rowData.sortKey = entry.to.toLower();
            break;
        case 2: // Sujet
            rowData.sortKey = entry.subject.toLower();
            break;
        case 3: // Type
            rowData.sortKey = entry.type.toLower();
            break;
        case 4: // Commande
            rowData.sortKey = entry.orderID.toLower();
            break;
        default:
            rowData.sortKey = "";
        }

        rows.append(rowData);
    }

    // Trier les lignes
    if (currentSortOrder == Qt::AscendingOrder) {
        std::sort(rows.begin(), rows.end(), [](const RowData &a, const RowData &b) {
            return a.sortKey < b.sortKey;
        });
    } else {
        std::sort(rows.begin(), rows.end(), [](const RowData &a, const RowData &b) {
            return a.sortKey > b.sortKey;
        });
    }

    // Réorganiser la table selon l'ordre trié
    table_email_history->setRowCount(0);

    for (const RowData &rowData : rows) {
        int i = rowData.originalIndex;
        const EmailHistory &entry = emailHistory[i];

        int row = table_email_history->rowCount();
        table_email_history->insertRow(row);

        table_email_history->setItem(row, 0, new QTableWidgetItem(entry.dateTime.toString("dd/MM/yyyy hh:mm")));
        table_email_history->setItem(row, 1, new QTableWidgetItem(entry.to));
        table_email_history->setItem(row, 2, new QTableWidgetItem(entry.subject));
        table_email_history->setItem(row, 3, new QTableWidgetItem(entry.type));
        table_email_history->setItem(row, 4, new QTableWidgetItem(entry.orderID));

        // Bouton pour renvoyer
        QPushButton *btnResend = new QPushButton("🔄 Renvoyer");
        btnResend->setStyleSheet("background-color: #28a745; color: white; padding: 5px 10px; border-radius: 3px;");
        btnResend->setProperty("historyIndex", i);
        connect(btnResend, &QPushButton::clicked, this, &MainWindow::resendEmailFromHistory);

        table_email_history->setCellWidget(row, 5, btnResend);
    }

    // Réappliquer les filtres après le tri
    filterEmailHistory();

    // Afficher un indicateur de tri dans l'en-tête
    QString sortIndicator = (currentSortOrder == Qt::AscendingOrder) ? " ▲" : " ▼";
    QStringList headers = {"📅 Date", "📧 Destinataire", "📝 Sujet", "📂 Type", "🔖 Commande", "Actions"};
    headers[column] += sortIndicator;
    table_email_history->setHorizontalHeaderLabels(headers);

    qDebug() << "Tri appliqué: colonne" << column
             << (currentSortOrder == Qt::AscendingOrder ? "Ascendant" : "Descendant");
}

// === FONCTIONNALITÉS DE LA PAGE LISTE ===

void MainWindow::searchOrdersList()
{
    QString searchText = ui->le_search->text().toLower();

    // Parcourir toutes les lignes et afficher/masquer selon les critères
    for (int row = 0; row < ui->table_list->rowCount(); ++row) {
        bool match = false;

        if (searchText.isEmpty()) {
            match = true;
        } else {
            // Chercher dans toutes les colonnes
            for (int col = 0; col < ui->table_list->columnCount(); ++col) {
                QTableWidgetItem *item = ui->table_list->item(row, col);
                if (item && item->text().toLower().contains(searchText)) {
                    match = true;
                    break;
                }
            }
        }

        ui->table_list->setRowHidden(row, !match);
    }

    // Compter les résultats visibles
    int visibleCount = 0;
    for (int row = 0; row < ui->table_list->rowCount(); ++row) {
        if (!ui->table_list->isRowHidden(row)) {
            visibleCount++;
        }
    }

    qDebug() << "Recherche liste: " << visibleCount << "/" << ui->table_list->rowCount()
             << "commandes affichées (recherche:" << searchText << ")";
}

void MainWindow::sortOrdersList()
{
    QString sortType = ui->cb_sort->currentText();

    qDebug() << "Tri de la liste par:" << sortType;

    int column = 0;
    Qt::SortOrder order = Qt::AscendingOrder;

    if (sortType == "Trier par Date") {
        column = 5; // Date Commande
        order = Qt::DescendingOrder; // Plus récent en premier
    } else if (sortType == "Trier par Quantité") {
        column = 2; // Quantité
        order = Qt::DescendingOrder;
    } else if (sortType == "Trier par Prix (Croissant)") {
        column = 7; // Prix
        order = Qt::AscendingOrder;
    } else if (sortType == "Trier par Prix (Decroissant)") {
        column = 7; // Prix
        order = Qt::DescendingOrder;
    } else if (sortType == "Trier par Type") {
        column = 1; // Type
        order = Qt::AscendingOrder;
    } else if (sortType == "Trier par Statut") {
        column = 9; // Statut
        order = Qt::AscendingOrder;
    }

    ui->table_list->setSortingEnabled(true);
    ui->table_list->sortItems(column, order);
    searchOrdersList();
}

void MainWindow::exportListToPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter la liste en PDF",
                                                    QString("commandes_liste_%1.pdf").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
                                                    "PDF Files (*.pdf)");

    if (fileName.isEmpty()) return;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setPageOrientation(QPageLayout::Landscape); // Paysage pour tableau large
    pdfWriter.setPageMargins(QMarginsF(15, 15, 15, 15));

    QPainter painter(&pdfWriter);

    // Titre
    QFont titleFont = painter.font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(100, 100, "📋 LISTE DES COMMANDES");

    // Date d'export
    QFont normalFont = painter.font();
    normalFont.setPointSize(10);
    normalFont.setBold(false);
    painter.setFont(normalFont);
    painter.drawText(100, 200, QString("Date d'export: %1").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm")));

    // Compter les commandes visibles
    int visibleRows = 0;
    for (int row = 0; row < ui->table_list->rowCount(); ++row) {
        if (!ui->table_list->isRowHidden(row)) {
            visibleRows++;
        }
    }
    painter.drawText(100, 300, QString("Nombre de commandes: %1").arg(visibleRows));

    // Dessiner le tableau
    int yPos = 500;
    int xStart = 100;
    int rowHeight = 250;

    // Colonnes réduites pour tenir sur la page (on exclut Ville et cinEmploye)
    QStringList headers = {"ID", "Type", "Qté", "Email", "Date Cmd", "Date Livr", "Prix", "Statut"};
    QList<int> columnIndices = {0, 1, 2, 3, 5, 6, 7, 9};
    QList<int> columnWidths = {400, 800, 400, 1200, 800, 800, 600, 700}; // Largeurs en unités PDF

    // En-têtes
    QFont headerFont = painter.font();
    headerFont.setBold(true);
    headerFont.setPointSize(9);
    painter.setFont(headerFont);

    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(QBrush(QColor(139, 69, 19))); // Marron

    int xPos = xStart;
    for (int i = 0; i < headers.size(); ++i) {
        painter.fillRect(xPos, yPos, columnWidths[i], rowHeight, QBrush(QColor(139, 69, 19)));
        painter.setPen(Qt::white);
        painter.drawRect(xPos, yPos, columnWidths[i], rowHeight);
        painter.drawText(QRect(xPos + 20, yPos, columnWidths[i] - 40, rowHeight),
                         Qt::AlignVCenter | Qt::AlignLeft, headers[i]);
        xPos += columnWidths[i];
        painter.setPen(QPen(Qt::black, 2));
    }

    yPos += rowHeight;

    // Données
    painter.setFont(normalFont);
    normalFont.setPointSize(8);
    painter.setFont(normalFont);

    int rowCount = 0;
    const int maxRowsPerPage = 20;

    for (int row = 0; row < ui->table_list->rowCount(); ++row) {
        if (ui->table_list->isRowHidden(row)) continue;

        // Nouvelle page si nécessaire
        if (rowCount > 0 && rowCount % maxRowsPerPage == 0) {
            pdfWriter.newPage();
            yPos = 100;
        }

        xPos = xStart;

        // Alterner les couleurs de fond
        if (rowCount % 2 == 0) {
            painter.fillRect(xStart, yPos, 5700, rowHeight, QBrush(QColor(255, 248, 220))); // Beige clair
        }

        for (int i = 0; i < columnIndices.size(); ++i) {
            int col = columnIndices[i];
            QTableWidgetItem *item = ui->table_list->item(row, col);
            QString text = item ? item->text() : "";

            painter.setPen(Qt::black);
            painter.drawRect(xPos, yPos, columnWidths[i], rowHeight);

            // Coloration spéciale pour le statut
            if (col == 9 && item) {
                QString status = text;
                if (status == "Prête") painter.setPen(QColor(0, 128, 0)); // Vert
                else if (status == "En cours") painter.setPen(QColor(255, 140, 0)); // Orange
                else if (status == "Complétée") painter.setPen(QColor(0, 0, 255)); // Bleu
                else if (status == "En attente") painter.setPen(QColor(255, 0, 0)); // Rouge
            }

            painter.drawText(QRect(xPos + 20, yPos, columnWidths[i] - 40, rowHeight),
                             Qt::AlignVCenter | Qt::AlignLeft, text);

            xPos += columnWidths[i];
            painter.setPen(Qt::black);
        }

        yPos += rowHeight;
        rowCount++;
    }

    painter.end();

    QMessageBox::information(this, "✅ Export réussi",
                             QString("La liste des commandes a été exportée en PDF:\n%1\n\n"
                                     "Nombre de commandes exportées: %2").arg(fileName).arg(visibleRows));

    // Proposer d'ouvrir le fichier
    if (QMessageBox::question(this, "Ouvrir le PDF?",
                              "Voulez-vous ouvrir le fichier PDF maintenant?") == QMessageBox::Yes) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    }

    qDebug() << "Export PDF terminé:" << fileName << "(" << visibleRows << "commandes)";
}

// -------------------------------------------------------------
// Suppliers & Employees (import legacy UI + supplier CRUD only)
// -------------------------------------------------------------

void MainWindow::setupSupplierUI()
{
    if (!ui->verticalLayout_suppliers) return;

    // Clean host layout
    QLayoutItem *child;
    while ((child = ui->verticalLayout_suppliers->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }
    ui->verticalLayout_suppliers->setContentsMargins(12, 12, 12, 12);
    ui->verticalLayout_suppliers->setSpacing(10);

    // Top tab buttons
    auto *tabLayout = new QHBoxLayout();
    tabLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *btnTabAdd = new QPushButton("Ajouter un fournisseur", ui->page_suppliers);
    QPushButton *btnTabUpdate = new QPushButton("Modifier un fournisseur", ui->page_suppliers);
    QPushButton *btnTabDelete = new QPushButton("Supprimer un fournisseur", ui->page_suppliers);
    QPushButton *btnTabList = new QPushButton("Liste / Stats", ui->page_suppliers);
    supplierTabButtons.clear();
    supplierTabButtons << btnTabAdd << btnTabUpdate << btnTabDelete << btnTabList;
    applyBarStyle(supplierTabButtons, true);
    for (QPushButton *b : std::as_const(supplierTabButtons)) tabLayout->addWidget(b);
    ui->verticalLayout_suppliers->addLayout(tabLayout);

    // Stacked pages
    supplierStack = new QStackedWidget(ui->page_suppliers);
    ui->verticalLayout_suppliers->addWidget(supplierStack, 1);

    // Helpers
    auto makeLine = [&](const QString &ph, QWidget *parent){ auto *le=new QLineEdit(parent); le->setPlaceholderText(ph); return le;};

    // --- Add page ---
    QWidget *pageAdd = new QWidget(supplierStack);
    auto *formAdd = new QFormLayout(pageAdd);
    supNameInput = makeLine("Nom fournisseur", pageAdd);
    supTypeInput = makeLine("Type (cuir, textile...)", pageAdd);
    supAddressInput = makeLine("Adresse", pageAdd);
    supPhoneInput = makeLine("Téléphone", pageAdd);
    supEmailInput = makeLine("Email", pageAdd);
    supDelayInput = new QSpinBox(pageAdd); supDelayInput->setRange(0,365); supDelayInput->setSuffix(" j");
    supPriceInput = new QDoubleSpinBox(pageAdd); supPriceInput->setRange(0,10000000); supPriceInput->setDecimals(2); supPriceInput->setSuffix(" TND");
    supStatusInput = new QComboBox(pageAdd); supStatusInput->addItems({"Actif","Suspendu"});
    formAdd->addRow("Nom *", supNameInput);
    formAdd->addRow("Type *", supTypeInput);
    formAdd->addRow("Adresse *", supAddressInput);
    formAdd->addRow("Téléphone *", supPhoneInput);
    formAdd->addRow("Email *", supEmailInput);
    formAdd->addRow("Délai moyen", supDelayInput);
    formAdd->addRow("Prix moyen", supPriceInput);
    formAdd->addRow("Statut", supStatusInput);
    auto *btnAddSave = new QPushButton("Enregistrer", pageAdd);
    auto *btnAddReset = new QPushButton("Réinitialiser", pageAdd);
    auto *hAdd = new QHBoxLayout(); hAdd->addWidget(btnAddSave); hAdd->addWidget(btnAddReset); formAdd->addRow(hAdd);
    supplierStack->addWidget(pageAdd);

    // --- Update page ---
    QWidget *pageUpd = new QWidget(supplierStack);
    auto *vUpd = new QVBoxLayout(pageUpd);
    auto *searchUpd = new QHBoxLayout();
    supIdInput = makeLine("ID à charger", pageUpd);
    QPushButton *btnLoadUpd = new QPushButton("Charger", pageUpd);
    searchUpd->addWidget(supIdInput); searchUpd->addWidget(btnLoadUpd);
    vUpd->addLayout(searchUpd);
    auto *formUpd = new QFormLayout();
    supNameInput = supNameInput; // reused pointers
    QLineEdit *nameUpd = makeLine("Nom", pageUpd);
    QLineEdit *typeUpd = makeLine("Type", pageUpd);
    QLineEdit *addrUpd = makeLine("Adresse", pageUpd);
    QLineEdit *phoneUpd = makeLine("Téléphone", pageUpd);
    QLineEdit *emailUpd = makeLine("Email", pageUpd);
    QSpinBox *delayUpd = new QSpinBox(pageUpd); delayUpd->setRange(0,365); delayUpd->setSuffix(" j");
    QDoubleSpinBox *priceUpd = new QDoubleSpinBox(pageUpd); priceUpd->setRange(0,10000000); priceUpd->setDecimals(2); priceUpd->setSuffix(" TND");
    QComboBox *statusUpd = new QComboBox(pageUpd); statusUpd->addItems({"Actif","Suspendu"});
    formUpd->addRow("Nom", nameUpd);
    formUpd->addRow("Type", typeUpd);
    formUpd->addRow("Adresse", addrUpd);
    formUpd->addRow("Téléphone", phoneUpd);
    formUpd->addRow("Email", emailUpd);
    formUpd->addRow("Délai moyen", delayUpd);
    formUpd->addRow("Prix moyen", priceUpd);
    formUpd->addRow("Statut", statusUpd);
    QPushButton *btnUpd = new QPushButton("Mettre à jour", pageUpd);
    formUpd->addRow(btnUpd);
    vUpd->addLayout(formUpd);
    supplierStack->addWidget(pageUpd);

    // --- Delete page ---
    QWidget *pageDel = new QWidget(supplierStack);
    auto *vDel = new QVBoxLayout(pageDel);
    QLineEdit *idDel = makeLine("ID à supprimer", pageDel);
    QLabel *lblDelInfo = new QLabel("(Aucun fournisseur chargé)", pageDel);
    QPushButton *btnLoadDel = new QPushButton("Charger", pageDel);
    QPushButton *btnDel = new QPushButton("Supprimer", pageDel);
    auto *hDel = new QHBoxLayout(); hDel->addWidget(idDel); hDel->addWidget(btnLoadDel); hDel->addWidget(btnDel);
    vDel->addLayout(hDel); vDel->addWidget(lblDelInfo);
    supplierStack->addWidget(pageDel);

    // --- List / Stats page ---
    QWidget *pageList = new QWidget(supplierStack);
    auto *vList = new QVBoxLayout(pageList);
    auto *statsLayout = new QHBoxLayout();
    auto makeStatCard = [&](const QString &title, QLabel **valueLabel, const QString &color) {
        auto *card = new QFrame(pageList);
        card->setFrameShape(QFrame::StyledPanel);
        card->setStyleSheet(QString("background:%1; border-radius:10px; padding:12px; color:white;").arg(color));
        auto *v = new QVBoxLayout(card);
        auto *titleLbl = new QLabel(title, card);
        titleLbl->setObjectName(QString("supplierStatTitle_%1").arg(QString(title).replace(' ', '_')));
        titleLbl->setStyleSheet("font-weight:bold;");
        *valueLabel = new QLabel("0", card);
        (*valueLabel)->setObjectName(QString("supplierStatValue_%1").arg(QString(title).replace(' ', '_')));
        (*valueLabel)->setStyleSheet("font-size:20px; font-weight:bold;");
        v->addWidget(titleLbl);
        v->addWidget(*valueLabel);
        v->addStretch();
        statsLayout->addWidget(card);
    };
    makeStatCard("Total", &supStatTotal, "#8B4513");
    makeStatCard("Actifs", &supStatActive, "#3D362D");
    makeStatCard("Prix moyen", &supStatAvgPrice, "#C68E65");
    makeStatCard("Délai moyen", &supStatAvgDelay, "#4E2C23");
    vList->addLayout(statsLayout);

    suppliersTable = new QTableWidget(0, 9, pageList);
    suppliersTable->setHorizontalHeaderLabels({"ID","Nom","Type","Adresse","Téléphone","Email","Délai (j)","Prix (TND)","Statut"});
    suppliersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    suppliersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    suppliersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    suppliersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vList->addWidget(suppliersTable, 1);
    supplierStack->addWidget(pageList);

    // Tabs behavior
    auto setTab = [&](int idx){ for(int i=0;i<supplierTabButtons.size();++i) supplierTabButtons[i]->setChecked(i==idx); if(supplierStack) supplierStack->setCurrentIndex(idx); };
    connect(btnTabAdd, &QPushButton::clicked, this, [=](){ setTab(0); });
    connect(btnTabUpdate, &QPushButton::clicked, this, [=](){ setTab(1); });
    connect(btnTabDelete, &QPushButton::clicked, this, [=](){ setTab(2); });
    connect(btnTabList, &QPushButton::clicked, this, [=](){ setTab(3); refreshSupplierTable(); });
    setTab(0);

    // Actions
    connect(btnAddSave, &QPushButton::clicked, this, [=](){
        bool ok=false; Supplier s=supplierFromForm(&ok); if(!ok) return;
        if(addSupplier(s)){ clearSupplierForm(); refreshSupplierTable(); QMessageBox::information(this,"Succès","Fournisseur ajouté."); }
        else QMessageBox::warning(this,"Erreur","Ajout impossible.");
    });
    connect(btnAddReset, &QPushButton::clicked, this, [=](){ clearSupplierForm(); });

    connect(btnLoadUpd, &QPushButton::clicked, this, [=](){
        QString id = supIdInput ? supIdInput->text().trimmed() : QString();
        if(id.isEmpty()){ QMessageBox::warning(this,"Champ manquant","Entrer un ID."); return; }
        Supplier *s = findSupplier(id);
        if(!s){ QMessageBox::information(this,"Introuvable","Aucun fournisseur trouvé."); return; }
        nameUpd->setText(s->nom); typeUpd->setText(s->type); addrUpd->setText(s->adresse);
        phoneUpd->setText(s->telephone); emailUpd->setText(s->email); delayUpd->setValue(s->delai);
        priceUpd->setValue(s->prix); statusUpd->setCurrentText(s->statut);
        supplierLoadedId = s->id;
    });

    connect(btnUpd, &QPushButton::clicked, this, [=](){
        QString id = supIdInput ? supIdInput->text().trimmed() : QString();
        if(id.isEmpty()){ QMessageBox::warning(this,"Champ manquant","Entrer l'ID chargé."); return; }
        Supplier s; s.id=id; s.nom=nameUpd->text().trimmed(); s.type=typeUpd->text().trimmed();
        s.adresse=addrUpd->text().trimmed(); s.telephone=phoneUpd->text().trimmed(); s.email=emailUpd->text().trimmed();
        s.delai=delayUpd->value(); s.prix=priceUpd->value(); s.statut=statusUpd->currentText();
        if(s.nom.isEmpty()||s.type.isEmpty()||s.telephone.isEmpty()||s.email.isEmpty()){
            QMessageBox::warning(this,"Champs requis","Remplir nom/type/tel/email."); return; }
        if(updateSupplier(id,s)){ refreshSupplierTable(); QMessageBox::information(this,"Succès","Fournisseur mis à jour."); }
        else QMessageBox::warning(this,"Erreur","Mise à jour impossible.");
    });

    connect(btnLoadDel, &QPushButton::clicked, this, [=](){
        QString id = idDel->text().trimmed();
        if(id.isEmpty()){ QMessageBox::warning(this,"Champ manquant","Entrer un ID."); return; }
        Supplier *s = findSupplier(id);
        if(!s){ lblDelInfo->setText("(Aucun fournisseur)"); QMessageBox::information(this,"Introuvable","Aucun fournisseur trouvé."); return; }
        lblDelInfo->setText(QString("%1 - %2 (%3)").arg(s->id, s->nom, s->type));
        supplierLoadedId = s->id;
    });

    connect(btnDel, &QPushButton::clicked, this, [=](){
        QString id = idDel->text().trimmed();
        if(id.isEmpty()){ QMessageBox::warning(this,"Champ manquant","Entrer un ID."); return; }
        if(QMessageBox::question(this,"Confirmation","Supprimer ce fournisseur ?")!=QMessageBox::Yes) return;
        if(deleteSupplier(id)){ lblDelInfo->setText("(Aucun fournisseur)"); idDel->clear(); refreshSupplierTable(); }
        else QMessageBox::warning(this,"Erreur","Suppression impossible.");
    });

    connect(suppliersTable, &QTableWidget::itemSelectionChanged, this, [=]() {
        if (!suppliersTable || suppliersTable->currentRow() < 0) return;
        const QString id = suppliersTable->item(suppliersTable->currentRow(), 0)->text();
        supIdInput->setText(id);
    });
}

void MainWindow::setupEmployeeUI()
{
    if (!ui->verticalLayout_employees) return;

    // Clean layout
    QLayoutItem *child;
    while ((child = ui->verticalLayout_employees->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }
    ui->verticalLayout_employees->setContentsMargins(12, 12, 12, 12);
    ui->verticalLayout_employees->setSpacing(10);

    // Model + headers
    const QStringList empHeaders = {"CIN/ID","Nom","Poste","Adresse","Téléphone","Date Embauche","Salaire","Statut","Sexe"};
    employeeModel = new QStandardItemModel(0, empHeaders.size(), this);
    employeeModel->setHorizontalHeaderLabels(empHeaders);

    // Tabs
    auto *tabLayout = new QHBoxLayout();
    tabLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *btnTabAdd = new QPushButton("Ajouter", ui->page_employees);
    QPushButton *btnTabEdit = new QPushButton("Modifier", ui->page_employees);
    QPushButton *btnTabDelete = new QPushButton("Supprimer", ui->page_employees);
    QPushButton *btnTabList = new QPushButton("Liste / Stats", ui->page_employees);
    employeeTabButtons.clear();
    employeeTabButtons << btnTabAdd << btnTabEdit << btnTabDelete << btnTabList;
    applyBarStyle(employeeTabButtons, true);
    for (QPushButton *b : std::as_const(employeeTabButtons)) tabLayout->addWidget(b);
    ui->verticalLayout_employees->addLayout(tabLayout);

    employeeStack = new QStackedWidget(ui->page_employees);
    ui->verticalLayout_employees->addWidget(employeeStack,1);

    auto makeLine = [&](const QString &ph, QWidget *p){ auto *le=new QLineEdit(p); le->setPlaceholderText(ph); return le;};

    // Add page
    QWidget *pageAdd = new QWidget(employeeStack);
    auto *formAdd = new QFormLayout(pageAdd);
    empIdAdd = makeLine("CIN/ID", pageAdd);
    empNameAdd = makeLine("Nom", pageAdd);
    empAddrAdd = makeLine("Adresse", pageAdd);
    empEmailAdd = makeLine("Email (non stocké)", pageAdd);
    empPhoneAdd = makeLine("Téléphone", pageAdd);
    empHireDateAdd = new QDateEdit(QDate::currentDate(), pageAdd); empHireDateAdd->setCalendarPopup(true);
    empPosteAdd = new QComboBox(pageAdd); empPosteAdd->addItems({"Employé","Manager","Technicien","RH"});
    empSalaryAdd = new QDoubleSpinBox(pageAdd); empSalaryAdd->setRange(0,1000000); empSalaryAdd->setDecimals(2);
    empSexAdd = new QComboBox(pageAdd); empSexAdd->addItems({"Homme","Femme"});

    // --- Sélection photo ---
    auto *photoWidget = new QWidget(pageAdd);
    auto *photoLayout = new QHBoxLayout(photoWidget);
    photoLayout->setContentsMargins(0,0,0,0);
    photoLayout->setSpacing(10);

    auto *photoPreview = new QLabel(photoWidget);
    photoPreview->setFixedSize(80, 80);
    photoPreview->setAlignment(Qt::AlignCenter);
    photoPreview->setStyleSheet(
        "border:2px dashed #C68E65;border-radius:40px;"
        "background:#f7f3ee;color:#8c7b70;font-size:10px;");
    photoPreview->setText("Photo");

    auto *btnPhoto = new QPushButton("📷  Choisir une photo", photoWidget);
    btnPhoto->setFixedHeight(36);
    btnPhoto->setStyleSheet(
        "QPushButton{background:#f7f3ee;color:#7B3F00;border:1px solid #C68E65;"
        "border-radius:6px;font-size:12px;padding:0 12px;}"
        "QPushButton:hover{background:#ede5dc;}");
    auto *btnClearPhoto = new QPushButton("✕", photoWidget);
    btnClearPhoto->setFixedSize(36,36);
    btnClearPhoto->setStyleSheet(
        "QPushButton{background:#fff0f0;color:#c0392b;border:1px solid #e0c0c0;"
        "border-radius:6px;font-size:14px;}"
        "QPushButton:hover{background:#ffe0e0;}");
    btnClearPhoto->setToolTip("Supprimer la photo");

    photoLayout->addWidget(photoPreview);
    photoLayout->addWidget(btnPhoto);
    photoLayout->addWidget(btnClearPhoto);

    // Bouton enregistrer visage Face ID
    auto *btnFaceEnroll = new QPushButton("  🔍  Enregistrer visage Face ID", photoWidget);
    btnFaceEnroll->setFixedHeight(36);
    btnFaceEnroll->setStyleSheet(
        "QPushButton{background:#f7f3ee;color:#7B3F00;border:1px solid #C68E65;"
        "border-radius:6px;font-size:12px;padding:0 12px;}"
        "QPushButton:hover{background:#ede5dc;}");
    photoLayout->addWidget(btnFaceEnroll);
    photoLayout->addStretch();

    connect(btnFaceEnroll, &QPushButton::clicked, this, [=](){
        // Demander confirmation mot de passe
        bool ok = false;
        QString pwd = QInputDialog::getText(this, "Confirmation",
            "Mot de passe admin pour enregistrer le visage :",
            QLineEdit::Password, "", &ok);
        if (!ok || pwd != "admin123") {
            QMessageBox::warning(this, "Accès refusé", "Mot de passe incorrect.");
            return;
        }
        // Ouvrir le popup de capture
        FaceScanPopup *popup = new FaceScanPopup("admin", "enroll", this);
        if (popup->exec() == QDialog::Accepted) {
            btnFaceEnroll->setText("  ✓  Visage enregistré");
            btnFaceEnroll->setStyleSheet(
                "QPushButton{background:#e8f5e9;color:#2ea043;border:1px solid #2ea043;"
                "border-radius:6px;font-size:12px;padding:0 12px;}");
        }
        delete popup;
    });

    // Stocker le chemin de la photo sélectionnée
    QString *selectedPhotoPath = new QString();

    connect(btnPhoto, &QPushButton::clicked, this, [=](){
        QString path = QFileDialog::getOpenFileName(
            this, "Choisir une photo", "",
            "Images (*.png *.jpg *.jpeg *.bmp)");
        if (path.isEmpty()) return;
        *selectedPhotoPath = path;
        QPixmap pix(path);
        if (!pix.isNull()) {
            // Afficher en cercle
            QPixmap rounded(80, 80);
            rounded.fill(Qt::transparent);
            QPainter painter(&rounded);
            painter.setRenderHint(QPainter::Antialiasing);
            QPainterPath clipPath;
            clipPath.addEllipse(0, 0, 80, 80);
            painter.setClipPath(clipPath);
            painter.drawPixmap(0, 0, 80, 80,
                pix.scaled(80, 80, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
            photoPreview->setPixmap(rounded);
            photoPreview->setStyleSheet("border:2px solid #C68E65;border-radius:40px;");
        }
    });

    connect(btnClearPhoto, &QPushButton::clicked, this, [=](){
        *selectedPhotoPath = "";
        photoPreview->setPixmap(QPixmap());
        photoPreview->setText("Photo");
        photoPreview->setStyleSheet(
            "border:2px dashed #C68E65;border-radius:40px;"
            "background:#f7f3ee;color:#8c7b70;font-size:10px;");
    });

    formAdd->addRow("CIN/ID *", empIdAdd);
    formAdd->addRow("Nom *", empNameAdd);
    formAdd->addRow("Photo", photoWidget);
    formAdd->addRow("Adresse", empAddrAdd);
    formAdd->addRow("Email", empEmailAdd);
    formAdd->addRow("Téléphone", empPhoneAdd);
    formAdd->addRow("Date embauche", empHireDateAdd);
    formAdd->addRow("Poste", empPosteAdd);
    formAdd->addRow("Salaire", empSalaryAdd);
    formAdd->addRow("Sexe", empSexAdd);
    auto *btnAddEmp = new QPushButton("Enregistrer", pageAdd);
    auto *btnResetEmp = new QPushButton("Réinitialiser", pageAdd);
    auto *hAdd = new QHBoxLayout(); hAdd->addWidget(btnAddEmp); hAdd->addWidget(btnResetEmp); formAdd->addRow(hAdd);
    employeeStack->addWidget(pageAdd);

    // Edit page
    QWidget *pageEdit = new QWidget(employeeStack);
    auto *vEdit = new QVBoxLayout(pageEdit);
    auto *hSearch = new QHBoxLayout();
    empSearchEdit = makeLine("CIN à modifier", pageEdit);
    QPushButton *btnSearchEdit = new QPushButton("Charger", pageEdit);
    hSearch->addWidget(empSearchEdit); hSearch->addWidget(btnSearchEdit);
    vEdit->addLayout(hSearch);
    auto *formEdit = new QFormLayout();
    empEditName = makeLine("Nom", pageEdit);
    empEditPhone = makeLine("Téléphone", pageEdit);
    empEditSalary = new QDoubleSpinBox(pageEdit); empEditSalary->setRange(0,1000000); empEditSalary->setDecimals(2);
    formEdit->addRow("Nom", empEditName);
    formEdit->addRow("Téléphone", empEditPhone);
    formEdit->addRow("Salaire", empEditSalary);
    QPushButton *btnApplyEdit = new QPushButton("Mettre à jour", pageEdit);
    formEdit->addRow(btnApplyEdit);
    vEdit->addLayout(formEdit);
    employeeStack->addWidget(pageEdit);

    // Delete page
    QWidget *pageDel = new QWidget(employeeStack);
    auto *hDel = new QHBoxLayout(pageDel);
    empDeleteId = makeLine("CIN à supprimer", pageDel);
    QPushButton *btnDel = new QPushButton("Supprimer", pageDel);
    hDel->addWidget(empDeleteId); hDel->addWidget(btnDel);
    employeeStack->addWidget(pageDel);

    // List page
    QWidget *pageList = new QWidget(employeeStack);
    auto *vList = new QVBoxLayout(pageList);
    vList->setSpacing(8);

    // --- Barre recherche / tri ---
    auto *searchSortBar = new QHBoxLayout();
    auto *empSearchBar = new QLineEdit(pageList);
    empSearchBar->setPlaceholderText("🔍  Rechercher par nom, CIN, poste...");
    empSearchBar->setFixedHeight(36);
    empSearchBar->setStyleSheet(
        "QLineEdit{background:#fff;border:1px solid #C68E65;border-radius:6px;"
        "padding:0 10px;color:#3b2a20;font-size:13px;}"
        "QLineEdit:focus{border:2px solid #8B4513;}");

    auto *empSortCombo = new QComboBox(pageList);
    empSortCombo->setFixedHeight(36);
    empSortCombo->addItems({"Trier par...", "Nom ↑", "Nom ↓", "Salaire ↑", "Salaire ↓", "Poste ↑"});
    empSortCombo->setStyleSheet(
        "QComboBox{background:#fff;border:1px solid #C68E65;border-radius:6px;"
        "padding:0 10px;color:#3b2a20;font-size:13px;min-width:140px;}"
        "QComboBox QAbstractItemView{background:#fff;color:#3b2a20;border:1px solid #C68E65;}");

    searchSortBar->addWidget(empSearchBar, 1);
    searchSortBar->addWidget(empSortCombo);

    // Bouton Export PDF
    auto *btnEmpPdf = new QPushButton("📄  Export PDF", pageList);
    btnEmpPdf->setFixedHeight(36);
    btnEmpPdf->setStyleSheet(
        "QPushButton{background:#8B4513;color:white;border:none;border-radius:6px;"
        "font-size:12px;font-weight:600;padding:0 14px;}"
        "QPushButton:hover{background:#a05a22;}");
    searchSortBar->addWidget(btnEmpPdf);

    vList->addLayout(searchSortBar);

    employeesTable = new QTableWidget(0, empHeaders.size(), pageList);
    employeesTable->setHorizontalHeaderLabels(empHeaders);
    employeesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    employeesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    employeesTable->setAlternatingRowColors(true);
    employeesTable->setShowGrid(true);
    employeesTable->setWordWrap(false);
    employeesTable->setStyleSheet(
        "QHeaderView::section{background:#3b2a20;color:white;font-weight:bold;padding:6px;border:1px solid #C68E65;}"
        "QTableWidget{gridline-color:#C68E65;alternate-background-color:#fdf7f2;background:#ffffff;color:#1a1a1a;selection-background-color:#f1c8a8;selection-color:#1a1a1a;}"
        );
    QPalette employeePalette = employeesTable->palette();
    employeePalette.setColor(QPalette::Base, QColor(255, 255, 255));
    employeePalette.setColor(QPalette::AlternateBase, QColor(253, 247, 242));
    employeePalette.setColor(QPalette::Text, QColor(26, 26, 26));
    employeePalette.setColor(QPalette::HighlightedText, QColor(26, 26, 26));
    employeesTable->setPalette(employeePalette);
    vList->addWidget(employeesTable);

    // Recherche en temps réel
    connect(empSearchBar, &QLineEdit::textChanged, this, [=](const QString &text){
        const QString filter = text.trimmed().toLower();
        for (int row = 0; row < employeesTable->rowCount(); ++row) {
            bool match = filter.isEmpty();
            if (!match) {
                for (int col = 0; col < employeesTable->columnCount(); ++col) {
                    auto *item = employeesTable->item(row, col);
                    if (item && item->text().toLower().contains(filter)) { match = true; break; }
                }
            }
            employeesTable->setRowHidden(row, !match);
        }
    });

    // Tri
    connect(empSortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int idx){
        if (idx == 0) return;
        int col = 1; Qt::SortOrder order = Qt::AscendingOrder;
        if      (idx == 1) { col = 1; order = Qt::AscendingOrder; }
        else if (idx == 2) { col = 1; order = Qt::DescendingOrder; }
        else if (idx == 3) { col = 6; order = Qt::AscendingOrder; }
        else if (idx == 4) { col = 6; order = Qt::DescendingOrder; }
        else if (idx == 5) { col = 2; order = Qt::AscendingOrder; }
        employeesTable->sortItems(col, order);
    });

    // Export PDF
    connect(btnEmpPdf, &QPushButton::clicked, this, [=](){
        QString fileName = QFileDialog::getSaveFileName(
            this, "Exporter les employés en PDF", "employes.pdf", "PDF (*.pdf)");
        if (fileName.isEmpty()) return;

        QPdfWriter writer(fileName);
        writer.setPageSize(QPageSize(QPageSize::A4));
        writer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

        QPainter painter(&writer);
        painter.setRenderHint(QPainter::Antialiasing);

        const int W = writer.width();
        int y = 0;

        // Titre
        painter.setFont(QFont("Arial", 16, QFont::Bold));
        painter.setPen(QColor(59, 42, 32));
        painter.drawText(QRect(0, y, W, 120), Qt::AlignCenter, "Liste des Employés");
        y += 140;

        // Date
        painter.setFont(QFont("Arial", 8));
        painter.setPen(QColor(140, 123, 112));
        painter.drawText(QRect(0, y, W, 60), Qt::AlignRight,
            "Généré le " + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm"));
        y += 80;

        // En-têtes du tableau
        const QStringList headers = {"CIN/ID","Nom","Poste","Adresse","Téléphone","Date Emb.","Salaire","Statut","Sexe"};
        const int rowH = 80;
        const int colW = W / headers.size();

        painter.setFont(QFont("Arial", 8, QFont::Bold));
        painter.fillRect(QRect(0, y, W, rowH), QColor(59, 42, 32));
        painter.setPen(Qt::white);
        for (int c = 0; c < headers.size(); ++c)
            painter.drawText(QRect(c*colW+4, y+4, colW-8, rowH-8),
                             Qt::AlignVCenter | Qt::AlignLeft, headers[c]);
        y += rowH;

        // Lignes
        painter.setFont(QFont("Arial", 7));
        for (int row = 0; row < employeesTable->rowCount(); ++row) {
            if (employeesTable->isRowHidden(row)) continue;
            QColor bg = (row % 2 == 0) ? QColor(255,255,255) : QColor(253,247,242);
            painter.fillRect(QRect(0, y, W, rowH), bg);
            painter.setPen(QColor(59, 42, 32));
            for (int c = 0; c < headers.size(); ++c) {
                auto *item = employeesTable->item(row, c);
                QString txt = item ? item->text() : "";
                painter.drawText(QRect(c*colW+4, y+4, colW-8, rowH-8),
                                 Qt::AlignVCenter | Qt::AlignLeft | Qt::TextWordWrap, txt);
            }
            // Ligne séparatrice
            painter.setPen(QPen(QColor(198,142,101), 1));
            painter.drawLine(0, y+rowH, W, y+rowH);
            y += rowH;

            // Nouvelle page si nécessaire
            if (y + rowH > writer.height() - 100) {
                writer.newPage();
                y = 0;
            }
        }

        // Total
        painter.setFont(QFont("Arial", 8, QFont::Bold));
        painter.setPen(QColor(59, 42, 32));
        painter.drawText(QRect(0, y+20, W, 60), Qt::AlignRight,
            QString("Total : %1 employé(s)").arg(employeesTable->rowCount()));

        painter.end();
        QMessageBox::information(this, "Export PDF", "PDF exporté avec succès :\n" + fileName);
    });

    // --- Statistiques style commandes ---
    auto makeStatCard = [&](const QString &icon, const QString &title, QLabel *&valLabel, const QString &color, QWidget *parent) -> QFrame* {
        auto *card = new QFrame(parent);
        card->setStyleSheet(QString(
            "QFrame{background:%1;border-radius:10px;padding:4px;}"
            "QLabel{border:none;background:transparent;}").arg(color));
        card->setMinimumHeight(80);
        auto *lay = new QVBoxLayout(card);
        lay->setContentsMargins(12,8,12,8);
        lay->setSpacing(2);
        auto *top = new QLabel(icon + "  " + title, card);
        top->setObjectName(QString("employeeStatTitle_%1").arg(QString(title).replace(' ', '_')));
        top->setStyleSheet("font-size:11px;color:#ffffff;font-weight:600;opacity:0.9;");
        valLabel = new QLabel("—", card);
        valLabel->setObjectName(QString("employeeStatValue_%1").arg(QString(title).replace(' ', '_')));
        valLabel->setStyleSheet("font-size:22px;font-weight:bold;color:#ffffff;");
        valLabel->setAlignment(Qt::AlignLeft);
        lay->addWidget(top);
        lay->addWidget(valLabel);
        return card;
    };

    auto *statsRow1 = new QHBoxLayout();
    statsRow1->setSpacing(10);
    statsRow1->addWidget(makeStatCard("👥", "Total employés",  empStatsTotal,      "#5D4037", pageList));
    statsRow1->addWidget(makeStatCard("👨", "Hommes",          empStatsHommes,     "#6D4C41", pageList));
    statsRow1->addWidget(makeStatCard("👩", "Femmes",          empStatsFemmes,     "#8D6E63", pageList));
    statsRow1->addWidget(makeStatCard("💰", "Salaire moyen",   empStatsAvgSalary,  "#4E342E", pageList));
    statsRow1->addWidget(makeStatCard("📉", "Salaire min",     empStatsSalMin,     "#795548", pageList));
    statsRow1->addWidget(makeStatCard("📈", "Salaire max",     empStatsSalMax,     "#3E2723", pageList));
    vList->addLayout(statsRow1);

    empStatsPoste = new QLabel("📊  Répartition par poste : —", pageList);
    empStatsPoste->setStyleSheet(
        "background:#fff8f4;border:1px solid #C68E65;border-radius:8px;"
        "padding:10px 16px;color:#3b2a20;font-size:12px;font-weight:500;");
    empStatsPoste->setWordWrap(true);
    vList->addWidget(empStatsPoste);

    employeeStack->addWidget(pageList);

    // Tabs behavior
    auto setTab=[&](int idx){ for(int i=0;i<employeeTabButtons.size();++i) employeeTabButtons[i]->setChecked(i==idx); if(employeeStack) employeeStack->setCurrentIndex(idx); };
    connect(btnTabAdd,&QPushButton::clicked,this,[=](){ setTab(0); });
    connect(btnTabEdit,&QPushButton::clicked,this,[=](){ setTab(1); });
    connect(btnTabDelete,&QPushButton::clicked,this,[=](){ setTab(2); });
    setTab(0);

    // Actions
    auto refreshEmpTable = [this](){
        QSqlDatabase db = Connection::instance()->database();
        if (!db.isOpen()) {
            Connection::instance()->createConnect();
            db = Connection::instance()->database();
        }
        if (!employeesTable) return;

        employeesTable->setRowCount(0);
        if (employeeModel) employeeModel->removeRows(0, employeeModel->rowCount());

        const QString employeeTableName = detectEmployeeTableName(db);
        qDebug() << "refreshEmpTable: employeeTableName=" << employeeTableName;
        if (employeeTableName.isEmpty()) {
            QMessageBox::critical(this, "Erreur employe",
                                  "Impossible de detecter la table des employes dans Oracle.");
            return;
        }

        const EmpColumns cols = detectEmpColumns(db, employeeTableName);
        qDebug() << "refreshEmpTable: detected columns:" << cols.id << cols.nom << cols.poste << cols.adresse
                 << cols.telephone << cols.dateEmbauche << cols.salaire << cols.statut << cols.sexe;
        if (!cols.valid) {
            QMessageBox::critical(this, "Erreur employe",
                                  QString("Impossible de détecter les colonnes de la table '%1'.\nColonnes disponibles: %2")
                                  .arg(employeeTableName, getTableColumns(db, employeeTableName).join(", ")));
            return;
        }

        // Build SELECT with actual columns, using COALESCE for NULL-able columns
        QStringList selCols;
        selCols << (cols.id.isEmpty() ? "NULL" : cols.id);
        selCols << (cols.nom.isEmpty() ? "NULL" : cols.nom);
        selCols << (cols.poste.isEmpty() ? "NULL" : cols.poste);
        selCols << (cols.adresse.isEmpty() ? "NULL" : cols.adresse);
        selCols << (cols.telephone.isEmpty() ? "NULL" : cols.telephone);
        selCols << (cols.dateEmbauche.isEmpty() ? "NULL" : cols.dateEmbauche);
        selCols << (cols.salaire.isEmpty() ? "0" : cols.salaire);
        selCols << (cols.statut.isEmpty() ? "NULL" : cols.statut);
        selCols << (cols.sexe.isEmpty() ? "NULL" : cols.sexe);

        const QString sql = QString("SELECT %1 FROM %2 ORDER BY %3")
                                .arg(selCols.join(", "), employeeTableName, cols.id.isEmpty() ? "ROWNUM" : cols.id);

        QSqlQuery q(db);
        q.setForwardOnly(true);
        if (!q.exec(sql)) {
            qDebug() << "refreshEmpTable error:" << q.lastError().text() << "SQL:" << sql;
            QMessageBox::critical(this, "Erreur employe",
                                  QString("Erreur lors du chargement des employés : %1\nSQL : %2")
                                  .arg(q.lastError().text(), sql));
            return;
        }

        const QStringList empHeaders = {"CIN/ID","Nom","Poste","Adresse","Téléphone","Date Embauche","Salaire","Statut","Sexe"};
        double sum = 0.0, salMin = 0.0, salMax = 0.0;
        int total = 0, hommes = 0, femmes = 0;
        QMap<QString, int> posteCounts;
        bool firstSal = true;

        while (q.next()) {
            int row = employeesTable->rowCount();
            employeesTable->insertRow(row);
            if (employeeModel) employeeModel->insertRow(employeeModel->rowCount());

            // Safe column access: use index i to track what we're reading
            int i = 0;
            for (int c = 0; c < 9 && i < q.record().count(); ++c, ++i) {
                const QVariant raw = q.value(i);
                QString val;
                if (!raw.isNull() && raw.isValid()) {
                    if (raw.typeId() == QMetaType::QDate)
                        val = raw.toDate().toString("dd/MM/yyyy");
                    else if (raw.typeId() == QMetaType::QDateTime)
                        val = raw.toDateTime().date().toString("dd/MM/yyyy");
                    else if (raw.typeId() == QMetaType::Double || raw.typeId() == QMetaType::Float)
                        val = QString::number(raw.toDouble(), 'f', 2);
                    else
                        val = raw.toString();
                }
                auto *item = new QTableWidgetItem(val);
                item->setTextAlignment(Qt::AlignCenter);
                item->setForeground(QColor(26, 26, 26));
                item->setBackground((row % 2 == 0) ? QColor(255, 255, 255) : QColor(253, 247, 242));
                employeesTable->setItem(row, c, item);
                if (employeeModel) {
                    int r = employeeModel->rowCount() - 1;
                    employeeModel->setData(employeeModel->index(r, c), val);
                }
            }

            // Statistics - use safe index access
            if (q.record().count() > 6) {
                double sal = q.value(6).toDouble();
                sum += sal;
                if (firstSal) { salMin = salMax = sal; firstSal = false; }
                else { salMin = qMin(salMin, sal); salMax = qMax(salMax, sal); }
            }
            if (q.record().count() > 2) {
                QString poste = q.value(2).toString().trimmed();
                if (!poste.isEmpty()) posteCounts[poste]++;
            }
            if (q.record().count() > 8) {
                QString sexe = q.value(8).toString().trimmed().toLower();
                if (sexe == "homme" || sexe == "m") hommes++;
                else if (sexe == "femme" || sexe == "f") femmes++;
            }
            ++total;
        }

        if(empStatsTotal)    empStatsTotal->setText(QString::number(total));
        if(empStatsAvgSalary)empStatsAvgSalary->setText(QString::number(total?sum/total:0.0,'f',0)+" DT");
        if(empStatsHommes)   empStatsHommes->setText(QString::number(hommes));
        if(empStatsFemmes)   empStatsFemmes->setText(QString::number(femmes));
        if(empStatsSalMin)   empStatsSalMin->setText(firstSal ? "—" : QString::number(salMin,'f',0)+" DT");
        if(empStatsSalMax)   empStatsSalMax->setText(firstSal ? "—" : QString::number(salMax,'f',0)+" DT");
        if(empStatsPoste) {
            QStringList parts;
            for(auto it=posteCounts.begin();it!=posteCounts.end();++it)
                parts << QString("%1: %2").arg(it.key()).arg(it.value());
            empStatsPoste->setText("📊  Postes — " + (parts.isEmpty() ? "—" : parts.join("  |  ")));
        }
        employeesTable->viewport()->update();
    };
    connect(btnTabList,&QPushButton::clicked,this,[=](){ setTab(3); refreshEmpTable(); });

    connect(btnAddEmp,&QPushButton::clicked,this,[=](){
        QString id   = empIdAdd->text().trimmed();
        QString name = empNameAdd->text().trimmed();
        QString addr = empAddrAdd->text().trimmed();
        QString phone= empPhoneAdd->text().trimmed();
        double salary= empSalaryAdd->value();
        QDate hireDate = empHireDateAdd->date();

        // --- Validations ---
        // CIN obligatoire et numérique (8 chiffres)
        if (id.isEmpty()) {
            QMessageBox::warning(this,"Saisie invalide","Le CIN/ID est obligatoire."); empIdAdd->setFocus(); return;
        }
        QRegularExpression cinRx("^\\d{1,20}$");
        if (!cinRx.match(id).hasMatch()) {
            QMessageBox::warning(this,"Saisie invalide","Le CIN/ID doit contenir uniquement des chiffres."); empIdAdd->setFocus(); return;
        }
        // Nom obligatoire, lettres et espaces uniquement
        if (name.isEmpty()) {
            QMessageBox::warning(this,"Saisie invalide","Le Nom est obligatoire."); empNameAdd->setFocus(); return;
        }
        QRegularExpression nameRx("^[\\p{L} '\\-]{2,50}$");
        if (!nameRx.match(name).hasMatch()) {
            QMessageBox::warning(this,"Saisie invalide","Le Nom ne doit contenir que des lettres (2-50 caractères)."); empNameAdd->setFocus(); return;
        }
        // Téléphone : chiffres uniquement si renseigné
        if (!phone.isEmpty()) {
            QRegularExpression phoneRx("^[0-9+\\s\\-]{6,20}$");
            if (!phoneRx.match(phone).hasMatch()) {
                QMessageBox::warning(this,"Saisie invalide","Le téléphone doit contenir uniquement des chiffres (6-20 caractères)."); empPhoneAdd->setFocus(); return;
            }
        }
        // Salaire non négatif
        if (salary < 0) {
            QMessageBox::warning(this,"Saisie invalide","Le salaire ne peut pas être négatif."); empSalaryAdd->setFocus(); return;
        }
        // Date d'embauche pas dans le futur
        if (hireDate > QDate::currentDate()) {
            QMessageBox::warning(this,"Saisie invalide","La date d'embauche ne peut pas être dans le futur."); empHireDateAdd->setFocus(); return;
        }

        QSqlDatabase db = Connection::instance()->database();
        if (!db.isOpen()) { Connection::instance()->createConnect(); db = Connection::instance()->database(); }
        if (!db.isOpen()) { QMessageBox::critical(this,"Base de données","Connexion indisponible"); return; }

        const QString tbl = detectEmployeeTableName(db);
        if (tbl.isEmpty()) { QMessageBox::critical(this,"Erreur","Table employés introuvable."); return; }
        const EmpColumns cols = detectEmpColumns(db, tbl);
        if (!cols.valid) { QMessageBox::critical(this,"Erreur","Colonnes employés non détectées."); return; }

        QSqlQuery qCheck(db);
        qCheck.prepare(QString("SELECT 1 FROM %1 WHERE %2=:id").arg(tbl, cols.id));
        qCheck.bindValue(":id", id);
        if (qCheck.exec() && qCheck.next()) { QMessageBox::warning(this,"Doublon","Un employé avec ce CIN existe déjà."); return; }

        QStringList insCols = {cols.id, cols.nom};
        QStringList insVals = {":id", ":nom"};
        if (!cols.poste.isEmpty())        { insCols << cols.poste;        insVals << ":poste"; }
        if (!cols.adresse.isEmpty())      { insCols << cols.adresse;      insVals << ":adresse"; }
        if (!cols.telephone.isEmpty())    { insCols << cols.telephone;    insVals << ":tel"; }
        if (!cols.dateEmbauche.isEmpty()) { insCols << cols.dateEmbauche; insVals << ":date_emb"; }
        if (!cols.salaire.isEmpty())      { insCols << cols.salaire;      insVals << ":salaire"; }
        if (!cols.statut.isEmpty())       { insCols << cols.statut;       insVals << ":statut"; }
        if (!cols.sexe.isEmpty())         { insCols << cols.sexe;         insVals << ":sexe"; }

        QSqlQuery q(db);
        q.prepare(QString("INSERT INTO %1 (%2) VALUES (%3)").arg(tbl, insCols.join(","), insVals.join(",")));
        q.bindValue(":id", id);
        q.bindValue(":nom", name);
        if (!cols.poste.isEmpty())        q.bindValue(":poste",    empPosteAdd->currentText());
        if (!cols.adresse.isEmpty())      q.bindValue(":adresse",  addr);
        if (!cols.telephone.isEmpty())    q.bindValue(":tel",      phone);
        if (!cols.dateEmbauche.isEmpty()) q.bindValue(":date_emb", hireDate);
        if (!cols.salaire.isEmpty())      q.bindValue(":salaire",  salary);
        if (!cols.statut.isEmpty())       q.bindValue(":statut",   QStringLiteral("Actif"));
        if (!cols.sexe.isEmpty())         q.bindValue(":sexe",     empSexAdd->currentText());
        if (!q.exec()) { QMessageBox::critical(this,"Erreur insertion", q.lastError().text()); return; }
        QSqlQuery commit(db); commit.exec("COMMIT");
        refreshEmpTable(); setTab(3);
        QMessageBox::information(this,"Succès","Employé ajouté.");
    });
    connect(btnResetEmp,&QPushButton::clicked,this,[=](){
        empIdAdd->clear(); empNameAdd->clear(); empAddrAdd->clear(); empEmailAdd->clear(); empPhoneAdd->clear();
        empHireDateAdd->setDate(QDate::currentDate()); empPosteAdd->setCurrentIndex(0); empSalaryAdd->setValue(0); empSexAdd->setCurrentIndex(0);
    });

    connect(btnSearchEdit,&QPushButton::clicked,this,[=](){
        QString id=empSearchEdit->text().trimmed();
        QSqlDatabase db = Connection::instance()->database();
        if (!db.isOpen()) { Connection::instance()->createConnect(); db = Connection::instance()->database(); }
        const QString tbl = detectEmployeeTableName(db);
        if (tbl.isEmpty()) { QMessageBox::critical(this,"Erreur","Table employés introuvable."); return; }
        const EmpColumns cols = detectEmpColumns(db, tbl);
        if (!cols.valid) { QMessageBox::critical(this,"Erreur","Colonnes employés non détectées."); return; }

        QStringList selCols;
        selCols << selectExpr(cols.nom, "EMP_NAME")
                << selectExpr(cols.telephone, "EMP_PHONE")
                << selectExpr(cols.salaire, "EMP_SALARY", "0");

        QSqlQuery q(db);
        q.prepare(QString("SELECT %1 FROM %2 WHERE %3=:id").arg(selCols.join(", "), tbl, cols.id));
        q.bindValue(":id", id);
        if (q.exec() && q.next()) {
            empEditName->setText(q.value(0).toString());
            empEditPhone->setText(q.value(1).toString());
            empEditSalary->setValue(q.value(2).toDouble());
            return;
        }
        QMessageBox::warning(this,"Introuvable","Employé non trouvé.");
    });
    connect(btnApplyEdit,&QPushButton::clicked,this,[=](){
        QString id   = empSearchEdit->text().trimmed();
        QString name = empEditName->text().trimmed();
        QString phone= empEditPhone->text().trimmed();
        double salary= empEditSalary->value();

        // Validations
        if (id.isEmpty()) {
            QMessageBox::warning(this,"Saisie invalide","Veuillez d'abord charger un employé."); return;
        }
        if (name.isEmpty()) {
            QMessageBox::warning(this,"Saisie invalide","Le Nom est obligatoire."); empEditName->setFocus(); return;
        }
        QRegularExpression nameRx("^[\\p{L} '\\-]{2,50}$");
        if (!nameRx.match(name).hasMatch()) {
            QMessageBox::warning(this,"Saisie invalide","Le Nom ne doit contenir que des lettres (2-50 caractères)."); empEditName->setFocus(); return;
        }
        if (!phone.isEmpty()) {
            QRegularExpression phoneRx("^[0-9+\\s\\-]{6,20}$");
            if (!phoneRx.match(phone).hasMatch()) {
                QMessageBox::warning(this,"Saisie invalide","Le téléphone doit contenir uniquement des chiffres."); empEditPhone->setFocus(); return;
            }
        }
        if (salary < 0) {
            QMessageBox::warning(this,"Saisie invalide","Le salaire ne peut pas être négatif."); empEditSalary->setFocus(); return;
        }

        QSqlDatabase db = Connection::instance()->database();
        if (!db.isOpen()) { Connection::instance()->createConnect(); db = Connection::instance()->database(); }
        const QString tbl = detectEmployeeTableName(db);
        if (tbl.isEmpty()) { QMessageBox::critical(this,"Erreur","Table employés introuvable."); return; }
        const EmpColumns cols = detectEmpColumns(db, tbl);
        if (!cols.valid) { QMessageBox::critical(this,"Erreur","Colonnes employés non détectées."); return; }

        QStringList setParts;
        setParts << cols.nom + "=:nom";
        if (!cols.telephone.isEmpty()) setParts << cols.telephone + "=:tel";
        if (!cols.salaire.isEmpty())   setParts << cols.salaire   + "=:sal";

        QSqlQuery q(db);
        q.prepare(QString("UPDATE %1 SET %2 WHERE %3=:id").arg(tbl, setParts.join(","), cols.id));
        q.bindValue(":nom", name);
        if (!cols.telephone.isEmpty()) q.bindValue(":tel", phone);
        if (!cols.salaire.isEmpty())   q.bindValue(":sal", salary);
        q.bindValue(":id", id);
        if (!q.exec()) { QMessageBox::critical(this,"Erreur", q.lastError().text()); return; }
        QSqlQuery commit(db); commit.exec("COMMIT");
        refreshEmpTable(); setTab(3);
        QMessageBox::information(this,"Succès","Employé mis à jour.");
    });

    connect(btnDel,&QPushButton::clicked,this,[=](){
        QString id=empDeleteId->text().trimmed();
        if (id.isEmpty()) { QMessageBox::warning(this,"ID manquant","Entrer un CIN."); return; }
        QSqlDatabase db = Connection::instance()->database();
        if (!db.isOpen()) { Connection::instance()->createConnect(); db = Connection::instance()->database(); }
        const QString tbl = detectEmployeeTableName(db);
        if (tbl.isEmpty()) { QMessageBox::critical(this,"Erreur","Table employés introuvable."); return; }
        const EmpColumns cols = detectEmpColumns(db, tbl);
        if (!cols.valid) { QMessageBox::critical(this,"Erreur","Colonnes employés non détectées."); return; }

        if (QMessageBox::question(this, "Confirmation",
                QString("Supprimer l'employé '%1' ?\nSes commandes liées seront dissociées.").arg(id),
                QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
            return;

        // Trouver toutes les tables/colonnes qui référencent la table employé via FK
        // et mettre ces colonnes à NULL pour éviter ORA-02292
        QSqlQuery fkQuery(db);
        fkQuery.prepare(
            "SELECT fkc.table_name, fkc.column_name "
            "FROM user_constraints fk "
            "JOIN user_constraints pk ON fk.r_constraint_name = pk.constraint_name "
            "JOIN user_cons_columns fkc ON fk.constraint_name = fkc.constraint_name "
            "WHERE fk.constraint_type = 'R' "
            "AND pk.table_name = :tbl"
        );
        fkQuery.bindValue(":tbl", tbl.toUpper());
        if (fkQuery.exec()) {
            while (fkQuery.next()) {
                const QString childTable  = fkQuery.value(0).toString();
                const QString childColumn = fkQuery.value(1).toString();
                QSqlQuery qNull(db);
                qNull.prepare(QString("UPDATE %1 SET %2=NULL WHERE %2=:id")
                                  .arg(childTable, childColumn));
                qNull.bindValue(":id", id);
                if (!qNull.exec())
                    qDebug() << "Dissociation FK échouée:" << qNull.lastError().text();
            }
        }
        // COMMIT intermédiaire pour valider les NULLs avant le DELETE
        QSqlQuery commitNull(db); commitNull.exec("COMMIT");

        QSqlQuery q(db);
        q.prepare(QString("DELETE FROM %1 WHERE %2=:id").arg(tbl, cols.id));
        q.bindValue(":id", id);
        if (!q.exec()) { QMessageBox::critical(this,"Erreur", q.lastError().text()); return; }
        QSqlQuery commit(db); commit.exec("COMMIT");
        refreshEmpTable(); setTab(3);
        QMessageBox::information(this,"Succès","Employé supprimé.");
    });

    refreshEmpTable();
}

Supplier MainWindow::supplierFromForm(bool *ok) const
{
    if (ok) *ok = false;
    Supplier s;
    s.id = supIdInput ? supIdInput->text().trimmed() : QString();
    s.nom = supNameInput ? supNameInput->text().trimmed() : QString();
    s.type = supTypeInput ? supTypeInput->text().trimmed() : QString();
    s.adresse = supAddressInput ? supAddressInput->text().trimmed() : QString();
    s.telephone = supPhoneInput ? supPhoneInput->text().trimmed() : QString();
    s.email = supEmailInput ? supEmailInput->text().trimmed() : QString();
    s.delai = supDelayInput ? supDelayInput->value() : 0;
    s.prix = supPriceInput ? supPriceInput->value() : 0.0;
    s.statut = supStatusInput ? supStatusInput->currentText() : QString();

    if (s.nom.isEmpty() || s.type.isEmpty() || s.telephone.isEmpty() || s.email.isEmpty()) {
        QMessageBox::warning(const_cast<MainWindow*>(this), "Champs requis", "Nom, type, téléphone et email sont obligatoires.");
        return s;
    }
    if (ok) *ok = true;
    return s;
}

void MainWindow::fillSupplierForm(const Supplier &supplier)
{
    if (supIdInput) supIdInput->setText(supplier.id);
    if (supNameInput) supNameInput->setText(supplier.nom);
    if (supTypeInput) supTypeInput->setText(supplier.type);
    if (supAddressInput) supAddressInput->setText(supplier.adresse);
    if (supPhoneInput) supPhoneInput->setText(supplier.telephone);
    if (supEmailInput) supEmailInput->setText(supplier.email);
    if (supDelayInput) supDelayInput->setValue(supplier.delai);
    if (supPriceInput) supPriceInput->setValue(supplier.prix);
    if (supStatusInput) {
        int idx = supStatusInput->findText(supplier.statut, Qt::MatchFixedString | Qt::MatchCaseSensitive);
        supStatusInput->setCurrentIndex(idx >= 0 ? idx : 0);
    }
}

void MainWindow::clearSupplierForm()
{
    if (supIdInput) supIdInput->clear();
    if (supNameInput) supNameInput->clear();
    if (supTypeInput) supTypeInput->clear();
    if (supAddressInput) supAddressInput->clear();
    if (supPhoneInput) supPhoneInput->clear();
    if (supEmailInput) supEmailInput->clear();
    if (supDelayInput) supDelayInput->setValue(0);
    if (supPriceInput) supPriceInput->setValue(0.0);
    if (supStatusInput) supStatusInput->setCurrentIndex(0);
    supplierLoadedId.clear();
}

bool MainWindow::addSupplier(const Supplier &supplier)
{
    QSqlDatabase db = Connection::instance()->database();
    if (!db.isOpen()) {
        qDebug() << "addSupplier: DB closed";
        return false;
    }
    QSqlQuery q(db);

    int newId = 1;
    QSqlQuery qMax(db);
    qMax.prepare("SELECT COALESCE(MAX(ID_FOURNISSEUR),0)+1 FROM FOURNISSEUR");
    if (qMax.exec() && qMax.next()) {
        newId = qMax.value(0).toInt();
    }

    q.prepare("INSERT INTO FOURNISSEUR (ID_FOURNISSEUR, NOM, TYPE_MATIERE_FOURNIE, ADRESSE, TELEPHONE, EMAIL, DELAI_LIVRAISON_MOYEN, PRIX_MOYEN_MATIERES, STATUT)"
              " VALUES (:id, :nom, :type, :adresse, :tel, :email, :delai, :prix, :statut)");
    q.bindValue(":id", newId);
    q.bindValue(":nom", supplier.nom);
    q.bindValue(":type", supplier.type);
    q.bindValue(":adresse", supplier.adresse);
    q.bindValue(":tel", supplier.telephone);
    q.bindValue(":email", supplier.email);
    q.bindValue(":delai", supplier.delai);
    q.bindValue(":prix", supplier.prix);
    q.bindValue(":statut", supplier.statut);

    if (!q.exec()) {
        qDebug() << "addSupplier error:" << q.lastError();
        return false;
    }
    QSqlQuery commitQuery(db);
    commitQuery.exec("COMMIT");
    return true;
}

bool MainWindow::updateSupplier(const QString &id, const Supplier &supplier)
{
    QSqlDatabase db = Connection::instance()->database();
    if (!db.isOpen()) {
        qDebug() << "updateSupplier: DB closed";
        return false;
    }
    QSqlQuery q(db);
    q.prepare("UPDATE FOURNISSEUR SET NOM = :nom, TYPE_MATIERE_FOURNIE = :type, ADRESSE = :adresse, TELEPHONE = :tel, EMAIL = :email, DELAI_LIVRAISON_MOYEN = :delai, PRIX_MOYEN_MATIERES = :prix, STATUT = :statut WHERE ID_FOURNISSEUR = :id");
    q.bindValue(":nom", supplier.nom);
    q.bindValue(":type", supplier.type);
    q.bindValue(":adresse", supplier.adresse);
    q.bindValue(":tel", supplier.telephone);
    q.bindValue(":email", supplier.email);
    q.bindValue(":delai", supplier.delai);
    q.bindValue(":prix", supplier.prix);
    q.bindValue(":statut", supplier.statut);
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "updateSupplier error:" << q.lastError();
        return false;
    }
    QSqlQuery commitQuery(db);
    commitQuery.exec("COMMIT");
    return true;
}

bool MainWindow::deleteSupplier(const QString &id)
{
    QSqlDatabase db = Connection::instance()->database();
    if (!db.isOpen()) {
        qDebug() << "deleteSupplier: DB closed";
        return false;
    }
    QSqlQuery q(db);
    q.prepare("DELETE FROM FOURNISSEUR WHERE ID_FOURNISSEUR = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "deleteSupplier error:" << q.lastError();
        return false;
    }
    QSqlQuery commitQuery(db);
    commitQuery.exec("COMMIT");
    return true;
}

Supplier* MainWindow::findSupplier(const QString &id)
{
    QSqlDatabase db = Connection::instance()->database();
    QSqlQuery q(db);
    q.prepare("SELECT ID_FOURNISSEUR, NOM, TYPE_MATIERE_FOURNIE, ADRESSE, TELEPHONE, EMAIL, DELAI_LIVRAISON_MOYEN, PRIX_MOYEN_MATIERES, STATUT FROM FOURNISSEUR WHERE ID_FOURNISSEUR = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "findSupplier error:" << q.lastError();
        return nullptr;
    }
    if (q.next()) {
        currentSupplier.id = q.value("ID_FOURNISSEUR").toString();
        currentSupplier.nom = q.value("NOM").toString();
        currentSupplier.type = q.value("TYPE_MATIERE_FOURNIE").toString();
        currentSupplier.adresse = q.value("ADRESSE").toString();
        currentSupplier.telephone = q.value("TELEPHONE").toString();
        currentSupplier.email = q.value("EMAIL").toString();
        currentSupplier.delai = q.value("DELAI_LIVRAISON_MOYEN").toInt();
        currentSupplier.prix = q.value("PRIX_MOYEN_MATIERES").toDouble();
        currentSupplier.statut = q.value("STATUT").toString();
        return &currentSupplier;
    }
    return nullptr;
}

void MainWindow::refreshSupplierTable()
{
    if (!suppliersTable) return;

    QSqlDatabase db = Connection::instance()->database();
    QSqlQuery q(db);
    if (!q.exec("SELECT ID_FOURNISSEUR, NOM, TYPE_MATIERE_FOURNIE, ADRESSE, TELEPHONE, EMAIL, DELAI_LIVRAISON_MOYEN, PRIX_MOYEN_MATIERES, STATUT FROM FOURNISSEUR ORDER BY ID_FOURNISSEUR")) {
        qDebug() << "refreshSupplierTable error:" << q.lastError();
        return;
    }

    suppliersTable->setRowCount(0);
    int row = 0;
    int total = 0;
    int active = 0;
    double sumPrice = 0.0;
    double sumDelai = 0.0;

    while (q.next()) {
        suppliersTable->insertRow(row);
        suppliersTable->setItem(row, 0, new QTableWidgetItem(q.value("ID_FOURNISSEUR").toString()));
        suppliersTable->setItem(row, 1, new QTableWidgetItem(q.value("NOM").toString()));
        suppliersTable->setItem(row, 2, new QTableWidgetItem(q.value("TYPE_MATIERE_FOURNIE").toString()));
        suppliersTable->setItem(row, 3, new QTableWidgetItem(q.value("ADRESSE").toString()));
        suppliersTable->setItem(row, 4, new QTableWidgetItem(q.value("TELEPHONE").toString()));
        suppliersTable->setItem(row, 5, new QTableWidgetItem(q.value("EMAIL").toString()));
        suppliersTable->setItem(row, 6, new QTableWidgetItem(q.value("DELAI_LIVRAISON_MOYEN").toString()));
        suppliersTable->setItem(row, 7, new QTableWidgetItem(q.value("PRIX_MOYEN_MATIERES").toString()));
        suppliersTable->setItem(row, 8, new QTableWidgetItem(q.value("STATUT").toString()));

        row++;
        total++;
        if (q.value("STATUT").toString().compare("Actif", Qt::CaseInsensitive) == 0) {
            active++;
        }
        sumPrice += q.value("PRIX_MOYEN_MATIERES").toDouble();
        sumDelai += q.value("DELAI_LIVRAISON_MOYEN").toDouble();
    }

    if (supStatTotal) supStatTotal->setText(QString::number(total));
    if (supStatActive) supStatActive->setText(QString::number(active));
    if (supStatAvgPrice) supStatAvgPrice->setText(total > 0 ? QString::number(sumPrice / total, 'f', 2) : "0");
    if (supStatAvgDelay) supStatAvgDelay->setText(total > 0 ? QString::number(sumDelai / total, 'f', 1) : "0");
}

void MainWindow::applyBarStyle(const QList<QPushButton*> &buttons, bool checkable)
{
    for (auto *b : buttons) {
        if (!b) continue;
        if (checkable) b->setCheckable(true);
        b->setStyleSheet(buttonBarStyle);
    }
}

void MainWindow::appendAiMessage(const QString &speaker, const QString &text)
{
    if (!ui || !ui->tb_ai_log) return;
    // Style bulle + avatar
    QString html;
    if (speaker == "Vous") {
        html = QString(R"(<div style='margin:8px 0; display:flex; align-items:flex-end; justify-content:flex-end;'>
            <div style='max-width:70%%; background:#e0e7ef; color:#2a2a2a; border-radius:16px 16px 4px 16px; padding:10px 16px; font-size:15px; box-shadow:0 2px 8px #0001;'>%1</div>
            <img src='https://ui-avatars.com/api/?name=Vous&background=8B4513&color=fff&size=32' style='margin-left:8px; border-radius:50%%; width:32px; height:32px;'>
        </div>)").arg(text.toHtmlEscaped());
    } else {
        html = QString(R"(<div style='margin:8px 0; display:flex; align-items:flex-end;'>
            <img src='https://ui-avatars.com/api/?name=AI&background=F2D2B5&color=8B4513&size=32' style='margin-right:8px; border-radius:50%%; width:32px; height:32px;'>
            <div style='max-width:70%%; background:#fffbe6; color:#8B4513; border-radius:16px 16px 16px 4px; padding:10px 16px; font-size:15px; box-shadow:0 2px 8px #0001;'><b>Assistant</b><br>%1</div>
        </div>)").arg(text.toHtmlEscaped());
    }
    ui->tb_ai_log->append(html);
}

void MainWindow::sendAiMessage(const QString &userText)
{
    const QString trimmed = userText.trimmed();
    if (trimmed.isEmpty()) return;

    appendAiMessage("Vous", trimmed);


    // Mode connecté à la base : réponses variées et naturelles
    QString question = trimmed.toLower();
    QSqlDatabase db = Connection::instance()->database();
    if (!db.isOpen()) {
        appendAiMessage("Assistant", "Je ne peux pas accéder à la base de données pour le moment. Veuillez réessayer plus tard.");
        return;
    }

    // 1. Stock matière (détection naturelle)
    QStringList matieres = {"cuir", "tissu", "fil", "accessoire", "synthétique"};
    QString matiereCherchee;
    for (const QString &mat : matieres) {
        QRegularExpression re("\\b" + mat + "s?\\b");
        if (question.contains(re)) {
            matiereCherchee = mat;
            break;
        }
    }
    // Expressions variées pour demander le stock
    QStringList exprStock = {"stock", "combien", "reste", "quantité", "y a-t-il", "disponible", "en as-tu", "possèdes-tu", "as-tu", "combien de"};
    bool demandeStock = false;
    for (const QString &expr : exprStock) {
        if (question.contains(expr)) { demandeStock = true; break; }
    }
    if (!matiereCherchee.isEmpty() && demandeStock) {
        QSqlQuery q(db);
        extern QString gMaterialsTableName;
        QString sql = "SELECT NVL(nom_matiere, type_matiere), quantite_stock FROM " + gMaterialsTableName + " WHERE LOWER(NVL(nom_matiere, type_matiere)) LIKE :matiere";
        q.prepare(sql);
        q.bindValue(":matiere", "%" + matiereCherchee + "%");
        if (!q.exec()) {
            appendAiMessage("Assistant", "Désolé, je n'ai pas pu obtenir le stock : " + q.lastError().text());
            return;
        }
        if (q.next()) {
            QString nom = q.value(0).toString();
            int stock = q.value(1).toInt();
            appendAiMessage("Assistant", QString("Il reste <b>%1</b> unité(s) de <b>%2</b> en stock.").arg(stock).arg(nom));
        } else {
            appendAiMessage("Assistant", "Je n'ai trouvé aucune matière correspondant à : " + matiereCherchee);
        }
        return;
    }

    // 2. Conseil anti-gaspillage (expressions variées)
    QStringList exprConseil = {"gaspillage", "conseil", "économiser", "astuce", "éviter de gaspiller", "réduire les pertes", "optimiser", "moins de perte", "éviter le gaspillage"};
    for (const QString &expr : exprConseil) {
        if (question.contains(expr)) {
            appendAiMessage("Assistant", "Pour limiter le gaspillage, pensez à réutiliser les chutes de matières et à optimiser la découpe. Surveillez aussi les stocks faibles pour éviter les ruptures.");
            return;
        }
    }

    // 3. Alerte stock faible (expressions variées)
    QStringList exprAlerte = {"alerte", "rupture", "alerter", "faible", "bientôt fini", "presque vide", "plus beaucoup", "manque", "bientôt en rupture"};
    for (const QString &expr : exprAlerte) {
        if (question.contains(expr)) {
            QSqlQuery q(db);
            extern QString gMaterialsTableName;
            QString sql = "SELECT NVL(nom_matiere, type_matiere), quantite_stock FROM " + gMaterialsTableName + " WHERE quantite_stock < 10 ORDER BY quantite_stock ASC";
            if (!q.exec(sql)) {
                appendAiMessage("Assistant", "Impossible d'obtenir la liste des stocks faibles : " + q.lastError().text());
                return;
            }
            QStringList alertes;
            while (q.next()) {
                QString nom = q.value(0).toString();
                int stock = q.value(1).toInt();
                alertes << QString("%1 (%2 unité(s))").arg(nom).arg(stock);
            }
            if (alertes.isEmpty()) {
                appendAiMessage("Assistant", "Aucune matière n'est en rupture ou proche de la rupture.");
            } else {
                appendAiMessage("Assistant", "Attention, stock faible pour : " + alertes.join(", "));
            }
            return;
        }
    }

    // 4. Statistiques globales (expressions variées)
    QStringList exprStats = {"stat", "statistique", "moyenne", "total", "combien de matières", "nombre de matières", "bilan", "résumé", "synthèse"};
    for (const QString &expr : exprStats) {
        if (question.contains(expr)) {
            QSqlQuery q(db);
            extern QString gMaterialsTableName;
            QString sql = "SELECT COUNT(*), SUM(quantite_stock), AVG(rendement), AVG(perte) FROM " + gMaterialsTableName;
            if (!q.exec(sql) || !q.next()) {
                appendAiMessage("Assistant", "Impossible d'obtenir les statistiques globales.");
                return;
            }
            int totalMat = q.value(0).toInt();
            int totalStock = q.value(1).toInt();
            double moyRendement = q.value(2).toDouble();
            double moyPerte = q.value(3).toDouble();
            appendAiMessage("Assistant", QString("Il y a <b>%1</b> matières différentes pour un total de <b>%2</b> unités en stock. Rendement moyen : <b>%3%</b>, Perte moyenne : <b>%4%</b>.")
                                             .arg(QString::number(totalMat),
                                                  QString::number(totalStock),
                                                  QString::number(moyRendement, 'f', 1),
                                                  QString::number(moyPerte, 'f', 1)));
            return;
        }
    }

    // 5. Réponse par défaut
    appendAiMessage("Assistant", "Je peux répondre sur le stock, donner des conseils anti-gaspillage, signaler les alertes de stock faible ou fournir des statistiques. Posez-moi une question sur ces sujets !");
    return;
}

void MainWindow::sendGeminiMessage(const QString &userText)
{
    if (!aiNetwork) {
        appendAiMessage("Assistant", "Service réseau indisponible.");
        return;
    }
    if (geminiApiKey.isEmpty()) {
        appendAiMessage("Assistant", "Configurez GEMINI_API_KEY ou .env.local pour activer Gemini.");
        return;
    }

    appendAiMessage("Assistant", "(réflexion en cours via Gemini...)");

    QJsonObject part;
    part["text"] = userText;
    QJsonObject content;
    content["parts"] = QJsonArray{part};
    QJsonObject payload;
    payload["contents"] = QJsonArray{content};

    // Modèle Gemini forcé, ignore toute variable d'environnement ou .env.local
    const QString modelName = "gemini-1.5-flash-latest";
    const QUrl url(QString("https://generativelanguage.googleapis.com/v1/models/%1:generateContent?key=%2").arg(modelName, geminiApiKey));
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto *reply = aiNetwork->post(req, QJsonDocument(payload).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const QByteArray data = reply->readAll();
        const QString body = QString::fromUtf8(data);

        if (reply->error() != QNetworkReply::NoError) {
            appendAiMessage("Assistant", QString("Erreur Gemini (%1): %2").arg(statusCode).arg(reply->errorString()));
            if (!body.isEmpty()) appendAiMessage("Assistant", QString("Détail: %1").arg(body.left(400)));
            return;
        }
        if (statusCode >= 300) {
            appendAiMessage("Assistant", QString("Erreur HTTP Gemini %1").arg(statusCode));
            if (!body.isEmpty()) appendAiMessage("Assistant", QString("Détail: %1").arg(body.left(400)));
            return;
        }

        const QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isObject()) {
            appendAiMessage("Assistant", "Réponse Gemini invalide.");
            return;
        }
        const QJsonArray candidates = doc.object().value("candidates").toArray();
        if (candidates.isEmpty()) {
            appendAiMessage("Assistant", "Pas de réponse Gemini.");
            return;
        }
        const QJsonObject contentObj = candidates.first().toObject().value("content").toObject();
        const QJsonArray parts = contentObj.value("parts").toArray();
        if (parts.isEmpty()) {
            appendAiMessage("Assistant", "Réponse Gemini vide.");
            return;
        }
        const QString text = parts.first().toObject().value("text").toString().trimmed();
        appendAiMessage("Assistant", text.isEmpty() ? "(réponse vide)" : text);
    });
}

void MainWindow::update_label()
{
    // Accumulation des données reçues dans le buffer MainWindow::data
    data += A.read_from_arduino();
    
    // On traite chaque ligne complète terminée par '\n'
    while (data.contains('\n')) {
        int pos = data.indexOf('\n');
        QByteArray line = data.left(pos).trimmed();
        data.remove(0, pos + 1);

        QString id = QString::fromLatin1(line);
        if (id.isEmpty()) continue;

        qDebug() << "ID Arduino reçu:" << id;

        QSqlDatabase db = Connection::instance()->getDatabase();
        if (!db.isOpen()) {
            qDebug() << "Base de données non disponible pour l'auth Arduino";
            A.write_to_arduino("KO\n");
            continue;
        }

        const QString tbl = detectEmployeeTableName(db);
        const EmpColumns cols = detectEmpColumns(db, tbl);

        if (tbl.isEmpty() || !cols.id.isEmpty() == false) { // cols.valid is not explicitly set in the user's latest code but cols.id.isEmpty() is a good check
            qDebug() << "Table employés ou colonnes non détectées";
            A.write_to_arduino("KO\n");
            continue;
        }

        QSqlQuery query(db);
        // Utilisation de TRIM pour gérer les types CHAR (espaces résiduels) dans Oracle
        query.prepare(QString("SELECT %1 FROM %2 WHERE TRIM(%3) = :id").arg(cols.nom, tbl, cols.id));
        query.bindValue(":id", id);

        if (query.exec()) {
            if (query.next()) {
                QString nom = query.value(0).toString();
                // Format attendu par l'Arduino: "OK [NOM]\n"
                A.write_to_arduino("OK " + nom.toUtf8() + "\n");
                qDebug() << "Authentification réussie pour: " << nom;

                // Feedback visuel sur l'UI Qt
                if (lbl_door_status) {
                    lbl_door_status->setText("🔓 PORTE OUVERTE\n" + nom);
                    lbl_door_status->setStyleSheet("color: white; font-size: 16px; font-weight: bold; background: #2ea043; border-radius: 8px; padding: 10px; margin: 10px;");
                    lbl_door_status->setVisible(true);
                    
                    // Masquer après 3 secondes (cohérence avec le délai Arduino)
                    QTimer::singleShot(3000, this, [=](){
                        lbl_door_status->setVisible(false);
                    });
                }
            } else {
                // Pas trouvé
                A.write_to_arduino("KO\n");
                qDebug() << "Authentification échouée: ID " << id << " inconnu dans " << tbl;
            }
        } else {
            qDebug() << "Erreur SQL Auth Arduino:" << query.lastError().text();
            A.write_to_arduino("KO\n");
        }
    }
}

