#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include "materialswindow.h"
#include <QMessageBox>
#include <QClipboard>
#include <QPrinter>
#include <QtConcurrent/QtConcurrent>
#include <QUuid>
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
#include <QRegularExpressionValidator>
#include <QIntValidator>
#include <QVariant>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
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
#include <QSerialPortInfo>
#include <algorithm>
#include <functional>
#include <memory>
#include <QDesktopServices>
#include <QUrl>
#include <QUrlQuery>
#include <QTextStream>
#include <QCoreApplication>
#include <QEventLoop>
#include <QStringList>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

namespace {
constexpr bool kSmtpEnabled = true; // toggle SMTP sending
constexpr const char* kDefaultEmployeeCin = "12345678"; // CIN temporaire en attendant un champ UI dédié

QString detectOrdersTableName(QSqlDatabase &db)
{
    auto tableExists = [&](const QString &name) -> bool {
        QSqlQuery t(db);
        t.prepare("SELECT COUNT(*) FROM user_tables WHERE table_name = :t");
        t.bindValue(":t", name.toUpper());
        if (!t.exec() || !t.next()) {
            return false;
        }
        return t.value(0).toInt() > 0;
    };

    if (tableExists("COMMANDES")) {
        return "COMMANDES";
    }
    if (tableExists("COMMANDE")) {
        return "COMMANDE";
    }
    return {};
}

QString detectMaterialsTableName(QSqlDatabase &db)
{
    QStringList candidates;
    if (!gMaterialsTableName.trimmed().isEmpty()) {
        candidates << gMaterialsTableName.trimmed();
    }
    candidates << "MATIERE_PREMIERE"
               << "RAYEN.MATIERE_PREMIERE"
               << "\"RAYEN\".\"MATIERE_PREMIERE\""
               << "MARYEM.MATIERE_PREMIERE"
               << "\"MARYEM\".\"MATIERE_PREMIERE\""
               << "MATERIALS"
               << "RAYEN.MATERIALS"
               << "\"RAYEN\".\"MATERIALS\""
               << "MARYEM.MATERIALS"
               << "\"MARYEM\".\"MATERIALS\"";
    candidates.removeDuplicates();

    for (const QString &candidate : candidates) {
        QSqlQuery probe(db);
        if (probe.exec(QString("SELECT 1 FROM %1 WHERE ROWNUM = 1").arg(candidate))) {
            gMaterialsTableName = candidate;
            return candidate;
        }
    }

    return {};
}

QString inferMaterialTypeFromOrderType(const QString &orderType)
{
    const QString normalized = orderType.trimmed().toLower();

    if (normalized.contains("tissu")) {
        return "Tissu";
    }
    if (normalized.contains("fil")) {
        return "Fil";
    }
    if (normalized.contains("accessoire")) {
        return "Accessoire";
    }
    if (normalized.contains("synth")) {
        return "Synthetique";
    }

    // Les types de produits actuels (sac, veste, chaussures, ceinture, portefeuille)
    // consomment principalement du cuir.
    return "Cuir";
}

bool adjustMaterialStockForOrderDelta(QSqlDatabase &db,
                                      const QString &orderType,
                                      int qtyDelta,
                                      QString *infoOut = nullptr,
                                      QString *errorOut = nullptr,
                                      QStringList *usedMaterialIdsOut = nullptr)
{
    if (usedMaterialIdsOut) {
        usedMaterialIdsOut->clear();
    }

    if (qtyDelta == 0) {
        if (infoOut) {
            *infoOut = "Aucun ajustement de stock matiere.";
        }
        return true;
    }

    const QString tableName = detectMaterialsTableName(db);
    if (tableName.isEmpty()) {
        if (errorOut) {
            *errorOut = "Table matiere introuvable (MATIERE_PREMIERE/MATERIALS).";
        }
        return false;
    }

    const QString wantedType = inferMaterialTypeFromOrderType(orderType);
    const int absoluteQty = qAbs(qtyDelta);

    struct MaterialRow {
        QVariant id;
        QString type;
        int stock = 0;
    };

    auto appendRows = [](QSqlQuery &query, QList<MaterialRow> &rows) {
        while (query.next()) {
            MaterialRow row;
            row.id = query.value(0);
            row.type = query.value(1).toString().trimmed();
            row.stock = query.value(2).toInt();
            if (!row.id.isValid()) {
                continue;
            }
            rows << row;
        }
    };

    if (qtyDelta > 0) {
        QList<MaterialRow> candidates;

        QSqlQuery byType(db);
        byType.prepare(QString(
            "SELECT ID_MATIERE, TYPE_MATIERE, QUANTITE_STOCK "
            "FROM %1 "
            "WHERE UPPER(TRIM(TYPE_MATIERE)) = UPPER(TRIM(:type)) AND QUANTITE_STOCK > 0 "
            "ORDER BY QUANTITE_STOCK DESC, ID_MATIERE ASC"
        ).arg(tableName));
        byType.bindValue(":type", wantedType);

        if (!byType.exec()) {
            if (errorOut) {
                *errorOut = "Echec lecture stock matiere: " + byType.lastError().text();
            }
            return false;
        }
        appendRows(byType, candidates);

        bool usedFallback = false;
        if (candidates.isEmpty()) {
            QSqlQuery anyType(db);
            anyType.prepare(QString(
                "SELECT ID_MATIERE, TYPE_MATIERE, QUANTITE_STOCK "
                "FROM %1 "
                "WHERE QUANTITE_STOCK > 0 "
                "ORDER BY QUANTITE_STOCK DESC, ID_MATIERE ASC"
            ).arg(tableName));

            if (!anyType.exec()) {
                if (errorOut) {
                    *errorOut = "Echec lecture stock matiere (fallback): " + anyType.lastError().text();
                }
                return false;
            }
            appendRows(anyType, candidates);
            usedFallback = true;
        }

        if (candidates.isEmpty()) {
            if (errorOut) {
                *errorOut = "Aucune matiere disponible pour ajuster le stock.";
            }
            return false;
        }

        int totalAvailable = 0;
        for (const MaterialRow &row : candidates) {
            totalAvailable += qMax(0, row.stock);
        }

        if (totalAvailable < absoluteQty) {
            if (errorOut) {
                const QString stockScope = usedFallback
                    ? "stock global"
                    : QString("type '%1'").arg(wantedType);
                *errorOut = QString("Stock insuffisant (%1): disponible=%2, demande=%3.")
                    .arg(stockScope, QString::number(totalAvailable), QString::number(absoluteQty));
            }
            return false;
        }

        int remaining = absoluteQty;
        QStringList details;
        QStringList usedMaterialIds;

        for (const MaterialRow &row : candidates) {
            if (remaining <= 0) {
                break;
            }

            const int takeQty = qMin(remaining, qMax(0, row.stock));
            if (takeQty <= 0) {
                continue;
            }

            QSqlQuery consumeOne(db);
            consumeOne.prepare(QString(
                "UPDATE %1 "
                "SET QUANTITE_STOCK = QUANTITE_STOCK - :qty "
                "WHERE ID_MATIERE = :id"
            ).arg(tableName));
            consumeOne.bindValue(":qty", takeQty);
            consumeOne.bindValue(":id", row.id);

            if (!consumeOne.exec() || consumeOne.numRowsAffected() != 1) {
                if (errorOut) {
                    *errorOut = "Echec de mise a jour du stock matiere: " + consumeOne.lastError().text();
                }
                return false;
            }

            remaining -= takeQty;
            details << QString("%1 (ID %2): -%3")
                          .arg(row.type.isEmpty() ? wantedType : row.type,
                               row.id.toString(),
                               QString::number(takeQty));
              usedMaterialIds << row.id.toString();
        }

        if (remaining > 0) {
            if (errorOut) {
                *errorOut = "Ajustement de stock incomplet (etat incoherent).";
            }
            return false;
        }

        if (infoOut) {
            *infoOut = QString("Stock matiere mis a jour: -%1. %2")
                .arg(QString::number(absoluteQty), details.join(", "));
        }

        if (usedMaterialIdsOut) {
            *usedMaterialIdsOut = usedMaterialIds;
        }

        return true;
    }

    // qtyDelta < 0 : recredit de stock (choisit une ligne du type, sinon fallback global)
    MaterialRow target;
    QSqlQuery creditByType(db);
    creditByType.prepare(QString(
        "SELECT ID_MATIERE, TYPE_MATIERE, QUANTITE_STOCK "
        "FROM ("
        "  SELECT ID_MATIERE, TYPE_MATIERE, QUANTITE_STOCK "
        "  FROM %1 "
        "  WHERE UPPER(TRIM(TYPE_MATIERE)) = UPPER(TRIM(:type)) "
        "  ORDER BY QUANTITE_STOCK DESC, ID_MATIERE ASC"
        ") WHERE ROWNUM = 1"
    ).arg(tableName));
    creditByType.bindValue(":type", wantedType);

    if (creditByType.exec() && creditByType.next()) {
        target.id = creditByType.value(0);
        target.type = creditByType.value(1).toString().trimmed();
        target.stock = creditByType.value(2).toInt();
    } else {
        QSqlQuery creditAny(db);
        creditAny.prepare(QString(
            "SELECT ID_MATIERE, TYPE_MATIERE, QUANTITE_STOCK "
            "FROM ("
            "  SELECT ID_MATIERE, TYPE_MATIERE, QUANTITE_STOCK "
            "  FROM %1 "
            "  ORDER BY QUANTITE_STOCK DESC, ID_MATIERE ASC"
            ") WHERE ROWNUM = 1"
        ).arg(tableName));

        if (!creditAny.exec() || !creditAny.next()) {
            if (errorOut) {
                *errorOut = "Aucune matiere disponible pour ajuster le stock.";
            }
            return false;
        }

        target.id = creditAny.value(0);
        target.type = creditAny.value(1).toString().trimmed();
        target.stock = creditAny.value(2).toInt();
    }

    if (!target.id.isValid()) {
        if (errorOut) {
            *errorOut = "ID matiere invalide (selection impossible).";
        }
        return false;
    }

    QSqlQuery creditStock(db);
    creditStock.prepare(QString(
        "UPDATE %1 "
        "SET QUANTITE_STOCK = QUANTITE_STOCK + :qty "
        "WHERE ID_MATIERE = :id"
    ).arg(tableName));
    creditStock.bindValue(":qty", absoluteQty);
    creditStock.bindValue(":id", target.id);

    if (!creditStock.exec() || creditStock.numRowsAffected() != 1) {
        if (errorOut) {
            *errorOut = "Echec de mise a jour du stock matiere: " + creditStock.lastError().text();
        }
        return false;
    }

    if (infoOut) {
        *infoOut = QString("Stock matiere mis a jour (%1, ID %2): +%3, nouveau stock %4.")
            .arg(target.type.isEmpty() ? wantedType : target.type,
                 target.id.toString(),
                 QString::number(absoluteQty),
                 QString::number(target.stock + absoluteQty));
    }

    return true;
}

bool consumeMaterialStockForOrder(QSqlDatabase &db,
                                  const QString &orderType,
                                  int orderQty,
                                  QString *infoOut = nullptr,
                                  QString *errorOut = nullptr,
                                  QStringList *usedMaterialIdsOut = nullptr)
{
    return adjustMaterialStockForOrderDelta(db, orderType, orderQty, infoOut, errorOut, usedMaterialIdsOut);
}

QString detectOrderMaterialUsageTableName(QSqlDatabase &db)
{
    QStringList candidates;
    candidates << "UTILISER"
               << "RAYEN.UTILISER"
               << "\"RAYEN\".\"UTILISER\""
               << "UTILISE"
               << "RAYEN.UTILISE"
               << "\"RAYEN\".\"UTILISE\"";

    for (const QString &candidate : candidates) {
        QSqlQuery probe(db);
        if (probe.exec(QString("SELECT 1 FROM %1 WHERE ROWNUM = 1").arg(candidate))) {
            return candidate;
        }
    }

    return {};
}

bool saveOrderMaterialUsageLinks(QSqlDatabase &db,
                                 const QString &orderId,
                                 const QStringList &materialIds,
                                 QString *errorOut = nullptr)
{
    const QString usageTable = detectOrderMaterialUsageTableName(db);
    if (usageTable.isEmpty()) {
        // Ne pas bloquer la commande si la table n'existe pas dans ce schema.
        return true;
    }

    QSqlQuery del(db);
    del.prepare(QString("DELETE FROM %1 WHERE ID_COMMANDE = :id").arg(usageTable));
    del.bindValue(":id", orderId);
    if (!del.exec()) {
        if (errorOut) {
            *errorOut = "Echec nettoyage table UTILISER: " + del.lastError().text();
        }
        return false;
    }

    QStringList uniqueIds;
    for (const QString &rawId : materialIds) {
        const QString idTrimmed = rawId.trimmed();
        if (idTrimmed.isEmpty()) {
            continue;
        }
        if (!uniqueIds.contains(idTrimmed)) {
            uniqueIds << idTrimmed;
        }
    }

    for (const QString &materialId : uniqueIds) {
        QSqlQuery ins(db);
        ins.prepare(QString("INSERT INTO %1 (ID_COMMANDE, ID_MATIERE) VALUES (:idCommande, :idMatiere)").arg(usageTable));
        ins.bindValue(":idCommande", orderId);
        ins.bindValue(":idMatiere", materialId);

        if (!ins.exec()) {
            if (errorOut) {
                *errorOut = QString("Echec insertion UTILISER (commande=%1, matiere=%2): %3")
                    .arg(orderId, materialId, ins.lastError().text());
            }
            return false;
        }
    }

    return true;
}

bool syncMaterialStockAfterOrderUpdate(QSqlDatabase &db,
                                       const QString &oldType,
                                       int oldQty,
                                       const QString &newType,
                                       int newQty,
                                       QString *infoOut = nullptr,
                                       QString *errorOut = nullptr)
{
    QStringList infos;

    const QString oldNorm = oldType.trimmed();
    const QString newNorm = newType.trimmed();

    if (oldNorm.isEmpty()) {
        QString info;
        if (!adjustMaterialStockForOrderDelta(db, newNorm, newQty, &info, errorOut)) {
            return false;
        }
        if (!info.isEmpty()) {
            infos << info;
        }
    } else if (oldNorm.compare(newNorm, Qt::CaseInsensitive) == 0) {
        const int delta = newQty - oldQty;
        if (delta != 0) {
            QString info;
            if (!adjustMaterialStockForOrderDelta(db, newNorm, delta, &info, errorOut)) {
                return false;
            }
            if (!info.isEmpty()) {
                infos << info;
            }
        }
    } else {
        if (oldQty > 0) {
            QString infoRestore;
            if (!adjustMaterialStockForOrderDelta(db, oldNorm, -oldQty, &infoRestore, errorOut)) {
                return false;
            }
            if (!infoRestore.isEmpty()) {
                infos << ("Annulation ancien type: " + infoRestore);
            }
        }

        if (newQty > 0) {
            QString infoConsume;
            if (!adjustMaterialStockForOrderDelta(db, newNorm, newQty, &infoConsume, errorOut)) {
                return false;
            }
            if (!infoConsume.isEmpty()) {
                infos << ("Application nouveau type: " + infoConsume);
            }
        }
    }

    if (infoOut) {
        *infoOut = infos.join("\n");
    }
    return true;
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

const QRegularExpression &emailPattern()
{
    static const QRegularExpression kEmailPattern(
        "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$"
    );
    return kEmailPattern;
}

const QRegularExpression &phonePattern()
{
    static const QRegularExpression kPhonePattern(
        "^\\+?[0-9][0-9\\s\\-]{6,18}$"
    );
    return kPhonePattern;
}

bool isValidEmail(const QString &email)
{
    const QString trimmed = email.trimmed();
    return !trimmed.isEmpty() && emailPattern().match(trimmed).hasMatch();
}

bool isValidPhone(const QString &phone)
{
    const QString trimmed = phone.trimmed();
    if (trimmed.isEmpty() || !phonePattern().match(trimmed).hasMatch()) {
        return false;
    }

    int digitCount = 0;
    for (const QChar ch : trimmed) {
        if (ch.isDigit()) {
            ++digitCount;
        }
    }
    return digitCount >= 8 && digitCount <= 15;
}

bool isPositiveIntegerText(const QString &text)
{
    bool ok = false;
    const int value = text.trimmed().toInt(&ok);
    return ok && value > 0;
}

bool isPlaceholderSelection(const QString &text)
{
    const QString normalized = text.trimmed().toLower();
    return normalized.isEmpty()
        || normalized.startsWith("--")
        || normalized.contains("selectionner")
        || normalized.contains(QString::fromUtf8("sélectionner"));
}

bool isValidPersonName(const QString &name)
{
    const QString trimmed = name.trimmed();
    if (trimmed.isEmpty()) {
        return false;
    }

    static const QRegularExpression kNamePattern("^[A-Za-zÀ-ÖØ-öø-ÿ' -]{2,80}$");
    return kNamePattern.match(trimmed).hasMatch();
}

bool isCancelledOrderStatus(const QString &status)
{
    const QString normalized = status.trimmed().toLower();
    return normalized.contains("annul") || normalized.contains("cancel");
}

void ensureCancelledStatusOption(QComboBox *combo)
{
    if (!combo) {
        return;
    }

    for (int i = 0; i < combo->count(); ++i) {
        if (isCancelledOrderStatus(combo->itemText(i))) {
            return;
        }
    }

    combo->addItem(QString::fromUtf8("Annulée"));
}

bool hasMeaningfulText(const QString &text)
{
    int symbols = 0;
    for (const QChar c : text) {
        if (c.isLetterOrNumber()) {
            ++symbols;
        }
    }
    return symbols >= 2;
}

struct MailStatusPalette {
    QString label;
    QString textColor;
    QString backgroundColor;
    QString borderColor;
};

MailStatusPalette resolveMailStatusPalette(const QString &rawStatus)
{
    const QString normalized = rawStatus.trimmed().toLower();

    MailStatusPalette palette;
    palette.label = rawStatus.trimmed();
    if (palette.label.isEmpty()) {
        palette.label = "Non defini";
    }

    if (normalized.contains("retard")) {
        palette.textColor = "#8b1e1e";
        palette.backgroundColor = "#fdecec";
        palette.borderColor = "#e7a3a3";
    } else if (normalized.contains(QString::fromUtf8("attente"))) {
        palette.textColor = "#805200";
        palette.backgroundColor = "#fff4de";
        palette.borderColor = "#f0cf94";
    } else if (normalized.contains(QString::fromUtf8("en cours")) || normalized.contains("cours")) {
        palette.textColor = "#0f4b6e";
        palette.backgroundColor = "#e3f3fb";
        palette.borderColor = "#9bc8e0";
    } else if (normalized.contains(QString::fromUtf8("prête")) || normalized.contains("prete") || normalized.contains("livraison")) {
        palette.textColor = "#1f5c2e";
        palette.backgroundColor = "#e8f6ea";
        palette.borderColor = "#a8d3b0";
    } else if (normalized.contains(QString::fromUtf8("compl")) || normalized.contains("termine") || normalized.contains("complete")) {
        palette.textColor = "#30518a";
        palette.backgroundColor = "#e8eefb";
        palette.borderColor = "#adc0e9";
    } else {
        palette.textColor = "#5e3a28";
        palette.backgroundColor = "#f5ece3";
        palette.borderColor = "#d8bca8";
    }

    return palette;
}

QString statusBadgeHtml(const QString &statusText)
{
    const MailStatusPalette palette = resolveMailStatusPalette(statusText);
    return QString(
        "<span style='display:inline-block;padding:6px 14px;border-radius:999px;"
        "background:%1;color:%2;border:1px solid %3;font-weight:700;font-size:12px;'>%4</span>"
    ).arg(
        palette.backgroundColor,
        palette.textColor,
        palette.borderColor,
        palette.label.toHtmlEscaped()
    );
}

QString detailRowHtml(const QString &label, const QString &value)
{
    const QString safeValue = value.trimmed().isEmpty() ? "-" : value.trimmed();
    return QString(
        "<tr>"
        "<td style='padding:10px 0;border-bottom:1px solid #efe1d6;color:#6d5647;width:44%%;'><strong>%1</strong></td>"
        "<td style='padding:10px 0;border-bottom:1px solid #efe1d6;color:#2f241d;'>%2</td>"
        "</tr>"
    ).arg(label.toHtmlEscaped(), safeValue.toHtmlEscaped());
}

QString detailStatusRowHtml(const QString &label, const QString &status)
{
    return QString(
        "<tr>"
        "<td style='padding:10px 0;border-bottom:1px solid #efe1d6;color:#6d5647;width:44%%;'><strong>%1</strong></td>"
        "<td style='padding:10px 0;border-bottom:1px solid #efe1d6;'>%2</td>"
        "</tr>"
    ).arg(label.toHtmlEscaped(), statusBadgeHtml(status));
}

QString changesBlockHtml(const QString &rawChanges)
{
    if (rawChanges.trimmed().isEmpty()) {
        return {};
    }

    QString items;
    const QStringList lines = rawChanges.split('\n', Qt::SkipEmptyParts);
    for (QString line : lines) {
        line = line.trimmed();
        while (!line.isEmpty() && (line.startsWith(QChar(0x2022)) || line.startsWith('-'))) {
            line.remove(0, 1);
            line = line.trimmed();
        }
        if (line.isEmpty()) {
            continue;
        }
        items += QString("<li style='margin:0 0 8px 0;'>%1</li>").arg(line.toHtmlEscaped());
    }

    if (items.isEmpty()) {
        return {};
    }

    return QString(
        "<div style='margin-top:20px;background:#fff3df;border:1px solid #f2d3a1;border-radius:14px;padding:16px 18px;'>"
        "<h3 style='margin:0 0 10px 0;color:#7a4a10;font-size:16px;'>Modifications effectuees</h3>"
        "<ul style='margin:0;padding-left:20px;color:#5a3a24;line-height:1.6;'>%1</ul>"
        "</div>"
    ).arg(items);
}

QString detailsCardHtml(const QString &title, const QString &rows, const QString &borderColor)
{
    return QString(
        "<div style='margin-top:18px;background:#ffffff;border:1px solid %3;border-radius:14px;padding:16px 18px;'>"
        "<h3 style='margin:0 0 12px 0;color:#3d2b20;font-size:18px;'>%1</h3>"
        "<table style='width:100%%;border-collapse:collapse;'>%2</table>"
        "</div>"
    ).arg(title.toHtmlEscaped(), rows, borderColor);
}

QString wrapMailTemplate(const QString &headline,
                         const QString &subline,
                         const QString &accentStart,
                         const QString &accentEnd,
                         const QString &bodyHtml)
{
    return QString(
        "<html><body style='margin:0;padding:24px;background:#f2ede8;font-family:Segoe UI, Trebuchet MS, Arial, sans-serif;color:#2f241d;'>"
        "<div style='max-width:760px;margin:0 auto;background:#fffaf5;border:1px solid #e5d4c6;border-radius:18px;overflow:hidden;'>"
        "<div style='padding:26px 30px;background:linear-gradient(135deg,%1,%2);'>"
        "<h2 style='margin:0;color:#ffffff;font-size:24px;font-weight:700;'>%3</h2>"
        "<p style='margin:8px 0 0 0;color:#f9efe7;font-size:14px;'>%4</p>"
        "</div>"
        "<div style='padding:24px 30px;'>%5"
        "<p style='margin:24px 0 0 0;color:#7f6756;font-size:12px;'>"
        "Ceci est un email automatique, merci de ne pas y repondre."
        "</p>"
        "</div>"
        "</div>"
        "</body></html>"
    ).arg(
        accentStart,
        accentEnd,
        headline.toHtmlEscaped(),
        subline.toHtmlEscaped(),
        bodyHtml
    );
}

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Garantir que le statut d'annulation est toujours disponible.
    ensureCancelledStatusOption(ui->cb_status);
    ensureCancelledStatusOption(ui->cb_status_update);

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
    if (ui->label4) ui->label4->setVisible(false);
    if (ui->le_proof) ui->le_proof->setVisible(false);
    if (ui->label9) ui->label9->setVisible(false);

    if (ui->cb_city_update) ui->cb_city_update->setVisible(false);
    if (ui->label14) ui->label14->setVisible(false);
    if (ui->le_proof_update) ui->le_proof_update->setVisible(false);
    if (ui->label19) ui->label19->setVisible(false);

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

    if (ui->cb_sort && ui->cb_sort->findText("Trier par ID") == -1) {
        ui->cb_sort->addItem("Trier par ID");
    }

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
        ui->statsMainGrid->setHorizontalSpacing(14);
        ui->statsMainGrid->setVerticalSpacing(14);
    }
    if (ui->scrollArea_stats) {
        ui->scrollArea_stats->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->scrollArea_stats->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }

