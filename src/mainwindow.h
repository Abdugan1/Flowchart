#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "diagramscene.h"
#include "diagrampolygonitem.h"

#include <QMainWindow>

class QGraphicsView;

class QButtonGroup;
class QAbstractButton;
class QToolBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void buttonGroupClicked(QAbstractButton* button);
private:
    void createToolBox();
    QWidget* createCellWidget(const QString& text, DiagramItem::DiagramType type);
private:
    DiagramScene* scene_;
    QGraphicsView* view_;

    QButtonGroup* buttonGroup_;
    QToolBox*     toolBox_;
};

#endif // MAINWINDOW_H
