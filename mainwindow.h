#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void initializeMaterialsTab();
    void addMaterial();
    void clearMaterialForm();
    void deleteMaterial();
    void exportMaterials();
    void showStatistics();
    void filterMaterials();
    void searchMaterials();
    
    // Commands Tab Functions
    void initializeCommandsTab();
    void addCommand();
    void editCommand();
    void clearCommandForm();
    void deleteCommand();
    void onCommandTableRowClicked(int row, int column);
    void exportCommands();
    void showCommandStatistics();
    void filterCommands();
    void searchCommands();
    void sortCommands();
    void sendNotification();

private:
    Ui::MainWindow *ui;
    int editingRow = -1;  // Track which row is being edited (-1 means new command)
};
#endif // MAINWINDOW_H
