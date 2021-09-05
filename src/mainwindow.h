#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "diagramscene.h"
#include "diagramview.h"
#include "diagramitem.h"

#include <QMainWindow>

class QGraphicsView;

class QButtonGroup;
class QAbstractButton;
class QToolBox;
class QToolButton;
class QAction;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void buttonGroupClicked(QAbstractButton* button);
    void onSaveAsJson();
    void onOpenDiagram();
    void exportToPng();

    void loadFromSaveFile(const QString& fileName);

private:
    void createSideMenu();
    void createActions();
    void createMenus();
    QToolButton* createSideMenuDiagramButton(const QString& text,
                                             DiagramItem::DiagramType type);
    QToolButton* createSideMenuButton(const QString& text,
                                      const QPixmap& pixmap);

private:
    DiagramScene* scene_ = nullptr;
    DiagramView*  view_  = nullptr;

    QButtonGroup* buttonGroup_        = nullptr;
    QToolButton*  saveJsonButton_     = nullptr;
    QToolButton*  loadFromJsonButton_ = nullptr;
    QScrollArea*  sideMenu_           = nullptr;

    QAction* deleteAction_    = nullptr;
    QAction* selectAllAction_ = nullptr;
    QAction* copyAction_      = nullptr;
    QAction* pasteAction_     = nullptr;
    QAction* aboutQtAction_   = nullptr;

    QAction* saveAsJsonAction_  = nullptr;
    QAction* openDiagramAction_ = nullptr;

    QAction* exportToPngAction_ = nullptr;

    QMenu* fileMenu_ = nullptr;
    QMenu* editMenu_ = nullptr;
    QMenu* helpMenu_ = nullptr;
};

#endif // MAINWINDOW_H
