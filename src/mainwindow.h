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
class QAction;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void buttonGroupClicked(QAbstractButton* button);
    void deleteItem();

private:
    void createToolBox();
    void createActions();
    void createMenus();
    QWidget* createCellWidget(const QString& text, DiagramItem::DiagramType type);

private:
    DiagramScene* scene_;
    DiagramView*  view_;

    QButtonGroup* buttonGroup_;
    QToolBox*     toolBox_;

    QAction* deleteAction_;
    QAction* selectAllAction_;

    QMenu* itemMenu_;
};

#endif // MAINWINDOW_H
