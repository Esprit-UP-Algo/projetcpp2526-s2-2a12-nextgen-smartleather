#ifndef MATERIALSWINDOW_H
#define MATERIALSWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MaterialsWindow;
}
QT_END_NAMESPACE

class MaterialsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MaterialsWindow(QWidget *parent = nullptr);
    ~MaterialsWindow();

    // Mode intégré : cache la sidebar/top nav et étire le contenu
    void setEmbeddedMode(bool embedded);

private:
    Ui::MaterialsWindow *ui;
    bool embeddedMode = false;
    void adjustEmbeddedLayout();
    void refreshStats();
    void loadFromDatabase();           // filled when DB connection established

protected:
    void resizeEvent(QResizeEvent *event) override;
};
#endif // MATERIALSWINDOW_H

// Pour accès cross-fichier au nom de la table matières
extern QString gMaterialsTableName;
