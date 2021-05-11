#include "mainwindow.h"

#include <QGraphicsView>
#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    scene_ = new DiagramScene(this);
    scene_->setSceneRect(QRectF(0, 0, DiagramScene::Width, DiagramScene::Height));
    scene_->addRect(QRectF(0, 0, DiagramScene::Width, DiagramScene::Height));

    view_ = new QGraphicsView(scene_);
    view_->setRenderHint(QPainter::Antialiasing, true);

    QGraphicsTextItem* text = scene_->addText("Hello, World!", scene_->font());
    text->setFlag(QGraphicsItem::ItemIsMovable, true);

    DiagramPolygonItem* desicion = new DiagramPolygonItem(DiagramPolygonItem::Desicion);
    DiagramPolygonItem* process  = new DiagramPolygonItem(DiagramPolygonItem::Process);
    DiagramPolygonItem* inout    = new DiagramPolygonItem(DiagramPolygonItem::InOut);

    scene_->addItem(desicion);
    scene_->addItem(process);
    scene_->addItem(inout);

    desicion->setFlag(QGraphicsItem::ItemIsMovable);
    process->setFlag(QGraphicsItem::ItemIsMovable);
    inout->setFlag(QGraphicsItem::ItemIsMovable);

    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addWidget(view_);

    QWidget* widget = new QWidget;
    widget->setLayout(hLayout);
    setCentralWidget(widget);
}
