#ifndef EMPLOYEWINDOW_H
#define EMPLOYEWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QSqlQuery>
#include "employe.h"

namespace Ui { class EmployeWindow; }

class EmployeWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit EmployeWindow(QWidget *parent = nullptr);
    ~EmployeWindow();

private slots:
    // Navigation onglets
    void showAdd();
    void showEdit();
    void showDelete();
    void showList();

    // CRUD
    void onAdd();
    void onResetAdd();
    void onLoad();
    void onUpdate();
    void onDelete();

    // Liste
    void onRefresh();
    void onSearch();
    void onSort(int col);
    void onExportPDF();
    void onRowSelected();

private:
    Ui::EmployeWindow *ui;

    void setupConnections();
    void setupTable();
    void loadTable(const QString &search = "");
    void updateStats();
    void clearAddForm();
    void setTabActive(int index);

    // Tri
    int  m_sortCol = -1;
    bool m_sortAsc = true;
};

#endif // EMPLOYEWINDOW_H
