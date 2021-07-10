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

private:
    void createSideMenu();
    void createActions();
    void createMenus();
    QToolButton* createSideMenuButton(const QString& text,
                                      DiagramItem::DiagramType type);

private:
    DiagramScene* scene_;
    DiagramView*  view_;

    QButtonGroup* buttonGroup_;
    QFrame*       sideMenu_;

    QAction* deleteAction_;
    QAction* selectAllAction_;

    QMenu* itemMenu_;
};

#endif // MAINWINDOW_H
