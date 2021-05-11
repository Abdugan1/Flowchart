#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "diagramscene.h"
#include "diagrampolygonitem.h"

#include <QMainWindow>

class QGraphicsView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:
private:
    DiagramScene* scene_;
    QGraphicsView* view_;
};

#endif // MAINWINDOW_H