    if (ui->label_stats_title) {
        ui->label_stats_title->setText("TABLEAU DE BORD COMMANDES");
        ui->label_stats_title->setStyleSheet(
            "font-size: 30px;"
            "font-weight: 800;"
            "letter-spacing: 1px;"
            "color: #4a2f22;"
            "padding: 18px 12px;"
        );
    }

    if (ui->lbl_main_chart_title) {
        ui->lbl_main_chart_title->setText("REPARTITION PAR TYPE DE PRODUIT");
        ui->lbl_main_chart_title->setStyleSheet(
            "font-size: 19px;"
            "font-weight: 800;"
            "color: #3d2b20;"
            "padding-bottom: 6px;"
        );
    }

    for (QFrame *card : {ui->stat_main_card_orders, ui->stat_main_card_revenue, ui->stat_main_card_pending}) {
        if (card) {
            card->setMinimumHeight(160);
        }
    }

    for (QLabel *titleLbl : {ui->lbl_main_orders_title, ui->lbl_main_revenue_title, ui->lbl_main_pending_title}) {
        if (!titleLbl) continue;
        titleLbl->setStyleSheet("font-size:14px;font-weight:700;color:white;background:transparent;border:none;padding:0;");
    }
    for (QLabel *valueLbl : {ui->lbl_main_orders_value, ui->lbl_main_revenue_value, ui->lbl_main_pending_value}) {
        if (!valueLbl) continue;
        valueLbl->setMinimumHeight(58);
        valueLbl->setAlignment(Qt::AlignCenter);
        valueLbl->setStyleSheet("font-size:34px;font-weight:800;color:white;background:transparent;border:none;padding:0;");
    }

    if (ui->stats_chart_frame) {
        ui->stats_chart_frame->setStyleSheet(
            "QFrame{"
            "background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #fffdfb,stop:1 #f8efe8);"
            "border:1px solid #d8c3b3;"
            "border-radius:16px;"
            "padding:18px;"
            "}"
        );
    }

    if (ui->stats_main_details_container) {
        ui->stats_main_details_container->setStyleSheet(
            "QFrame{"
            "background:#fffdfb;"
            "border:1px solid #d8c3b3;"
            "border-radius:16px;"
            "padding:16px;"
            "}"
        );
    }

    // Stats chart: pie chart by order type
    if (ui->chartMainLayout) {
        auto chartView = new QChartView(ui->stats_chart_frame);
        chartView->setObjectName("typePieChartView");
        chartView->setMinimumHeight(300);
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
    const QString ordersPanel = "background:#fffdfb;border:1px solid #C68E65;border-radius:10px;";
    const QString scrollBarStyle =
        "QScrollBar:vertical{background:#fdf7f2;width:14px;margin:4px 0;border:1px solid #C68E65;border-radius:7px;}"
        "QScrollBar::handle:vertical{background:#8B4513;min-height:24px;border-radius:6px;}"
        "QScrollBar::handle:vertical:hover{background:#a05a22;}"
        "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0px;}"
        "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:transparent;}";

    auto applyScrollStyle = [&](QWidget *w){ if (w) w->setStyleSheet(w->styleSheet() + scrollBarStyle); };
    auto applyOrdersPanel = [&](QWidget *page){
        if (!page) return;
        // Scope le style au conteneur de page uniquement (évite de styliser tous les labels/enfants).
        const QString objectName = page->objectName().trimmed();
        if (!objectName.isEmpty()) {
            page->setStyleSheet(QString("#%1{%2}").arg(objectName, ordersPanel));
        } else {
            page->setStyleSheet(ordersPanel);
        }
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
    applyOrdersPanel(ui->page_calendar);

    // Scrollbars brun sur les widgets listés des pages commandes
    applyScrollStyle(ui->table_list);
    applyScrollStyle(ui->table_delete);
    applyScrollStyle(ui->scrollArea_stats);
    applyScrollStyle(ui->tabWidget_stats);
    applyScrollStyle(ui->list_deliveries);

    // Calendrier: style premium cohérent avec les pages stats
    if (ui->label_calendar_title) {
        ui->label_calendar_title->setText("CALENDRIER DES LIVRAISONS");
        ui->label_calendar_title->setStyleSheet(
            "font-size: 30px;"
            "font-weight: 800;"
            "letter-spacing: 1px;"
            "color: #4a2f22;"
            "padding: 18px 12px;"
        );
    }

    if (ui->calendar_container) {
        ui->calendar_container->setStyleSheet(
            "QFrame#calendar_container{"
            "background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #fffdfb,stop:1 #f8efe8);"
            "border:1px solid #d8c3b3;"
            "border-radius:16px;"
            "padding:14px;"
            "}"
        );
    }

    if (ui->calendarWidget) {
        ui->calendarWidget->setStyleSheet(
            "QCalendarWidget{background:#fffaf5;border:1px solid #d8c3b3;border-radius:12px;}"
            "QCalendarWidget QWidget#qt_calendar_navigationbar{background:#f5e8dd;border:1px solid #e7d3c3;border-radius:10px;}"
            "QCalendarWidget QToolButton{color:#4a2f22;background:#fff7f0;border:1px solid #d9c3b0;border-radius:8px;padding:6px 10px;font-weight:700;}"
            "QCalendarWidget QToolButton:hover{background:#f2ddcb;}"
            "QCalendarWidget QSpinBox,QCalendarWidget QComboBox{color:#4a2f22;background:#fff7f0;border:1px solid #d9c3b0;border-radius:8px;padding:4px 8px;font-weight:700;}"
            "QCalendarWidget QTableView{background:#fffaf5;alternate-background-color:#fffaf5;selection-background-color:#f7d974;selection-color:#2d221a;gridline-color:#ead9cb;outline:0;}"
            "QCalendarWidget QAbstractItemView:enabled{color:#2f241d;background:#fffaf5;selection-background-color:#f7d974;selection-color:#2d221a;}"
            "QCalendarWidget QHeaderView::section{background:#f5e8dd;color:#6b4c37;border:1px solid #e7d3c3;padding:6px;font-weight:700;}"
        );
    }

    if (ui->deliveries_list_container) {
        ui->deliveries_list_container->setStyleSheet(
            "QFrame#deliveries_list_container{"
            "background:#fffdfb;"
            "border:1px solid #d8c3b3;"
            "border-radius:16px;"
            "padding:14px;"
            "}"
        );
    }

    if (ui->calendar_stats_container) {
        ui->calendar_stats_container->setStyleSheet(
            "QFrame#calendar_stats_container{"
            "background:#fffdfb;"
            "border:1px solid #d8c3b3;"
            "border-radius:16px;"
            "padding:16px;"
            "}"
        );
    }

    if (ui->list_deliveries) {
        ui->list_deliveries->setStyleSheet(
            "QListWidget{background:#ffffff;color:#3D362D;border:1px solid #d9c3b0;border-radius:10px;}"
            "QListWidget::item{padding:10px;border-bottom:1px solid #f1dfd1;}"
            "QListWidget::item:selected{background:#C68E65;color:white;}"
        );
    }

    if (ui->btn_export_calendar) {
        ui->btn_export_calendar->setMinimumHeight(46);
        ui->btn_export_calendar->setStyleSheet(
            "QPushButton{background:#8B4513;color:white;padding:10px 18px;border-radius:10px;font-weight:700;border:none;}"
            "QPushButton:hover{background:#a05a22;}"
            "QPushButton:pressed{background:#723a0f;}"
        );
    }

    for (QLabel *legendLbl : {ui->lbl_legend_today, ui->lbl_legend_delivery, ui->lbl_legend_urgent}) {
        if (!legendLbl) continue;
        legendLbl->setStyleSheet("font-size:13px;font-weight:700;color:#5a3a24;background:transparent;border:none;padding:2px 4px;");
    }

    for (QLabel *titleLbl : {ui->lbl_this_week_title, ui->lbl_this_month_title, ui->lbl_overdue_title}) {
        if (!titleLbl) continue;
        titleLbl->setStyleSheet("font-size:13px;font-weight:700;color:white;background:transparent;border:none;padding:0;");
    }
    for (QLabel *valueLbl : {ui->lbl_this_week_value, ui->lbl_this_month_value, ui->lbl_overdue_value}) {
        if (!valueLbl) continue;
        valueLbl->setMinimumHeight(56);
        valueLbl->setStyleSheet("font-size:32px;font-weight:800;color:white;background:transparent;border:none;padding:0;");
    }

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
    geminiPreferredModel = loadKey("GEMINI_MODEL").trimmed();

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
    });

