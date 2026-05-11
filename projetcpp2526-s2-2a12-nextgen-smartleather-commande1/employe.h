#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class EmployeWindow; }
QT_END_NAMESPACE

class EmployeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EmployeWindow(QWidget *parent = nullptr);
    ~EmployeWindow();

private slots:
    void addEmployee();
    void loadEmployee();
    void updateEmployee();
    void deleteEmployee();
    void refreshEmployees();
    void setPage(int index);

private:
    Ui::EmployeWindow *ui;
    QString m_loadedCin;

    bool ensureDatabase();
    void populateTable();
    void clearAddForm();
    void clearEditForm();
    void clearDeleteForm();
    void updateStats(int totalRows, double salarySum);
};

#endif // EMPLOYE_H