    // 4. FOURNISSEURS
    connect(ui->btn_nav_suppliers, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_suppliers);
        ui->top_nav->setVisible(false);
    });

    // 5. CHAT BOT
    connect(ui->btn_nav_ai, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_ai);
        ui->top_nav->setVisible(false);
    });

    auto handleAiSend = [=]() {
        if (!ui->le_ai_input) return;
        const QString userText = ui->le_ai_input->text().trimmed();
        if (userText.isEmpty()) {
            QMessageBox::warning(this, "Saisie requise", "Veuillez écrire une question avant d'envoyer.");
            ui->le_ai_input->setFocus();
            return;
        }
        if (userText.size() > 300) {
            QMessageBox::warning(this, "Saisie trop longue", "La question ne doit pas dépasser 300 caractères.");
            ui->le_ai_input->setFocus();
            return;
        }
        if (!hasMeaningfulText(userText)) {
            QMessageBox::warning(this, "Saisie invalide", "Veuillez saisir une question lisible.");
            ui->le_ai_input->setFocus();
            return;
        }
        ui->le_ai_input->clear();
        sendAiMessage(userText);
    };

    if (ui->tb_ai_log) {
        ui->tb_ai_log->setOpenExternalLinks(true);
        if (!geminiApiKey.trimmed().isEmpty()) {
            appendAiMessage("Chat Bot", "Mode intelligent actif. Je peux répondre à presque toutes vos questions.");
        } else {
            appendAiMessage("Chat Bot", "Mode local actif. Je réponds aux questions limitées: salutation, stock, alertes, statistiques, conseils.");
        }
    }
    if (ui->label_ai_intro) {
        if (!geminiApiKey.trimmed().isEmpty()) {
            ui->label_ai_intro->setText("Chat Bot intelligent: posez vos questions librement (gestion, métier, ou général).");
        } else {
            ui->label_ai_intro->setText("Chat Bot local: posez des questions courtes sur les matières et le stock.");
        }
    }
    if (ui->btn_ai_send) {
        connect(ui->btn_ai_send, &QPushButton::clicked, this, handleAiSend);
    }
    if (ui->le_ai_input) {
        ui->le_ai_input->setMaxLength(300);
        connect(ui->le_ai_input, &QLineEdit::returnPressed, this, handleAiSend);
    }

    // Style unifié pour le menu latéral
    applyBarStyle({ui->btn_nav_orders, ui->btn_nav_material, ui->btn_nav_employees, ui->btn_nav_suppliers, ui->btn_nav_ai}, false);

    // Intégration des interfaces employé/fournisseur depuis l'ancien projet
    setupSupplierUI();
    setupEmployeeUI();
    refreshSupplierTable();

    // Activer l'animation pour tous les boutons déjà présents dans la fenêtre.
    for (QPushButton *btn : this->findChildren<QPushButton*>()) {
        ButtonAnimationHelper::setupLuxuryButtonAnimation(btn);
    }

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
        if (isPlaceholderSelection(type)) {
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
        if (!isValidEmail(email)) {
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
        if (isPlaceholderSelection(status)) {
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

        // Eviter ORA-00001: verifier explicitement si l'ID existe deja.
        {
            QSqlQuery checkId(db);
            checkId.prepare(QString("SELECT COUNT(*) FROM %1 WHERE ID_COMMANDE = :id").arg(tableName));
            checkId.bindValue(":id", id);

            if (!checkId.exec() || !checkId.next()) {
                QMessageBox::critical(this, "Erreur Base de donnees",
                    "Impossible de verifier l'unicite de l'ID commande.\n" + checkId.lastError().text());
                return;
            }

            if (checkId.value(0).toInt() > 0) {
                QMessageBox::warning(this, "ID deja utilise",
                    "Ce numero de commande existe deja.\n"
                    "Choisissez un autre ID commande.");
                ui->le_id->setFocus();
                ui->le_id->selectAll();
                return;
            }
        }

        QSqlQuery query(db);
        query.prepare(QString(
            "INSERT INTO %1 (ID_COMMANDE, TYPE_PRODUIT, QUANTITE, EMAIL_CLIENT, "
            "STATUT, DATE_COMMANDE, DATE_LIVRAISON_PREVUE, PRIX_TOTAL, CIN_EMPLOYE) "
            "VALUES (:id, :type, :qty, :email, :status, :date, :dateDel, :price, :cin)"
        ).arg(tableName));
        
        query.bindValue(":id", id);
        query.bindValue(":type", type);
        query.bindValue(":qty", qty);
        query.bindValue(":email", email);
        query.bindValue(":status", status);
        query.bindValue(":date", dateOrder);
        query.bindValue(":dateDel", dateDel);
        query.bindValue(":price", price);
        query.bindValue(":cin", QString::fromLatin1(kDefaultEmployeeCin));
        
        if (!query.exec()) {
            const QString sqlError = query.lastError().text();
            if (sqlError.contains("ORA-00001", Qt::CaseInsensitive)) {
                QMessageBox::warning(this, "Contrainte unique", 
                    "Insertion refusee: une valeur unique existe deja (ex: ID commande deja utilise).\n"
                    "Merci de verifier les champs uniques puis reessayer.");
            } else {
                QMessageBox::critical(this, "Erreur Base de donnees", 
                    "Erreur lors de l'insertion dans la base de donnees:\n" + sqlError);
            }
            qDebug() << "Erreur SQL:" << sqlError;
            return;
        }

        if (query.numRowsAffected() != 1) {
            qDebug() << "INSERT exécuté mais lignes affectées =" << query.numRowsAffected();
        }

        QString stockUpdateInfo;
        QString stockUpdateError;
        QStringList usedMaterialIds;
        if (!consumeMaterialStockForOrder(db, type, qty, &stockUpdateInfo, &stockUpdateError, &usedMaterialIds)) {
            QSqlQuery rollbackQuery(db);
            if (!rollbackQuery.exec("ROLLBACK")) {
                qDebug() << "Erreur ROLLBACK (insert):" << rollbackQuery.lastError().text();
            }

            QMessageBox::critical(this, "Stock matiere",
                "Commande annulee: impossible de mettre a jour le stock matiere.\n" + stockUpdateError);
            qDebug() << "Echec destockage matiere:" << stockUpdateError;
            return;
        }

        QString usageSaveError;
        if (!saveOrderMaterialUsageLinks(db, id, usedMaterialIds, &usageSaveError)) {
            QSqlQuery rollbackQuery(db);
            if (!rollbackQuery.exec("ROLLBACK")) {
                qDebug() << "Erreur ROLLBACK (insert links):" << rollbackQuery.lastError().text();
            }

            QMessageBox::critical(this, "Table UTILISER",
                "Commande annulee: impossible d'enregistrer les liens commande-matiere.\n" + usageSaveError);
            qDebug() << "Echec insertion UTILISER:" << usageSaveError;
            return;
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
        if (materialsWindow) {
            materialsWindow->refreshFromDb();
        }

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

        QString successMessage = "La commande a ete enregistree !";
        if (!stockUpdateInfo.isEmpty()) {
            successMessage += "\n" + stockUpdateInfo;
        }
        QMessageBox::information(this, "Succes", successMessage);
        
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
        if (!isPositiveIntegerText(searchId)) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ L'ID recherché doit être un nombre entier positif.");
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
        q.prepare(QString(
            "SELECT ID_COMMANDE, TYPE_PRODUIT, QUANTITE, EMAIL_CLIENT, STATUT, "
            "DATE_COMMANDE, DATE_LIVRAISON_PREVUE, PRIX_TOTAL, CIN_EMPLOYE "
            "FROM %1 WHERE ID_COMMANDE = :id"
        ).arg(tableName));
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
            int idx = ui->cb_status_update->findText(val, Qt::MatchFixedString | Qt::MatchCaseSensitive);
            if (idx < 0) {
                const QString needle = val.trimmed();
                for (int s = 0; s < ui->cb_status_update->count(); ++s) {
                    const QString candidate = ui->cb_status_update->itemText(s).trimmed();
                    if (candidate.compare(needle, Qt::CaseInsensitive) == 0) {
                        idx = s;
                        break;
                    }
                }
            }
            if (idx < 0 && isCancelledOrderStatus(val)) {
                for (int s = 0; s < ui->cb_status_update->count(); ++s) {
                    if (isCancelledOrderStatus(ui->cb_status_update->itemText(s))) {
                        idx = s;
                        break;
                    }
                }
            }
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
        if (isPlaceholderSelection(type)) {
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
        if (qty > 10000) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ La quantité semble anormalement grande (>10000).");
            ui->sb_qty_update->setFocus();
            return;
        }

        // Validation Email
        if (email.isEmpty()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Veuillez entrer l'email du client.");
            ui->le_email_update->setFocus();
            return;
        }
        if (!isValidEmail(email)) {
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
        if (price > 1000000) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Le prix semble anormalement élevé (>1M).");
            ui->dsb_price_update->setFocus();
            return;
        }

        // Validation Statut
        if (isPlaceholderSelection(status)) {
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
        
        QSqlQuery qOld(db);
        qOld.prepare(QString("SELECT TYPE_PRODUIT, QUANTITE, EMAIL_CLIENT, STATUT, "
                            "DATE_COMMANDE, DATE_LIVRAISON_PREVUE, PRIX_TOTAL "
                            "FROM %1 WHERE ID_COMMANDE = :id").arg(tableName));
        qOld.bindValue(":id", id);
        if (qOld.exec() && qOld.next()) {
            oldType = qOld.value(0).toString().trimmed();
            oldQty = qOld.value(1).toInt();
            oldEmail = qOld.value(2).toString().trimmed();
            oldStatus = qOld.value(3).toString().trimmed();
            oldOrderDate = qOld.value(4).toDate();
            oldDeliveryDate = qOld.value(5).toDate();
            oldPrice = qOld.value(6).toDouble();
        }

        QSqlQuery q(db);
        q.prepare(QString(
            "UPDATE %1 SET "
            "TYPE_PRODUIT = :type, QUANTITE = :qty, EMAIL_CLIENT = :email, "
            "STATUT = :status, DATE_COMMANDE = :date, DATE_LIVRAISON_PREVUE = :dateDel, PRIX_TOTAL = :price "
            "WHERE ID_COMMANDE = :id"
        ).arg(tableName));
        q.bindValue(":type", type);
        q.bindValue(":qty", qty);
        q.bindValue(":email", email);
        q.bindValue(":status", status);
        q.bindValue(":date", dateOrder);
        q.bindValue(":dateDel", dateDel);
        q.bindValue(":price", price);
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

        QString stockSyncInfo;
        QString stockSyncError;
        const bool wasCancelled = isCancelledOrderStatus(oldStatus);
        const bool nowCancelled = isCancelledOrderStatus(status);

        bool stockSyncOk = true;

        if (!wasCancelled && nowCancelled) {
            // Passage vers statut annulee: on rend la matiere qui etait consommee.
            if (oldQty > 0) {
                stockSyncOk = adjustMaterialStockForOrderDelta(
                    db,
                    oldType,
                    -oldQty,
                    &stockSyncInfo,
                    &stockSyncError
                );
            }
        } else if (wasCancelled && !nowCancelled) {
            // Sortie du statut annulee: on reconsomme la matiere selon les nouvelles valeurs.
            if (qty > 0) {
                stockSyncOk = adjustMaterialStockForOrderDelta(
                    db,
                    type,
                    qty,
                    &stockSyncInfo,
                    &stockSyncError
                );
            }
        } else if (!wasCancelled && !nowCancelled) {
            // Flux standard (commande active avant/apres): ajustement delta type/quantite.
            stockSyncOk = syncMaterialStockAfterOrderUpdate(
                db,
                oldType,
                oldQty,
                type,
                qty,
                &stockSyncInfo,
                &stockSyncError
            );
        } else {
            // Annulee -> annulee: aucun impact stock.
            stockSyncInfo = "Commande deja annulee: aucun ajustement de stock matiere.";
        }

        if (!stockSyncOk) {
            QSqlQuery rollbackQuery(db);
            if (!rollbackQuery.exec("ROLLBACK")) {
                qDebug() << "Erreur ROLLBACK (update):" << rollbackQuery.lastError().text();
            }

            QMessageBox::critical(this, "Stock matiere",
                "Mise a jour annulee: impossible d'ajuster le stock matiere.\n" + stockSyncError);
            qDebug() << "Echec ajustement stock (update):" << stockSyncError;
            return;
        }

        QSqlQuery commitQuery(db);
        if (!commitQuery.exec("COMMIT")) {
            qDebug() << "Erreur COMMIT (update):" << commitQuery.lastError().text();
            QMessageBox::warning(this, "Erreur", "La mise à jour a pu ne pas être enregistrée correctement.");
            return;
        }

        reloadOrdersFromDb();
        if (materialsWindow) {
            materialsWindow->refreshFromDb();
        }

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
            for (const QString &change : changes) {
                qDebug() << "  •" << change;
            }
        } else {
            qDebug() << "Aucune modification détectée, pas d'email envoyé.";
        }

        QString updateOkMsg = "Commande mise a jour avec succes !";
        if (!stockSyncInfo.isEmpty()) {
            updateOkMsg += "\n" + stockSyncInfo;
        }
        QMessageBox::information(this, "Succes", updateOkMsg);
    });

    // --- SUPPRIMER UNE COMMANDE (PAGE SUPPRIMER) ---
    connect(ui->btn_delete_action, &QPushButton::clicked, this, [=]() {
        QString id = ui->le_id_to_delete->text().trimmed();
        if (id.isEmpty()) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ Veuillez entrer l'ID de commande à supprimer.");
            ui->le_id_to_delete->setFocus();
            return;
        }
        if (!isPositiveIntegerText(id)) {
            QMessageBox::warning(this, "Erreur Saisie", "❌ L'ID à supprimer doit être un nombre entier positif.");
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

        QString deletedOrderType;
        int deletedOrderQty = 0;
        {
            QSqlQuery orderInfo(db);
            orderInfo.prepare(QString("SELECT TYPE_PRODUIT, QUANTITE FROM %1 WHERE ID_COMMANDE = :id").arg(tableName));
            orderInfo.bindValue(":id", id);

            if (!orderInfo.exec()) {
                QMessageBox::critical(this, "Erreur Base de donnees",
                    "Impossible de charger les informations de la commande avant suppression.\n" + orderInfo.lastError().text());
                return;
            }

            if (!orderInfo.next()) {
                QMessageBox::warning(this, "Introuvable", "❌ Aucune commande trouvee pour cet ID.");
                return;
            }

            deletedOrderType = orderInfo.value(0).toString();
            deletedOrderQty = qMax(0, orderInfo.value(1).toInt());
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

        QString stockDeleteInfo;
        QString stockDeleteError;
        if (deletedOrderQty > 0
            && !adjustMaterialStockForOrderDelta(db, deletedOrderType, -deletedOrderQty, &stockDeleteInfo, &stockDeleteError)) {
            QSqlQuery rollbackQuery(db);
            if (!rollbackQuery.exec("ROLLBACK")) {
                qDebug() << "Erreur ROLLBACK (delete):" << rollbackQuery.lastError().text();
            }

            QMessageBox::critical(this, "Stock matiere",
                "Suppression annulee: impossible de recrediter le stock matiere.\n" + stockDeleteError);
            qDebug() << "Echec ajustement stock (delete):" << stockDeleteError;
            return;
        }

        QSqlQuery commitQuery(db);
        if (!commitQuery.exec("COMMIT")) {
            qDebug() << "Erreur COMMIT (delete):" << commitQuery.lastError().text();
            QMessageBox::warning(this, "Erreur", "La suppression a pu ne pas être enregistrée correctement.");
            return;
        }

        reloadOrdersFromDb();
        if (materialsWindow) {
            materialsWindow->refreshFromDb();
        }

        ui->le_id_to_delete->clear();
        QString deleteOkMsg = "Commande supprimee avec succes !";
        if (!stockDeleteInfo.isEmpty()) {
            deleteOkMsg += "\n" + stockDeleteInfo;
        }
        QMessageBox::information(this, "Succes", deleteOkMsg);
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
    onCalendarDateChanged();
    updateStatistics();
    checkForNotifications(); // Vérification initiale
    loadEmailHistory(); // Charger l'historique des emails
    
    // Configuration SMTP Gmail pré-configurée
    smtpUsername = "rayenbenabdallah21@gmail.com";
    smtpPassword = "imghwonvblnjenfv"; // Mot de passe d'application (sans espaces)
    qDebug() << "✅ Configuration SMTP: Email configuré pour" << smtpUsername;
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

    QSqlQuery q(db);
    q.prepare(QString(
        "SELECT ID_COMMANDE, TYPE_PRODUIT, QUANTITE, EMAIL_CLIENT, STATUT, "
        "DATE_COMMANDE, DATE_LIVRAISON_PREVUE, PRIX_TOTAL, CIN_EMPLOYE "
        "FROM %1 ORDER BY ID_COMMANDE"
    ).arg(tableName));

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
        QString id = q.value(0).toString();
        QString type = q.value(1).toString();
        int qty = q.value(2).toInt();
        QString email = q.value(3).toString();
        QString status = q.value(4).toString();
        QString date = normalizeDateText(q.value(5));
        QString dateDel = normalizeDateText(q.value(6));
        double price = q.value(7).toDouble();
        QString cinEmploye = q.value(8).toString();

        // Liste (10 colonnes)
        {
            int row = ui->table_list->rowCount();
            ui->table_list->insertRow(row);

            auto *idItem = new QTableWidgetItem();
            bool idOk = false;
            const int idNumber = id.toInt(&idOk);
            if (idOk) {
                idItem->setData(Qt::DisplayRole, idNumber);
            } else {
                idItem->setText(id);
            }

            auto *qtyItem = new QTableWidgetItem();
            qtyItem->setData(Qt::DisplayRole, qty);

            auto *priceItem = new QTableWidgetItem();
            priceItem->setData(Qt::DisplayRole, price);

            ui->table_list->setItem(row, 0, idItem);
            ui->table_list->setItem(row, 1, new QTableWidgetItem(type));
            ui->table_list->setItem(row, 2, qtyItem);
            ui->table_list->setItem(row, 3, new QTableWidgetItem(email));
            ui->table_list->setItem(row, 4, new QTableWidgetItem(""));
            ui->table_list->setItem(row, 5, new QTableWidgetItem(date));
            ui->table_list->setItem(row, 6, new QTableWidgetItem(dateDel));
            ui->table_list->setItem(row, 7, priceItem);
            ui->table_list->setItem(row, 8, new QTableWidgetItem(cinEmploye));
            ui->table_list->setItem(row, 9, new QTableWidgetItem(status));
        }

        // Supprimer (5 colonnes)
        {
            int row = ui->table_delete->rowCount();
            ui->table_delete->insertRow(row);

            auto *deleteIdItem = new QTableWidgetItem();
            bool delIdOk = false;
            const int deleteIdNumber = id.toInt(&delIdOk);
            if (delIdOk) {
                deleteIdItem->setData(Qt::DisplayRole, deleteIdNumber);
            } else {
                deleteIdItem->setText(id);
            }

            auto *deleteQtyItem = new QTableWidgetItem();
            deleteQtyItem->setData(Qt::DisplayRole, qty);

            ui->table_delete->setItem(row, 0, deleteIdItem);
            ui->table_delete->setItem(row, 1, new QTableWidgetItem(type));
            ui->table_delete->setItem(row, 2, deleteQtyItem);
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
    
    // Animer l'apparition des tableaux chargés
    animateTableItemAppearance(ui->table_list);
    animateTableItemAppearance(ui->table_delete);

    // ========== Initialiser Arduino Sensor ==========
    qDebug() << "📡 Création ArduinoSensor...";
    arduinoSensor = new ArduinoSensor(this);
    qDebug() << "✅ ArduinoSensor créé!";
    
    // Connecter les signaux Arduino
    qDebug() << "🔗 Connexion des signaux...";
    connect(arduinoSensor, &ArduinoSensor::temperatureReceived,
            this, &MainWindow::onArduinoTemperatureReceived);
    connect(arduinoSensor, &ArduinoSensor::errorOccurred,
            this, &MainWindow::onArduinoErrorOccurred);
    connect(arduinoSensor, &ArduinoSensor::connectedStatusChanged,
            this, &MainWindow::onArduinoConnectedStatusChanged);
    qDebug() << "✅ Signaux connectés!";
    
    // Timer pour tentative de reconnexion automatique toutes les 30 secondes
    arduinoAutoConnectTimer = new QTimer(this);
    connect(arduinoAutoConnectTimer, &QTimer::timeout, this, [this]() {
        if (!arduinoSensor->isConnected()) {
            connectToArduino();
        }
    });
    arduinoAutoConnectTimer->start(30000);
    
    // Première tentative de connexion au démarrage
    qDebug() << "📱 APPEL connectToArduino()...";
    connectToArduino();
    qDebug() << "✅ Constructeur MainWindow terminé!";
}

MainWindow::~MainWindow()
{
    // Nettoyer Arduino
    if (arduinoAutoConnectTimer) {
        arduinoAutoConnectTimer->stop();
    }
    if (arduinoSensor) {
        arduinoSensor->disconnectFromPort();
    }
    
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
            for (const QDate& nextDate : allDates) {
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
    defaultFormat.setForeground(QColor("#2f241d"));
    defaultFormat.setBackground(QColor("#fffaf5"));
    
    QTextCharFormat weekendFormat;
    weekendFormat.setForeground(QColor("#b33232"));
    weekendFormat.setBackground(QColor("#fff2f2"));
    weekendFormat.setFontWeight(QFont::Bold);
    
    QTextCharFormat todayFormat;
    todayFormat.setBackground(QColor("#f7d974"));
    todayFormat.setForeground(QColor("#2f241d"));
    todayFormat.setFontWeight(QFont::Bold);
    
    // Couleurs par statut
    QTextCharFormat readyFormat;
    readyFormat.setBackground(QColor("#d8f1de"));
    readyFormat.setForeground(QColor("#245b34"));
    readyFormat.setFontWeight(QFont::Bold);
    
    QTextCharFormat inProgressFormat;
    inProgressFormat.setBackground(QColor("#ffe3bd"));
    inProgressFormat.setForeground(QColor("#7a4a10"));
    inProgressFormat.setFontWeight(QFont::Bold);
    
    QTextCharFormat pendingFormat;
    pendingFormat.setBackground(QColor("#ffd8d8"));
    pendingFormat.setForeground(QColor("#7f1d1d"));
    pendingFormat.setFontWeight(QFont::Bold);
    
    QTextCharFormat completedFormat;
    completedFormat.setBackground(QColor("#dbe9ff"));
    completedFormat.setForeground(QColor("#26437a"));
    completedFormat.setFontWeight(QFont::Bold);
    
    QTextCharFormat deliveryFormat;
    deliveryFormat.setBackground(QColor("#ece6df"));
    deliveryFormat.setForeground(QColor("#3d2f24"));
    deliveryFormat.setFontWeight(QFont::Bold);
    
    const QDate today = QDate::currentDate();
    const int shownYear = ui->calendarWidget ? ui->calendarWidget->yearShown() : today.year();
    const int shownMonth = ui->calendarWidget ? ui->calendarWidget->monthShown() : today.month();
    const QDate monthStart(shownYear, shownMonth, 1);
    QDate monthEnd = monthStart.addMonths(1).addDays(-1);
    
    // Parcourir tout le mois et appliquer les mises en forme
    for (QDate date = monthStart; date <= monthEnd; date = date.addDays(1)) {
        if (date == today) {
            // Aujourd'hui : priorité la plus haute
            ui->calendarWidget->setDateTextFormat(date, todayFormat);
        } else if (deliveryDates.contains(date)) {
            // Déterminer le pire statut pour la date (utiliser celui-ci pour la couleur)
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
    
    const double overdueRatio = (thisMonthCount > 0) ? (overdueCount * 100.0 / thisMonthCount) : 0.0;

    auto cardStyle = [](const QString &startColor,
                        const QString &endColor,
                        const QString &borderColor,
                        const QString &textColor = "#ffffff") {
        return QString(
            "QFrame{"
            "background:qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 %1,stop:1 %2);"
            "border:1px solid %3;"
            "border-radius:12px;"
            "padding:14px;"
            "}"
            "QLabel{color:%4;}"
        ).arg(startColor, endColor, borderColor, textColor);
    };

    if (ui->stat_this_week) {
        if (thisWeekCount > 0) {
            ui->stat_this_week->setStyleSheet(cardStyle("#c68e65", "#8b5e3b", "#a26f4f"));
        } else {
            ui->stat_this_week->setStyleSheet(cardStyle("#8f8f8f", "#6f6f6f", "#5f5f5f"));
        }
    }

    if (ui->stat_this_month) {
        ui->stat_this_month->setStyleSheet(cardStyle("#4e4338", "#2f2822", "#3d322a"));
    }

    if (ui->stat_overdue) {
        if (overdueCount > 0 && overdueRatio >= 40.0) {
            ui->stat_overdue->setStyleSheet(cardStyle("#c95a5a", "#8f2b2b", "#7e2222"));
        } else if (overdueCount > 0) {
            ui->stat_overdue->setStyleSheet(cardStyle("#d8944d", "#9f6631", "#895629"));
        } else {
            ui->stat_overdue->setStyleSheet(cardStyle("#5c946f", "#3e6f52", "#315a43"));
        }
    }

    ui->lbl_this_week_value->setText(QString::number(thisWeekCount));
    ui->lbl_this_month_value->setText(QString::number(thisMonthCount));
    ui->lbl_overdue_value->setText(QString::number(overdueCount));

    const QString statsTooltip = QString(
        "Pretes: %1\nEn cours: %2\nEn attente: %3\nCompletees: %4\nRevenu total: %5 DT"
    ).arg(readyCount)
     .arg(inProgressCount)
     .arg(pendingCount)
     .arg(completedCount)
     .arg(QString::number(totalRevenue, 'f', 2));

    if (ui->lbl_this_week_value) ui->lbl_this_week_value->setToolTip(statsTooltip);
    if (ui->lbl_this_month_value) ui->lbl_this_month_value->setToolTip(statsTooltip);
    if (ui->lbl_overdue_value) ui->lbl_overdue_value->setToolTip(statsTooltip);
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
    if (!ui || !ui->table_list) {
        return;
    }

    const int totalOrders = ui->table_list->rowCount();
    double totalRevenue = 0.0;
    int pendingOrders = 0;
    int deliveredOrders = 0;
    QMap<QString, int> typeCount;

    auto tableText = [this](int row, int col) -> QString {
        QTableWidgetItem *it = ui->table_list->item(row, col);
        return it ? it->text().trimmed() : QString();
    };

    for (int row = 0; row < totalOrders; ++row) {
        bool okPrice = false;
        const double price = tableText(row, 7).toDouble(&okPrice);
        if (okPrice) {
            totalRevenue += price;
        }

        const QString status = tableText(row, 9).toLower();
        if (status.contains("attente") || status.contains("en cours")) {
            ++pendingOrders;
        }
        if (status.contains("livr") || status.contains("exped") || status.contains("complete")) {
            ++deliveredOrders;
        }

        QString type = tableText(row, 1);
        if (type.isEmpty()) {
            type = "Inconnu";
        }
        typeCount[type] += 1;
    }

    const double avgPrice = (totalOrders > 0) ? (totalRevenue / totalOrders) : 0.0;
    const double pendingRatio = (totalOrders > 0) ? (pendingOrders * 100.0 / totalOrders) : 0.0;

    auto cardStyle = [](const QString &startColor,
                        const QString &endColor,
                        const QString &borderColor,
                        const QString &textColor = "#ffffff") {
        return QString(
            "QFrame{"
            "background:qlineargradient(x1:0,y1:0,x2:1,y2:1,stop:0 %1,stop:1 %2);"
            "border:1px solid %3;"
            "border-radius:14px;"
            "padding:18px;"
            "}"
            "QLabel{color:%4;}"
        ).arg(startColor, endColor, borderColor, textColor);
    };

    if (ui->stat_main_card_orders) {
        ui->stat_main_card_orders->setStyleSheet(cardStyle("#c68e65", "#8b5e3b", "#a26f4f"));
    }
    if (ui->stat_main_card_revenue) {
        ui->stat_main_card_revenue->setStyleSheet(cardStyle("#4e4338", "#2f2822", "#3d322a"));
    }
    if (ui->stat_main_card_pending) {
        if (pendingRatio >= 50.0) {
            ui->stat_main_card_pending->setStyleSheet(cardStyle("#c95a5a", "#8f2b2b", "#7e2222"));
        } else if (pendingRatio >= 25.0) {
            ui->stat_main_card_pending->setStyleSheet(cardStyle("#d8944d", "#9f6631", "#895629"));
        } else {
            ui->stat_main_card_pending->setStyleSheet(cardStyle("#5c946f", "#3e6f52", "#315a43"));
        }
    }

    if (ui->stats_main_details_container) {
        ui->stats_main_details_container->setStyleSheet(
            "QFrame{background:#fffdfb;border:1px solid #d8c3b3;border-radius:16px;padding:16px;}"
            "QLabel{color:#4a3528;}"
        );
    }

    ui->lbl_main_orders_value->setText(QString::number(totalOrders));
    ui->lbl_main_revenue_value->setText(QString::number(totalRevenue, 'f', 2) + " DT");
    ui->lbl_main_pending_value->setText(QString::number(pendingOrders));

    QString topProduct = "-";
    int maxTypeCount = 0;
    for (auto it = typeCount.begin(); it != typeCount.end(); ++it) {
        if (it.value() > maxTypeCount) {
            maxTypeCount = it.value();
            topProduct = QString("%1 (%2 commandes)").arg(it.key()).arg(it.value());
        }
    }
    ui->lbl_main_top_product_value->setText(topProduct);
    ui->lbl_main_top_city_value->setText("N/A");
    ui->lbl_main_avg_price_value->setText(QString::number(avgPrice, 'f', 2) + " DT");

    QString typeStats = "Repartition par type:\n";
    for (auto it = typeCount.begin(); it != typeCount.end(); ++it) {
        const double percentage = (totalOrders > 0) ? (it.value() * 100.0 / totalOrders) : 0.0;
        typeStats += QString("- %1: %2 (%3%)\n")
                         .arg(it.key())
                         .arg(it.value())
                         .arg(QString::number(percentage, 'f', 1));
    }
    ui->lbl_main_type_stats->setText(typeCount.isEmpty() ? "Aucune donnee disponible." : typeStats);

    if (ui->stats_chart_frame) {
        auto chartView = ui->stats_chart_frame->findChild<QChartView*>("typePieChartView");
        if (chartView) {
            auto *series = new QPieSeries();
            series->setHoleSize(0.35);

            if (totalOrders > 0) {
                const QList<QColor> palette = {
                    QColor("#8B5E3B"),
                    QColor("#C68E65"),
                    QColor("#4A6A7F"),
                    QColor("#6E8B74"),
                    QColor("#A3755B"),
                    QColor("#A2A05A")
                };

                int idx = 0;
                int maxCount = 0;
                QPieSlice *largest = nullptr;
                for (auto it = typeCount.begin(); it != typeCount.end(); ++it, ++idx) {
                    QPieSlice *slice = series->append(it.key(), it.value());
                    slice->setBrush(palette[idx % palette.size()]);
                    slice->setLabel(QString("%1 (%2%)")
                                        .arg(it.key())
                                        .arg(QString::number(slice->percentage() * 100.0, 'f', 1)));
                    slice->setLabelVisible(true);
                    if (it.value() > maxCount) {
                        maxCount = it.value();
                        largest = slice;
                    }
                }
                if (largest) {
                    largest->setExploded(true);
                    largest->setExplodeDistanceFactor(0.08);
                }
            } else {
                QPieSlice *slice = series->append("Aucune donnee", 1);
                slice->setBrush(QColor("#c9b8aa"));
                slice->setLabelVisible(true);
            }

            auto *chart = new QChart();
            chart->addSeries(series);
            chart->setTitle(QString());
            chart->setAnimationOptions(QChart::SeriesAnimations);
            chart->legend()->setVisible(true);
            chart->legend()->setAlignment(Qt::AlignBottom);
            chart->legend()->setLabelColor(QColor("#4e3a2d"));
            chart->setBackgroundVisible(false);
            chart->setMargins(QMargins(0, 0, 0, 0));

            if (chartView->chart()) {
                chartView->chart()->deleteLater();
            }
            chartView->setChart(chart);
            chartView->setStyleSheet("background: transparent;");
        }
    }

    ui->lbl_total_orders_val->setText(QString::number(totalOrders));
    ui->lbl_top_client_val->setText("N/A");

    QString typeStatsTab = "Repartition par type:\n";
    for (auto it = typeCount.begin(); it != typeCount.end(); ++it) {
        typeStatsTab += QString("- %1: %2 commandes\n").arg(it.key()).arg(it.value());
    }
    ui->lbl_type_stats->setText(typeCount.isEmpty() ? "Aucune donnee." : typeStatsTab);

    ui->lbl_city_stats->setText("Repartition par ville: N/A");

    const QString priceStatsTab = QString("Revenu total: %1 DT\nPrix moyen: %2 DT\nEn attente: %3\nLivrees/expediees: %4")
                                     .arg(QString::number(totalRevenue, 'f', 2))
                                     .arg(QString::number(avgPrice, 'f', 2))
                                     .arg(pendingOrders)
                                     .arg(deliveredOrders);
    ui->lbl_price_stats->setText(priceStatsTab);

    // Animer la mise à jour des statistiques
    animateStatisticsUpdate();
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
        
        // Animer l'apparition de la table
        animateTableItemAppearance(table_update);
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
        // Créer une notification personnalisée avec style luxe
        QDialog* notifDialog = new QDialog(this);
        notifDialog->setWindowTitle("Notifications de Livraison");
        notifDialog->setAttribute(Qt::WA_DeleteOnClose);
        notifDialog->setMinimumWidth(550);
        
        // Déterminer la couleur selon la priorité
        QString bgGradient = "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #5A6C7D, stop:1 #3D4E5C)";
        QString borderColor = "#2C3E50";
        QString titleText = "🔔 Notifications de Livraison";
        
        if (overdueCount > 0) {
            bgGradient = "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #5A6C7D, stop:1 #3D4E5C)";
            titleText = "⚠ NOTIFICATIONS DE LIVRAISON";
        }
        
        QString dialogStyle = QString(
            "QDialog { background: %1; border: 2px solid %2; border-radius: 12px; }"
        ).arg(bgGradient, borderColor);
        
        notifDialog->setStyleSheet(dialogStyle);
        
        QVBoxLayout* layout = new QVBoxLayout(notifDialog);
        layout->setSpacing(12);
        layout->setContentsMargins(20, 20, 20, 20);
        
        // Titre
        QLabel* titleLabel = new QLabel(titleText);
        titleLabel->setStyleSheet(
            "color: white; font-weight: bold; font-size: 14px; "
            "background: transparent; padding: 8px;"
        );
        layout->addWidget(titleLabel);
        
        // Contenu
        QLabel* contentLabel = new QLabel(notificationText);
        contentLabel->setWordWrap(true);
        contentLabel->setStyleSheet(
            "color: white; font-size: 12px; "
            "background: rgba(255, 255, 255, 0.15); "
            "border-left: 4px solid #A8D5E2; "
            "border-radius: 6px; padding: 12px; "
            "margin: 8px 0px;"
        );
        layout->addWidget(contentLabel);
        
        // Bouton OK
        QPushButton* okBtn = new QPushButton("OK");
        okBtn->setStyleSheet(
            "QPushButton { "
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #E8E8E8, stop:1 #D0D0D0); "
            "color: #2C3E50; font-weight: bold; border: none; border-radius: 6px; "
            "padding: 8px 20px; min-width: 80px; } "
            "QPushButton:hover { "
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #FFFFFF, stop:1 #E0E0E0); } "
            "QPushButton:pressed { background: #D0D0D0; }"
        );
        
        layout->addWidget(okBtn, 0, Qt::AlignRight);
        
        QObject::connect(okBtn, &QPushButton::clicked, notifDialog, &QDialog::accept);
        
        notifDialog->exec();
    }
}

// --- FONCTIONNALITÉ D'ENVOI D'EMAIL ---

QString MainWindow::getEmailTemplate(const QString &templateName, const QMap<QString, QString> &data)
{
    const QString orderId = data.value("id").trimmed().isEmpty() ? "-" : data.value("id").trimmed();
    const QString type = data.value("type");
    const QString qty = data.value("qty");
    const QString price = data.value("price");
    const QString orderDate = data.value("orderDate");
    const QString deliveryDate = data.value("deliveryDate");
    const QString status = data.value("status");

    const QString standardRows =
        detailRowHtml("Type de produit", type) +
        detailRowHtml("Quantite", qty) +
        detailRowHtml("Prix total", price + " DT") +
        detailRowHtml("Date de commande", orderDate) +
        detailRowHtml("Date de livraison", deliveryDate) +
        detailStatusRowHtml("Statut", status);

    if (templateName == "confirmation") {
        const QString body = QString(
            "<p style='margin:0 0 12px 0;'>Bonjour,</p>"
            "<p style='margin:0;'>Votre commande <strong>#%1</strong> a ete confirmee avec succes.</p>"
            "%2"
            "<p style='margin:18px 0 0 0;'>Merci pour votre confiance.</p>"
        ).arg(orderId.toHtmlEscaped(), detailsCardHtml("Recapitulatif de votre commande", standardRows, "#d9b79e"));

        return wrapMailTemplate(
            "Confirmation de commande",
            "Votre demande est bien enregistree.",
            "#8B5E3C",
            "#C68E65",
            body
        );
    }

    if (templateName == "livraison") {
        const QString rows =
            detailRowHtml("Produit", type) +
            detailRowHtml("Quantite", qty) +
            detailRowHtml("Date de livraison", deliveryDate) +
            detailStatusRowHtml("Statut", status);

        const QString body = QString(
            "<p style='margin:0 0 12px 0;'>Bonjour,</p>"
            "<p style='margin:0;'>Votre commande <strong>#%1</strong> est prete pour la livraison.</p>"
            "%2"
            "<div style='margin-top:18px;background:#eef8ef;border:1px solid #b9dfc2;border-radius:12px;padding:14px 16px;color:#2a5f38;'>"
            "Veuillez vous assurer d'etre disponible a la date indiquee."
            "</div>"
        ).arg(orderId.toHtmlEscaped(), detailsCardHtml("Informations de livraison", rows, "#a8d3b0"));

        return wrapMailTemplate(
            "Notification de livraison",
            "Votre commande avance vers la livraison.",
            "#3f8f6a",
            "#2f6f54",
            body
        );
    }

    if (templateName == "retard") {
        const QString rows =
            detailRowHtml("Produit", type) +
            detailRowHtml("Date initialement prevue", deliveryDate) +
            detailStatusRowHtml("Statut", status);

        const QString body = QString(
            "<p style='margin:0 0 12px 0;'>Bonjour,</p>"
            "<p style='margin:0;'>La commande <strong>#%1</strong> subit un retard de traitement.</p>"
            "%2"
            "<div style='margin-top:18px;background:#fdecec;border:1px solid #e7a3a3;border-radius:12px;padding:14px 16px;color:#7a2323;'>"
            "Nous mettons tout en oeuvre pour accelerer la preparation et vous informer de la nouvelle echeance."
            "</div>"
        ).arg(orderId.toHtmlEscaped(), detailsCardHtml("Point de situation", rows, "#e7a3a3"));

        return wrapMailTemplate(
            "Notification de retard",
            "Mise a jour importante sur votre commande.",
            "#b95e5e",
            "#8b1e1e",
            body
        );
    }

    if (templateName == "attente") {
        const QString rows =
            detailRowHtml("Produit", type) +
            detailRowHtml("Quantite", qty) +
            detailRowHtml("Date de commande", orderDate) +
            detailRowHtml("Date de livraison prevue", deliveryDate) +
            detailStatusRowHtml("Statut", status);

        const QString body = QString(
            "<p style='margin:0 0 12px 0;'>Bonjour,</p>"
            "<p style='margin:0;'>Votre commande <strong>#%1</strong> est en attente de traitement.</p>"
            "%2"
            "<div style='margin-top:18px;background:#fff7e6;border:1px solid #ecd2a0;border-radius:12px;padding:14px 16px;color:#6e4b19;'>"
            "Nous vous informerons des que le traitement passe a l'etape suivante."
            "</div>"
        ).arg(orderId.toHtmlEscaped(), detailsCardHtml("Suivi de la commande", rows, "#ecd2a0"));

        return wrapMailTemplate(
            "Commande en attente",
            "Votre dossier est bien pris en charge.",
            "#c38a45",
            "#8f622d",
            body
        );
    }

    if (templateName == "modification") {
        const QString body = QString(
            "<p style='margin:0 0 12px 0;'>Bonjour,</p>"
            "<p style='margin:0;'>Votre commande <strong>#%1</strong> a ete mise a jour.</p>"
            "%2"
            "%3"
            "<p style='margin:18px 0 0 0;'>Merci pour votre confiance.</p>"
        ).arg(
            orderId.toHtmlEscaped(),
            detailsCardHtml("Etat actuel de la commande", standardRows, "#9bc8e0"),
            changesBlockHtml(data.value("changes"))
        );

        return wrapMailTemplate(
            "Mise a jour de commande",
            "Les informations de votre commande viennent d'etre actualisees.",
            "#5a8db8",
            "#2f658e",
            body
        );
    }

    const QString fallbackRows =
        detailRowHtml("Type de produit", type) +
        detailRowHtml("Quantite", qty) +
        detailStatusRowHtml("Statut", status);

    const QString body = QString(
        "<p style='margin:0 0 12px 0;'>Bonjour,</p>"
        "<p style='margin:0;'>Ceci est une notification concernant votre commande <strong>#%1</strong>.</p>"
        "%2"
    ).arg(orderId.toHtmlEscaped(), detailsCardHtml("Informations disponibles", fallbackRows, "#d8bca8"));

    return wrapMailTemplate(
        "Notification commande",
        "Mise a jour d'information.",
        "#8b6b54",
        "#6f4f39",
        body
    );
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

    // Envoi synchrone contrôlé: évite les faux timeouts/UI parasites.
    SmtpClient *smtp = new SmtpClient(this);
    QEventLoop loop;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);

    bool finished = false;
    bool success = false;
    QString resultMessage;

    connect(smtp, &SmtpClient::emailSent, this,
            [&](bool ok, const QString &message) {
                success = ok;
                resultMessage = message;
                finished = true;
                loop.quit();
            });

    connect(&timeoutTimer, &QTimer::timeout, this,
            [&]() {
                if (finished) {
                    return;
                }
                success = false;
                resultMessage = "Timeout SMTP: aucune reponse du serveur.";
                finished = true;
                loop.quit();
            });

    qDebug() << "========================================";
    qDebug() << "🚀 LANCEMENT DE L'ENVOI SMTP...";
    qDebug() << "Serveur:" << smtpServer << ":" << smtpPort;
    qDebug() << "De:" << smtpUsername;
    qDebug() << "À:" << to;
    qDebug() << "========================================";

    timeoutTimer.start(30000);
    smtp->sendEmail(smtpUsername, to, subject, body, smtpServer, smtpPort, smtpUsername, smtpPassword);

    if (!finished) {
        loop.exec();
    }

    timeoutTimer.stop();
    smtp->deleteLater();

    if (!success) {
        qDebug() << "❌ Echec SMTP:" << resultMessage;
    } else {
        qDebug() << "✅ Email SMTP envoye a" << to;
    }

    return success;
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
    for (const EmailHistory &entry : emailHistory) {
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
    
    // Animer l'apparition de la table d'historique
    if (table_email_history) {
        animateTableItemAppearance(table_email_history);
    }
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
            QMessageBox::information(this, "✅ Renvoi effectué", "L'email a été renvoyé avec succès.");
        } else {
            QMessageBox::warning(this, "❌ Renvoi échoué",
                                 "Impossible de renvoyer l'email pour le moment.\n"
                                 "Vérifiez la connexion SMTP et réessayez.");
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
    QString sortType = ui->cb_sort->currentText().trimmed();
    const QString normalizedSortType = sortType.toLower();

    qDebug() << "Tri de la liste par:" << sortType;

    int column = 0;
    Qt::SortOrder order = Qt::AscendingOrder;

    if (normalizedSortType.contains("id")) {
        column = 0; // ID
        order = Qt::AscendingOrder;
    } else if (normalizedSortType.contains("date")) {
        column = 5; // Date Commande
        order = Qt::DescendingOrder; // Plus récent en premier
    } else if (normalizedSortType.contains("quant")) {
        column = 2; // Quantité
        order = Qt::DescendingOrder;
    } else if (normalizedSortType.contains("prix") &&
               (normalizedSortType.contains("decro") || normalizedSortType.contains(QString::fromUtf8("décro")))) {
        column = 7; // Prix
        order = Qt::DescendingOrder;
    } else if (normalizedSortType.contains("prix")) {
        column = 7; // Prix
        order = Qt::AscendingOrder;
    } else if (normalizedSortType.contains("type")) {
        column = 1; // Type
        order = Qt::AscendingOrder;
    } else if (normalizedSortType.contains("statut")) {
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
    for (auto *b : supplierTabButtons) tabLayout->addWidget(b);
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

    // Contraintes de saisie fournisseurs
    supNameInput->setMaxLength(80);
    supTypeInput->setMaxLength(80);
    supAddressInput->setMaxLength(160);
    supPhoneInput->setMaxLength(20);
    supEmailInput->setMaxLength(120);
    nameUpd->setMaxLength(80);
    typeUpd->setMaxLength(80);
    addrUpd->setMaxLength(160);
    phoneUpd->setMaxLength(20);
    emailUpd->setMaxLength(120);

    supIdInput->setValidator(new QIntValidator(1, 2147483647, pageUpd));
    idDel->setValidator(new QIntValidator(1, 2147483647, pageDel));

    supPhoneInput->setValidator(new QRegularExpressionValidator(phonePattern(), pageAdd));
    phoneUpd->setValidator(new QRegularExpressionValidator(phonePattern(), pageUpd));
    supEmailInput->setValidator(new QRegularExpressionValidator(emailPattern(), pageAdd));
    emailUpd->setValidator(new QRegularExpressionValidator(emailPattern(), pageUpd));

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
        titleLbl->setStyleSheet("font-weight:bold;");
        *valueLabel = new QLabel("0", card);
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
        if(!isPositiveIntegerText(id)){ QMessageBox::warning(this,"ID invalide","L'ID fournisseur doit être un entier positif."); return; }
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
        if(!isPositiveIntegerText(id)){ QMessageBox::warning(this,"ID invalide","L'ID fournisseur doit être un entier positif."); return; }
        Supplier s; s.id=id; s.nom=nameUpd->text().trimmed(); s.type=typeUpd->text().trimmed();
        s.adresse=addrUpd->text().trimmed(); s.telephone=phoneUpd->text().trimmed(); s.email=emailUpd->text().trimmed();
        s.delai=delayUpd->value(); s.prix=priceUpd->value(); s.statut=statusUpd->currentText();
        if(s.nom.isEmpty()||s.type.isEmpty()||s.adresse.isEmpty()||s.telephone.isEmpty()||s.email.isEmpty()){
            QMessageBox::warning(this,"Champs requis","Remplir nom/type/adresse/tel/email."); return; }
        if(!isValidPersonName(s.nom)){ QMessageBox::warning(this,"Saisie invalide","Nom fournisseur invalide (lettres et espaces uniquement)."); return; }
        if(!isValidPhone(s.telephone)){ QMessageBox::warning(this,"Saisie invalide","Téléphone fournisseur invalide."); return; }
        if(!isValidEmail(s.email)){ QMessageBox::warning(this,"Saisie invalide","Email fournisseur invalide."); return; }
        if(updateSupplier(id,s)){ refreshSupplierTable(); QMessageBox::information(this,"Succès","Fournisseur mis à jour."); }
        else QMessageBox::warning(this,"Erreur","Mise à jour impossible.");
    });

    connect(btnLoadDel, &QPushButton::clicked, this, [=](){
        QString id = idDel->text().trimmed();
        if(id.isEmpty()){ QMessageBox::warning(this,"Champ manquant","Entrer un ID."); return; }
        if(!isPositiveIntegerText(id)){ QMessageBox::warning(this,"ID invalide","L'ID fournisseur doit être un entier positif."); return; }
        Supplier *s = findSupplier(id);
        if(!s){ lblDelInfo->setText("(Aucun fournisseur)"); QMessageBox::information(this,"Introuvable","Aucun fournisseur trouvé."); return; }
        lblDelInfo->setText(QString("%1 - %2 (%3)").arg(s->id, s->nom, s->type));
        supplierLoadedId = s->id;
    });

    connect(btnDel, &QPushButton::clicked, this, [=](){
        QString id = idDel->text().trimmed();
        if(id.isEmpty()){ QMessageBox::warning(this,"Champ manquant","Entrer un ID."); return; }
        if(!isPositiveIntegerText(id)){ QMessageBox::warning(this,"ID invalide","L'ID fournisseur doit être un entier positif."); return; }
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
    for (auto *b : employeeTabButtons) tabLayout->addWidget(b);
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
    formAdd->addRow("CIN/ID *", empIdAdd);
    formAdd->addRow("Nom *", empNameAdd);
    formAdd->addRow("Adresse", empAddrAdd);
    formAdd->addRow("Email", empEmailAdd); // Champ non présent dans la table, conservé pour UI uniquement
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

    // Contraintes de saisie employés
    empIdAdd->setMaxLength(12);
    empSearchEdit->setMaxLength(12);
    empDeleteId->setMaxLength(12);
    empNameAdd->setMaxLength(80);
    empEditName->setMaxLength(80);
    empAddrAdd->setMaxLength(160);
    empPhoneAdd->setMaxLength(20);
    empEditPhone->setMaxLength(20);
    empEmailAdd->setMaxLength(120);

    empIdAdd->setValidator(new QIntValidator(1, 999999999, pageAdd));
    empSearchEdit->setValidator(new QIntValidator(1, 999999999, pageEdit));
    empDeleteId->setValidator(new QIntValidator(1, 999999999, pageDel));
    empPhoneAdd->setValidator(new QRegularExpressionValidator(phonePattern(), pageAdd));
    empEditPhone->setValidator(new QRegularExpressionValidator(phonePattern(), pageEdit));
    empEmailAdd->setValidator(new QRegularExpressionValidator(emailPattern(), pageAdd));
    empHireDateAdd->setMaximumDate(QDate::currentDate());

    // List page
    QWidget *pageList = new QWidget(employeeStack);
    auto *vList = new QVBoxLayout(pageList);
    employeesTable = new QTableWidget(0, empHeaders.size(), pageList);
    employeesTable->setHorizontalHeaderLabels(empHeaders);
    employeesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    employeesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    employeesTable->setAlternatingRowColors(true);
    employeesTable->setStyleSheet(
        "QHeaderView::section{background:#3b2a20;color:white;font-weight:bold;padding:6px;border:1px solid #C68E65;}"
        "QTableWidget{gridline-color:#C68E65;alternate-background-color:#fdf7f2;background:#fffafa;selection-background-color:#f1c8a8;}"
    );
    vList->addWidget(employeesTable);
    auto *statsLayout = new QHBoxLayout();
    empStatsTotal = new QLabel("0", pageList);
    empStatsAvgSalary = new QLabel("0", pageList);
    statsLayout->addWidget(new QLabel("Total:")); statsLayout->addWidget(empStatsTotal);
    statsLayout->addWidget(new QLabel("Salaire moyen:")); statsLayout->addWidget(empStatsAvgSalary);
    vList->addLayout(statsLayout);
    employeeStack->addWidget(pageList);

    // Tabs behavior
    auto setTab=[&](int idx){ for(int i=0;i<employeeTabButtons.size();++i) employeeTabButtons[i]->setChecked(i==idx); if(employeeStack) employeeStack->setCurrentIndex(idx); };
    connect(btnTabAdd,&QPushButton::clicked,this,[=](){ setTab(0); });
    connect(btnTabEdit,&QPushButton::clicked,this,[=](){ setTab(1); });
    connect(btnTabDelete,&QPushButton::clicked,this,[=](){ setTab(2); });
    connect(btnTabList,&QPushButton::clicked,this,[=](){ setTab(3); updateStatistics(); });
    setTab(0);

    // Actions
    auto refreshEmpTable = [=](){
        QSqlDatabase db = Connection::instance()->database();
        if (!db.isOpen()) {
            Connection::instance()->createConnect();
            db = Connection::instance()->database();
        }
        employeesTable->setRowCount(0);
        employeeModel->removeRows(0, employeeModel->rowCount());

        QSqlQuery q(db);
        q.prepare("SELECT CIN, NOM, POSTE, ADRESSE, TELEPHONE, DATE_EMBAUCHE, SALAIRE, STATUT, SEXE FROM EMPLOYE ORDER BY CIN");
        if (!q.exec()) {
            qDebug() << "refreshEmpTable error:" << q.lastError();
            return;
        }
        const int colCount = q.record().count();
        if (colCount < empHeaders.size()) {
            qDebug() << "refreshEmpTable: colonne manquante" << colCount << "attendu" << empHeaders.size();
        }
        double sum = 0.0; int total = 0;
        while (q.next()) {
            int r = employeeModel->rowCount();
            employeeModel->insertRow(r);
            for(int c=0;c<empHeaders.size();++c){
                if (c < colCount) {
                    employeeModel->setData(employeeModel->index(r,c), q.value(c).toString());
                }
            }
            int row = employeesTable->rowCount(); employeesTable->insertRow(row);
            for(int c=0;c<empHeaders.size();++c){
                if (c < colCount) {
                    employeesTable->setItem(row,c,new QTableWidgetItem(q.value(c).toString()));
                }
            }
            sum += (colCount > 6 ? q.value(6).toDouble() : 0.0);
            ++total;
        }
        if(empStatsTotal) empStatsTotal->setText(QString::number(total));
        if(empStatsAvgSalary) empStatsAvgSalary->setText(QString::number(total?sum/total:0.0,'f',2));
    };

    connect(btnAddEmp, &QPushButton::clicked, this, [=]() {
        QString id = empIdAdd->text().trimmed();
        QString name = empNameAdd->text().trimmed();
        if (id.isEmpty() || name.isEmpty()) {
            QMessageBox::warning(this, "Champs requis", "CIN et Nom sont obligatoires.");
            return;
        }
        if (!isPositiveIntegerText(id)) {
            QMessageBox::warning(this, "Saisie invalide", "Le CIN/ID doit être un nombre entier positif.");
            return;
        }
        if (!isValidPersonName(name)) {
            QMessageBox::warning(this, "Saisie invalide", "Le nom employé est invalide.");
            return;
        }

        // Capturer toutes les valeurs UI sur le thread principal (évite les accès QWidget depuis le worker).
        const QString poste = empPosteAdd->currentText();
        const QString adresse = empAddrAdd->text().trimmed();
        const QString emailUi = empEmailAdd->text().trimmed();
        const QString tel = empPhoneAdd->text().trimmed();
        const QDate dateEmb = empHireDateAdd->date();
        const double salaire = empSalaryAdd->value();
        const QString sexe = empSexAdd->currentText();

        if (!emailUi.isEmpty() && !isValidEmail(emailUi)) {
            QMessageBox::warning(this, "Saisie invalide", "L'email employé est invalide.");
            return;
        }
        if (!tel.isEmpty() && !isValidPhone(tel)) {
            QMessageBox::warning(this, "Saisie invalide", "Le téléphone employé est invalide.");
            return;
        }
        if (!dateEmb.isValid() || dateEmb > QDate::currentDate()) {
            QMessageBox::warning(this, "Saisie invalide", "La date d'embauche ne peut pas être dans le futur.");
            return;
        }
        if (salaire <= 0.0) {
            QMessageBox::warning(this, "Saisie invalide", "Le salaire doit être supérieur à 0.");
            return;
        }

        QSqlDatabase dbBase = Connection::instance()->database();
        if (!dbBase.isOpen()) {
            Connection::instance()->createConnect();
            dbBase = Connection::instance()->database();
        }
        if (!dbBase.isValid()) {
            QMessageBox::critical(this, "Base de données", "Connexion indisponible");
            return;
        }

        const QString driverName = dbBase.driverName();
        const QString hostName = dbBase.hostName();
        const QString databaseName = dbBase.databaseName();
        const QString userName = dbBase.userName();
        const QString password = dbBase.password();
        const int port = dbBase.port();
        const QString connectOptions = dbBase.connectOptions();

        btnAddEmp->setEnabled(false);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        auto futureAddEmp = QtConcurrent::run([=]() {
            const QString connName = QString("emp_async_%1").arg(QUuid::createUuid().toString(QUuid::WithoutBraces));
            QString title;
            QString message;
            bool isWarning = false;
            bool success = false;

            QSqlDatabase db = QSqlDatabase::addDatabase(driverName, connName);
            db.setHostName(hostName);
            db.setDatabaseName(databaseName);
            db.setUserName(userName);
            db.setPassword(password);
            if (port > 0) {
                db.setPort(port);
            }
            db.setConnectOptions(connectOptions);

            do {
                if (!db.open()) {
                    title = "Base de données";
                    const QString errOpen = db.lastError().text();
                    message = errOpen.isEmpty() ? "Impossible d'ouvrir la connexion." : errOpen;
                    break;
                }

                {
                    QSqlQuery qCheck(db);
                    qCheck.prepare("SELECT 1 FROM EMPLOYE WHERE CIN=:id");
                    qCheck.bindValue(":id", id);
                    if (qCheck.exec() && qCheck.next()) {
                        title = "Doublon";
                        message = "ID existe déjà en base.";
                        isWarning = true;
                        break;
                    }
                    if (qCheck.lastError().isValid()) {
                        title = "Erreur";
                        message = qCheck.lastError().text();
                        break;
                    }

                    QSqlQuery q(db);
                    q.prepare("INSERT INTO EMPLOYE (CIN, NOM, POSTE, ADRESSE, TELEPHONE, DATE_EMBAUCHE, SALAIRE, STATUT, SEXE) "
                              "VALUES (:id,:nom,:poste,:adresse,:tel,:date_emb,:salaire,:statut,:sexe)");
                    q.bindValue(":id", id);
                    q.bindValue(":nom", name);
                    q.bindValue(":poste", poste);
                    q.bindValue(":adresse", adresse);
                    q.bindValue(":tel", tel);
                    q.bindValue(":date_emb", dateEmb);
                    q.bindValue(":salaire", salaire);
                    q.bindValue(":statut", QStringLiteral("Actif"));
                    q.bindValue(":sexe", sexe);
                    if (!q.exec()) {
                        title = "Erreur insertion";
                        message = q.lastError().text();
                        break;
                    }

                    QSqlQuery commit(db);
                    commit.exec("COMMIT");
                }

                success = true;
            } while (false);

            if (db.isValid()) {
                db.close();
                db = QSqlDatabase();
            }
            QSqlDatabase::removeDatabase(connName);

            QMetaObject::invokeMethod(qApp, [=]() {
                QApplication::restoreOverrideCursor();
                btnAddEmp->setEnabled(true);
                if (success) {
                    refreshEmpTable();
                    QMessageBox::information(this, "Succès", "Employé ajouté avec succès.");
                    return;
                }
                if (isWarning) {
                    QMessageBox::warning(nullptr, title, message);
                } else {
                    QMessageBox::critical(nullptr, title, message);
                }
            }, Qt::QueuedConnection);
        });
        Q_UNUSED(futureAddEmp);
    });
    connect(btnResetEmp,&QPushButton::clicked,this,[=](){
        empIdAdd->clear(); empNameAdd->clear(); empAddrAdd->clear(); empEmailAdd->clear(); empPhoneAdd->clear();
        empHireDateAdd->setDate(QDate::currentDate()); empPosteAdd->setCurrentIndex(0); empSalaryAdd->setValue(0); empSexAdd->setCurrentIndex(0);
    });

    connect(btnSearchEdit,&QPushButton::clicked,this,[=](){
        QString id=empSearchEdit->text().trimmed();
        if (id.isEmpty()) { QMessageBox::warning(this,"ID manquant","Entrer un CIN à rechercher."); return; }
        if (!isPositiveIntegerText(id)) { QMessageBox::warning(this,"ID invalide","Le CIN doit être un entier positif."); return; }
        QSqlDatabase db = Connection::instance()->database();
        if (!db.isOpen()) { Connection::instance()->createConnect(); db = Connection::instance()->database(); }
        QSqlQuery q(db);
        q.prepare("SELECT NOM, TELEPHONE, SALAIRE FROM EMPLOYE WHERE CIN=:id");
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
        QString id=empSearchEdit->text().trimmed();
        const QString updatedName = empEditName->text().trimmed();
        const QString updatedPhone = empEditPhone->text().trimmed();
        const double updatedSalary = empEditSalary->value();
        if (!isPositiveIntegerText(id)) { QMessageBox::warning(this,"ID invalide","Le CIN doit être un entier positif."); return; }
        if (!isValidPersonName(updatedName)) { QMessageBox::warning(this,"Saisie invalide","Le nom employé est invalide."); return; }
        if (!updatedPhone.isEmpty() && !isValidPhone(updatedPhone)) { QMessageBox::warning(this,"Saisie invalide","Le téléphone employé est invalide."); return; }
        if (updatedSalary <= 0.0) { QMessageBox::warning(this,"Saisie invalide","Le salaire doit être supérieur à 0."); return; }
        QSqlDatabase db = Connection::instance()->database();
        if (!db.isOpen()) { Connection::instance()->createConnect(); db = Connection::instance()->database(); }
        QSqlQuery q(db);
        q.prepare("UPDATE EMPLOYE SET NOM=:nom, TELEPHONE=:tel, SALAIRE=:sal WHERE CIN=:id");
        q.bindValue(":nom", updatedName);
        q.bindValue(":tel", updatedPhone);
        q.bindValue(":sal", updatedSalary);
        q.bindValue(":id", id);
        if (!q.exec()) { QMessageBox::critical(this,"Erreur", q.lastError().text()); return; }
        QSqlQuery commit(db); commit.exec("COMMIT");
        refreshEmpTable(); QMessageBox::information(this,"Succès","Employé mis à jour.");
    });

    connect(btnDel,&QPushButton::clicked,this,[=](){
        QString id=empDeleteId->text().trimmed();
        if (id.isEmpty()) { QMessageBox::warning(this,"ID manquant","Entrer un CIN."); return; }
        if (!isPositiveIntegerText(id)) { QMessageBox::warning(this,"ID invalide","Le CIN doit être un entier positif."); return; }
        if (QMessageBox::question(this,"Confirmation","Supprimer cet employé ?") != QMessageBox::Yes) return;
        QSqlDatabase db = Connection::instance()->database();
        if (!db.isOpen()) { Connection::instance()->createConnect(); db = Connection::instance()->database(); }
        QSqlQuery q(db);
        q.prepare("DELETE FROM EMPLOYE WHERE CIN=:id");
        q.bindValue(":id", id);
        if (!q.exec()) { QMessageBox::critical(this,"Erreur", q.lastError().text()); return; }
        QSqlQuery commit(db); commit.exec("COMMIT");
        refreshEmpTable(); QMessageBox::information(this,"Succès","Employé supprimé.");
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

    if (s.nom.isEmpty() || s.type.isEmpty() || s.adresse.isEmpty() || s.telephone.isEmpty() || s.email.isEmpty()) {
        QMessageBox::warning(const_cast<MainWindow*>(this), "Champs requis", "Nom, type, adresse, téléphone et email sont obligatoires.");
        return s;
    }
    if (!isValidPersonName(s.nom)) {
        QMessageBox::warning(const_cast<MainWindow*>(this), "Saisie invalide", "Nom fournisseur invalide.");
        return s;
    }
    if (s.type.size() < 2) {
        QMessageBox::warning(const_cast<MainWindow*>(this), "Saisie invalide", "Type fournisseur invalide.");
        return s;
    }
    if (!isValidPhone(s.telephone)) {
        QMessageBox::warning(const_cast<MainWindow*>(this), "Saisie invalide", "Téléphone fournisseur invalide.");
        return s;
    }
    if (!isValidEmail(s.email)) {
        QMessageBox::warning(const_cast<MainWindow*>(this), "Saisie invalide", "Email fournisseur invalide.");
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
        ButtonAnimationHelper::setupLuxuryButtonAnimation(b);
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
            <div style='margin-left:8px; width:32px; height:32px; border-radius:50%%; background:#8B4513; color:white; font-size:13px; font-weight:700; display:flex; align-items:center; justify-content:center;'>V</div>
        </div>)").arg(text.toHtmlEscaped());
    } else {
        const QString botName = speaker.isEmpty() ? "Chat Bot" : speaker;
        const QString botInitial = botName.left(1).toUpper().toHtmlEscaped();
        html = QString(R"(<div style='margin:8px 0; display:flex; align-items:flex-end;'>
            <div style='margin-right:8px; width:32px; height:32px; border-radius:50%%; background:#F2D2B5; color:#8B4513; font-size:13px; font-weight:700; display:flex; align-items:center; justify-content:center;'>%1</div>
            <div style='max-width:70%%; background:#fffbe6; color:#8B4513; border-radius:16px 16px 16px 4px; padding:10px 16px; font-size:15px; box-shadow:0 2px 8px #0001;'><b>%2</b><br>%3</div>
        </div>)")
            .arg(botInitial)
            .arg(botName.toHtmlEscaped())
            .arg(text.toHtmlEscaped());
    }
    ui->tb_ai_log->append(html);
}

void MainWindow::sendAiMessage(const QString &userText)
{
    const QString trimmed = userText.trimmed();
    if (trimmed.isEmpty()) return;

    appendAiMessage("Vous", trimmed);

    // Si Gemini est configuré, il servira de fallback pour toute question non reconnue localement.
    const bool geminiEnabled = aiNetwork && !geminiApiKey.trimmed().isEmpty();

    
    // Mode assistant: réponses métier + conversation naturelle
    QString question = trimmed.toLower();
    QSqlDatabase db = Connection::instance()->database();
    auto ensureDbOpen = [&]() -> bool {
        if (!db.isOpen()) {
            Connection::instance()->createConnect();
            db = Connection::instance()->database();
        }
        return db.isOpen();
    };

    auto containsAny = [&](const QStringList &tokens) -> bool {
        for (const QString &token : tokens) {
            if (question.contains(token)) {
                return true;
            }
        }
        return false;
    };

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
        if (!ensureDbOpen()) {
            appendAiMessage("Chat Bot", "Je ne peux pas accéder à la base de données pour le moment. Veuillez réessayer plus tard.");
            return;
        }
        QSqlQuery q(db);
        extern QString gMaterialsTableName;
        QString sql = "SELECT NVL(nom_matiere, type_matiere), quantite_stock FROM " + gMaterialsTableName + " WHERE LOWER(NVL(nom_matiere, type_matiere)) LIKE :matiere";
        q.prepare(sql);
        q.bindValue(":matiere", "%" + matiereCherchee + "%");
        if (!q.exec()) {
            appendAiMessage("Chat Bot", "Désolé, je n'ai pas pu obtenir le stock : " + q.lastError().text());
            return;
        }
        if (q.next()) {
            QString nom = q.value(0).toString();
            int stock = q.value(1).toInt();
            appendAiMessage("Chat Bot", QString("Il reste <b>%1</b> unité(s) de <b>%2</b> en stock.").arg(stock).arg(nom));
        } else {
            appendAiMessage("Chat Bot", "Je n'ai trouvé aucune matière correspondant à : " + matiereCherchee);
        }
        return;
    }

    // 2. Alerte stock faible (expressions variées)
    QStringList exprAlerte = {"alerte", "rupture", "alerter", "faible", "bientôt fini", "presque vide", "plus beaucoup", "manque", "bientôt en rupture"};
    for (const QString &expr : exprAlerte) {
        if (question.contains(expr)) {
            if (!ensureDbOpen()) {
                appendAiMessage("Chat Bot", "Je ne peux pas accéder à la base de données pour le moment. Veuillez réessayer plus tard.");
                return;
            }
            QSqlQuery q(db);
            extern QString gMaterialsTableName;
            QString sql = "SELECT NVL(nom_matiere, type_matiere), quantite_stock FROM " + gMaterialsTableName + " WHERE quantite_stock < 10 ORDER BY quantite_stock ASC";
            if (!q.exec(sql)) {
                appendAiMessage("Chat Bot", "Impossible d'obtenir la liste des stocks faibles : " + q.lastError().text());
                return;
            }
            QStringList alertes;
            while (q.next()) {
                QString nom = q.value(0).toString();
                int stock = q.value(1).toInt();
                alertes << QString("%1 (%2 unité(s))").arg(nom).arg(stock);
            }
            if (alertes.isEmpty()) {
                appendAiMessage("Chat Bot", "Aucune matière n'est en rupture ou proche de la rupture.");
            } else {
                appendAiMessage("Chat Bot", "Attention, stock faible pour : " + alertes.join(", "));
            }
            return;
        }
    }

    // 3. Statistiques globales (expressions variées)
    QStringList exprStats = {"stat", "statistique", "moyenne", "total", "combien de matières", "nombre de matières", "bilan", "résumé", "synthèse"};
    for (const QString &expr : exprStats) {
        if (question.contains(expr)) {
            if (!ensureDbOpen()) {
                appendAiMessage("Chat Bot", "Je ne peux pas accéder à la base de données pour le moment. Veuillez réessayer plus tard.");
                return;
            }
            QSqlQuery q(db);
            extern QString gMaterialsTableName;
            QString sql = "SELECT COUNT(*), SUM(quantite_stock), AVG(rendement), AVG(perte) FROM " + gMaterialsTableName;
            if (!q.exec(sql) || !q.next()) {
                appendAiMessage("Chat Bot", "Impossible d'obtenir les statistiques globales.");
                return;
            }
            int totalMat = q.value(0).toInt();
            int totalStock = q.value(1).toInt();
            double moyRendement = q.value(2).toDouble();
            double moyPerte = q.value(3).toDouble();
            appendAiMessage("Chat Bot", QString("Il y a %1 matières différentes pour un total de %2 unités en stock. Rendement moyen : %3%, Perte moyenne : %4%.")
                .arg(totalMat).arg(totalStock).arg(QString::number(moyRendement, 'f', 1)).arg(QString::number(moyPerte, 'f', 1)));
            return;
        }
    }

    // 4. Conseil anti-gaspillage (expressions variées)
    QStringList exprConseil = {"gaspillage", "conseil", "économiser", "astuce", "éviter de gaspiller", "réduire les pertes", "optimiser", "moins de perte", "éviter le gaspillage"};
    for (const QString &expr : exprConseil) {
        if (question.contains(expr)) {
            appendAiMessage("Chat Bot", "Pour limiter le gaspillage, pensez à réutiliser les chutes de matières et à optimiser la découpe. Surveillez aussi les stocks faibles pour éviter les ruptures.");
            return;
        }
    }

    // 5. Conversation naturelle (mode local)
    if (containsAny({"bonjour", "salut", "bonsoir", "coucou", "hello", "hi"})) {
        appendAiMessage("Chat Bot", "Bonjour ! Qu'est-ce que je peux faire pour vous aider ?");
        return;
    }
    if (containsAny({"aide", "help", "que peux-tu faire", "que peux tu faire", "qu'est-ce que tu peux faire", "qu est ce que tu peux faire", "comment tu peux m'aider", "comment tu peux m aider"})) {
        appendAiMessage("Chat Bot", "Je peux vous aider sur : le stock des matières, les alertes de rupture, les statistiques globales et des conseils anti-gaspillage.");
        return;
    }
    if (containsAny({"comment ça va", "comment ca va", "ça va", "ca va", "cv"})) {
        appendAiMessage("Chat Bot", "Je vais très bien, merci. Et vous ?");
        return;
    }
    if (containsAny({"qui es-tu", "qui es tu", "tu es qui", "ton nom", "présente-toi", "presente-toi"})) {
        appendAiMessage("Chat Bot", "Je suis votre Chat Bot de gestion. Je suis là pour vous aider rapidement sur vos données et vos questions métier.");
        return;
    }
    if (containsAny({"merci", "thanks", "thank you"})) {
        appendAiMessage("Chat Bot", "Avec plaisir ! Si vous voulez, je peux aussi vous donner un résumé du stock actuel.");
        return;
    }
    if (containsAny({"au revoir", "aurevoir", "bye", "à bientôt", "a bientot"})) {
        appendAiMessage("Chat Bot", "À bientôt ! N'hésitez pas si vous avez besoin d'aide.");
        return;
    }

    // 6. Réponse par défaut: si possible, répondre via Gemini pour couvrir toutes les questions.
    if (geminiEnabled) {
        sendGeminiMessage(trimmed);
        return;
    }

    appendAiMessage("Chat Bot", "Je suis en mode local. J'ai répondu au mieux, mais pour traiter toutes les questions activez GEMINI_API_KEY dans .env.local.");
    return;
}

void MainWindow::sendGeminiMessage(const QString &userText)
{
    if (!aiNetwork) {
        appendAiMessage("Chat Bot", "Service réseau indisponible.");
        return;
    }

    const QString apiKey = geminiApiKey.trimmed();
    if (apiKey.isEmpty()) {
        appendAiMessage("Chat Bot", "Configurez GEMINI_API_KEY ou .env.local pour activer Gemini.");
        return;
    }

    auto normalizeModelName = [](QString modelName) -> QString {
        modelName = modelName.trimmed();
        if (modelName.isEmpty()) {
            return {};
        }
        if (!modelName.startsWith("models/", Qt::CaseInsensitive)) {
            modelName.prepend("models/");
        }
        return modelName;
    };

    auto isAudioOnlyChatModel = [](const QString &modelName) -> bool {
        const QString normalized = modelName.trimmed().toLower();
        return normalized.contains("-tts")
            || normalized.contains("/tts")
            || normalized.contains("audio");
    };

    const QString preferredModel = normalizeModelName(geminiPreferredModel);

    appendAiMessage("Chat Bot", "(réflexion en cours via Gemini...)");

    auto sanitize = [apiKey](QString text) -> QString {
        if (!apiKey.isEmpty()) {
            text.replace(apiKey, "***");
        }
        text.replace(QRegularExpression("key=[^&\\s]+"), "key=***");
        return text;
    };

    QJsonObject part;
    part["text"] = userText;
    QJsonObject content;
    content["parts"] = QJsonArray{part};

    QJsonObject systemPart;
    systemPart["text"] = QString::fromUtf8(
        "Réponds en français avec une réponse courte et directe: 1 à 2 phrases maximum,"
        " maximum 40 mots, sans liste longue."
    );
    QJsonObject systemInstruction;
    systemInstruction["parts"] = QJsonArray{systemPart};

    QJsonObject generationConfig;
    generationConfig["maxOutputTokens"] = 160;
    generationConfig["temperature"] = 0.2;

    QJsonObject payload;
    payload["contents"] = QJsonArray{content};
    payload["systemInstruction"] = systemInstruction;
    payload["generationConfig"] = generationConfig;

    const QByteArray payloadBytes = QJsonDocument(payload).toJson();

    auto modelsToTry = std::make_shared<QStringList>();
    auto modelIndex = std::make_shared<int>(0);
    auto tryNextModel = std::make_shared<std::function<void()>>();

    // Tente les modèles un par un, et saute automatiquement ceux non disponibles.
    *tryNextModel = [this, apiKey, payloadBytes, modelsToTry, modelIndex, tryNextModel, sanitize]() {
        while (*modelIndex < modelsToTry->size() && modelsToTry->at(*modelIndex).trimmed().isEmpty()) {
            ++(*modelIndex);
        }

        if (*modelIndex >= modelsToTry->size()) {
            appendAiMessage("Chat Bot", "Aucun modèle Gemini compatible n'a été trouvé pour cette clé API.");
            return;
        }

        QString modelResource = modelsToTry->at(*modelIndex).trimmed();
        if (!modelResource.startsWith("models/")) {
            modelResource.prepend("models/");
        }

        const QUrl url(QString("https://generativelanguage.googleapis.com/v1beta/%1:generateContent?key=%2")
                           .arg(modelResource, apiKey));
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        auto *reply = aiNetwork->post(req, payloadBytes);
        connect(reply, &QNetworkReply::finished, this, [this, reply, modelResource, modelsToTry, modelIndex, tryNextModel, sanitize]() {
            reply->deleteLater();

            const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            const QByteArray data = reply->readAll();
            const QString body = QString::fromUtf8(data);
            const QString safeBody = sanitize(body);

            const bool modelNotFound = statusCode == 404
                || safeBody.contains("not found", Qt::CaseInsensitive)
                || safeBody.contains("not supported for generateContent", Qt::CaseInsensitive);

            const bool textModalityUnsupported = statusCode == 400
                && (safeBody.contains("response modalities (TEXT) is not supported", Qt::CaseInsensitive)
                    || safeBody.contains("accepts the following combination of response modalities", Qt::CaseInsensitive)
                    || safeBody.contains("INVALID_ARGUMENT", Qt::CaseInsensitive));

            const bool transientUnavailable = statusCode == 429
                || statusCode == 500
                || statusCode == 503
                || safeBody.contains("UNAVAILABLE", Qt::CaseInsensitive)
                || safeBody.contains("high demand", Qt::CaseInsensitive)
                || safeBody.contains("temporarily unavailable", Qt::CaseInsensitive);

            const bool keyRejected = statusCode == 401
                || statusCode == 403
                || safeBody.contains("PERMISSION_DENIED", Qt::CaseInsensitive)
                || safeBody.contains("reported as leaked", Qt::CaseInsensitive)
                || safeBody.contains("invalid api key", Qt::CaseInsensitive)
                || safeBody.contains("API key", Qt::CaseInsensitive);

            if (keyRejected) {
                geminiApiKey.clear();
                appendAiMessage("Chat Bot", "Votre clé Gemini est bloquée ou invalide. Remplacez GEMINI_API_KEY dans .env.local puis redémarrez l'application.");
                appendAiMessage("Chat Bot", "Je passe automatiquement en mode local pour continuer à vous aider.");
                return;
            }

            if ((reply->error() != QNetworkReply::NoError || statusCode >= 300)
                && (modelNotFound || transientUnavailable || textModalityUnsupported)) {
                ++(*modelIndex);
                if (*modelIndex < modelsToTry->size()) {
                    (*tryNextModel)();
                    return;
                }

                if (transientUnavailable) {
                    appendAiMessage("Chat Bot", "Les modèles Gemini sont temporairement surchargés (503). Réessayez dans quelques instants.");
                    appendAiMessage("Chat Bot", "Je reste disponible en mode local pour les questions stock/statistiques.");
                } else if (textModalityUnsupported) {
                    appendAiMessage("Chat Bot", "Le modèle configuré est audio/TTS et ne peut pas répondre en texte.");
                    appendAiMessage("Chat Bot", "Utilisez un modèle texte, par exemple: models/gemini-2.5-flash.");
                } else {
                    appendAiMessage("Chat Bot", "Aucun modèle Gemini compatible n'a été trouvé pour cette clé API.");
                }
                return;
            }

            if (reply->error() != QNetworkReply::NoError) {
                appendAiMessage("Chat Bot", QString("Erreur Gemini (%1): %2")
                                               .arg(statusCode)
                                               .arg(sanitize(reply->errorString())));
                if (!safeBody.isEmpty()) {
                    appendAiMessage("Chat Bot", QString("Détail (%1): %2").arg(modelResource, safeBody.left(700)));
                }
                return;
            }

            if (statusCode >= 300) {
                appendAiMessage("Chat Bot", QString("Erreur HTTP Gemini %1").arg(statusCode));
                if (!safeBody.isEmpty()) {
                    appendAiMessage("Chat Bot", QString("Détail (%1): %2").arg(modelResource, safeBody.left(700)));
                }
                return;
            }

            const QJsonDocument doc = QJsonDocument::fromJson(data);
            if (!doc.isObject()) {
                appendAiMessage("Chat Bot", "Réponse Gemini invalide.");
                return;
            }

            QString responseText;
            const QJsonArray candidates = doc.object().value("candidates").toArray();
            for (const QJsonValue &candidateValue : candidates) {
                const QJsonObject contentObj = candidateValue.toObject().value("content").toObject();
                const QJsonArray parts = contentObj.value("parts").toArray();

                QStringList textParts;
                for (const QJsonValue &partValue : parts) {
                    const QString text = partValue.toObject().value("text").toString().trimmed();
                    if (!text.isEmpty()) {
                        textParts << text;
                    }
                }

                if (!textParts.isEmpty()) {
                    responseText = textParts.join(" ").simplified();
                    break;
                }
            }

            if (responseText.isEmpty()) {
                appendAiMessage("Chat Bot", "Réponse Gemini vide.");
                return;
            }

            // Si un modèle renvoie une réponse tronquée (ex: "Vous pouvez"),
            // on tente automatiquement le modèle suivant.
            const QStringList rawWords = responseText.split(' ', Qt::SkipEmptyParts);
            const bool suspiciouslyShort = rawWords.size() <= 2
                && !responseText.endsWith('.')
                && !responseText.endsWith('!')
                && !responseText.endsWith('?');
            if (suspiciouslyShort && (*modelIndex + 1) < modelsToTry->size()) {
                ++(*modelIndex);
                (*tryNextModel)();
                return;
            }

            // Garde-fou UX: forcer une réponse courte même si le modèle déborde.
            QString shortText = responseText.simplified();

            int sentenceCount = 0;
            int cutPos = -1;
            for (int i = 0; i < shortText.size(); ++i) {
                const QChar ch = shortText.at(i);
                if (ch == '.' || ch == '!' || ch == '?') {
                    ++sentenceCount;
                    if (sentenceCount >= 2) {
                        cutPos = i + 1;
                        break;
                    }
                }
            }
            if (cutPos > 0 && cutPos < shortText.size()) {
                shortText = shortText.left(cutPos).trimmed();
            }

            QStringList words = shortText.split(' ', Qt::SkipEmptyParts);
            if (words.size() > 40) {
                shortText = words.mid(0, 40).join(' ') + "...";
            }

            appendAiMessage("Chat Bot", shortText);
        });
    };

    // Découvrir les modèles de la clé puis préparer une file d'essais robuste.
    const QUrl listUrl(QString("https://generativelanguage.googleapis.com/v1beta/models?key=%1").arg(apiKey));
    QNetworkRequest listReq(listUrl);
    auto *listReply = aiNetwork->get(listReq);
    connect(listReply, &QNetworkReply::finished, this, [this, listReply, modelsToTry, modelIndex, tryNextModel, sanitize, preferredModel, normalizeModelName, isAudioOnlyChatModel]() {
        listReply->deleteLater();

        const int statusCode = listReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const QByteArray listData = listReply->readAll();
        const QString safeListBody = sanitize(QString::fromUtf8(listData));

        const bool keyRejected = statusCode == 401
            || statusCode == 403
            || safeListBody.contains("PERMISSION_DENIED", Qt::CaseInsensitive)
            || safeListBody.contains("reported as leaked", Qt::CaseInsensitive)
            || safeListBody.contains("invalid api key", Qt::CaseInsensitive)
            || safeListBody.contains("API key", Qt::CaseInsensitive);

        if (keyRejected) {
            geminiApiKey.clear();
            appendAiMessage("Chat Bot", "Votre clé Gemini est bloquée ou invalide. Remplacez GEMINI_API_KEY dans .env.local puis redémarrez l'application.");
            appendAiMessage("Chat Bot", "Je passe automatiquement en mode local pour continuer à vous aider.");
            return;
        }

        QStringList availableModels;

        if (listReply->error() == QNetworkReply::NoError) {
            const QJsonDocument listDoc = QJsonDocument::fromJson(listData);
            if (listDoc.isObject()) {
                const QJsonArray models = listDoc.object().value("models").toArray();
                for (const QJsonValue &v : models) {
                    const QJsonObject modelObj = v.toObject();
                    const QString name = modelObj.value("name").toString().trimmed();
                    if (!name.startsWith("models/gemini-", Qt::CaseInsensitive)) {
                        continue;
                    }

                    if (isAudioOnlyChatModel(name)) {
                        continue;
                    }

                    const QJsonArray methods = modelObj.value("supportedGenerationMethods").toArray();
                    bool supportsGenerate = methods.isEmpty();
                    for (const QJsonValue &m : methods) {
                        if (m.toString().compare("generateContent", Qt::CaseInsensitive) == 0) {
                            supportsGenerate = true;
                            break;
                        }
                    }

                    if (supportsGenerate && !availableModels.contains(name)) {
                        availableModels << name;
                    }
                }
            }
        }

        QStringList rankedModels;

        const QString forcedModel = normalizeModelName(preferredModel);
        if (!forcedModel.isEmpty() && !isAudioOnlyChatModel(forcedModel)) {
            rankedModels << forcedModel;
        } else if (!forcedModel.isEmpty()) {
            appendAiMessage("Chat Bot", QString("Le modèle configuré (%1) est audio/TTS. Bascule auto vers un modèle texte.").arg(forcedModel));
        }

        const QStringList preferredPrefixes = {
            "models/gemini-2.5-flash",
            "models/gemini-2.5-pro",
            "models/gemini-2.0-flash",
            "models/gemini-2.0-flash-lite",
            "models/gemini-1.5-flash",
            "models/gemini-1.5-pro"
        };

        for (const QString &prefix : preferredPrefixes) {
            for (const QString &model : availableModels) {
                if (model.startsWith(prefix, Qt::CaseInsensitive) && !rankedModels.contains(model)) {
                    rankedModels << model;
                }
            }
        }

        for (const QString &model : availableModels) {
            if (!rankedModels.contains(model)) {
                rankedModels << model;
            }
        }

        const QStringList fallbackModels = {
            "models/gemini-2.5-flash",
            "models/gemini-2.5-pro",
            "models/gemini-2.0-flash",
            "models/gemini-2.0-flash-lite",
            "models/gemini-2.0-flash-exp",
            "models/gemini-1.5-flash",
            "models/gemini-1.5-pro"
        };

        for (const QString &model : fallbackModels) {
            if (!rankedModels.contains(model)) {
                rankedModels << model;
            }
        }

        *modelsToTry = rankedModels;
        *modelIndex = 0;
        (*tryNextModel)();
    });
}

// ============================================================================
// Animation Methods Implementation
// ============================================================================

void MainWindow::animateFadeInWidget(QWidget* widget, int duration)
{
    if (!widget) return;
    QPropertyAnimation* fadeIn = AnimationUtils::createFadeInAnimation(widget, duration);
    connect(fadeIn, &QPropertyAnimation::finished, [fadeIn]() { fadeIn->deleteLater(); });
    fadeIn->start();
}

void MainWindow::animateFadeOutWidget(QWidget* widget, int duration)
{
    if (!widget) return;
    QPropertyAnimation* fadeOut = AnimationUtils::createFadeOutAnimation(widget, duration);
    connect(fadeOut, &QPropertyAnimation::finished, [fadeOut]() { fadeOut->deleteLater(); });
    fadeOut->start();
}

void MainWindow::animateSlideInWidget(QWidget* widget, bool fromLeft, int duration)
{
    if (!widget) return;
    QPropertyAnimation* slideIn = fromLeft 
        ? AnimationUtils::createSlideInLeftAnimation(widget, duration)
        : AnimationUtils::createSlideInRightAnimation(widget, duration);
    connect(slideIn, &QPropertyAnimation::finished, [slideIn]() { slideIn->deleteLater(); });
    slideIn->start();
}

void MainWindow::animatePulseWidget(QWidget* widget, int duration)
{
    if (!widget) return;
    QSequentialAnimationGroup* pulse = AnimationUtils::createPulseAnimation(widget, duration);
    connect(pulse, &QSequentialAnimationGroup::finished, [pulse]() { pulse->deleteLater(); });
    pulse->start();
}

void MainWindow::animateStatisticsUpdate()
{
    // Animer l'apparition des cartes de statistiques avec le style luxe
    // Cette fonction est appelée après updateStatistics()
    if (ui && ui->stat_main_card_orders) {
        animateFadeInWidget(ui->stat_main_card_orders, 900);
        QTimer::singleShot(150, [this]() {
            if (ui && ui->stat_main_card_orders) {
                animatePulseWidget(ui->stat_main_card_orders, 1000);
            }
        });
    }
    if (ui && ui->stat_main_card_revenue) {
        animateFadeInWidget(ui->stat_main_card_revenue, 900);
        QTimer::singleShot(200, [this]() {
            if (ui && ui->stat_main_card_revenue) {
                animatePulseWidget(ui->stat_main_card_revenue, 1000);
            }
        });
    }
    if (ui && ui->stat_main_card_pending) {
        animateFadeInWidget(ui->stat_main_card_pending, 900);
        QTimer::singleShot(250, [this]() {
            if (ui && ui->stat_main_card_pending) {
                animatePulseWidget(ui->stat_main_card_pending, 1000);
            }
        });
    }
}

void MainWindow::animateTableItemAppearance(QTableWidget* table)
{
    if (!table || table->rowCount() == 0) return;
    
    // Animer l'apparition de chaque ligne du tableau avec un délai cascadé
    // Pour un effet luxe : cascadé lent et fluide
    for (int row = 0; row < table->rowCount() && row < 20; ++row) {
        QTimer::singleShot(row * 50, [this, table, row]() {
            for (int col = 0; col < table->columnCount(); ++col) {
                QTableWidgetItem* item = table->item(row, col);
                if (item) {
                    // Les items vont progressivement apparaître via l'opacité
                    QWidget* widget = table->cellWidget(row, col);
                    if (widget) {
                        animateFadeInWidget(widget, 500);
                    }
                }
            }
        });
    }
}

// ============================================================================
// ARDUINO SENSOR IMPLEMENTATION
// ============================================================================

void MainWindow::connectToArduino()
{
    if (!arduinoSensor) {
        qDebug() << "ArduinoSensor non initialise";
        return;
    }

    if (arduinoSensor->isConnected()) {
        return;
    }

    QStringList portCandidates;
    const QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : availablePorts) {
        const QString portName = portInfo.portName().trimmed();
        if (!portName.isEmpty()) {
            portCandidates << portName;
        }
    }

    if (portCandidates.isEmpty()) {
        // Fallback defensif si aucun port n'est expose par QSerialPortInfo.
        portCandidates = {"COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "COM10"};
    }

    portCandidates.removeDuplicates();
    qDebug() << "🔌 Tentative de connexion Arduino...";
    qDebug() << "Ports candidats:" << portCandidates;

    for (const QString &port : portCandidates) {
        qDebug() << "  → Essai port" << port;
        if (arduinoSensor->connectToPort(port, 9600)) {
            lastArduinoPort = port;
            arduinoUnavailablePopupShown = false;
            qDebug() << "✅ CONNECTÉ au port" << port;
            if (!arduinoConnectedPopupShown) {
                arduinoConnectedPopupShown = true;
                QMessageBox::information(this,
                                         "Connexion Arduino",
                                         QString("Arduino connecté sur %1.").arg(port));
            }
            return;
        }
    }

    qDebug() << "❌ Aucun port ne marche!";
    if (!arduinoUnavailablePopupShown) {
        arduinoUnavailablePopupShown = true;
        QMessageBox::warning(this,
                             "Arduino non trouvé",
                             "Arduino introuvable sur les ports COM disponibles.\n"
                             "L'application retentera automatiquement dans 30 secondes.");
    }
}

void MainWindow::disconnectFromArduino()
{
    if (arduinoSensor) {
        arduinoSensor->disconnectFromPort();
        qDebug() << "Arduino déconnecté";
    }
}

void MainWindow::onArduinoTemperatureReceived(float temperature, float humidity)
{
    // Mettre à jour les variables
    currentTemperature = temperature;
    currentHumidity = humidity;
    
    // Debug: afficher dans la console
    qDebug() << QString("🌡️  Température: %1°C | Humidité: %2%")
                    .arg(temperature, 0, 'f', 1)
                    .arg(humidity, 0, 'f', 1);
    
    // Enregistrer dans la base de données
    saveSensorDataToDatabase(temperature, humidity);
    
    // Vérifier le seuil de température
    checkTemperatureThreshold(temperature);
}

void MainWindow::onArduinoErrorOccurred(const QString &error)
{
    qDebug() << "❌ Erreur Arduino:" << error;
    // Optionnel : afficher un message (ne pas spammer!)
}

void MainWindow::onArduinoConnectedStatusChanged(bool connected)
{
    if (connected) {
        arduinoUnavailablePopupShown = false;
        qDebug() << "✓ Connexion Arduino établie";
    } else {
        arduinoConnectedPopupShown = false;
        qDebug() << "✗ Connexion Arduino perdue";
    }
}

void MainWindow::saveSensorDataToDatabase(float temperature, float humidity)
{
    // Récupérer la base de données Oracle via Connection (même point d'entrée que le reste de l'app).
    QSqlDatabase db = Connection::instance()->getDatabase();

    if (!db.isOpen() || !db.isValid()) {
        Connection::instance()->createConnect();
        db = Connection::instance()->getDatabase();

        if (!db.isOpen() || !db.isValid()) {
            if (!sensorDbUnavailableWarningShown) {
                sensorDbUnavailableWarningShown = true;
                QMessageBox::warning(this,
                                     "Base de données",
                                     "Connexion Oracle indisponible.\n"
                                     "Les mesures ne sont pas enregistrées pour le moment.");
            }
            return;
        }
    }

    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO SENSOR_TEMPERATURE_LOG (TEMPERATURE, HUMIDITY) "
        "VALUES (:temperature, :humidity)"
    );
    
    query.addBindValue(temperature);
    query.addBindValue(humidity);

    if (!query.exec()) {
        qDebug() << "❌ ERREUR INSERTION:" << query.lastError().text();
    } else {
        if (!db.commit()) {
            qDebug() << "⚠️  Insertion OK mais COMMIT échoué:" << db.lastError().text();
        }
        sensorDbUnavailableWarningShown = false;
    }
}

void MainWindow::checkTemperatureThreshold(float temperature)
{
    // Vérifier si la température dépasse le seuil
    if (temperature >= temperatureThreshold) {
        if (!temperatureAlertShown) {
            temperatureAlertShown = true;
            qDebug() << "🚨 ALERTE TEMPÉRATURE:" << temperature << "°C >= " << temperatureThreshold << "°C";
            showTemperatureAlert(temperature);
        }
    } else {
        // Réinitialiser le flag quand la température revient à la normale
        temperatureAlertShown = false;
    }
}

void MainWindow::showTemperatureAlert(float temperature)
{
    QString message = QString(
        "🌡️ ALERTE TEMPÉRATURE ÉLEVÉE!\n\n"
        "Température actuelle: %1°C\n"
        "Seuil d'alerte: %2°C\n\n"
        "Veuillez vérifier le système de refroidissement."
    ).arg(temperature, 0, 'f', 1).arg(temperatureThreshold, 0, 'f', 1);
    
    // Utiliser QMessageBox qui est plus fiable
    QMessageBox alertBox(QMessageBox::Warning, 
                         "⚠️ Alerte Température", 
                         message, 
                         QMessageBox::Ok, 
                         this);
    alertBox.setStyleSheet(
        "QMessageBox {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #5A6C7D, stop:1 #3D4E5C);"
        "}"
        "QMessageBox QLabel {"
        "  color: #FFFFFF;"
        "}"
        "QMessageBox QPushButton {"
        "  background: #8B5E3B;"
        "  color: #FFFFFF;"
        "  border: none;"
        "  border-radius: 6px;"
        "  padding: 8px 20px;"
        "  min-width: 80px;"
        "}"
        "QMessageBox QPushButton:hover {"
        "  background: #A0704A;"
        "}"
    );
    
    alertBox.exec();
    
    qDebug() << QString("⚠️ ALERTE TEMPÉRATURE: %1°C (Seuil: %2°C)")
                    .arg(temperature, 0, 'f', 1)
                    .arg(temperatureThreshold, 0, 'f', 1);
}
